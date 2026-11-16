/**
 * @file date_time.h
 * @brief Local date and time utilities with subsecond components.
 * @author songhuirong
 * @date 2023/02/10
 */

#ifndef DATE_TIME_H
#define DATE_TIME_H
#include <string>
#include <chrono>
#include <ctime>
#include <tuple>
#include "cxx_export.h"
#include "cxx_def.h"

namespace cxx
{
    /**
     * @brief Stores local calendar time and subsecond components.
     * @details The year is the full calendar year; the month is in the range 1 to 12.
     * Each subsecond field holds the remainder relative to the preceding unit.
     */
    struct CXX_EXPORT tm_date_time
	{
		decltype(std::declval<std::tm>().tm_year) m_year{}; ///< Full calendar year, including the std::tm base-year offset.
		decltype(std::declval<std::tm>().tm_mon) m_mon{}; ///< Calendar month in the range 1 to 12.
		decltype(std::declval<std::tm>().tm_mday) m_mday{}; ///< Day of the month in the range 1 to 31.
		decltype(std::declval<std::tm>().tm_hour) m_hour{}; ///< Local hour in the range 0 to 23.
		decltype(std::declval<std::tm>().tm_min) m_min{}; ///< Minute within the hour, in the range 0 to 59.
		decltype(std::declval<std::tm>().tm_sec) m_sec{}; ///< Second within the minute, as reported by std::tm.
		decltype(std::declval<std::chrono::milliseconds>().count()) m_millisec{}; ///< Milliseconds remaining after whole seconds, normally 0 to 999.
		decltype(std::declval<std::chrono::microseconds>().count()) m_microsec{}; ///< Microseconds remaining after whole milliseconds, normally 0 to 999.
		decltype(std::declval<std::chrono::nanoseconds>().count()) m_nanosec{}; ///< Nanoseconds remaining after whole microseconds, normally 0 to 999.
	};

    /**
     * @brief Provides the current local time as components or a formatted string.
     *
     * @code{.cpp}
     * const auto tm_dt{ cxx::date_time::current() };
     * const auto tm_t{ cxx::date_time::current(cxx::date_time::precision::millisecond) };
     * @endcode
     */
    class CXX_EXPORT date_time
	{
	public:
		/**
		 * @brief Selects the fractional time components appended to formatted output.
		 */
		enum class precision
		{
			second, ///< Second precision.
			millisecond, ///< Millisecond precision.
			microsecond, ///< Microsecond precision.
			nanosecond ///< Nanosecond precision.
		};

		/**
		 * @brief Gets the current local time and subsecond components.
		 * @return A tuple containing a success flag followed by the date-time components.
		 */
		static std::tuple<bool, tm_date_time> current();

		/**
		 * @brief Formats the current local time with the requested fractional precision.
		 * @param[in] precision_t The finest fractional component to include.
		 * @param[in] format The calendar time format passed to std::put_time.
		 * @return The formatted local time, or an empty string if conversion fails.
		 * @note Fractional components are appended with period separators without zero-padding.
		 */
		static std::string current(const precision precision_t, const std::string &format = DEFAULT_DATE_TIME_FORMAT);
	};
}

#endif // DATE_TIME_H