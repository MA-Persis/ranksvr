#include "feature/twoval_combiner_v2.h"

#include "store/info_center.h"

#include "feature/feature_funcs.h"

namespace qihoo { namespace ad { namespace ranking {

ExtractorBase * TwoValCombinerV2::Clone() const
{
    return new TwoValCombinerV2();
}

TwoValCombinerV2::TwoValCombinerV2()
    : _left(0)
    , _right(0)
    , _prefix("")
    , _seprator("")
{
    _namestr = "TwoValCombinerV2";
    _typenum = FT_COMBINER;
}

TwoValCombinerV2::~TwoValCombinerV2()
{
}

bool TwoValCombinerV2::Init(const std::string& params,
    InfoCenter& ic)
{
    ConfigMap cfg;
    parse_string_tomap(params.c_str(), "=,", cfg);

    ConfigMap::iterator it;
    it = cfg.find("seprator");
    if (it == cfg.end())
    {
        return false;
    }

    _seprator = it->second;

    it = cfg.find("prefix");
    if (it == cfg.end())
    {
        return false;
    }

    _prefix = it->second;

    it = cfg.find("left");
    if (it == cfg.end())
    {
        return false;
    }

    _left = atoi(it->second.c_str());

    it = cfg.find("right");
    if (it == cfg.end())
    {
        return false;
    }

    _right = atoi(it->second.c_str());

    it = cfg.find("right_type");
    if (it == cfg.end())
    {
        return false;
    }

    _right_type = it->second;

    it = cfg.find("left_type");
    if (it == cfg.end())
    {
        return false;
    }

    _left_type = it->second;

    _slot_id = -1;

    it = cfg.find("slot");
    if (it != cfg.end()) {
        _slot_id = atoi(it->second.c_str());
    }else {
        _slot_id = atoi(_prefix.c_str());
    }

    _tag = "";
    it = cfg.find("tag");
    if (it != cfg.end()) {
        _tag = it->second;
    }

    return true;
}

int TwoValCombinerV2::_GetKey(InfoCenter& ic, const std::string& type,
                     char* out, int len, int index)
{
    if ("uint32_t" == type)
    {
        const uint32_t* val = ic.GetStorageVal<uint32_t>(index);
        int ret = snprintf(out, len, "%u", *val);
        if (0 > ret || ret >= len)
            return -1;

        return ret;
    }
    else if ("uint64_t" == type)
    {
        const uint64_t* val = ic.GetStorageVal<uint64_t>(index);
        int ret = snprintf(out, len, "%lu", *val);
        if (0 > ret || ret >= len)
            return -1;

        return ret;
    }
    else if ("string" == type)
    {
        int size = 0;
        const char* val = ic.GetStorageArr<char>(index, size);

        std::string str(val, size);

        int ret = snprintf(out, len, "%s", str.c_str());
        if (0 > ret || ret >= len)
            return -1;

        return ret;
    }

    return -1;
}

bool TwoValCombinerV2::ExtractDNN(InfoCenter& ic)
{
    char pout[CHAR_BUFF_SIZE] = {0};

    int remain_len = CHAR_BUFF_SIZE;

    char* p = pout;

    int len = _GetKey(ic, _left_type, p, remain_len, _left);
    if (0 > len || len >= remain_len)
    {
        LOG(NOTICE) << "TwoValCombinerV2 pout limit size";
        return false;
    }

    p += len;
    remain_len -= len;
    len = snprintf(p, remain_len, "%s", _seprator.c_str());
    if (0 > len || len >= remain_len)
    {
        LOG(NOTICE) << "TwoValCombinerV2 pout limit size";
        return false;
    }

    p += len;
    remain_len -= len;
    len = _GetKey(ic, _right_type, p, remain_len, _right);
    if (0 > len || len >= remain_len)
    {
        LOG(NOTICE) << "TwoValCombinerV2 pout limit size";
        return false;
    }
    p += len;
    remain_len -= len;

    std::string key_str = _prefix + pout;

    return true;
}

} } } // namespace qihoo ad ranking

