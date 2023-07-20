#ifndef QIHOO_AD_RANKING_RANKER_H_
#define QIHOO_AD_RANKING_RANKER_H_

#include <iostream>
#include <vector>

#include "request/request.h"
#include "feature/feature_adapter.h"
#include "store/info_center.h"


namespace qihoo { namespace ad { namespace ranking {
class InfoCenter;

class RankerBase
{
public:
  RankerBase() { }
  virtual ~RankerBase() {}

  // ----- 基本接口，初始化和重新载入 -----
  virtual bool Build(const std::string& jsonstr);
  virtual bool Reload(const std::string& jsonstr);

  virtual int GetFeatureKey(example::RankStruct &rank_struct,
                            iRequest& request, iResult& result) {
    return -1;
  }

  virtual int DoRank(example::RankStruct &rank_struct,
                     iRequest& request, iResult& result) {
    return -1;
  }

protected:
    // 在线计算的默认实现
    virtual bool ExtractOnlineFeature(iVisit& visit);
    virtual bool ExtractOnlineFeature(iQuery& query);
    virtual bool ExtractOnlineFeature(iAdpack& adpack);

protected:
  DefaultFeatureAdapter _feature_adapter;
  InfoCenter _info_center;      
};

} } }

#endif

