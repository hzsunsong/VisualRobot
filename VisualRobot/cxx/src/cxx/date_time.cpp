/**
 * @file date_time.cpp
 * @brief Local calendar time extraction and fractional time formatting.
 */

#include <sstream>
#include <iomanip>
#include <cassert>
#include "date_time.h"

namespace
{
	static const std::string PRECISION_SPLITTER{ "." };
}

/**
 * @brief Gets the remainder after subtracting a coarser epoch count.
 * @param[in] t1 The epoch count in the finer unit.
 * @param[in] t2 The epoch count in a unit 1000 times coarser.
 * @return The remainder in the finer unit.
 */
std::time_t convert_time(const std::time_t t1, const std::time_t t2)
{
	return t1 - t2 * 1000;
}

/**
 * @brief Formats the fractional components for the requested precision.
 * @param[in] precision_t The requested precision.
 * @param[in] tm_date_time_t The date-time components to format.
 * @return Period-separated fractional components, or an empty string for seconds.
 */
std::string precision_string(const cxx::date_time::precision precision_t, const cxx::tm_date_time tm_date_time_t)
{
	std::string precision_str;

	switch (precision_t)
	{
	case cxx::date_time::precision::second:
		break;
	case cxx::date_time::precision::millisecond:
	{
		precision_str = PRECISION_SPLITTER + std::to_string(tm_date_time_t.m_millisec);
	}
		break;
	case cxx::date_time::precision::microsecond:
	{
		precision_str = PRECISION_SPLITTER + std::to_string(tm_date_time_t.m_millisec)
			+ PRECISION_SPLITTER + std::to_string(tm_date_time_t.m_microsec);
	}
		break;
	case cxx::date_time::precision::nanosecond:
	{
		precision_str = PRECISION_SPLITTER + std::to_string(tm_date_time_t.m_millisec)
			+ PRECISION_SPLITTER + std::to_string(tm_date_time_t.m_microsec)
			+ PRECISION_SPLITTER + std::to_string(tm_date_time_t.m_nanosec);
	}
		break;
	default:
	{
		assert(false);
	}
		break;
	}

	return precision_str;
}

/**
 * @brief Reads the current local time and its subsecond components.
 * @return A tuple containing a success flag, the date-time components, and local calendar time.
 */
std::tuple<bool, cxx::tm_date_time, std::tm> make_date_time()
{
	const auto now_tp{ std::chrono::system_clock::now() };
	const auto tt{ std::chrono::system_clock::to_time_t(now_tp) };
	std::tm tm_t;

	if (localtime_s(&tm_t, &tt)) return{};

	cxx::tm_date_time tm_date_time_t;
	tm_date_time_t.m_year = tm_t.tm_year + cxx::DATE_TIME_SINCE_YEAR;
	tm_date_time_t.m_mon = tm_t.tm_mon + cxx::DATE_TIME_MON_DIFF;
	tm_date_time_t.m_mday = tm_t.tm_mday;
	tm_date_time_t.m_hour = tm_t.tm_hour;
	tm_date_time_t.m_min = tm_t.tm_min;
	tm_date_time_t.m_sec = tm_t.tm_sec;

	const auto sec_count{ std::chrono::time_point_cast<std::chrono::seconds>(now_tp).time_since_epoch().count() };
	const auto millisec_count{ std::chrono::time_point_cast<std::chrono::milliseconds>(now_tp).time_since_epoch().count() };
	const auto millisec{ convert_time(millisec_count, sec_count) };
	tm_date_time_t.m_millisec = millisec;
	const auto microsec_count{ std::chrono::time_point_cast<std::chrono::microseconds>(now_tp).time_since_epoch().count() };
	const auto microsec{ convert_time(microsec_count, millisec_count) };
	tm_date_time_t.m_microsec = microsec;
	const auto nanosec_count{ std::chrono::time_point_cast<std::chrono::nanoseconds>(now_tp).time_since_epoch().count() };
	const auto nanosec{ convert_time(nanosec_count, microsec_count) };
	tm_date_time_t.m_nanosec = nanosec;

	return std::make_tuple(true, tm_date_time_t, tm_t);
}

std::tuple<bool, cxx::tm_date_time> cxx::date_time::current()
{
	const auto tp{ make_date_time() };
	auto ok{ false };
	tm_date_time tm_date_time_t;
	std::tie(ok, tm_date_time_t, std::ignore) = tp;

	return std::make_tuple(ok, tm_date_time_t);
}

std::string cxx::date_time::current(const precision precision_t, const std::string &format /*= DEFAULT_DATE_TIME_FORMAT*/)
{
	const auto tp{ make_date_time() };
	auto ok{ false };
	tm_date_time tm_date_time_t;
	std::tm tm_t;
	std::tie(ok, tm_date_time_t, tm_t) = tp;

	if (!ok) return{};

	std::ostringstream oss;
	oss << std::put_time(&tm_t, format.data());

	return oss.str() + precision_string(precision_t, tm_date_time_t);
}