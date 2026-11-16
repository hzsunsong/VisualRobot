/**
 * @file logger.cpp
 * @brief Ownership and initialization of the logging lifecycle guard.
 */

#include <iostream>
#include <memory>
#include "cxx/logger.h"
#include "spdlog_guard.h"

cxx::logger::logger()
    : m_spdlog_guard{new(std::nothrow) cxx::spdlog_guard}
{
}

cxx::logger::~logger() = default;

void cxx::logger::initial(const std::string &filename_prefix, const std::optional<int> &log_level)
{
    if (!m_spdlog_guard)
    {
        std::cerr << __func__ << ">>>the m_spdlog_guard is nullptr." << std::endl;

        return;
    }

    m_spdlog_guard->initial(filename_prefix, log_level);
}