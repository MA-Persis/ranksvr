#ifndef QIHOO_AD_RANKING_SEARCH_RANKER_H_
#define QIHOO_AD_RANKING_SEARCH_RANKER_H_

#include <vector>
#include "ranker/ranker.h"
// #include "store/info_impl.h"

namespace qihoo { namespace ad { namespace ranking {

class SearchOnlineRanker: public RankerBase
{
  public:
    SearchOnlineRanker();
    virtual ~SearchOnlineRanker();

    virtual int GetFeatureKey(example::RankStruct &rank_struct,
      iRequest& request, iResult& result);

    virtual int DoRank(example::RankStruct& rank_struct,
      iRequest& request, iResult& result);
};

} } }

#endif