/**
 * @file string_util.h
 * @brief Generic utilities for basic strings.
 */

#ifndef STRING_UTIL_H
#define STRING_UTIL_H
#include "cxx_def.h"

namespace cxx
{
    /**
     * @brief Provides string prefix, suffix, splitting, trimming, and joining utilities.
     * @tparam CharT The string character type.
     */
    template <typename CharT = char>
    class string_util
    {
    public:
        /**
         * @brief Checks whether a string starts with a substring.
         * @param[in] str The string to inspect.
         * @param[in] sub_str The prefix to test.
         * @return True if the string starts with the specified prefix.
         */
        static bool string_start_with(const std::basic_string<CharT> &str, const std::basic_string<CharT> &sub_str);

        /**
         * @brief Checks whether a string ends with a substring.
         * @param[in] str The string to inspect.
         * @param[in] sub_str The suffix to test.
         * @return True when rfind(sub_str) plus sub_str.length() equals str.length().
         * @note The implementation does not test for npos before addition. If an absent
         * suffix is exactly one character longer than the input, unsigned wraparound can also yield true.
         */
        static bool string_end_with(const std::basic_string<CharT> &str, const std::basic_string<CharT> &sub_str);

        /**
         * @brief Splits a string at each occurrence of a separator.
         * @param[in] str The string to split.
         * @param[in] separator The non-empty separator.
         * @note An empty separator triggers an assertion and returns an empty list when assertions are disabled.
         * @return The resulting segments; a final empty segment is omitted.
         */
        static string_list<CharT> string_split(const std::basic_string<CharT> &str, const std::basic_string<CharT> &separator);

        /**
         * @brief Removes leading and trailing whitespace recognized by isspace().
         * @param[in] str The string to trim; characters must be valid arguments to the C isspace() function.
         * @note Whitespace recognition follows the current C locale.
         * @return A trimmed copy of the input string.
         */
        static std::basic_string<CharT> string_trimmed(const std::basic_string<CharT> &str);

        /**
         * @brief Joins strings using a separator between adjacent elements.
         * @param[in] strings The strings to join.
         * @param[in] separator The separator, which may be empty.
         * @return The joined string.
         */
        static std::basic_string<CharT> string_join(const string_list<CharT> &strings, const std::basic_string<CharT> &separator);
    };
}

#endif // STRING_UTIL_H
