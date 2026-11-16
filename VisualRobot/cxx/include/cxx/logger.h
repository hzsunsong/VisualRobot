/**
 * @file logger.h
 * @brief Public logging interface backed by the spdlog lifecycle guard.
 * @author songhuirong
 * @date 2026/10/27
 * @copyright Copyright By songhuirong, All Rights Reserved.
 */

#ifndef LOGGER_H
#define LOGGER_H
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include "spdlog_guard.h"
#include "cxx_export.h"

namespace cxx
{
    /**
     * @brief Provides the application logging interface and owns its lifecycle.
     *
     * Create one logger instance to initialize and clean up the underlying spdlog logger.
     */
    class CXX_EXPORT logger
    {
    public:
        /**
         * @brief Creates the guard that manages the underlying spdlog lifecycle.
         */
        logger();

        /**
         * @brief Prevents copying the owner of the global logging lifecycle.
         */
        logger(const logger &) = delete;

        /**
         * @brief Prevents copying the owner of the global logging lifecycle.
         */
        logger &operator=(const logger &) = delete;

        /**
         * @brief Prevents moving the owner of the global logging lifecycle.
         */
        logger(logger &&) = delete;

        /**
         * @brief Prevents moving the owner of the global logging lifecycle.
         */
        logger &operator=(logger &&) = delete;

        /**
         * @brief Releases the underlying guard, flushing and shutting down spdlog.
         */
        ~logger();

        /**
         * @brief Initializes the asynchronous logger and its console and file sinks.
         * @param[in] filename_prefix The prefix used in the timestamped file under log/.
         * @param[in] log_level The optional spdlog severity; defaults to info and also sets the flush threshold.
         * @note Initialization failures are reported on standard error; no success status is returned.
         */
        void initial(const std::string &filename_prefix, const std::optional<int> &log_level = {});

        /**
         * @brief Forwards a trace-level message through spdlog_guard while preserving formatting arguments.
         * @tparam Args The formatting argument types.
         * @param format The format string.
         * @param args The arguments forwarded without preformatting the message.
         */
        template <typename... Args>
        static void trace(spdlog::format_string_t<Args...> format, Args &&...args)
        {
            spdlog_guard::trace(format, std::forward<Args>(args)...);
        }

        /**
         * @brief Forwards a debug-level message through spdlog_guard while preserving formatting arguments.
         * @tparam Args The formatting argument types.
         * @param format The format string.
         * @param args The arguments forwarded without preformatting the message.
         */
        template <typename... Args>
        static void debug(spdlog::format_string_t<Args...> format, Args &&...args)
        {
            spdlog_guard::debug(format, std::forward<Args>(args)...);
        }

        /**
         * @brief Forwards an info-level message through spdlog_guard while preserving formatting arguments.
         * @tparam Args The formatting argument types.
         * @param format The format string.
         * @param args The arguments forwarded without preformatting the message.
         */
        template <typename... Args>
        static void info(spdlog::format_string_t<Args...> format, Args &&...args)
        {
            spdlog_guard::info(format, std::forward<Args>(args)...);
        }

        /**
         * @brief Forwards a warning-level message through spdlog_guard while preserving formatting arguments.
         * @tparam Args The formatting argument types.
         * @param format The format string.
         * @param args The arguments forwarded without preformatting the message.
         */
        template <typename... Args>
        static void warn(spdlog::format_string_t<Args...> format, Args &&...args)
        {
            spdlog_guard::warn(format, std::forward<Args>(args)...);
        }

        /**
         * @brief Forwards an error-level message through spdlog_guard while preserving formatting arguments.
         * @tparam Args The formatting argument types.
         * @param format The format string.
         * @param args The arguments forwarded without preformatting the message.
         */
        template <typename... Args>
        static void error(spdlog::format_string_t<Args...> format, Args &&...args)
        {
            spdlog_guard::error(format, std::forward<Args>(args)...);
        }

        /**
         * @brief Forwards a critical-level message through spdlog_guard while preserving formatting arguments.
         * @tparam Args The formatting argument types.
         * @param format The format string.
         * @param args The arguments forwarded without preformatting the message.
         */
        template <typename... Args>
        static void critical(spdlog::format_string_t<Args...> format, Args &&...args)
        {
            spdlog_guard::critical(format, std::forward<Args>(args)...);
        }
    private:
        /**
         * @brief Owns the guard whose destruction flushes and shuts down the global spdlog backend.
         */
        std::unique_ptr<spdlog_guard> m_spdlog_guard;
    };
}

#endif // LOGGER_H