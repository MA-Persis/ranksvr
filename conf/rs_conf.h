#include "conf/config_reader.h"

struct RankInfo
{
  uint32_t      rank_type;
  std::string   rank_conf;
};

namespace rs {

class RsConfig {
public:
  static RsConfig& get_cur_config();
  int init(const std::string &confFile );

  uint16_t nListenPort;
  uint32_t nRankThreadNm;

  std::vector<RankInfo> m_rankInfoList;

private:
  RsConfig();

public:
  qihoo::ad::ConfigReader config;
};

} // namespace rs