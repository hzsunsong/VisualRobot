/**
 * @file spdlog_guard.h
 * @brief Asynchronous logging interfaces and spdlog lifecycle management.
 * @author songhuirong
 * @date 2026/10/26
 * @copyright Copyright By songhuirong, All Rights Reserved.
 */

#ifndef SPDLOG_GUARD_H
#define SPDLOG_GUARD_H
#include <string>
#include <optional>
#include <utility>
#include "spdlog/spdlog.h"
#include "cxx_export.h"

namespace cxx
{
    /**
     * @brief Manages the spdlog lifecycle and provides logging interfaces.
     *
     * The logger writes messages to both the console and a timestamped log file.
     */
    class spdlog_guard
    {
    public:
        /**
         * @brief Constructs a logging lifecycle guard.
         */
        spdlog_guard() = default;
        /**
         * @brief Prevents copying the owner of the global spdlog lifecycle.
         */
        spdlog_guard(const spdlog_guard &) = delete;
        /**
         * @brief Prevents copy assignment of the global spdlog lifecycle owner.
         */
        spdlog_guard &operator=(const spdlog_guard &) = delete;
        /**
         * @brief Prevents moving the owner of the global spdlog lifecycle.
         */
        spdlog_guard(spdlog_guard &&) = delete;
        /**
         * @brief Prevents move assignment of the global spdlog lifecycle owner.
         */
        spdlog_guard &operator=(spdlog_guard &&) = delete;

        /**
         * @brief Flushes pending log messages and shuts down spdlog.
         */
        ~spdlog_guard();

        /**
         * @brief Initializes the asynchronous logger and its console and file sinks.
         * @param[in] filename_prefix The prefix used in the timestamped filename under log/.
         * @param[in] log_level The optional spdlog severity; defaults to info and also sets the flush threshold.
         * @note Initialization exceptions are written to standard error and are not returned to the caller.
         */
        void initial(const std::string &filename_prefix, const std::optional<int> &log_level = {});

        /**
         * @brief Formats a trace-level message and forwards it to the DLL logging backend.
         * @tparam Args The formatting argument types.
         * @param format The format string.
         * @param args The arguments used to format the message before crossing the DLL boundary.
         */
        template <typename... Args>
        static void trace(spdlog::format_string_t<Args...> format, Args &&...args)
        {
            log(spdlog::level::trace, spdlog::fmt_lib::format(format, std::forward<Args>(args)...));
        }

        /**
         * @brief Formats a debug-level message and forwards it to the DLL logging backend.
         * @tparam Args The formatting argument types.
         * @param format The format string.
         * @param args The arguments used to format the message before crossing the DLL boundary.
         */
        template <typename... Args>
        static void debug(spdlog::format_string_t<Args...> format, Args &&...args)
        {
            log(spdlog::level::debug, spdlog::fmt_lib::format(format, std::forward<Args>(args)...));
        }

        /**
         * @brief Formats an info-level message and forwards it to the DLL logging backend.
         * @tparam Args The formatting argument types.
         * @param format The format string.
         * @param args The arguments used to format the message before crossing the DLL boundary.
         */
        template <typename... Args>
        static void info(spdlog::format_string_t<Args...> format, Args &&...args)
        {
            log(spdlog::level::info, spdlog::fmt_lib::format(format, std::forward<Args>(args)...));
        }

        /**
         * @brief Formats a warning-level message and forwards it to the DLL logging backend.
         * @tparam Args The formatting argument types.
         * @param format The format string.
         * @param args The arguments used to format the message before crossing the DLL boundary.
         */
        template <typename... Args>
        static void warn(spdlog::format_string_t<Args...> format, Args &&...args)
        {
            log(spdlog::level::warn, spdlog::fmt_lib::format(format, std::forward<Args>(args)...));
        }

        /**
         * @brief Formats an error-level message and forwards it to the DLL logging backend.
         * @tparam Args The formatting argument types.
         * @param format The format string.
         * @param args The arguments used to format the message before crossing the DLL boundary.
         */
        template <typename... Args>
        static void error(spdlog::format_string_t<Args...> format, Args &&...args)
        {
            log(spdlog::level::err, spdlog::fmt_lib::format(format, std::forward<Args>(args)...));
        }

        /**
         * @brief Formats a critical-level message and forwards it to the DLL logging backend.
         * @tparam Args The formatting argument types.
         * @param format The format string.
         * @param args The arguments used to format the message before crossing the DLL boundary.
         */
        template <typename... Args>
        static void critical(spdlog::format_string_t<Args...> format, Args &&...args)
        {
            log(spdlog::level::critical, spdlog::fmt_lib::format(format, std::forward<Args>(args)...));
        }
    private:
        /**
         * @brief Writes through the same DLL-local logger registry used during initialization.
         * @details Keeping registry access out of header templates prevents callers in other
         * modules from selecting their own statically linked spdlog default logger.
         * @param log_level The severity of the formatted message.
         * @param message The formatted message, copied into the asynchronous queue before returning.
         */
        static CXX_EXPORT void log(spdlog::level::level_enum log_level, const std::string &message);

        /**
         * @brief Flushes all pending log messages and shuts down spdlog.
         */
        void cleanup();
    };
}

#endif // SPDLOG_GUARD_H
