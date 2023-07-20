#ifndef _QIHOO_AD_RANKING_CONFIG_LOADER_H_
#define _QIHOO_AD_RANKING_CONFIG_LOADER_H_

#include <vector>
#include <string>

#include <butil/logging.h>

#include "base/def.h"

namespace qihoo { namespace ad { namespace ranking {

  struct JsonConfigLoader
  {
      static bool Load(const char* conf_file, ConfigMap& config_map);
      static bool Load(const std::string& conf_str, ConfigMap& config_map);
      static bool Get(ConfigMap& config_map, const std::string& name, std::string& value);
      static const std::string Dump(ConfigMap& config_map);
  };

} } }

#endif
