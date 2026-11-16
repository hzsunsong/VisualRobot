/**
 * @file string_util.cpp
 * @brief Prefix and suffix tests, splitting, trimming, and joining of basic strings.
 */

#include <cassert>
#include "string_util.h"

template<typename CharT>
bool cxx::string_util<CharT>::string_start_with(const std::basic_string<CharT> &str, const std::basic_string<CharT> &sub_str)
{
    const auto pos{str.find(sub_str)};

    return 0 == pos;
}

template<typename CharT>
bool cxx::string_util<CharT>::string_end_with(const std::basic_string<CharT> &str, const std::basic_string<CharT> &sub_str)
{
    const auto pos{str.rfind(sub_str)};

    return (pos + sub_str.length()) == str.length();
}

template<typename CharT>
cxx::string_list<CharT> cxx::string_util<CharT>::string_split(const std::basic_string<CharT> &str, const std::basic_string<CharT> &separator)
{
    if (separator.empty())
    {
        assert(false);

        return {};
    }

    string_list<CharT> strings;
    auto pos{static_cast<typename std::basic_string<CharT>::size_type>(0)};
    auto prev_pos{pos};

    while (std::basic_string<CharT>::npos != (pos = str.find(separator, prev_pos)))
    {
        const auto count{pos - prev_pos};
        const auto s{str.substr(prev_pos, count)};
        strings.emplace_back(s);
        prev_pos = pos + separator.length();
    }

    if (prev_pos < str.length())
    {
        strings.emplace_back(str.substr(prev_pos));
    }

    return strings;
}

template<typename CharT>
std::basic_string<CharT> cxx::string_util<CharT>::string_trimmed(const std::basic_string<CharT> &str)
{
    auto str_t{str};

    while (!str_t.empty() && isspace(str_t.front()))
    {
        str_t.erase(str_t.begin());
    }

    while (!str_t.empty() && isspace(str_t.back()))
    {
        str_t.pop_back();
    }

    return str_t;
}

template<typename CharT>
std::basic_string<CharT> cxx::string_util<CharT>::string_join(const string_list<CharT> &strings, const std::basic_string<CharT> &separator)
{
    std::basic_string<CharT> strings_ret;
    auto index{static_cast<typename std::basic_string<CharT>::size_type>(0)};

    for (auto itor = strings.begin(); itor != strings.end(); ++itor, ++index)
    {
        const auto str{*itor};
        strings_ret += str;

        if (index < strings.size() - 1)
        {
            strings_ret += separator;
        }
    }

    return strings_ret;
}