#ifndef _QIHOO_AD_RANKING_FEATURE_ADAPTER_H_
#define _QIHOO_AD_RANKING_FEATURE_ADAPTER_H_

#include <json/json.h>

#include <vector> 

#include "feature/feature_base.h"
#include "request/request.h"

namespace qihoo { namespace ad { namespace ranking {

typedef std::vector<ExtractorBase*> ExtractorPtrGroup;

template <typename Visit_t = iVisit,
          typename Query_t = iQuery,
          typename Adpack_t = iAdpack>
class FeatureAdapter {
public:
  FeatureAdapter() : _current_stage(STAGE_UNKNOWN) {}

  virtual ~FeatureAdapter() {
    // 释放Clone的所有特征
    for (size_t i = 0; i < STAGE_SIZE; ++i) {
      for (size_t j = 0; j < FT_ALL_SIZE; ++j) {
        for (size_t k = 0; k < _feature[i][j].size(); ++k) {
          DELETE_VAR_AND_SET_NULL(_feature[i][j][k]);
        }
      }
    }
  }

  bool Load(const std::string& input, InfoCenter& ic) {
    Json::Value lots;
    Json::Reader reader;
    if (!reader.parse(input, lots, false)) {
      LOG(FATAL) << "parse feature failed";
      return false;
    }

    FeatureCenter& fc = FeatureCenter::Instance();
    for (int i = 0; i < lots.size(); ++i) {
      Json::Value& one = lots[i];
      const std::string& name = one["name"].asString();
      const std::string& group = one["group"].asString();
      const std::string& params = one["params"].asString();
      ExtractorBase* base = fc.LoadFeature(name, params, ic);
      if (base) {
        Stage_t stage = STAGE_CROSS;
          if (group.empty()) {
          } else if (!group.compare("stage-left")) {
              stage = STAGE_LEFT;
          } else if (!group.compare("stage-right")) {
              stage = STAGE_RIGHT;
          } else if (!group.compare("stage-fs")) {
              stage = STAGE_FS;
          } else if (!group.compare("stage-admerge")){
              stage = STAGE_ADMERGE;
          }
          _feature[stage][base->Type()].push_back(base);
          //std::cout<<"stage:"<<stage<<" name:"<<name.c_str()<<std::endl;
          //CWARNING_LOG("add feature %s params %s group %s %d",
          //        name.c_str(), params.c_str(), group.c_str(),_feature[stage][base->Type()].size());
      } else {
        LOG(FATAL) << "bad feature " << name << " error " << params << " group " << group;
        return false;
      }
    }
    return true;
  }

  bool OnlinePrepare(Visit_t& visit, InfoCenter& ic) {
    ic.SetStorageVal<uint32_t>(Store_T<uint32_t>::UserProvince, visit.Province());
    ic.SetStorageVal<uint32_t>(Store_T<uint32_t>::UserCity, visit.City());
    
    const std::string& pvid = visit.Pvid();
    ic.AssignStorageArr<char>(Store_A<char>::Pvid, pvid.c_str(), pvid.c_str() + pvid.size());

    return true;
  }

  bool OnlinePrepare(Query_t& query, InfoCenter& ic) {
    const std::string& qrystr = query.QueryString();
    if(!ic.AssignStorageArr(Store_A<char>::QueryText,
        qrystr.c_str(), qrystr.c_str() + qrystr.size())) {
      Int_t pvid_size = 0;
      char* pvid_ptr = ic.GetStorageArr<char>(Store_A<char>::Pvid, pvid_size);
      LOG(NOTICE) << "OnlinePrepare visit failed [detail] pvid:" << pvid_ptr << "query:" << qrystr;
      return false;
    }
    return true;
  }

  bool OnlinePrepare(Adpack_t& adpack, InfoCenter& ic) {
    return true;
  }

private:
  Stage_t _current_stage;
  ExtractorPtrGroup _feature[STAGE_SIZE][FT_ALL_SIZE];
};

typedef FeatureAdapter<iVisit, iQuery, iAdpack> DefaultFeatureAdapter;

} } } 

#endif