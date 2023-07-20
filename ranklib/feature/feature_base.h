#ifndef QIHOO_AD_RANKING_FEATURE_BASE_H_
#define QIHOO_AD_RANKING_FEATURE_BASE_H_

#include "base/def.h"
#include "feature/feature_center.h"
#include "store/info_center.h"

namespace qihoo { namespace ad { namespace ranking {

typedef enum {
    FT_UNKNOWN = -1,
    FT_PROVIDER,
    FT_EXTRACTOR,
    FT_COMBINER,
    FT_ALL_SIZE,
} Feature_t;

typedef enum {
    STAGE_UNKNOWN = -1,
    STAGE_LEFT,
    STAGE_RIGHT,
    STAGE_CROSS, //默认是cross
    STAGE_FS, //用于offline提取topic特征阶段
    STAGE_ADMERGE, //用于admerge调OnlineRankPrepare阶段
    STAGE_SIZE,
} Stage_t;

class ExtractorBase
{
public:
    ExtractorBase() : _typenum(FT_UNKNOWN), _namestr("") {}
    virtual ~ExtractorBase() {}

    virtual bool Init(const std::string& param, InfoCenter& ic) = 0;
    bool DoInit(const std::string& param, InfoCenter& ic);

    virtual ExtractorBase* Clone() const = 0;

    Feature_t Type() const;
    const std::string& Name() const;

protected:
    Feature_t _typenum;
    std::string _namestr;
};

} } }

#endif