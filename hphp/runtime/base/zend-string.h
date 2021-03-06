/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-present Facebook, Inc. (http://www.facebook.com)  |
   | Copyright (c) 1998-2010 Zend Technologies Ltd. (http://www.zend.com) |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.00 of the Zend license,     |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.zend.com/license/2_00.txt.                                |
   | If you did not receive a copy of the Zend license and are unable to  |
   | obtain it through the world-wide-web, please send a note to          |
   | license@zend.com so we can mail you a copy immediately.              |
   +----------------------------------------------------------------------+
*/

#ifndef incl_HPHP_ZEND_STRING_H_
#define incl_HPHP_ZEND_STRING_H_

// NOTE: See also "hphp/zend/zend-string.*".

#include "hphp/zend/zend-string.h"
#include "hphp/runtime/base/type-string.h"

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////
/**
 * Low-level string functions PHP uses.
 *
 * 1. If a function returns a char *, it has malloc-ed a new string and it's
 *    caller's responsibility to free it.
 *
 * 2. If a function takes "int &len" right after the 1st string parameter, it
 *    is input string's length, and in return, it's return string's length.
 *
 * 3. All functions work with binary strings and all returned strings are
 *    NULL terminated, regardless of whether it's a binary string.
 */

/**
 * Changing string's cases in place. Return's length is always the same
 * as "len".
 */
void string_to_case(String& s, int (*tocase)(int));

// Use lambdas wrapping the ctype.h functions because of linker weirdness on
// OS X Mavericks.

#define string_to_upper(s)        \
  string_to_case((s), [] (int i) -> int { return toupper(i); })

/**
 * Pad a string with pad_string to pad_length. "len" is
 * input string's length, and in return, it's trimmed string's length. pad_type
 * can be k_STR_PAD_RIGHT, k_STR_PAD_LEFT or k_STR_PAD_BOTH.
 */
String string_pad(const char *input, int len, int pad_length,
                 const char *pad_string, int pad_str_len, int pad_type);

/**
 * Find a character or substring and return it's position (or -1 if not found).
 */
int string_find(const char *input, int len, char ch, int pos,
                bool case_sensitive);
int string_rfind(const char *input, int len, char ch, int pos,
                 bool case_sensitive);
int string_find(const char *input, int len, const char *s, int s_len,
                int pos, bool case_sensitive);
int string_rfind(const char *input, int len, const char *s, int s_len,
                 int pos, bool case_sensitive);

const char *string_memnstr(const char *haystack, const char *needle,
                           int needle_len, const char *end);

/**
 * Replace specified substring or search string with specified replacement.
 */
String string_replace(const char *s, int len, int start, int length,
                      const char *replacement, int len_repl);
String string_replace(const char *input, int len,
                      const char *search, int len_search,
                      const char *replacement, int len_replace,
                      int &count, bool case_sensitive);

/**
 * Replace a substr with another and return replaced one. Note, read
 * http://www.php.net/substr about meanings of negative start or length.
 *
 * The form that takes a "count" reference will still replace all occurrences
 * and return total replaced count in the out parameter. It does NOT mean
 * it will replace at most that many occurrences, so count's input value
 * is never checked.
 */
inline String string_replace(const String& str, int start, int length,
                             const String& repl) {
  return string_replace(str.data(), str.size(), start, length,
                        repl.data(), repl.size());
}

inline String string_replace(const String& str, const String& search,
                             const String& replacement,
                             int &count, bool caseSensitive) {
  count = 0;
  if (!search.empty() && !str.empty()) {
    auto ret = string_replace(str.data(), str.size(),
                              search.data(), search.size(),
                              replacement.data(), replacement.size(),
                              count, caseSensitive);
    if (!ret.isNull()) {
      return ret;
    }
  }
  return str;
}

inline String string_replace(const String& str, const String& search,
                             const String& replacement) {
  int count;
  return string_replace(str, search, replacement, count, true);
}

/**
 * Reverse, repeat or shuffle a string.
 */
String string_chunk_split(const char *src, int srclen, const char *end,
                          int endlen, int chunklen);

/**
 * Strip HTML and PHP tags.
 */
String string_strip_tags(const char *s, int len, const char *allow,
                         int allow_len, bool allow_tag_spaces);

/**
 * Encoding/decoding strings according to certain formats.
 */
String string_quoted_printable_encode(const char *input, int len);
String string_quoted_printable_decode(const char *input, int len, bool is_q);
String string_uuencode(const char *src, int src_len);
String string_uudecode(const char *src, int src_len);
String string_base64_encode(const char *input, int len);
String string_base64_decode(const char *input, int len, bool strict);
String string_escape_shell_arg(const char *str);
String string_escape_shell_cmd(const char *str);

std::string base64_encode(const char *input, int len);
std::string base64_decode(const char *input, int len, bool strict);

/**
 * Convert between strings and numbers.
 */
inline bool string_validate_base(int base) {
  return (2 <= base && base <= 36);
}
Variant string_base_to_numeric(const char *s, int len, int base);
String string_long_to_base(unsigned long value, int base);
String string_numeric_to_base(const Variant& value, int base);

/**
 * Translates characters in str_from into characters in str_to one by one,
 * assuming str_from and str_to have the same length of "trlen".
 */
void string_translate(char *str, int len, const char *str_from,
                      const char *str_to, int trlen);

/**
 * Formatting.
 */
String string_money_format(const char *format, double value);

String string_number_format(double d, int dec,
                            const String& dec_point,
                            const String& thousand_sep);

/**
 * Similarity and other properties of strings.
 */
int string_levenshtein(const char *s1, int l1, const char *s2, int l2,
                       int cost_ins, int cost_rep, int cost_del);
int string_similar_text(const char *t1, int len1,
                        const char *t2, int len2, double *percent);
String string_soundex(const String& str);

String string_metaphone(const char *input, int word_len, long max_phonemes,
                        int traditional);

/**
 * Locale strings.
 */
String string_convert_cyrillic_string(const String& input, char from, char to);
String string_convert_hebrew_string(const String& str, int max_chars_per_line,
                                    int convert_newlines);

///////////////////////////////////////////////////////////////////////////////
// helpers

/**
 * Fills a 256-byte bytemask with input. You can specify a range like 'a..z',
 * it needs to be incrementing. This function determines how "charlist"
 * parameters are interpreted in varies functions that take a list of
 * characters.
 */
void string_charmask(const char *input, int len, char *mask);

///////////////////////////////////////////////////////////////////////////////
}

#endif // incl_HPHP_ZEND_STRING_H_
