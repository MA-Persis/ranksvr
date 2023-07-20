#include "config/ConfigReader.h"

#include <cstring>
#include <cstdio>
#include <boost/regex.hpp>

namespace qihoo { namespace ad { namespace ranking {

using std::string;

bool ConfigReader::Init(const string& ini_file)
{
    _table.clear();

    if (access(ini_file.c_str(), R_OK) < 0) {
        // CFATAL_LOG("can NOT access file:%s", ini_file.c_str());
        return 0;
    }

    FILE* fp = fopen(ini_file.c_str(), "r");
    if (not fp) {
        // CFATAL_LOG("can NOT fopen file:%s", ini_file.c_str());
        return false;
    }

    boost::regex pattern_section("\\s*\\[(.*?)\\]\\s*.*");
    boost::regex pattern_item("\\s*([^\\s\\r\\=]*?)\\s*\\=\\s*([^\\s\\r\\n\\=]*?)([\\r\\n\\s]*)$");
    boost::cmatch match;

    char *line_read = NULL;
    size_t line_read_len = 0;
    ssize_t read_size = -1;

    std::string section_name,
        item_key,
        item_value;
    const char *start;

    while ((read_size = getline(&line_read, &line_read_len, fp)) != -1) {
        start = line_read;	
        while (*start == ' ' || *start == '\n' || *start == '\t' || *start == '\r')
            start++;

        // 略去空行和注释
        if (*start == '#' || *start == ';' || *start == '\0') {
            continue;
        }

        // 解析 section
        if (boost::regex_search(line_read, match, pattern_section)) {
            section_name.assign(match[1].first, match[1].second);
            continue;
        }

        if (section_name.empty()) {
            // CWARNING_LOG("Skip line without section:%s", line_read);
            continue;
        }

        // 解析 items
        if (boost::regex_search(line_read, match, pattern_item)) {
            item_key.assign(match[1].first, match[1].second);
            item_value.assign(match[2].first, match[2].second);

            // 特殊处理bool
            if (! strcasecmp(item_value.c_str(), "true")) { 
                item_value = "1";
            } else if (! strcasecmp(item_value.c_str(), "false")) {
                item_value = "0";
            }

            _table[section_name + SEC_KEY_DELIM_STR + item_key] = item_value;
        }
        else
        {
            // CWARNING_LOG("item not match:%s", line_read);
        }
    }

    if (line_read)
        free(line_read);

    if (fp)
        fclose(fp);

    _is_built = true;
    return true;
}

} } } //namespace qihoo { namespace ad { namespace ranking {
