#ifndef __STRNORMALIZE_H__
#define __STRNORMALIZE_H__

#include <string>

namespace oir {

/** @defgroup SNO_X 字符串转换选项 */
/** @{ */
const unsigned SNO_TO_LOWER      = 1; ///< 大写转小写
const unsigned SNO_TO_UPPER      = 2; ///< 小写转大写
const unsigned SNO_TO_HALF       = 4; ///< 全角转半角
const unsigned SNO_TO_SIMPLIFIED = 8; ///< 繁体转简体
/** @} */

/** 字符串大小写、全半角、简繁体转换（GBK 编码）
 *
 * @param  text    源字符串，要求满足 GBK 编码，就地转码。
 * @param  options 转换选项，是四个 SNO_X 选项的或。
 * @sa SNO_X
 */
void strNormalizeGbk(char *text, unsigned options
		= SNO_TO_LOWER | SNO_TO_HALF);

/** 字符串大小写、全半角、简繁体转换（GBK 编码）
 *
 * @param  text    源字符串，要求满足 GBK 编码，就地转码。
 * @param  options 转换选项，是四个 SNO_X 选项的或。
 * @sa SNO_X
 */
void strNormalizeGbk(std::string &text, unsigned options
		= SNO_TO_LOWER | SNO_TO_HALF);

/** 字符串大小写、全半角、简繁体转换（UTF8 编码）
 *
 * @param  text    源字符串，要求满足 UTF8 编码，就地转码。
 * @param  options 转换选项，是四个 SNO_X 选项的或。
 * @sa SNO_X
 */
void strNormalizeUtf8(char *text, unsigned options
		= SNO_TO_LOWER | SNO_TO_HALF);

/** 字符串大小写、全半角、简繁体转换（UTF8 编码）
 *
 * @param  text    源字符串，要求满足 UTF8 编码，就地转码。
 * @param  options 转换选项，是四个 SNO_X 选项的或。
 * @sa SNO_X
 */
void strNormalizeUtf8(std::string &text, unsigned options
		= SNO_TO_LOWER | SNO_TO_HALF);

/** GBK 编码转 UTF8 编码
 *
 * @param  text 源字符串，要求满足 GBK 编码。
 * @return      返回对应的 UTF8 编码字符串。如果 text
 *              中存在乱码，则乱码部分被舍弃。
 */
const std::string gbkToUtf8(const std::string &text);

/** UTF8 编码转 GBK 编码
 *
 * @param  text 源字符串，要求满足 UTF8 编码。
 * @return      返回对应的 GBK 编码字符串。如果 text
 *              中存在乱码，则乱码部分被舍弃。
 */
const std::string utf8ToGbk(const std::string &text);

/** 检查 code 是否为合法的 GBK 编码字符
 *
 * @param  code GBK 码值。例如“猪”的 GBK 编码是 D6 ED，那么
 *              code 是 0xD6ED。
 * @return      合法返回 true，否则返回 false。
 */
bool isGbk(unsigned short code);
uint32_t strlenutf8(const char *str, size_t str_len);

} // namespace tickle

#endif /* __STRNORMALIZE_H__ */
