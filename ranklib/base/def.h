#ifndef _QIHOO_AD_RANKING_DEFINE_H_
#define _QIHOO_AD_RANKING_DEFINE_H_

#include <float.h>
#include <utility>
#include <string>
#include <vector>

#include <tr1/unordered_map>

#include "pb/adinfo.pb.h"
#include "pb/echo.pb.h"

#define CACHED_FEATURE_TOPIC_INFO 0

namespace qihoo { namespace ad { namespace ranking {

  typedef int Int_t;
  typedef int const Int_const_t;
  typedef int const& Int_ctref_t;

  typedef std::pair<uint32_t, float> TopicPair;

  struct sort_topicpair {
    bool operator()(const TopicPair &left, const TopicPair &right) {
        return left.second > right.second;
    }
  };
  typedef std::tr1::unordered_map<uint32_t, float> TopicMap;
  typedef std::tr1::unordered_map<std::string, std::string> ConfigMap;
  typedef std::tr1::unordered_map<uint64_t, std::string> HistoryAdMap;

  enum {
    POS_STATUS_NULL = 0,
    POS_STATUS_FILTER = 1, // 做推左，可能过滤右侧
    POS_STATUS_NOLEFT = 2, // 做推右，取消推左操作
    POS_STATUS_CUTASN = 4, // 做过滤，减少广告展现
  };

  enum {
    UNKNOWN_STATUS,
    ANIMATE_STATUS,
    UNDERLYING_STATUS,
  };

  enum {
    STRATEGY_NON_MATCHED = 0,
    STRATEGY_BANDIT_RECALL,
    STRATEGY_PUSH_LEFT,
    STRATEGY_DISP_ADJUST,
  };

  enum {
    // STRATEGY_BANDIT_RECALL
    BRANCH_TEST_0 = 0,
    BRANCH_TEST_1,
    BRANCH_TEST_2,
    BRANCH_TEST_3,
    BRANCH_TEST_4,
    BRANCH_TEST_5,
    // STRATEGY_POS_ADJUST
    BRANCH_TEST_6,
    BRANCH_TEST_7,
  };

  typedef struct {
    uint32_t low;
    uint32_t high;
  } pair_u32_t;

  typedef union {
    uint64_t ul;
    pair_u32_t pu;
  } union_u64_t;

  struct RecallUint {
    uint32_t cnt_val;
    float sum_val;
    float top_val;
    float tail_val;
    std::vector<uint32_t> group_vec;
    RecallUint():
      cnt_val(0), sum_val(0.0),
      top_val(FLT_MIN), tail_val(FLT_MAX) {
      }
  };

  enum MatchType {
    EXACT = 1,
    PHRASE,
    QC,
    BROAD,
  };

  enum Level {
    MAGI = 0,
    SCTR,
    NEW_AD,
  };

  enum QuitType {
    LOW_BID_AD = 1,
    LOW_GROUP_AD,
    LOW_SCTR_AD,
    LOW_LANDPAGE_AD,
    //指定query不能展现
    NO_IMP_QUERY,
    //指定query下的adid不能展现
    NO_IMP_QUERY_AD,
    //不能展现的adid
    NO_IMP_AD,
    //低质量分的广告
    LOW_QUALITY_AD,
    //乱买词广告
    WORD_SPAMMER_AD,
    //非不可代入白名单
    NOT_UNWILDCARD_WHITELIST,
    //指定query不能推左
    NO_LEFT_QUERY = 101,
    //指定query下的adid不能推左
    NO_LEFT_QUERY_AD = 102,
    //不能推左的adid
    NO_LEFT_AD = 103,
    //移动搜索matchScore低于阈值不展现
    LOW_MOB_MATCH_SCORE_AD,
    //移动搜索simScore低于阈值不展现
    LOW_MOB_SIM_SCORE_AD,
    //移动搜索price低于阈值不展现
    LOW_MOB_PRICE_AD,
    //移动搜索sctr低于阈值不展现
    LOW_MOB_SCTR_AD,
    //移动搜索score低于阈值不展现
    LOW_MOB_SCORE_AD,
    //超过MAGI处理上限
    MAGI_ADNUM_LIMITED,
    //过滤线上magi没匹配上的广告，
    MAGI_FILTER_ONLINE,
    //右侧广告质量低
    RIGHT_LOW_QUALITY_AD,
    RIGHT_LOW_ECPM_AD,
  };

  // 位域操作
  #define WC_TITLE_MIN  1
  #define WC_DESCR_MIN  2

  // yixiang的默认取5条
  #define TOP_QUERY_TERM_CNT 5
  #define TOP_ADVERT_TERM_CNT 5

  // 展现截断设置
  #define MAX_TITLE_LENGTH 50
  #define MAX_DES_LENGTH 80

  typedef adinfo::AdInfo AdInfo;
  typedef example::VisitInfo VisitInfo;
  typedef example::QueryInfo QueryInfo;
  typedef example::RankStruct RankStruct;
  typedef example::AdInfo4Rank AdPackInfo;
  typedef userbehavior::UserBehavior UserBehavior;

  // utils
  #define DELETE_VAR_AND_SET_NULL(ptr) \
    if (ptr) { \
      delete ptr; \
      ptr = NULL; \
    }

  #define DELETE_ARR_SET_SET_NULL(ptr) \
    if (ptr) { \
      delete[] ptr; \
      ptr = NULL; \
    }

  #define SAVE_DIVIDE_OPERATION(v1, v2) \
    ((v2 > FLT_MIN || v2 < -FLT_MIN) ? (v1/v2) : (0.0))

  //////////////////////////////

  #define MAX_QUERY_TERM_CNT 512
  #define MAX_ADVERT_TERM_CNT 1024
  #define MAX_TOPIC_TERM_CNT 127
  // 2048 = 256 * sizeof(TopicPair)
  #define MAX_STORE_BUFF_SIZE 1024

  #define TOP_TERM_MAX 5

  #define TERM_BUFF_SIZE_LARGE 512
  #define TERM_BUFF_SIZE_MORE 256
  #define TERM_BUFF_SIZE_NORMAL 256
  #define TERM_BUFF_SIZE_LESS 128
  #define TERM_BUFF_SIZE_LITTLE 64

  struct TermPack {
    TermPack():
      termid(0), weight(0.0),
      count(0), istop(0), index(0) {
    }
    //TermPack(TermPack const & tp);
    //TermPack & operator=(TermPack const & tp);
    uint32_t termid;
    float weight;
    uint8_t count;
    uint8_t istop;
    uint8_t index;
    uint8_t reserved;
    uint8_t occur[TERM_BUFF_SIZE_LITTLE];
  };


} } }

#endif

