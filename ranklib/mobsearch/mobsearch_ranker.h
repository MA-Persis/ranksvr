#ifndef QIHOO_AD_RANKING_MOBSEARCH_RANKER_H_
#define QIHOO_AD_RANKING_MOBSEARCH_RANKER_H_

#include <vector>
#include "ranker/ranker.h"
// #include "request/request.h"

namespace qihoo { namespace ad { namespace ranking {

class MobSearchOnlineRanker: public RankerBase
{
  public:
    MobSearchOnlineRanker();
    virtual ~MobSearchOnlineRanker();

    virtual int GetFeatureKey(example::RankStruct &rank_struct,
                              iRequest& request, iResult& result);

    virtual int DoRank(example::RankStruct& rank_struct,
                       iRequest& request, iResult& result);
};

} } }

#endif