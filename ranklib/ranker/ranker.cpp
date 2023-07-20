
#include "ranker/ranker.h"

#include <time.h>
#include <sstream>
#include <math.h>
#include <sys/time.h>

#include "config/JsonConfigLoader.h"

using namespace std;

namespace qihoo { namespace ad { namespace ranking {

  // ---------- 基本接口实现 ----------
  bool RankerBase::Build(const std::string& jsonstr) {
    return Reload(jsonstr);
  }

  bool RankerBase::Reload(const std::string& jsonstr)
  {
    ConfigMap cfg_map;
    if(!JsonConfigLoader::Load(jsonstr, cfg_map)) {
      LOG(FATAL) << "reload rank failed";
      return false;
    }
    string format;
    if (!JsonConfigLoader::Get(cfg_map, "features", format)) {
      LOG(FATAL) << "get feature failed";
      return false;
    }
    if (!_feature_adapter.Load(format, _info_center)) {
      LOG(FATAL) << "load feature failed";
      return false;
    }
    return true;
  }

  // ---------- 细节调用实现 ----------
  bool RankerBase::ExtractOnlineFeature(iVisit& visit) {
    if (!_feature_adapter.OnlinePrepare(visit, _info_center)) {
      LOG(FATAL) << "prepare visit failed";
      return false;
    }
    return true;
  }

  bool RankerBase::ExtractOnlineFeature(iQuery& query) {
    if (!_feature_adapter.OnlinePrepare(query, _info_center)) {
      LOG(FATAL) << "prepare query failed";
      return false;
    }
      return true;
  }

  bool RankerBase::ExtractOnlineFeature(iAdpack& adpack) {
    if (!_feature_adapter.OnlinePrepare(adpack, _info_center)) {
      LOG(FATAL) << "prepare adpack failed";
      return false;
    }
    return true;
  }

} } }


