#include "conf/rs_conf.h"

#include <butil/logging.h>

namespace rs { 

RsConfig& RsConfig::get_cur_config() {
  // Note:
  // The first call of get_cur_config() will be initiated before entering
  // into multi-threaded context, so we don't need any synchronization for creating
  // the global variable s_rsConfig.
  static RsConfig s_rsConfig;
  return s_rsConfig;
}

RsConfig::RsConfig() {
  nListenPort = 9999;
  nRankThreadNm = 50;
}

int RsConfig::init(const string &confFile ) {
  if ( config.init(confFile) ) {
    if ( !config.getParameter( "CONCURRENT", "LISTEN_PORT", nListenPort ) ) {
      LOG(FATAL) <<  "LISTRN_PORT configuration is invalid";
      return -1;
    }

    if ( !config.getParameter( "CONCURRENT", "RANKER_NM", nRankThreadNm) ) {
      LOG(FATAL) << "RANKER_NM configuration is invalid";
      return -1;
    }
  }

  uint32_t rankListCnt = 0;
  if ( !config.getParameter( "RANK", "RANK_LIST_CNT", rankListCnt) ) {
    LOG(FATAL) << "RANK_LIST_CNT configuration is invalid";
    return -1;
  }

  for(uint32_t i = 0; i<rankListCnt; i++) {
    char buf[1000] = {'\0'};
    snprintf(buf,1000,"RANK_TYPE_%d",i);
    uint32_t rank_type;
    if ( !config.getParameter( "RANK", buf, rank_type) ) {
      LOG(FATAL) << "RANK configuration is invalid in %s";
      return -1;
    }

    memset(buf,'\0',1000);
    snprintf(buf,1000,"RANK_CONF_%d",i);
    std::string rank_conf;
    if ( !config.getParameter( "RANK", buf, rank_conf) ) {
      LOG(FATAL) << "RANK configuration is invalid in %s";
      return -1;
    }

    RankInfo info;
    info.rank_type = rank_type;
    info.rank_conf = rank_conf;
    m_rankInfoList.push_back(info);
  }

  LOG(NOTICE) << "LISTEN_PORT:" << nListenPort;
  LOG(NOTICE) << "RANKER_NM:" << nRankThreadNm;
  LOG(NOTICE) << "RANK_LIST_CNT:" << rankListCnt;

  return 0;
}

}