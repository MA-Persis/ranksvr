// Copyright (c) 2013 Qihoo 360 Technology Co. Ltd
// Author: Charles Wang

#ifndef _QIHOO_AD_RANKING_INFO_IMPL_H_
#define _QIHOO_AD_RANKING_INFO_IMPL_H_

#include <string>
#include <vector>
#include <utility>
#include "base/def.h"
#include "store/info_defs.h"
#include "gflags/gflags.h"
#include <stdint.h>
#include <stddef.h>
#include <signal.h>
#include <boost/shared_ptr.hpp>

namespace qihoo { namespace ad { namespace ranking {

  DECLARE_int32(IC_buffer_size_huge);
  DECLARE_int32(IC_buffer_size_large);
  DECLARE_int32(IC_buffer_size_more);
  DECLARE_int32(IC_buffer_size_normal);
  DECLARE_int32(IC_buffer_size_less);
  DECLARE_int32(IC_buffer_size_little);

  enum StorageGroup {
    GroupMain = 0, // 主要特征
    GroupUint32,
    GroupUint64,
    GroupTopicInfo,
    GroupCoefs, // 主要系数
    GroupTags,
    GroupChars,
    GroupStrings,
    GroupFields,
    GroupTopicPair,
    GroupFeatTrans,
    GroupTreeModel,
    GroupDebugInfo,
    GroupHistoryAdInfo,
  };

  ////////////////////////////////////////
  ////////// VALUE GROUP DEFINE //////////
  ////////////////////////////////////////

  // for float, double
  BEGIN_STORAGE_VAL(float, GroupMain); // main group
  ADD_STORAGE_VAL(OnlineStatSctr, 0);
  ADD_STORAGE_VAL(QueryLength, 1); // USED
  ADD_STORAGE_VAL(UniqQueryLength, 2); // USED
  ADD_STORAGE_VAL(QueryWeightSum, 3); // USED
  ADD_STORAGE_VAL(UniqQueryWeightSum, 4); // USED
  ADD_STORAGE_VAL(TitleLength, 5); // USED
  ADD_STORAGE_VAL(TitleWeightSum, 6); // USED
  ADD_STORAGE_VAL(TitleOrderNumber, 7); // USED
  ADD_STORAGE_VAL(TitleOrderRatio, 8); // USED
  ADD_STORAGE_VAL(TitleCover, 9); // USED
  ADD_STORAGE_VAL(TitleWeightCover, 10); // USED
  ADD_STORAGE_VAL(TitleWindow, 11); // USED
  ADD_STORAGE_VAL(TitleNeighbour, 12); // USED
  ADD_STORAGE_VAL(TitleBm25, 13); // USED
  ADD_STORAGE_VAL(TitleCosine, 14); // USED
  ADD_STORAGE_VAL(TitleLangWordJm, 15); // USED
  ADD_STORAGE_VAL(TitleLangWordDir, 16); // USED
  ADD_STORAGE_VAL(TitleLangWordAbs, 17); // USED
  ADD_STORAGE_VAL(TitleNoTopic, 18); // USED
  ADD_STORAGE_VAL(TitleAqKlDis, 19); // USED
  ADD_STORAGE_VAL(TitleQaKlDis, 20); // USED
  ADD_STORAGE_VAL(TitleSymDis, 21); // USED
  ADD_STORAGE_VAL(TitleCosDis, 22); // USED
  ADD_STORAGE_VAL(TitleBaDis, 23); // USED
  ADD_STORAGE_VAL(DescrLength, 24); // USED
  ADD_STORAGE_VAL(DescrWeightSum, 25); // USED
  ADD_STORAGE_VAL(DescrOrderNumber, 26); // USED
  ADD_STORAGE_VAL(DescrOrderRatio, 27); // USED
  ADD_STORAGE_VAL(DescrCover, 28); // USED
  ADD_STORAGE_VAL(DescrWeightCover, 29); // USED
  ADD_STORAGE_VAL(DescrWindow, 30); // USED
  ADD_STORAGE_VAL(DescrNeighbour, 31); // USED
  ADD_STORAGE_VAL(DescrBm25, 32); // USED
  ADD_STORAGE_VAL(DescrCosine, 33); // USED
  ADD_STORAGE_VAL(DescrLangWordJm, 34); // USED
  ADD_STORAGE_VAL(DescrLangWordDir, 35); // USED
  ADD_STORAGE_VAL(DescrLangWordAbs, 36); // USED
  ADD_STORAGE_VAL(DescrNoTopic, 37); // USED
  ADD_STORAGE_VAL(DescrAqKlDis, 38); // USED
  ADD_STORAGE_VAL(DescrQaKlDis, 39); // USED
  ADD_STORAGE_VAL(DescrSymDis, 40); // USED
  ADD_STORAGE_VAL(DescrCosDis, 41); // USED
  ADD_STORAGE_VAL(DescrBaDis, 42); // USED
  // sim coef use index, do not change upper
  ADD_STORAGE_VAL(QueryWeightMul, 43);
  ADD_STORAGE_VAL(TitleWeightMul, 44);
  ADD_STORAGE_VAL(DescrWeightMul, 45);
  ADD_STORAGE_VAL(SimilarityStage1, 46);
  ADD_STORAGE_VAL(SimilarityStage2, 47);
  ADD_STORAGE_VAL(SimilarityResult, 48);
  ADD_STORAGE_VAL(CtrEstimationResult, 49);
  ADD_STORAGE_VAL(OfflineSimResult, 50);
  ADD_STORAGE_VAL(OnlineSimResult, 51);
  ADD_STORAGE_VAL(SimilarityStage3, 52);
  ADD_STORAGE_VAL(FmContextScore, 53);
  ADD_STORAGE_VAL(RankCtrResult, 54);
  ADD_STORAGE_VAL(OfflineScore, 55); //magi offline ad score
  ADD_STORAGE_VAL(ContextScore, 56); //magi online context score
  ADD_STORAGE_VAL(SimilarityStage4, 57); 
  //2017-8-15 lihaixin@360.cn
  ADD_STORAGE_VAL(SoguidEC, 58);
  ADD_STORAGE_VAL(SoguidCOEC, 59);
  ADD_STORAGE_VAL(SoguidQueryEC, 60);
  ADD_STORAGE_VAL(SoguidQueryCOEC, 61);
  ADD_STORAGE_VAL(SoguidAdidEC, 62);
  ADD_STORAGE_VAL(SoguidAdidCOEC, 63);
  ADD_STORAGE_VAL(SoguidPlanidEC, 64);
  ADD_STORAGE_VAL(SoguidPlanidCOEC, 65);
  ADD_STORAGE_VAL(SoguidCustidEC, 66);
  ADD_STORAGE_VAL(SoguidCustidCOEC, 67);
  ADD_STORAGE_VAL(SoguidGroupidEC, 68);
  ADD_STORAGE_VAL(SoguidGroupidCOEC, 69);
  ADD_STORAGE_VAL(SoguidDomainEC, 70);
  ADD_STORAGE_VAL(SoguidDomainCOEC, 71);
  ADD_STORAGE_VAL(SoguidQueryDomainEC, 72);
  ADD_STORAGE_VAL(SoguidQueryDomainCOEC, 73);

  //2017-12-30 by wnf..
  ADD_STORAGE_VAL(MobCtrEstimationResult, 74);
  ADD_STORAGE_VAL(MobRankCtrResult, 75);
 
  //add for online simscore 
  ADD_STORAGE_VAL(BidwordLength, 76);
  ADD_STORAGE_VAL(UniqBidwordLength, 77);
  ADD_STORAGE_VAL(BidwordWeightSum, 78);
  ADD_STORAGE_VAL(UniqBidwordWeightSum, 79);
  ADD_STORAGE_VAL(BidwordWeightMul, 80);
  ADD_STORAGE_VAL(CvrEstimationResult, 81);
  ADD_STORAGE_VAL(DeepCvrEstimationResult, 82);
  ADD_STORAGE_VAL(OnlineStatScvr, 83);
  ADD_STORAGE_VAL(CtrPcLmStyleLrSharedWeight, 84);
  FINISH_STORAGE_VAL(85);


  // for uint32_t, unsigned
  BEGIN_STORAGE_VAL(uint32_t, GroupUint32);
  ADD_STORAGE_VAL(QueryTermNum, 0);
  ADD_STORAGE_VAL(BidwordTermNum, 1);
  ADD_STORAGE_VAL(TitleTermNum, 2);
  ADD_STORAGE_VAL(QueryTitleMatchNum, 3);
  ADD_STORAGE_VAL(QueryBidwordMatchNum, 4);
  ADD_STORAGE_VAL(TitleQueryMatchNum, 5);
  ADD_STORAGE_VAL(BidwordQueryMatchNum, 6);
  ADD_STORAGE_VAL(HourOfDay, 7);
  ADD_STORAGE_VAL(DayOfWeek, 8);
  ADD_STORAGE_VAL(TitleMinWildcardSize, 9);
  ADD_STORAGE_VAL(TitleMaxWildcardSize, 10);
  ADD_STORAGE_VAL(DescrMinWildcardSize, 11);
  ADD_STORAGE_VAL(DescrMaxWildcardSize, 12);
  ADD_STORAGE_VAL(AdUniqIdDeprecated, 13);
  ADD_STORAGE_VAL(AdGroupId, 14);
  ADD_STORAGE_VAL(AdPlanId, 15);
  ADD_STORAGE_VAL(AdAccountIdDeprecated, 16);
  ADD_STORAGE_VAL(AdMatchType, 17);
  ADD_STORAGE_VAL(AdExactMatch, 18);
  ADD_STORAGE_VAL(UserProvince, 19);
  ADD_STORAGE_VAL(UserCity, 20);
  ADD_STORAGE_VAL(StatSctrBin, 21);
  ADD_STORAGE_VAL(TitleMinWildcardTag, 22);
  ADD_STORAGE_VAL(TitleMaxWildcardTag, 23);
  ADD_STORAGE_VAL(DescrMinWildcardTag, 24);
  ADD_STORAGE_VAL(DescrMaxWildcardTag, 25);
  ADD_STORAGE_VAL(AdAccountTrade, 26);
  ADD_STORAGE_VAL(RankDebugMode, 27);
  ADD_STORAGE_VAL(RegionProvinceMatchMax,28);
  ADD_STORAGE_VAL(RegionCityMatchMax,29);
  ADD_STORAGE_VAL(ThreadIndex,30);
  ADD_STORAGE_VAL(IsNewAd,31);
  ADD_STORAGE_VAL(BrandQuery,32);
  ADD_STORAGE_VAL(IpRank,33);
  ADD_STORAGE_VAL(IsUnion,34);
  ADD_STORAGE_VAL(MobileOs, 35);
  ADD_STORAGE_VAL(NewSctrBin,36);
  ADD_STORAGE_VAL(ResultSize,37);
  ADD_STORAGE_VAL(MagiMatchNum,39);
  ADD_STORAGE_VAL(SoguidLevel,40);
  ADD_STORAGE_VAL(AdLevel,41);
  ADD_STORAGE_VAL(SrcType,42);
  ADD_STORAGE_VAL(RtUidClick,43);
  ADD_STORAGE_VAL(RtUidView,44); 
  ADD_STORAGE_VAL(RtUidCtr,45);
  ADD_STORAGE_VAL(RtIpClick,46);
  ADD_STORAGE_VAL(RtIpView,47); 
  ADD_STORAGE_VAL(RtIpCtr,48);
  ADD_STORAGE_VAL(DayMinutes,49);
  //2017-8-15 lihaixin@360.cn 计算好的EC&COEC bins
  ADD_STORAGE_VAL(SoguidEC, 50);
  ADD_STORAGE_VAL(SoguidCOEC, 51);
  ADD_STORAGE_VAL(SoguidQueryEC, 52);
  ADD_STORAGE_VAL(SoguidQueryCOEC, 53);
  ADD_STORAGE_VAL(SoguidAdidEC, 54);
  ADD_STORAGE_VAL(SoguidAdidCOEC, 55);
  ADD_STORAGE_VAL(SoguidPlanidEC, 56);
  ADD_STORAGE_VAL(SoguidPlanidCOEC, 57);
  ADD_STORAGE_VAL(SoguidCustidEC, 58);
  ADD_STORAGE_VAL(SoguidCustidCOEC, 59);
  ADD_STORAGE_VAL(SoguidGroupidEC, 60);
  ADD_STORAGE_VAL(SoguidGroupidCOEC, 61);
  ADD_STORAGE_VAL(SoguidDomainEC, 62);
  ADD_STORAGE_VAL(SoguidDomainCOEC, 63);
  ADD_STORAGE_VAL(SoguidQueryDomainEC, 64);
  ADD_STORAGE_VAL(SoguidQueryDomainCOEC, 65);
  ADD_STORAGE_VAL(SrcgType,66);
  ADD_STORAGE_VAL(Platform,67);
  ADD_STORAGE_VAL(SubMatchType,68);
  ADD_STORAGE_VAL(IsPpadQuery,69);
  ADD_STORAGE_VAL(HasBiyiImg,70);
  ADD_STORAGE_VAL(CatId,71);
  ADD_STORAGE_VAL(QueryEc, 72);
  ADD_STORAGE_VAL(QueryCoec, 73);
  ADD_STORAGE_VAL(QueryBidwordEc, 74);
  ADD_STORAGE_VAL(QueryBidwordCoec, 75);
  ADD_STORAGE_VAL(QueryCustidEc, 76);
  ADD_STORAGE_VAL(QueryCustidCoec, 77);
  ADD_STORAGE_VAL(QueryPlanidEc, 78);
  ADD_STORAGE_VAL(QueryPlanidCoec, 79);
  ADD_STORAGE_VAL(QueryGroupidEc, 80);
  ADD_STORAGE_VAL(QueryGroupidCoec, 81);
  ADD_STORAGE_VAL(QueryAdidEc, 82);
  ADD_STORAGE_VAL(QueryAdidCoec, 83);
  ADD_STORAGE_VAL(QueryDomainEc, 84);
  ADD_STORAGE_VAL(QueryDomainCoec, 85);
  ADD_STORAGE_VAL(BidwordEc, 86);
  ADD_STORAGE_VAL(BidwordCoec, 87);
  ADD_STORAGE_VAL(DomainEc, 88);
  ADD_STORAGE_VAL(DomainCoec, 89);
  ADD_STORAGE_VAL(CustidEc, 90);
  ADD_STORAGE_VAL(CustidCoec, 91);
  ADD_STORAGE_VAL(PlanidEc, 92);
  ADD_STORAGE_VAL(PlanidCoec, 93);
  ADD_STORAGE_VAL(GroupidEc, 94);
  ADD_STORAGE_VAL(GroupidCoec, 95);
  ADD_STORAGE_VAL(AdidEc, 96);
  ADD_STORAGE_VAL(AdidCoec, 97);
  ADD_STORAGE_VAL(MsoX, 98);
  ADD_STORAGE_VAL(MsoY, 99);
  ADD_STORAGE_VAL(ChannelId, 100);
  ADD_STORAGE_VAL(AdQueryTermNum, 101);
  ADD_STORAGE_VAL(AdQueryTermMatchQueryNum, 102);
  ADD_STORAGE_VAL(AsQueryTermNum, 103);
  ADD_STORAGE_VAL(AsQueryTermMatchQueryNum, 104);
  ADD_STORAGE_VAL(HisQueryTermNum, 105);
  ADD_STORAGE_VAL(HisQueryTermMatchBidwordNum, 106);
  ADD_STORAGE_VAL(RankPrintFeatureMode, 107);
  ADD_STORAGE_VAL(LastClickQueryTermNum, 108);
  ADD_STORAGE_VAL(LastClickQueryTermMatchQueryNum, 109);
  ADD_STORAGE_VAL(TopClickQueryTermNum, 110);
  ADD_STORAGE_VAL(TopClickQueryTermMatchBidwordNum, 111);
  ADD_STORAGE_VAL(LastClickQueryTermMatchTitleNum, 112);
  ADD_STORAGE_VAL(LongTermQueryCoec, 113);
  ADD_STORAGE_VAL(LongTermQueryBidwordCoec, 114);
  ADD_STORAGE_VAL(LongTermQueryCustidCoec, 115);
  ADD_STORAGE_VAL(LongTermQuerySrcCoec, 116);
  ADD_STORAGE_VAL(LongTermBidwordCoec, 117);
  ADD_STORAGE_VAL(IsOcpcBid, 118);
  ADD_STORAGE_VAL(ScvrBin, 119);
  ADD_STORAGE_VAL(WdHostMatched, 120);
  ADD_STORAGE_VAL(SearchUnionTag, 121);
  ADD_STORAGE_VAL(IsDeep, 122);
  ADD_STORAGE_VAL(IsUsePcLmStyleModel, 123);

  FINISH_STORAGE_VAL(124);

  // for uint64_t, unsigned long
  BEGIN_STORAGE_VAL(uint64_t, GroupUint64);
  ADD_STORAGE_VAL(BidwordTermSum, 0);
  ADD_STORAGE_VAL(TitleTermSum, 1);
  ADD_STORAGE_VAL(AdAccountId, 2);
  ADD_STORAGE_VAL(AdUniqId, 3);
  ADD_STORAGE_VAL(AdOcpcPackageId, 4);
  FINISH_STORAGE_VAL(5);

  ////////////////////////////////////////
  ////////// ARRAY GROUP DEFINE //////////
  ////////////////////////////////////////

  BEGIN_STORAGE_ARR(float, GroupCoefs);
  ADD_STORAGE_ARR(SimilarityCoefs, 0);
  ADD_STORAGE_ARR(SimilarityFeatures, 1);
  ADD_STORAGE_ARR(GbdtFeatures, 2);
  MIDDLE_STORAGE_ARR;
  LOAD_STORAGE_ARR(SimilarityCoefs, Store_T<float>::arr_max);
  LOAD_STORAGE_ARR(SimilarityFeatures, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(GbdtFeatures, FLAGS_IC_buffer_size_large);
  FINISH_STORAGE_ARR(3);

  // uint32_t/int array type
  BEGIN_STORAGE_ARR(uint32_t, GroupTags);
  ADD_STORAGE_ARR(QueryTermArr, 0);
  ADD_STORAGE_ARR(QueryTermTop, 1);
  ADD_STORAGE_ARR(BidwordTermArr, 2);
  ADD_STORAGE_ARR(BidwordTermTop, 3);
  ADD_STORAGE_ARR(TitleTermArr, 4);
  ADD_STORAGE_ARR(TitleTermTop, 5);
  ADD_STORAGE_ARR(BidwordMatchArr, 6);
  ADD_STORAGE_ARR(BidwordMatchTag, 7);
  ADD_STORAGE_ARR(QueryBidwordMatchTop, 8);
  ADD_STORAGE_ARR(BidwordQueryMatchTop, 9);
  ADD_STORAGE_ARR(QueryBidwordUnmatchArr, 10);
  ADD_STORAGE_ARR(QueryBidwordUnmatchTop, 11);
  ADD_STORAGE_ARR(BidwordQueryUnmatchArr, 12);
  ADD_STORAGE_ARR(BidwordQueryUnmatchTop, 13);
  ADD_STORAGE_ARR(TitleMatchArr, 14);
  ADD_STORAGE_ARR(TitleMatchTag, 15);
  ADD_STORAGE_ARR(QueryTitleMatchTop, 16);
  ADD_STORAGE_ARR(TitleQueryMatchTop, 17);
  ADD_STORAGE_ARR(QueryTitleUnmatchArr, 18);
  ADD_STORAGE_ARR(QueryTitleUnmatchTop, 19);
  ADD_STORAGE_ARR(TitleQueryUnmatchArr, 20);
  ADD_STORAGE_ARR(TitleQueryUnmatchTop, 21);
  ADD_STORAGE_ARR(DescrMatchArr, 22);
  ADD_STORAGE_ARR(DescrMatchTag, 23);
  ADD_STORAGE_ARR(MinSegQuery, 24);
  ADD_STORAGE_ARR(MaxSegQuery, 25);
  ADD_STORAGE_ARR(MinSegBidword, 26);
  ADD_STORAGE_ARR(MaxSegBidword, 27);
  ADD_STORAGE_ARR(ProcessedMinSegTitle, 28);
  ADD_STORAGE_ARR(ProcessedMaxSegTitle, 29);
  ADD_STORAGE_ARR(ProcessedMinSegDescr, 30);
  ADD_STORAGE_ARR(ProcessedMaxSegDescr, 31);
  ADD_STORAGE_ARR(OriginalMinSegTitle, 32);
  ADD_STORAGE_ARR(OriginalMaxSegTitle, 33);
  ADD_STORAGE_ARR(OriginalMinSegDescr, 34);
  ADD_STORAGE_ARR(OriginalMaxSegDescr, 35);
  ADD_STORAGE_ARR(DescTermTop, 36);
  ADD_STORAGE_ARR(AdQueryArr, 37);
  ADD_STORAGE_ARR(AdQueryMatchQueryArr, 38);
  ADD_STORAGE_ARR(AdQueryMatchTag, 39);
  ADD_STORAGE_ARR(AsQueryArr, 40);
  ADD_STORAGE_ARR(AsQueryMatchQueryArr, 41);
  ADD_STORAGE_ARR(AsQueryMatchTag, 42);
  ADD_STORAGE_ARR(HisQueryArr, 43);
  ADD_STORAGE_ARR(HisQueryMatchBidwordArr, 44);
  ADD_STORAGE_ARR(HisQueryMatchTag, 45);
  ADD_STORAGE_ARR(LastClickQueryArr, 46);
  ADD_STORAGE_ARR(LastClickQueryMatchQueryArr, 47);
  ADD_STORAGE_ARR(LastClickQueryMatchTag, 48);
  ADD_STORAGE_ARR(TopClickQueryArr, 49);
  ADD_STORAGE_ARR(TopClickQueryMatchBidwordArr, 50);
  ADD_STORAGE_ARR(TopClickQueryMatchTag, 51);
  ADD_STORAGE_ARR(LastClickQueryMatchTitleArr, 52);
  ADD_STORAGE_ARR(LastClickQueryMatchTitleTag, 53);
  ADD_STORAGE_ARR(WdHostForMatch, 54);
  MIDDLE_STORAGE_ARR;
  LOAD_STORAGE_ARR(QueryTermArr, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(QueryTermTop, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(BidwordTermArr, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(BidwordTermTop, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(TitleTermArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(TitleTermTop, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(BidwordMatchArr, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(BidwordMatchTag, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(QueryBidwordMatchTop, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(BidwordQueryMatchTop, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(QueryBidwordUnmatchArr, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(QueryBidwordUnmatchTop, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(BidwordQueryUnmatchArr, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(BidwordQueryUnmatchTop, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(TitleMatchArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(TitleMatchTag, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(QueryTitleMatchTop, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(TitleQueryMatchTop, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(QueryTitleUnmatchArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(QueryTitleUnmatchTop, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(TitleQueryUnmatchArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(TitleQueryUnmatchTop, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(DescrMatchArr, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(DescrMatchTag, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(MinSegQuery, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(MaxSegQuery, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(MinSegBidword, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(MaxSegBidword, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(ProcessedMinSegTitle, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(ProcessedMaxSegTitle, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(ProcessedMinSegDescr, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(ProcessedMaxSegDescr, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(OriginalMinSegTitle, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(OriginalMaxSegTitle, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(OriginalMinSegDescr, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(OriginalMaxSegDescr, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(DescTermTop, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(AdQueryArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(AdQueryMatchQueryArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(AdQueryMatchTag, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(AsQueryArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(AsQueryMatchQueryArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(AsQueryMatchTag, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(HisQueryArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(HisQueryMatchBidwordArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(HisQueryMatchTag, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(LastClickQueryArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(LastClickQueryMatchQueryArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(LastClickQueryMatchTag, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(TopClickQueryArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(TopClickQueryMatchBidwordArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(TopClickQueryMatchTag, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(LastClickQueryMatchTitleArr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(LastClickQueryMatchTitleTag, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(WdHostForMatch, FLAGS_IC_buffer_size_huge);
  FINISH_STORAGE_ARR(55);

  // for pchar_t and string
  BEGIN_STORAGE_ARR(char, GroupChars);
  ADD_STORAGE_ARR(AdPos, 0);
  ADD_STORAGE_ARR(AdRank, 1);
  ADD_STORAGE_ARR(AdContext, 2);
  ADD_STORAGE_ARR(BidwordText, 3);
  ADD_STORAGE_ARR(QueryText, 4);
  ADD_STORAGE_ARR(PvSource, 5);
  ADD_STORAGE_ARR(LsSource, 6);
  ADD_STORAGE_ARR(SegTitle360,7);
  ADD_STORAGE_ARR(SegQuery360,8);
  ADD_STORAGE_ARR(QueryTopic360,9);
  ADD_STORAGE_ARR(OrigQueryString,10);
  ADD_STORAGE_ARR(SoGuid, 11);
  ADD_STORAGE_ARR(Os, 12);
  ADD_STORAGE_ARR(Browser, 13);
  ADD_STORAGE_ARR(FmAdContext, 14);
  ADD_STORAGE_ARR(SearchIp, 15);
  ADD_STORAGE_ARR(MobileSid, 16);
  ADD_STORAGE_ARR(Pvid, 17);
  ADD_STORAGE_ARR(SearchIp0, 18);
  ADD_STORAGE_ARR(SearchIp1, 19);
  ADD_STORAGE_ARR(SearchIp2, 20);
  ADD_STORAGE_ARR(Sourceg, 21)
  ADD_STORAGE_ARR(ShowUrl, 22)
  ADD_STORAGE_ARR(Domain, 23)
  ADD_STORAGE_ARR(Imei, 24)
  ADD_STORAGE_ARR(NetStatus, 25)
  ADD_STORAGE_ARR(NetType, 26)
  ADD_STORAGE_VAL(Width, 27);
  ADD_STORAGE_VAL(Dpr, 28);
  ADD_STORAGE_VAL(Ua, 29);
  ADD_STORAGE_VAL(Brand, 30);
  ADD_STORAGE_VAL(DeviceFamily, 31);
  ADD_STORAGE_VAL(UaFamily, 32);
  ADD_STORAGE_VAL(OsVersion, 33);
  ADD_STORAGE_VAL(UaVersion, 34);
  ADD_STORAGE_VAL(Pre5QueryChar, 35);
  ADD_STORAGE_VAL(Pre1Query, 36);
  ADD_STORAGE_VAL(Pre2Query, 37);
  ADD_STORAGE_VAL(Pre1Bidword, 38);
  ADD_STORAGE_VAL(History10Query, 39);
  ADD_STORAGE_VAL(CurAdClicked, 40);
  ADD_STORAGE_VAL(CurAdHistory, 41);
  ADD_STORAGE_VAL(AdShowClkNum, 42);
  ADD_STORAGE_VAL(AdPosClk, 43);
  ADD_STORAGE_VAL(Pre1SearchGap, 44);
  ADD_STORAGE_VAL(Pre1AdClkSpan, 45);
  ADD_STORAGE_VAL(Pre1AdClkGap, 46);
  ADD_STORAGE_VAL(Pre1AdClkId, 47);
  ADD_STORAGE_VAL(Pre1BidwordHasClk, 48);
  ADD_STORAGE_VAL(ClkAdStyleInfoHistory, 49);
  ADD_STORAGE_VAL(CurAdStyleHistory, 50);
  ADD_STORAGE_VAL(CurAdComponentHistory, 51);
  ADD_STORAGE_VAL(OcpcConvertType, 52);
  ADD_STORAGE_ARR(PvSourceCvr, 53);
  ADD_STORAGE_ARR(SourcegCvr, 54)
  ADD_STORAGE_ARR(LsSourceCvr, 55);
  ADD_STORAGE_ARR(SrcLsCvr, 56);
  ADD_STORAGE_ARR(ChannelIdCvr, 57);
  ADD_STORAGE_ARR(AdIndustryId, 58);
  ADD_STORAGE_ARR(AdOcpcPackageIdStr, 59);
  ADD_STORAGE_ARR(Guid, 60);
  ADD_STORAGE_ARR(LastClickBidword, 61);
  ADD_STORAGE_ARR(Pre1Adid, 62);
  ADD_STORAGE_ARR(Pre1AdidHasClk, 63);
  ADD_STORAGE_ARR(LastClickQuery, 64);
  ADD_STORAGE_ARR(AdAccountCompanyName, 65);
  ADD_STORAGE_ARR(WdHost, 66);
  ADD_STORAGE_ARR(AdIndustryFid, 67);
  ADD_STORAGE_VAL(OcpcDeepConvertType, 68);
  
  ADD_STORAGE_VAL(UserOaid, 69);
  ADD_STORAGE_VAL(UserSex, 70);
  ADD_STORAGE_VAL(UserAge, 71);
  ADD_STORAGE_VAL(UserProvince, 72);
  ADD_STORAGE_VAL(UserCity, 73);
  ADD_STORAGE_VAL(UserCityLevel, 74);
  ADD_STORAGE_VAL(UserPhoneBrand, 75);
  ADD_STORAGE_VAL(UserPhoneType, 76);
  ADD_STORAGE_VAL(UserPhonePrice, 77);
  ADD_STORAGE_VAL(UserPhonePublishTime, 78);
  ADD_STORAGE_ARR(UserInstallApp, 79);
  ADD_STORAGE_ARR(UserInstallAppType, 80);

  ADD_STORAGE_ARR(ClickCustId, 81);
  ADD_STORAGE_ARR(ClickOcpcId, 82);
  ADD_STORAGE_ARR(ClickChannel, 83);
  ADD_STORAGE_ARR(OrderCustId, 84);
  ADD_STORAGE_ARR(OrderOcpcId, 85);
  ADD_STORAGE_ARR(OrderChannel, 86);
  ADD_STORAGE_ARR(Day60Click, 87);
  ADD_STORAGE_ARR(Day60Order, 88);
  ADD_STORAGE_ARR(Day30Click, 89);
  ADD_STORAGE_ARR(Day30Order, 90);
  ADD_STORAGE_ARR(Day7Click, 91);
  ADD_STORAGE_ARR(Day7Order, 92);
  ADD_STORAGE_ARR(Day1Click, 93);
  ADD_STORAGE_ARR(Day1Order, 94);

  MIDDLE_STORAGE_ARR;
  LOAD_STORAGE_ARR(AdPos, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(AdRank, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(AdContext, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(BidwordText, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(QueryText, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(PvSource, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(LsSource, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(SegTitle360,FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(SegQuery360,FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(QueryTopic360,FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(OrigQueryString, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(SoGuid, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(Os,FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(Browser, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(FmAdContext, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(SearchIp, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(MobileSid, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(Pvid, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(SearchIp0, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(SearchIp1, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(SearchIp2, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Sourceg, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(ShowUrl, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(Domain, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Imei, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(NetStatus, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(NetType, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Width, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Dpr, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Ua, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(Brand, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(DeviceFamily, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(UaFamily, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(OsVersion, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(UaVersion, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(Pre5QueryChar, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(Pre1Query, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(Pre2Query, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(Pre1Bidword, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(History10Query, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(CurAdClicked, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(CurAdHistory, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(AdShowClkNum, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(AdPosClk, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(Pre1SearchGap, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Pre1AdClkSpan, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Pre1AdClkGap, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Pre1AdClkId, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Pre1BidwordHasClk, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(ClkAdStyleInfoHistory, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(CurAdStyleHistory, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(CurAdComponentHistory, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(OcpcConvertType, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(PvSourceCvr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(SourcegCvr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(LsSourceCvr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(SrcLsCvr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(ChannelIdCvr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(AdIndustryId, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(AdOcpcPackageIdStr, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(Guid, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(LastClickBidword, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(Pre1Adid, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(Pre1AdidHasClk, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(LastClickQuery, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(AdAccountCompanyName, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(WdHost, FLAGS_IC_buffer_size_huge);
  LOAD_STORAGE_ARR(AdIndustryFid, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(OcpcDeepConvertType, FLAGS_IC_buffer_size_large);

  LOAD_STORAGE_ARR(UserOaid, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(UserSex, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(UserAge, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(UserProvince, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(UserCity, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(UserCityLevel, FLAGS_IC_buffer_size_less);
  LOAD_STORAGE_ARR(UserPhoneBrand, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(UserPhoneType, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(UserPhonePrice, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(UserPhonePublishTime, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(UserInstallApp, FLAGS_IC_buffer_size_huge);
  LOAD_STORAGE_ARR(UserInstallAppType, FLAGS_IC_buffer_size_huge);

  LOAD_STORAGE_ARR(ClickCustId, FLAGS_IC_buffer_size_large);
  LOAD_STORAGE_ARR(ClickOcpcId, FLAGS_IC_buffer_size_more);
  LOAD_STORAGE_ARR(ClickChannel, FLAGS_IC_buffer_size_more);
  LOAD_STORAGE_ARR(OrderCustId, FLAGS_IC_buffer_size_more);
  LOAD_STORAGE_ARR(OrderOcpcId, FLAGS_IC_buffer_size_more);
  LOAD_STORAGE_ARR(OrderChannel, FLAGS_IC_buffer_size_normal);
  LOAD_STORAGE_ARR(Day60Click, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Day60Order, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Day30Click, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Day30Order, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Day7Click, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Day7Order, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Day1Click, FLAGS_IC_buffer_size_little);
  LOAD_STORAGE_ARR(Day1Order, FLAGS_IC_buffer_size_little);

  FINISH_STORAGE_ARR(95);

  //////////////////////////////////////////

  class StoreImpl
  {
    public:
      StoreImpl() { ; }
      ~StoreImpl() { ; }

      // For Simple case
      template <typename Type_t>
      inline const Store_T<Type_t>* GetStorageT() const {
        std::cerr << __func__ << ": " << typeid(Type_t).name() << std::endl;
        raise(SIGINT);
        return NULL;
      }

      template <typename Type_t>
      inline Type_t* GetStoragePtrT() {
        std::cerr << __func__ << ": " << typeid(Type_t).name() << std::endl;
        raise(SIGINT);
        return NULL;
      }

      template <typename Type_t>
      inline void ClearStorageT() {
        std::cerr << __func__ << ": " << typeid(Type_t).name() << std::endl;
        raise(SIGINT);
      }

      // For Array Case
      template <typename Type_t>
      inline const Store_A<Type_t>* GetStorageA() const {
        std::cerr << __func__ << ": " << typeid(Type_t).name() << std::endl;
        raise(SIGINT);
        return NULL;
      }

      template <typename Type_t>
      inline Type_t* GetStoragePtrA(Int_ctref_t fid) {
        std::cerr << __func__ << ": " << typeid(Type_t).name() << std::endl;
        raise(SIGINT);
        return NULL;
      }

      template <typename Type_t>
      inline void ClearStorageA() {
        std::cerr << __func__ << ": " << typeid(Type_t).name() << std::endl;
        raise(SIGINT);
      }

      template <typename Type_t>
      inline bool SetStorageSizeA(Int_ctref_t fid, Int_ctref_t size) {
        std::cerr << __func__ << ": " << typeid(Type_t).name() << std::endl;
        raise(SIGINT);
        return false;
      }

      template <typename Type_t>
      inline Int_const_t GetStorageSizeA(Int_ctref_t fid) const {
        std::cerr << __func__ << ": " << typeid(Type_t).name() << std::endl;
        raise(SIGINT);
        return 0;
      }

      template <typename Type_t>
      inline Int_const_t GetStorageCapA(Int_ctref_t fid) const {
        std::cerr << __func__ << ": " << typeid(Type_t).name() << std::endl;
        raise(SIGINT);
        return 0;
      }

#if _DEBUG
    public:
#else
    private:
#endif
      DEFINE_STORAGE_VAL(float);
      DEFINE_STORAGE_VAL(uint32_t);
      DEFINE_STORAGE_VAL(uint64_t);
      DEFINE_STORAGE_ARR(float);
      DEFINE_STORAGE_ARR(uint32_t);
      DEFINE_STORAGE_ARR(char);
  };

  IMPLEMENT_STORAGE_VAL(float);
  IMPLEMENT_STORAGE_VAL(uint32_t);
  IMPLEMENT_STORAGE_VAL(uint64_t);
  IMPLEMENT_STORAGE_ARR(float);
  IMPLEMENT_STORAGE_ARR(uint32_t);
  IMPLEMENT_STORAGE_ARR(char);

} } }

#endif

