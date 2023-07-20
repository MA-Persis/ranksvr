#include <algorithm>
#include <cmath>

#include "mobsearch/mobsearch_ranker.h"
// #include "feature/feature_adapter.h"

namespace qihoo{ namespace ad {namespace ranking {
  MobSearchOnlineRanker::MobSearchOnlineRanker() {
  }

  MobSearchOnlineRanker::~MobSearchOnlineRanker() {
  }

  int MobSearchOnlineRanker::GetFeatureKey(
      example::RankStruct &rank_struct,
      iRequest& request, iResult& result) {
    iSearch& search = request.search;
    iVisit& visit = search.visit;
  }

  int MobSearchOnlineRanker::DoRank(example::RankStruct& rank_struct,
      iRequest& request, iResult& result) {
    iSearch& search = request.search;
    iVisit& visit = search.visit;
    return 0; 
  }

} } }