#include <gflags/gflags.h>

#include "feature/twoval_combiner_v2.h"

using namespace std;

namespace qihoo { namespace ad { namespace ranking {

FeatureCenter::FeatureCenter() {
    Init();
}

FeatureCenter::~FeatureCenter() {
    for (size_t i=0; i<_features_base.size(); ++i) {
        delete _features_base[i];
    }
}

void FeatureCenter::Init() {
    _features_base.clear();
    _features_base.push_back(new TwoValCombinerV2);
}

ExtractorBase* FeatureCenter::LoadFeature(const std::string& name,
        const std::string& params, InfoCenter& ic) {
    ExtractorBase* base = NULL;
    for (size_t j=0; j<_features_base.size(); ++j) {
        if (!_features_base[j]->Name().compare(name)) {
            base = _features_base[j]->Clone();
            if (base) {
                if (!base->DoInit(params, ic)) {
                    DELETE_VAR_AND_SET_NULL(base);
                }
            }
            break;
        }
    }
    return base;
}

} } }


