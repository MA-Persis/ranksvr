#include "feature/feature_funcs.h"
#include "util/StrNormalize.h"
#include <float.h>

namespace qihoo { namespace ad { namespace ranking { 

  void normalize_topic_info(
      std::tr1::unordered_map<uint32_t, float>& pzd)
  {
    float sum = 0.0;
    for (std::tr1::unordered_map<uint32_t, float>::iterator 
        iter = pzd.begin(); iter != pzd.end(); ++iter) {
      sum += iter->second;
    }

    if (FLT_MIN > sum) { return; }
    for (std::tr1::unordered_map<uint32_t, float>::iterator 
        iter = pzd.begin(); iter != pzd.end(); ++iter) {
      iter->second /= sum;
    }
  }

  void merge_topic_info(
      std::pair<uint32_t, float> * pzd_key, uint32_t key_size,
      std::pair<uint32_t, float> * pzd_ad, uint32_t ad_size, 
      std::tr1::unordered_map<uint32_t, float>& pzd) 
  {
    pzd.clear();
    //std::map<uint32_t, float> tmp_pzd;
    std::tr1::unordered_map<uint32_t, float> tmp_pzd;
    float sum = 0.0;
    for (uint32_t i = 0; i < key_size; ++i) {
      tmp_pzd[pzd_key[i].first] += pzd_key[i].second;
      sum += pzd_key[i].second;
    }
    for (uint32_t i = 0; i < ad_size; ++i) {
      tmp_pzd[pzd_ad[i].first] += pzd_ad[i].second;
      sum += pzd_ad[i].second;
    }

    sum *= 0.01;

    for (std::tr1::unordered_map<uint32_t, float>::iterator 
        iter = tmp_pzd.begin(); iter != tmp_pzd.end(); ++iter) {
      if (iter->second > sum) {
        pzd[iter->first] = iter->second;
      }
    }

    normalize_topic_info(pzd);
  }
  
  void splitString(const std::string &text,std::vector<std::string> &parts, const std::string &delimiter)
  {
        parts.clear();
        size_t delimiterPos = text.find(delimiter);
        size_t lastPos = 0;
        if (delimiterPos == std::string::npos)
        {
                parts.push_back(text);
                return;
        }

        while(delimiterPos != std::string::npos)
        {
                parts.push_back(text.substr(lastPos, delimiterPos - lastPos));
                lastPos = delimiterPos + delimiter.size();
                delimiterPos = text.find(delimiter, lastPos);
        }
        parts.push_back(text.substr(lastPos));
  }

} } }


