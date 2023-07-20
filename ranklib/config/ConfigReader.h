/**
 * auth: zhangyansheng@360.cn
 * date: 2018/04/02
 * desc: ʹ���������ʽʵ�ֵ�һ�����ö�ȡ����ע�����ú��治�����пո�
 */

#ifndef RANK_UTILITY_CONFIG_READER_H_
#define RANK_UTILITY_CONFIG_READER_H_

#include <string>

#include <butil/logging.h>

#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>

#define SEC_KEY_DELIM_STR "."

namespace qihoo { namespace ad { namespace ranking {

//we do this instead of inherit unordered_map for its destructor is not virutal 
template<class I> //iterator type
struct __config_reader_iterator
{
    typedef std::forward_iterator_tag iterator_category;
    typedef const boost::unordered_map<std::string, std::string> const_umap;
    typedef __config_reader_iterator<I> iterator;
    typedef typename const_umap::value_type value_type;
    typedef value_type & reference;
    typedef reference const_reference;

    __config_reader_iterator(const_umap * c, const I & iter)
        :__iter(iter), __c(c){}

    const_reference operator*()const
    {
        return *__iter;
    }

    const I & operator->()const
    {
        return __iter;
    }

    //prefix 
    iterator & operator++()
    {
        __iter++;
        return *this;
    }

    //postfix
    iterator operator++(int)
    {
        iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const iterator & other)const
    {
        return __iter == other.__iter;
    }

    bool operator!=(const iterator & other)const
    {
        return !operator==(other);
    }
private:
    I __iter;
    const_umap * __c;
};

class ConfigReader
{
public:

    typedef boost::unordered_map<std::string, std::string> umap;
    typedef __config_reader_iterator<umap::const_iterator> const_iterator;

public:
    ConfigReader()
        : _is_built(0)
    {}

    ~ConfigReader()
    {}

    const_iterator begin()const
    {
        return const_iterator(&this->_table, this->_table.begin());
    }

    const_iterator end()const
    {
        return const_iterator(&this->_table, this->_table.end());
    }

public:
    bool Init(const std::string& file);

    template<typename T>
    bool Get(const std::string& key, T& value,
            const T& default_value) const
    {
        if (Get(key, value))
            return true;

        value = default_value;
        return false;
    }

    template<typename T>
    bool Get(const std::string& key, T& value) const
    {
        if (!_is_built) {
            LOG(FATAL) << "ConfigReader NOT built";
            return false;
        }

        boost::unordered_map<std::string, std::string>::const_iterator iter = _table.find(key);

        if (iter == _table.end()) {
            LOG(FATAL) << "Can NOT find config item:" << key;
            return false;
        }

        try {
            value = boost::lexical_cast<T>(iter->second);
        } catch (boost::bad_lexical_cast& e) {
            LOG(NOTICE) << "bad type:" << key << ":" << e.what();
            return false;
        }

        return true;
    }

private:
    int _is_built;
    boost::unordered_map<std::string, std::string> _table;
};

} } } //namespace qihoo { namespace ad { namespace ranking {

#endif // !RANK_UTILITY_CONFIG_READER_H_

