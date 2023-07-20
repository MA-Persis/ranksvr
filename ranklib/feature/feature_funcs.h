#ifndef QIHOO_AD_RANKING_FEATURE_FUNCS_H_
#define QIHOO_AD_RANKING_FEATURE_FUNCS_H_
#include "request/request.h"

#include <string.h>
#include <map>
#include <tr1/unordered_map>
#include <string>
#include <iostream>
#include <algorithm>

#define CHAR_BUFF_SIZE 128
#define CHAR_BUFF_SIZE_LARGE 1024

namespace qihoo { namespace ad { namespace ranking {

void normalize_topic_info(
        std::tr1::unordered_map<uint32_t, float>& pzd);

void merge_topic_info(
        std::pair<uint32_t, float>* pzd_key, uint32_t key_size,
        std::pair<uint32_t, float>* pzd_ad, uint32_t ad_size,
        std::tr1::unordered_map<uint32_t, float>& pzd);

void splitString(const std::string &text,std::vector<std::string> &parts, const std::string &delimiter);

template <typename _Type_t>
inline void replace_ad_wildcard(
        uint32_t* orig, int orig_len, const _Type_t& wc, //int wc_len,
        uint32_t* subs, int subs_len, uint32_t* res, int& res_len,bool bid_replace)
{
    int cur = 0;
    int wc_len = wc.Size();

    res_len = 0;
    for (int i = 0; i < orig_len; ) {      
        if (cur < wc_len && i == wc.Start(cur) && bid_replace) {        
            if (subs_len > 0) {
                std::copy(subs, subs+subs_len, res+res_len);
                res_len += subs_len;
            }
            i = wc.End(cur);
            ++ cur;
        } else {
            res[res_len++] = orig[i];
            ++ i;
        }
    }

    if (cur < wc_len && orig_len == wc.Start(cur)) {
        std::copy(subs, subs+subs_len, res+res_len);
        res_len += subs_len;
    }
}

inline bool combine_token_arrays(uint32_t* pin[], int sarr[],
        uint32_t cnum, uint32_t sall, char pout[][CHAR_BUFF_SIZE],
        const char* prefix, const char* seprator)
{
    if (!pin || !sarr || !pout
            || !prefix || !seprator || !cnum || !sall) {
        return false;
    }
    uint32_t plen[sall];
    for (uint32_t k = 0; k < sall; ++k) {
        int ret = snprintf(pout[k],
                CHAR_BUFF_SIZE, "%s%u", prefix, pin[0][k%sarr[0]]);
        plen[k] = ret;
    }
    uint32_t roll = sarr[0];
    for (uint32_t i = 1; i < cnum; ++i) {
        for (uint32_t j = 0; j < sall; ++j) {
            int ret = snprintf(pout[j]+plen[j],
                    CHAR_BUFF_SIZE, "%s%u", seprator, pin[i][(j/sarr[i-1])%sarr[i]]);
            plen[j] += ret;
        }
        roll *= sarr[i]; // 历史版本实现的顺序如此，导致j/roll
    }
    return true;
}

inline bool combine_token_values(
        const uint32_t& left, const uint32_t& right,
        const char* prefix, const char* seprator,
        const int& size, char pout[])
{
    int ret = snprintf(pout, size,
            "%s%u%s%u", prefix, left, seprator, right);
    if (ret >= size) {
        return false;
    }
    return true;
}

inline bool combine_token_values(
        const uint32_t& left, const uint64_t& right,
        const char* prefix, const char* seprator,
        const int& size, char pout[])
{
    int ret = snprintf(pout, size,
            "%s%u%s%lu", prefix, left, seprator, right);
    if (ret >= size) {
        return false;
    }
    return true;
}

template <typename Type_t>
inline bool parse_string_tomap(const char ptr[], const char sep[], Type_t& dict) {
    if (!ptr || !sep) {
        return false;
    }
    const char *ita=ptr, *itb=NULL, *itc=NULL;
    char key[CHAR_BUFF_SIZE], value[CHAR_BUFF_SIZE];
    while (*ita != '\0') {
        itb = strpbrk(ita, sep);
        if (!itb || *itb != sep[0]) {
            return false;
        } else {
            itc = strpbrk(itb+1, sep);
            int sa = 0, sb = 0;
            if (!itc) {
                size_t n = strlen(itb+1);
                if (n == 0) return false;
                sa = itb-ita; sb = n;
            } else if (*itc != sep[1]) {
                return false;
            } else {
                sa = itb-ita; sb = itc-itb-1;
            }
            if (sa <= 0 || sa >= CHAR_BUFF_SIZE
                    || sb <= 0 || sb >= CHAR_BUFF_SIZE) {
                return false;
            } else {
                strncpy(key, ita, sa); key[sa] = '\0';
                strncpy(value, itb+1, sb); value[sb] = '\0';
                dict[key] = value;
            }
        }
        if (!itc) break;
        ita = itc+1;
    }
    return true;
}

template <typename _Type_t>
bool TermWeightComp(const _Type_t & left, const _Type_t & right) {
    return left->weight > right->weight;
}

} } }

#endif
