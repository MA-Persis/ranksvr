#ifndef BVAR_RECORD_H_
#define BVAR_RECORD_H_

#include <ctime>
#include <unistd.h>

#include <string>
#include <ostream>
#include <unordered_map>

#include <bvar/passive_status.h>
#include <brpc/server.h>
#include <bvar/status.h>
#include <bvar/multi_dimension.h>

#include <gflags/gflags.h>

DECLARE_int32(brpc_dummy_port);

namespace bvar {
DECLARE_int32(bvar_max_dump_multi_dimension_metric_number);
DECLARE_bool(mbvar_dump);
}

namespace example {

extern std::unordered_map<std::string, bvar::LatencyRecorder*> performance_bvar_map;
extern std::unordered_map<std::string, bvar::MultiDimension<bvar::LatencyRecorder>*> m_performance_bvar_map;

extern bvar::LatencyRecorder memcache_get_qps;
extern bvar::LatencyRecorder memcache_get_lose;
extern bvar::LatencyRecorder memcache_get_tm;
extern bvar::LatencyRecorder memcache_get_recall;

extern bvar::MultiDimension<bvar::LatencyRecorder> mbvar_memcache_get_qps;
extern bvar::MultiDimension<bvar::LatencyRecorder> mbvar_memcache_get_lose;
extern bvar::MultiDimension<bvar::LatencyRecorder> mbvar_memcache_get_tm;
extern bvar::MultiDimension<bvar::LatencyRecorder> mbvar_memcache_get_recall;

static void bvar_record() {
  // bvar::FLAGS_mbvar_dump = true;
  // bvar::FLAGS_bvar_max_dump_multi_dimension_metric_number = 2000;

  if (brpc::StartDummyServerAt(FLAGS_brpc_dummy_port) < 0) {
    LOG(INFO) << "start dummy server in " << FLAGS_brpc_dummy_port << " failed!";
  }

  memcache_get_qps.expose("ad_ranksvr_memcache_get_qps");
  memcache_get_lose.expose("ad_ranksvr_memcache_get_lose");
  memcache_get_tm.expose("ad_ranksvr_memcache_get_tm");
  memcache_get_recall.expose("ad_ranksvr_memcache_get_recall");

  mbvar_memcache_get_qps.expose("ad_ranksvr_m_memcache_get_qps");

  performance_bvar_map.insert({ { "memcache_get_qps", &memcache_get_qps },
                                { "memcache_get_lose", &memcache_get_lose },
                                { "memcache_get_tm", &memcache_get_tm },
                                { "memcache_get_recall", &memcache_get_recall }
                              });

  m_performance_bvar_map.insert({ { "mbvar_memcache_get_qps", &mbvar_memcache_get_qps },
                                  { "mbvar_memcache_get_lose", &mbvar_memcache_get_lose },
                                  { "mbvar_memcache_get_tm", &mbvar_memcache_get_tm },
                                  { "mbvar_memcache_get_recall", &mbvar_memcache_get_recall }
                                });
}

static void  bvar_record_performance(const std::string& name, int64_t param) {
  *performance_bvar_map[name] << param;
}

static void  mbvar_record_performance(const std::list<std::string>& labels, const std::string& name, int64_t tm) {
  if (!m_performance_bvar_map.count(name) || !m_performance_bvar_map[name]) {
    return;
  }

  bvar::LatencyRecorder* cost_latency = m_performance_bvar_map[name]->get_stats(labels);

  if (!cost_latency) {
    return;
  }

  LOG(INFO)  << "mbvar record";

  *cost_latency << tm;

  // 获取latency
  int64_t request_cost_latency = cost_latency->latency();
  // 获取max_latency
  int64_t request_cost_max_latency = cost_latency->max_latency();
  // 获取qps
  int64_t request_cost_qps = cost_latency->qps();
  // 获取count
  int64_t request_cost_count = cost_latency->count();

  LOG(INFO) << "latenty:" << request_cost_latency << "," << request_cost_count;
}

} // namespace example

#endif
