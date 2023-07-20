#include "json/json.h"
// #include "feature/feature_center.h"
#include "config/JsonConfigLoader.h"

#include "base/def.h"

#include <sstream>
#include <fstream>
#include <iostream>

namespace qihoo { namespace ad { namespace ranking {

  bool JsonConfigLoader::Load(const char* conf_file, ConfigMap& cfg_map)
  {
    if (!conf_file) {
      return false;
    }
    std::ifstream ifs(conf_file);

    Json::Reader reader;
    Json::Value jsoncfg; 
    if (!reader.parse(ifs, jsoncfg, false)) { 
        std::cerr << "json parse error:" << reader.getFormatedErrorMessages() << std::endl;
        return false; 
    }
    if (jsoncfg.type() != Json::objectValue) {
        LOG(FATAL) << "AdRank, root is not object";
        return false;
    }
    const Json::Value::Members& names = jsoncfg.getMemberNames();
    for (Json::Value::Members::const_iterator it = names.begin(); 
        it != names.end(); ++it) {
      const std::string &name = *it;
      if (jsoncfg[name].type() == Json::objectValue) {
        cfg_map[name] = jsoncfg[name].toStyledString();
      } else {
        cfg_map[name] = jsoncfg[name].asString();
      }
    }
    
    ifs.close();
    return true;
  }

  bool JsonConfigLoader::Load(const std::string& jsonstr, ConfigMap& cfg_map) 
  {
    Json::Reader reader;
    Json::Value jsoncfg; 
    if (!reader.parse(jsonstr, jsoncfg, false)) { 
        std::cerr << "json parse error:" << reader.getFormatedErrorMessages() << std::endl;
        return false;
    }
    if (jsoncfg.type() != Json::objectValue) {
      return false;
    }
    const Json::Value::Members& names = jsoncfg.getMemberNames();
    for (Json::Value::Members::const_iterator it = names.begin(); 
        it != names.end(); ++it) {
      const std::string &name = *it;
      cfg_map[name] = jsoncfg[name].toStyledString();
    }
    
    return true;
  }

  bool JsonConfigLoader::Get(ConfigMap& cfg_map,
      const std::string & name, std::string & value)
  {
    if (cfg_map.find(name) != cfg_map.end()) {
      value = cfg_map[name];
      return true;
    } else {
      LOG(NOTICE) << "config get key failed";
      return false;
    }
  }

  const std::string JsonConfigLoader::Dump(ConfigMap& cfg_map)
  {
    std::stringstream oss;
    ConfigMap::const_iterator it = cfg_map.begin();
    for (; it != cfg_map.end(); ++it) {
      oss << it->first << ":" << std::endl << it->second << std::endl;
    }
    return oss.str();
  }

} } }

