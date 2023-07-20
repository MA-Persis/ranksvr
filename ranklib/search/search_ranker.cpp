#include "search/search_ranker.h"

namespace qihoo{ namespace ad {namespace ranking {
  SearchOnlineRanker::SearchOnlineRanker(){
  }

  SearchOnlineRanker::~SearchOnlineRanker(){
  }

  int SearchOnlineRanker::GetFeatureKey(example::RankStruct &rank_struct,
      iRequest& request, iResult& result) {
    iSearch& search = request.search;
    iVisit& visit = search.visit;

    if (!ExtractOnlineFeature(visit)) {
      LOG(FATAL) << "visit extract failed";
      return -1;
    }

    const int query_sz = rank_struct.querylist_size();
    for (int qry_idx = 0; qry_idx < query_sz; ++qry_idx) {
      example::QueryInfo* query_info = rank_struct.mutable_querylist(qry_idx);
      int advert_sz = query_info->adlist_size();

      if (!search.SetQueryIndex(qry_idx)
          || !ExtractOnlineFeature(search.query)) {
        LOG(FATAL) << "query extract failed";
        return -1;
      }

      for (int ad_idx = 0; ad_idx < advert_sz; ++ad_idx) {
        example::AdInfo4Rank* ad_info = query_info->mutable_adlist(ad_idx);
        if (!search.SetAdpackIndex(ad_idx)
            || !ExtractOnlineFeature(search.adpack)) {
          LOG(FATAL) << "adpack extract failed";
          return -1;
        }
      }
    }

    return 0;   
  }

  int SearchOnlineRanker::DoRank(example::RankStruct& rank_struct,
      iRequest& request, iResult& result) {
    iSearch& search = request.search;
    iVisit& visit = search.visit;

    const int query_sz = rank_struct.querylist_size();
    for (int qry_idx = 0; qry_idx < query_sz; ++qry_idx) {
      const example::QueryInfo& query_info = rank_struct.querylist(qry_idx);
      if (!search.SetQueryIndex(qry_idx))
          return false;	  

      int advert_sz = query_info.adlist_size();
      
      for (int ad_idx = 0; ad_idx < advert_sz; ++ad_idx) {
        search.SetAdpackIndex(ad_idx);	      
        const example::AdInfo4Rank& ad_info = query_info.adlist(ad_idx);
      }
    }
    return 0; 
  }


} } }