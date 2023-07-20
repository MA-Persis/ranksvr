#ifndef QIHOO_AD_RANKING_TWOVAL_COMBINER_V2_H_
#define QIHOO_AD_RANKING_TWOVAL_COMBINER_V2_H_

#include "base/def.h"
#include "feature/feature_center.h"
#include "feature/feature_base.h"
#include "store/info_center.h"

namespace qihoo { namespace ad { namespace ranking {

class ExtractorBase;
class InfoCenter;

class TwoValCombinerV2: public ExtractorBase
{
public:
    TwoValCombinerV2();
    virtual ~TwoValCombinerV2();

    bool Init(const std::string& params, InfoCenter& ic);

    bool ExtractDNN(InfoCenter& ic);

    ExtractorBase* Clone() const;

protected:

    int _GetKey(InfoCenter& ic, const std::string& type,
            char* out, int len, int index);

private:
    int _left; //json conf 里配的左边特征在 ic 的位置
    int _right;//json conf 里配的右边特征在 ic 的位置
    std::string _prefix;//feature 的前缀
    std::string _seprator;//left与right之间的分隔符，如果是\4，json conf中就配置 \u0004
    std::string _left_type;
    std::string _right_type;
    std::string _tag; //feature的cvr标记

    int _slot_id;
};

} } } // namespace qihoo ad ranking

#endif // !QIHOO_AD_RANKING_TWOVAL_COMBINER_V2_H_
