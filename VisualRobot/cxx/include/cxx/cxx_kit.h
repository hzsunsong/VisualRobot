/**
 * @file cxx_kit.h
 * @brief String conversion, byte formatting, thread, and container utilities.
 * @author songhuirong
 * @date 2025/09/10
 * @copyright Copyright By songhuirong, All Rights Reserved.
 */

#ifndef CXX_KIT_H
#define CXX_KIT_H
#include <string>
#include "cxx_export.h"
#include "cxx_def.h"

namespace cxx
{
    /**
     * @brief Converts a list of strings into a single string, joined by the specified separator.
     * @param strs The list of strings to convert.
     * @param specifier The separator.
     * @return Returns the concatenated string.
     */
    CXX_EXPORT std::string to_string(const string_list<> &strs, const std::string &specifier = {});

    /**
     * @brief Converts a multibyte string to a wstring using the chs C locale.
     * @param str The string object to convert.
     * @return The converted wide string.
     * @note Temporarily changes the process-wide C locale; concurrent locale-sensitive operations require coordination.
     * Conversion errors are not reported through this interface.
     */
    CXX_EXPORT std::wstring string_to_wstring(const std::string &str);

    /**
     * @brief Converts a wstring to a multibyte string using the chs C locale.
     * @param str The wstring object to convert.
     * @return The converted multibyte string.
     * @note Temporarily changes the process-wide C locale; concurrent locale-sensitive operations require coordination.
     * Conversion errors are not reported through this interface.
     */
    CXX_EXPORT std::string wstring_to_string(const std::wstring &str);

    /**
     * @brief Converts a byte array to a hexadecimal string, using "specifier" as the separator.
     * @param ba The byte array to convert.
     * @param specifier The separator.
     * @return Returns the converted string.
     */
    CXX_EXPORT std::string to_hex(const cxx::byte_array &ba, const std::string &specifier = {});

    /**
     * @brief Gets the current thread ID.
     * @return Returns the current thread ID.
     */
    CXX_EXPORT std::string current_thread_id();

    /**
     * @brief Swaps the container with an empty temporary to release its elements and storage.
     * @tparam Container The container type.
     * @param[in,out] container The container to clear.
     * @note Raw pointer elements are discarded without deleting the objects they point to.
     */
    template <typename Container>
    void free(Container &container)
    {
        Container{}.swap(container);
    }
} // namespace cxx

#endif // CXX_KIT_H
