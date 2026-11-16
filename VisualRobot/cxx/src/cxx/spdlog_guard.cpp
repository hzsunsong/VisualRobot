/**
 * @file spdlog_guard.cpp
 * @brief Asynchronous console and file logging setup, message dispatch, and shutdown.
 */

#include <iostream>
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "cxx/date_time.h"
#include "spdlog_guard.h"

namespace
{
    static constexpr auto LOG_QUEUE_SIZE{8192};
    static constexpr auto LOG_THREAD_COUNT{1};
    static constexpr auto LOGGER_NAME{"spdlog"};
    static constexpr auto LOG_DIR{"log/"};
    static constexpr auto LOG_FILENAME_SUFFIX{".log"};
}

cxx::spdlog_guard::~spdlog_guard()
{
    cleanup();
}

void cxx::spdlog_guard::initial(const std::string &filename_prefix, const std::optional<int> &log_level)
{
    try
    {
        std::string log_filename{LOG_DIR};
        log_filename += filename_prefix;
        log_filename += "_";
        log_filename += cxx::date_time::current(cxx::date_time::precision::millisecond, "%Y%m%d_%H%M%S");
        log_filename += std::string{LOG_FILENAME_SUFFIX};
        spdlog::init_thread_pool(LOG_QUEUE_SIZE, LOG_THREAD_COUNT);
        const auto console_sink{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};
        const auto file_sink{std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_filename)};
        std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
        const auto logger{std::make_shared<spdlog::async_logger>(LOGGER_NAME, sinks.cbegin(), sinks.cend(), spdlog::thread_pool())};
        spdlog::register_logger(logger);
        spdlog::set_default_logger(logger);

        if (!log_level)
        {
            spdlog::set_level(spdlog::level::info);
            spdlog::flush_on(spdlog::level::info);
        }
        else
        {
            const auto log_level_t{static_cast<spdlog::level::level_enum>(*log_level)};
            spdlog::set_level(log_level_t);
            spdlog::flush_on(log_level_t);
        }

        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [thread: %t] [%^%l%$] %v");
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cerr << "initial the spdlog failed!spdlog error:" << ex.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "initial the spdlog failed!std error:" << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "initial the spdlog failed!unknown error!" << std::endl;
    }
}

void cxx::spdlog_guard::log(spdlog::level::level_enum log_level, const std::string &message)
{
    spdlog::log(log_level, "{}", message);
}

void cxx::spdlog_guard::cleanup()
{
    spdlog::flush_all();
    spdlog::shutdown();
}