#include <stdio.h>  // setbuf, perror
#include <unistd.h> // fork, syscall
#include <stdint.h> // uint32_t
#include <sys/mman.h> // mmap
#include <stdlib.h> // exit
#include <linux/futex.h> // futex
#include <sys/syscall.h> // SYS_futex
#include <errno.h> // error
// #include <stdatomic.h> // atomic_compare_exchange_strong
#include <sys/wait.h> // wait
#include <atomic> 

// https://man7.org/linux/man-pages/man2/futex.2.html
// https://ost.51cto.com/posts/10402

#define errExit(msg) \   
do { \
  perror(msg); \ 
  exit(EXIT_FAILURE); \
} while (0)

static uint32_t *iaddr;
// static uint32_t *futex1, *futex2;
std::atomic<uint32_t*> futex1;
std::atomic<uint32_t*> futex2;

static int futex(uint32_t *uaddr, int futex_op, uint32_t val,
                 const struct timespec *timeout, uint32_t *uaddr2, uint32_t val3) {
  return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

/// 申请快锁
static void fwait(uint32_t *futexp) {
  long s;
  while (1) {
    uint32_t one = 1;
    uint32_t value0 = 0;
    if (futex1.compare_exchange_strong(&one, &value0))
      break; //申请快锁成功
    //申请快锁失败,需等待
    s = futex(futexp, FUTEX_WAIT, 0, NULL, NULL, 0);
    if (s == -1 && errno != EAGAIN)
      errExit("futex-FUTEX_WAIT");
  }
}
/// 释放快锁
static void fpost(uint32_t *futexp) {
  long s;
  const uint32_t zero = 0;
  uint32_t value1 = 0;
  if (futex2.compare_exchange_strong(&zero, value1)) {//释放快锁成功
    s = futex(futexp, FUTEX_WAKE, 1, NULL, NULL, 0);//唤醒等锁 进程/线程
    if (s  == -1)
      errExit("futex-FUTEX_WAKE");
  }
}

int main(int argc, char** argv) {
  pid_t childPid;

  int nloops = 3;

  setbuf(stdout, nullptr);

  iaddr = static_cast<uint32_t*>(mmap(NULL, sizeof(*iaddr) * 2, PROT_READ | PROT_WRITE,
                                 MAP_ANONYMOUS | MAP_SHARED, -1, 0));//创建可读可写匿名共享内存;

  if (iaddr == MAP_FAILED)
    errExit("mmap");

  futex1 = &iaddr[0]; //绑定锁一地址
  futex2 = &iaddr[1]; //绑定锁二地址
  *futex1 = 0; // 锁一不可申请 
  *futex2 = 1; // 锁二可申请

  childPid = fork();
  if (childPid == -1)
    errExit("fork");
  if (childPid == 0) {//子进程返回
    for (int j = 0; j < nloops; j++) {
      fwait(futex1);//申请锁一
      printf("子进程  (%jd) %d\n", (intmax_t) getpid(), j);
      fpost(futex2);//释放锁二
    }
    exit(EXIT_SUCCESS);
  }
    // 父进程返回执行
  for (int j = 0; j < nloops; j++) {
    fwait(futex2);//申请锁二
    printf("父进程 (%jd) %d\n", (intmax_t) getpid(), j);
    fpost(futex1);//释放锁一
  }
  wait(NULL);
  exit(EXIT_SUCCESS);
}