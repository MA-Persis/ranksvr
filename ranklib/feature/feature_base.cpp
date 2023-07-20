#include "feature/feature_base.h"

namespace qihoo { namespace ad { namespace ranking {

bool ExtractorBase::DoInit(const std::string& param, 
                           InfoCenter& ic) {
    return Init(param, ic);
}

Feature_t ExtractorBase::Type() const {
    return _typenum;
}

const std::string& ExtractorBase::Name() const {
    return _namestr;
}

} } }