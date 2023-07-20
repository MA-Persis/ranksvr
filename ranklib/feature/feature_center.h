#ifndef _QIHOO_AD_RANKING_FEATURE_CENTER_H_
#define _QIHOO_AD_RANKING_FEATURE_CENTER_H_

#include <vector>

#include "feature/feature_base.h"

namespace qihoo { namespace ad { namespace ranking {

class ExtractorBase;
class InfoCenter;

class FeatureCenter
{
public:
    static FeatureCenter& Instance() {
        static FeatureCenter fa;
        return fa;
    }

    void Init();
    
    ExtractorBase* LoadFeature(const std::string& name, 
            const std::string& params, InfoCenter& ic);
private: 
    std::vector<ExtractorBase*> _features_base;

    FeatureCenter();
    FeatureCenter(FeatureCenter const& fc);
    FeatureCenter& operator=(FeatureCenter const& fc);
    ~FeatureCenter();
};


} } }

#endif