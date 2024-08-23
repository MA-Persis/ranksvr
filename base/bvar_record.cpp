#include "base/bvar_record.h"

namespace example {

std::unordered_map<std::string, bvar::LatencyRecorder*> performance_bvar_map;

std::unordered_map<std::string, bvar::MultiDimension<bvar::LatencyRecorder>*> m_performance_bvar_map;

bvar::LatencyRecorder memcache_get_qps;
bvar::LatencyRecorder memcache_get_lose;
bvar::LatencyRecorder memcache_get_tm;
bvar::LatencyRecorder memcache_get_recall;

// bvar::MultiDimension<bvar::LatencyRecorder> mbvar_memcache_get_qps("ad_ranksvr_m_memcache_get_qps", {"bucket", "pf"});
bvar::MultiDimension<bvar::LatencyRecorder> mbvar_memcache_get_qps("ad_ranksvr_m_memcache_get_qps", {"bucket", "pf"});
bvar::MultiDimension<bvar::LatencyRecorder> mbvar_memcache_get_lose("ad_ranksvr_m_memcache_get_lose", {"bucket", "pf"});
bvar::MultiDimension<bvar::LatencyRecorder> mbvar_memcache_get_tm("ad_ranksvr_m_memcache_get_tm", {"bucket", "pf"});
bvar::MultiDimension<bvar::LatencyRecorder> mbvar_memcache_get_recall("ad_ranksvr_m_memcache_get_recall", {"bucket", "pf"});

}