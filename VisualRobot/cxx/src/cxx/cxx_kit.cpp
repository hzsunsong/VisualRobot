/**
 * @file cxx_kit.cpp
 * @brief String conversions, hexadecimal formatting, and current thread identification.
 */

#include <clocale>
#include <sstream>
#include <thread>
#include <numeric>
#include <algorithm>
#include "cxx_kit.h"

std::string cxx::to_string(const string_list<> &strs, const std::string &specifier)
{
    if (strs.empty()) return {};

    return std::accumulate(std::next(strs.cbegin()), strs.cend(), strs[0], [=](const auto &a, const auto &b) {
        return a + specifier + b;
    });
}

std::wstring cxx::string_to_wstring(const std::string &str)
{
    const std::string current_locale{ setlocale(LC_ALL, nullptr) };
    setlocale(LC_ALL, "chs");
    const auto src_str{ str.data() };
    const auto char_size{ sizeof(char) * str.size() + 1 };
    std::vector<wchar_t> dest(char_size);
    std::size_t retval{};
    mbstowcs_s(&retval, dest.data(), dest.size(), src_str, dest.size());
    setlocale(LC_ALL, current_locale.data());

    return std::wstring(dest.data());
}

std::string cxx::wstring_to_string(const std::wstring &str)
{
    const std::string current_locale{ setlocale(LC_ALL, nullptr) };
    setlocale(LC_ALL, "chs");
    const auto src_str{ str.data() };
    const auto char_size{ sizeof(wchar_t) * str.size() + 1 };
    std::vector<char> dest(char_size);
    std::size_t retval{};
    wcstombs_s(&retval, dest.data(), dest.size(), src_str, dest.size());
    setlocale(LC_ALL, current_locale.data());

    return std::string(dest.data());
}

std::string cxx::to_hex(const byte_array &ba, const std::string &specifier)
{
    if (ba.empty()) return {};

    std::string hex_str;
    std::ostringstream oss1;
    oss1.width(2);
    oss1.fill('0');
    oss1 << std::hex;
    oss1 << static_cast<int>(ba[0]);
    hex_str += oss1.str();

    for_each(ba.cbegin() + 1, ba.cend(), [&](const auto &v) {
        hex_str += specifier;
        std::ostringstream oss2;
        oss2.width(2);
        oss2.fill('0');
        oss2 << std::hex;
        oss2 << static_cast<int>(v);
        hex_str += oss2.str();
    });

    return hex_str;
}

std::string cxx::current_thread_id()
{
    std::ostringstream oss;
    oss << std::this_thread::get_id();

    return oss.str();
}
