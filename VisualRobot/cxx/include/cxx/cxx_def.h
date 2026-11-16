/**
 * @file cxx_def.h
 * @brief Common type aliases, constants, and utility macros.
 * @author songhuirong
 * @date 2025/09/09
 * @copyright Copyright By songhuirong, All Rights Reserved.
 */

#ifndef CXX_DEF_H
#define CXX_DEF_H
#include <cstdint>
#include <string>
#include <vector>
#include <cassert>
#include "spdlog/spdlog.h"

/**
 * @brief Gets the number of elements in a built-in array.
 * @param A The built-in array.
 * @return The number of array elements.
 */
#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))
/**
 * @brief Marks an expression as intentionally unused.
 * @param X The expression to discard.
 */
#define UNUSE(X) ((void)X)

/**
 * @brief Locks a mutex for the lifetime of the surrounding scope.
 * @param MUTEX The mutex to lock.
 */
#define LOCK_GUARD(MUTEX) \
    std::lock_guard<decltype(MUTEX)> lock_guard_t{MUTEX}; \
    UNUSE(lock_guard_t)

/**
 * @brief Logs a null pointer and returns from the current void function.
 * @param PTR The pointer to check.
 */
#define CHECK_PTR_RETURN(PTR) \
if (!PTR) \
{ \
    spdlog::error("the {} is null.", #PTR); \
 \
    return; \
}

/**
 * @brief Logs a null pointer and returns false from the current function.
 * @param PTR The pointer to check.
 */
#define CHECK_PTR_RETURN_FALSE(PTR) \
if (!PTR) \
{ \
    spdlog::error("the {} is null.", #PTR); \
 \
    return false; \
}

/**
 * @brief Logs a null pointer and returns the supplied value.
 * @param PTR The pointer to check.
 * @param RETVAL The value returned when PTR is null.
 */
#define CHECK_PTR_RETURN_VALUE(PTR, RETVAL) \
if (!PTR) \
{ \
    spdlog::error("the {} is null.", #PTR); \
 \
    return RETVAL; \
}

/**
 * @brief Logs a null pointer and continues the nearest enclosing loop.
 * @param PTR The pointer or pointer-like expression to check.
 * @note This macro must be used directly within a loop body.
 */
#define CHECK_PTR_CONTINUE(PTR) \
if (!PTR) \
{ \
    spdlog::error("the {} is null.", #PTR); \
 \
    continue; \
}

/**
 * @brief Defaults construction, destruction, copying, and moving.
 * @param CLASS_NAME The enclosing class name.
 */
#define CLASS_DEFAULT_OPERATION(CLASS_NAME) \
CLASS_NAME() = default; \
CLASS_NAME(const CLASS_NAME &) = default; \
CLASS_NAME &operator=(const CLASS_NAME &) = default; \
CLASS_NAME(CLASS_NAME &&) = default; \
CLASS_NAME &operator=(CLASS_NAME &&) = default; \
~CLASS_NAME() = default

/**
 * @brief Defaults special members with a virtual destructor.
 * @param CLASS_NAME The enclosing class name.
 */
#define CLASS_DEFAULT_OPERATION_V(CLASS_NAME) \
CLASS_NAME() = default; \
CLASS_NAME(const CLASS_NAME &) = default; \
CLASS_NAME &operator=(const CLASS_NAME &) = default; \
CLASS_NAME(CLASS_NAME &&) = default; \
CLASS_NAME &operator=(CLASS_NAME &&) = default; \
virtual ~CLASS_NAME() = default

/**
 * @brief Defaults copy and move construction and assignment.
 * @param CLASS_NAME The enclosing class name.
 */
#define CLASS_DEFAULT_COPY_MOVE(CLASS_NAME) \
CLASS_NAME(const CLASS_NAME &) = default; \
CLASS_NAME &operator=(const CLASS_NAME &) = default; \
CLASS_NAME(CLASS_NAME &&) = default; \
CLASS_NAME &operator=(CLASS_NAME &&) = default

/**
 * @brief Defaults construction and destruction and deletes copying and moving.
 * @param CLASS_NAME The enclosing class name.
 */
#define DELETE_CLASS_DEFAULT_OPERATION(CLASS_NAME) \
CLASS_NAME() = default; \
CLASS_NAME(const CLASS_NAME &) = delete; \
CLASS_NAME &operator=(const CLASS_NAME &) = delete; \
CLASS_NAME(CLASS_NAME &&) = delete; \
CLASS_NAME &operator=(CLASS_NAME &&) = delete; \
~CLASS_NAME() = default

/**
 * @brief Defaults construction and virtual destruction and deletes copying and moving.
 * @param CLASS_NAME The enclosing class name.
 */
#define DELETE_CLASS_DEFAULT_OPERATION_V(CLASS_NAME) \
CLASS_NAME() = default; \
CLASS_NAME(const CLASS_NAME &) = delete; \
CLASS_NAME &operator=(const CLASS_NAME &) = delete; \
CLASS_NAME(CLASS_NAME &&) = delete; \
CLASS_NAME &operator=(CLASS_NAME &&) = delete; \
virtual ~CLASS_NAME() = default

/**
 * @brief Deletes copy and move construction and assignment without declaring a constructor or destructor.
 * @param CLASS_NAME The enclosing class name.
 */
#define DELETE_CLASS_DEFAULT_COPY_MOVE(CLASS_NAME) \
CLASS_NAME(const CLASS_NAME &) = delete; \
CLASS_NAME &operator=(const CLASS_NAME &) = delete; \
CLASS_NAME(CLASS_NAME &&) = delete; \
CLASS_NAME &operator=(CLASS_NAME &&) = delete

/**
 * @brief Deletes a single object and resets the supplied pointer to nullptr.
 * @param PTR A modifiable pointer expression owning an object allocated with new.
 * @note Resetting this expression does not invalidate other aliases to the same object.
 */
#define DELETE(PTR) \
delete PTR; \
PTR = nullptr \

/**
 * @brief Deletes an array and resets the supplied pointer to nullptr.
 * @param ARRAY A modifiable pointer expression owning an array allocated with new[].
 */
#define DELETE_ARRAY(ARRAY) \
delete [] ARRAY; \
ARRAY = nullptr \

namespace cxx
{
    using byte = uint8_t; ///< An unsigned byte.
    using byte_array = std::vector<byte>; ///< An owning array of bytes.

    /**
     * @brief A list of basic strings.
     * @tparam CharT The string character type.
     */
    template <typename CharT = char>
    using string_list = std::vector<std::basic_string<CharT>>;

    static constexpr auto DATE_TIME_SINCE_YEAR{ 1900 }; ///< Base year used by std::tm::tm_year.
    static constexpr auto DATE_TIME_MON_DIFF{ 1 }; ///< Offset converting a zero-based month to a calendar month.
    static constexpr std::streamsize DEFAULT_FILE_BLOCK_SIZE{ 1024 }; ///< Default file I/O block size in bytes.
    static const char *DEFAULT_DATE_TIME_FORMAT{ "%Y-%m-%d %H:%M:%S" }; ///< Default local date and time format.
} // namespace cxx

#endif // CXX_DEF_H
