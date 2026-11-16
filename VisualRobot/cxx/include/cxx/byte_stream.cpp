/**
 * @file byte_stream.cpp
 * @brief Template serialization and consuming read implementations for byte streams.
 */

#include <algorithm>
#include "byte_stream.h"

/**
 * @brief Appends the native object bytes of an arithmetic value.
 * @note With reversal enabled, the entire backing array is reversed after appending, including earlier data.
 * @tparam T The arithmetic value type.
 * @param[in] val The value to write.
 * @return A reference to this stream.
 */
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, cxx::byte_stream> &cxx::byte_stream::operator<<(const T val)
{
    const byte *data{ reinterpret_cast<const byte *>(&val) };
	const auto data_size{ sizeof(val) };
	std::copy(data, data + data_size, std::back_inserter(m_data));

	if (m_reverse)
	{
		std::reverse(m_data.begin(), m_data.end());
	}

	return *this;
}

/**
 * @brief Reads and consumes an arithmetic value.
 * @tparam T The arithmetic value type.
 * @param[out] val Receives the value when sufficient bytes are available.
 * @return A reference to this stream.
 * @note Insufficient bytes trigger an assertion and set the stream failure flag.
 */
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, cxx::byte_stream> &cxx::byte_stream::operator>>(T &val)
{
	const auto data_size{ sizeof(val) };
	const auto byte_size{ m_data.size() };

	if (byte_size >= data_size)
	{
		if (m_reverse)
		{
			std::reverse(m_data.begin(), m_data.begin() + data_size);
		}

		val = *reinterpret_cast<T *>(m_data.data());
		m_data.erase(m_data.cbegin(), m_data.cbegin() + data_size);
	}
	else
	{
		assert(false);
		ok = false;
	}

	return *this;
}

/**
 * @brief Writes a string element count followed by that many character values converted to bytes.
 * @note This is a one-byte-per-character representation; wider character values are not preserved.
 * @tparam CharT The string character type.
 * @tparam SizeType The arithmetic type used for the length prefix.
 * @param[in] bss The wrapped string to write.
 * @return A reference to this stream.
 */
template <typename CharT, typename SizeType /*= stream_length_type*/>
cxx::byte_stream &cxx::byte_stream::operator<<(const basic_string_stream<CharT, SizeType> &bss)
{
	stream_data(bss);

	return *this;
}

/**
 * @brief Reads a character count and appends that many byte values to the wrapped string.
 * @tparam CharT The string character type.
 * @tparam SizeType The arithmetic type used for the length prefix.
 * @param[in,out] bss The destination string wrapper.
 * @return A reference to this stream.
 * @pre After reading the length prefix, the backing array must contain at least that many payload bytes.
 * @note The payload length is not checked; existing destination characters are retained.
 */
template <typename CharT, typename SizeType /*= stream_size_type*/>
cxx::byte_stream &cxx::byte_stream::operator>>(basic_string_stream<CharT, SizeType> &bss)
{
	*this >> bss.m_size;

	if (*this)
	{
		std::copy(m_data.cbegin(), m_data.cbegin() + bss.size(), std::back_inserter(bss.m_str));
		m_data.erase(m_data.cbegin(), m_data.cbegin() + bss.size());
	}

	return *this;
}

/**
 * @brief Writes an array length followed by its bytes.
 * @tparam SizeType The arithmetic type used for the length prefix.
 * @param[in] bas The wrapped byte array to write.
 * @return A reference to this stream.
 */
template <typename SizeType /*= stream_size_type*/>
cxx::byte_stream &cxx::byte_stream::operator<<(const byte_array_stream<SizeType> &bas)
{
	stream_data(bas);

	return *this;
}

/**
 * @brief Reads an array length and appends bytes to the wrapped array.
 * @tparam SizeType The arithmetic type used for the length prefix.
 * @param[in,out] bas The destination byte array wrapper.
 * @return A reference to this stream.
 * @pre After reading the length prefix, the backing array must contain at least that many payload bytes.
 * @note The payload length is not checked; existing destination bytes are retained.
 */
template <typename SizeType /*= stream_size_type*/>
cxx::byte_stream &cxx::byte_stream::operator>>(byte_array_stream<SizeType> &bas)
{
	*this >> bas.m_size;

	if (*this)
	{
		std::copy(m_data.cbegin(), m_data.cbegin() + bas.size(), std::back_inserter(bas.m_data));
		m_data.erase(m_data.cbegin(), m_data.cbegin() + bas.size());
	}

	return *this;
}

/**
 * @brief Writes a vector length followed by its serialized elements.
 * @tparam T The vector element type, which must support the corresponding stream operator.
 * @tparam SizeType The arithmetic type used for the length prefix.
 * @param[in] vs The wrapped vector to write.
 * @return A reference to this stream.
 */
template <typename T, typename SizeType /*= stream_length_type*/>
cxx::byte_stream &cxx::byte_stream::operator<<(const vector_stream<T, SizeType> &vs)
{
	const auto data_size{ vs.size() };
	*this << data_size;

    for_each(vs.vct().cbegin(), vs.vct().cend(), [&](const auto &v)
	{
		*this << v;
	});

	return *this;
}

/**
 * @brief Reads a vector length and appends deserialized elements to the vector.
 * @tparam T The vector element type, which must support the corresponding stream operator.
 * @tparam SizeType The arithmetic type used for the length prefix.
 * @param[in,out] vs The destination vector wrapper.
 * @return A reference to this stream.
 * @note A length or element read can set the failure flag; elements already appended are retained.
 */
template <typename T, typename SizeType /*= stream_size_type*/>
cxx::byte_stream &cxx::byte_stream::operator>>(vector_stream<T, SizeType> &vs)
{
	*this >> vs.m_size;

	if (*this)
	{
		for (auto i{ 0u }; i < vs.size(); ++i)
		{
			T t;
			*this >> t;

			if (*this)
			{
				vs.m_vct.emplace_back(t);
			}
		}
	}

	return *this;
}

/**
 * @brief Writes a wrapper length and copies size() elements into the byte buffer.
 * @tparam T A wrapper exposing size() and data().
 * @param[in] data The wrapper whose element count is serialized before its data.
 * @note Each element is converted to a byte; this does not copy multi-byte object representations.
 */
template <typename T>
void cxx::byte_stream::stream_data(const T &data)
{
	const auto data_size{ data.size() };
	*this << data_size;
	std::copy(data.data(), data.data() + data_size, std::back_inserter(m_data));
}