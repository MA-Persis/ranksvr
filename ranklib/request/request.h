#ifndef QIHOO_AD_RANKING_REQUEST_H_
#define QIHOO_AD_RANKING_REQUEST_H_

#include <iostream>
#include <algorithm>
#include <string>

#include <gflags/gflags.h>

#include "base/def.h"
#include "pb/echo.pb.h"
#include "pb/userbehavior.pb.h"
#include "pb/orderfeature.pb.h"

namespace qihoo { namespace ad { namespace ranking {

struct iResult {
    iResult() {}
};

typedef ::orderfeature::OrderInfoFeature OrderFeatureKv;

class iVisit {
public:
    iVisit(): _pvi(NULL) {
    }
    bool SetInstance(example::VisitInfo* pvi) {
        if (!pvi) return false;
        _pvi = pvi;
        return true;
    }
    uint32_t Province() {
        if (!_pvi)  return 0;
        return _pvi->province();
    }

    uint32_t City() {
        if (!_pvi) return 0;
        return _pvi->city();
    }

    const std::string& Pvid(){
        if (!_pvi) {
            const static std::string nullstr("");
            return nullstr;
        }
        return _pvi->pvid();
    }

    std::string OaidFeatures(){
        if (!_pvi) {
            std::string nullstr("");
            return nullstr;
        }
        return _pvi->oaid_fea().oaid_fea();
    }

    const std::string Oaid(){
        if (!_pvi) {
            const static std::string nullstr("");
            return nullstr;
        }
        return _pvi->oaid();
    }

    OrderFeatureKv* OrderFeatures() {
        if (!_pvi) return NULL;

        if (_pvi->has_order_feature())
            return _pvi->mutable_order_feature();
        else
            return NULL;
    }

private:
    example::VisitInfo* _pvi;
};

class iQuery {
public:
    iQuery(): _pqi(NULL) {
    }
    bool SetInstance(example::QueryInfo* pqi) {
        if (!pqi) return false;
        _pqi = pqi;
        return true;
    }
    example::QueryInfo* GetInstance() {
        return _pqi;
    }
    const std::string& QueryString() {
        if (!_pqi) {
            const static std::string nullstr("");
            return nullstr;
        }
        return _pqi->query();
    }

private:
    example::QueryInfo* _pqi;
};

class iAdpack {
public:
    iAdpack(): _pap(NULL) {
    }
    bool SetInstance(example::AdInfo4Rank* pap) {
        if (!pap) return false;
        _pap = pap;
        return true;
    }
    example::AdInfo4Rank* GetInstance() {
        return _pap;
    }

    inline uint64_t AdId() {
        if (!_pap) return 0;
        return _pap->adid();
    }

private:
    example::AdInfo4Rank* _pap;
};

template <typename VisitType,
         typename QueryType,
         typename AdpackType>
class iSearchImpl {
public:
    VisitType visit;
    QueryType query;
    AdpackType adpack;

public:
    iSearchImpl(): _prs(NULL), _qidx(-1), _aidx(-1) {
    }
    bool SetInstance(example::RankStruct* prs) {
        if (!prs) return false;
        _prs = prs; _qidx = -1; _aidx = -1;
        visit.SetInstance(_prs->mutable_visitinfo());
        return true;
    }
    inline bool SetQueryIndex(int qidx) {
        /*if (qidx<0 || qidx >= QuerySize()) {
        return false; }*/ // 外部保证
        _qidx = qidx;
        query.SetInstance(
            _prs->mutable_querylist(_qidx));
        return true;
    }
    inline bool SetAdpackIndex(int aidx) {
        /*if (_qidx<0 || _qidx >= QuerySize()
        || aidx<0 || aidx >= AdvertSize()) {
        return false; }*/ // 外部保证
        _aidx = aidx;
        adpack.SetInstance(
            _prs->mutable_querylist(
            _qidx)->mutable_adlist(_aidx));
        return true;
    }
    const int AdvertSize() {
        if (!_prs) return 0;
        return _prs->querylist(_qidx).adlist_size();
    }
    const int QuerySize() {
        if (!_prs) return 0;
        return _prs->querylist_size();
    }

private:
    example::RankStruct* _prs;
    int _qidx;
    int _aidx;
};

typedef iSearchImpl<iVisit, iQuery, iAdpack> iSearch;

// basic class
template <typename VisitType = iVisit,
         typename QueryType = iQuery,
         typename AdpackType = iAdpack>
struct iRequestImpl {
    iSearchImpl<VisitType, QueryType, AdpackType> search;
};

typedef iRequestImpl<> iRequest;

} } }

#endif