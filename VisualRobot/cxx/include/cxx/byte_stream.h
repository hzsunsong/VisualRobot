/**
 * @file byte_stream.h
 * @brief Byte streams for arithmetic values, byte arrays, strings, and vectors.
 */

#ifndef BYTE_STREAM_H
#define BYTE_STREAM_H
#include <vector>
#include <iterator>
#include <cassert>
#include "cxx_def.h"

namespace cxx
{
    using stream_size_type = uint32_t; ///< Default type used for serialized element counts.

	template <typename CharT, 
		typename SizeType> 
	class basic_string_stream;

	template <typename SizeType>
	class byte_array_stream;

	template <typename T,
		typename SizeType>
	class vector_stream;

	/**
	 * @brief Serializes and deserializes values using a referenced byte array.
	 * @details Read operations remove consumed bytes from the backing array.
	 * @author songhuirong
	 * @date 2023/02/07
	 *
	 * @code{.cpp}
	 * struct person
	 * {
	 * 	friend cxx::byte_stream &operator<<(cxx::byte_stream &out, const person &rhs)
	 * 	{
	 * 		return out << rhs.id << rhs.age << rhs.name;
	 * 	}
	 *
	 * 	friend cxx::byte_stream &operator>>(cxx::byte_stream &in, person &rhs)
	 * 	{
	 * 		return in >> rhs.id >> rhs.age >> rhs.name;
	 * 	}
	 *
	 * 	person() = default;
	 *
	 * 	person(const uint16_t id, const uint8_t age, const std::string &name)
	 * 	:id{ id }, age{ age }, name{ cxx::basic_string_stream<char>{name} }
	 * 	{
	 * 	}
	 *
	 * 	uint16_t id{};
	 * 	uint8_t age{};
	 * 	cxx::basic_string_stream<char> name;
	 * };
	 *
	 * void stream_struct_out()
	 * {
	 * 	cxx::byte_array ba;
	 * 	cxx::byte_stream out{ ba };
	 * 	person mengping{ 1, 30, "mengping" };
	 * 	out << mengping;
	 *
	 * 	if (!out) return;
	 *
	 * 	//...
	 * }
	 *
	 * void stream_struct_in()
	 * {
	 * 	cxx::byte_array ba;//the source byte array, it can be from the network or others.
	 * 	cxx::byte_stream in{ ba };
	 * 	person mengping;
	 * 	in >> mengping;
	 *
	 * 	if (!in) return;
	 *
	 * 	//...
	 * }
	 * @endcode
	 */
	class byte_stream
	{
	public:
		/**
		 * @brief Creates a stream backed by an existing byte array.
		 * @param[in,out] data The array used as stream storage; it must outlive this stream.
		 * @param[in] reverse Whether arithmetic writes reverse the entire backing array and arithmetic reads reverse the consumed value bytes.
		 */
		byte_stream(byte_array &data, const bool reverse = false) :m_data{ data }, m_reverse{ reverse } {}

		/**
		 * @brief Reports the status of preceding checked read operations.
		 * @return The stored status flag; false after a checked read reports insufficient data.
		 * @note This flag does not validate unchecked string or byte-array payload lengths.
		 */
		operator bool() const { return ok; }

		/**
		 * @brief Gets the backing byte array.
		 * @return A constant reference to the stream storage.
		 */
		const byte_array &data(void) const
		{
			return m_data;
		}

		/**
		 * @brief Appends the native object bytes of an arithmetic value.
		 * @note With reversal enabled, the entire backing array is reversed after appending, including earlier data.
		 * @tparam T The arithmetic value type.
		 * @param[in] val The value to write.
		 * @return A reference to this stream.
		 *
		 * @code{.cpp}
		 * cxx::byte_array ba;
		 * cxx::byte_stream out{ ba, true };
		 * uint64_t val{ 0x1234dedf };
		 * out << val;
		 *
		 * if (!out) return;
		 *
		 * //...
		 * @endcode
		 */
		template <typename T>
		std::enable_if_t<std::is_arithmetic<T>::value, byte_stream> &operator<<(const T val);

		/**
		 * @brief Reads and consumes an arithmetic value.
		 * @tparam T The arithmetic value type.
		 * @param[out] val Receives the value when sufficient bytes are available.
		 * @return A reference to this stream.
		 * @note Insufficient bytes trigger an assertion and set the stream failure flag.
		 *
		 * @code{.cpp}
		 * cxx::byte_array ba{ 0xdf, 0xde, 0x34, 0x12 };
		 * cxx::byte_stream in{ ba };
		 * uint32_t val;
		 * in >> val;
		 *
		 * if (!in) return;
		 *
		 * //...
		 * @endcode
		 */
		template <typename T>
		std::enable_if_t<std::is_arithmetic<T>::value, byte_stream> &operator>>(T &val);

		/**
		 * @brief Writes a string element count followed by that many character values converted to bytes.
		 * @note This is a one-byte-per-character representation; wider character values are not preserved.
		 * @tparam CharT The string character type.
		 * @tparam SizeType The arithmetic type used for the length prefix.
		 * @param[in] bss The wrapped string to write.
		 * @return A reference to this stream.
		 *
		 * @code{.cpp}
		 * cxx::byte_array ba;
		 * cxx::byte_stream out{ ba };
		 * cxx::basic_string_stream<char> bss{ std::string{"stream the basic string."} };
		 * out << bss;
		 *
		 * if (!out) return;
		 *
		 * //...
		 *
		 * const auto bss_size{ bss.size() };
		 * const auto bss_data{ bss.data() };
		 * @endcode
		 */
		template <typename CharT, typename SizeType = stream_size_type>
		byte_stream &operator<<(const basic_string_stream<CharT, SizeType> &bss);

		/**
		 * @brief Reads a character count and appends that many byte values to the wrapped string.
		 * @tparam CharT The string character type.
		 * @tparam SizeType The arithmetic type used for the length prefix.
		 * @param[in,out] bss The destination string wrapper.
		 * @return A reference to this stream.
		 * @pre After reading the length prefix, the backing array must contain at least that many payload bytes.
		 * @note The payload length is not checked; existing destination characters are retained.
		 *
		 * @code{.cpp}
		 * cxx::byte_array ba; ///< the source byte array, it can from the network or others.
		 * cxx::byte_stream in{ ba };
		 * cxx::basic_string_stream<char> bss;
		 * in >> bss;
		 *
		 * if (!in) return;
		 *
		 * //...
		 * @endcode
		 */
		template <typename CharT, typename SizeType = stream_size_type>
		byte_stream &operator>>(basic_string_stream<CharT, SizeType> &bss);

		/**
		 * @brief Writes an array length followed by its bytes.
		 * @tparam SizeType The arithmetic type used for the length prefix.
		 * @param[in] bas The wrapped byte array to write.
		 * @return A reference to this stream.
		 *
		 * @code{.cpp}
		 * cxx::byte_array data;
		 * data.emplace_back(0x12);
		 * data.emplace_back(0x34);
		 * data.emplace_back(0xde);
		 * data.emplace_back(0xdf);
		 *
		 * cxx::byte_array ba;
		 * cxx::byte_stream out{ ba };
		 * cxx::byte_array_stream<uint32_t> bas{ data };
		 * out << bas;
		 *
		 * if (!out) return;
		 *
		 * //...
		 * const auto bas_size{ bas.size() };
		 * const auto bas_data{ bas.data() };
		 * @endcode
		 */
		template <typename SizeType = stream_size_type>
		byte_stream &operator<<(const byte_array_stream<SizeType> &bas);

		/**
		 * @brief Reads an array length and appends bytes to the wrapped array.
		 * @tparam SizeType The arithmetic type used for the length prefix.
		 * @param[in,out] bas The destination byte array wrapper.
		 * @return A reference to this stream.
		 * @pre After reading the length prefix, the backing array must contain at least that many payload bytes.
		 * @note The payload length is not checked; existing destination bytes are retained.
		 *
		 * @code{.cpp}
		 * cxx::byte_array ba; ///< the source byte array, it can be from the network or others.
		 * cxx::byte_stream in{ ba };
		 * cxx::byte_array_stream<uint32_t> bas;
		 * in >> bas;
		 *
		 * if (!in) return;
		 * @endcode
		 */
		template <typename SizeType = stream_size_type>
		byte_stream &operator>>(byte_array_stream<SizeType> &bas);

		/**
		 * @brief Appends raw bytes without writing a length prefix.
		 * @param[in] ba The bytes to append.
		 * @return A reference to this stream.
		 *
		 * @code{.cpp}
		 * cxx::byte_array data;
		 * data.emplace_back(0x12);
		 * data.emplace_back(0x34);
		 * data.emplace_back(0xde);
		 * data.emplace_back(0xdf);
		 *
		 * cxx::byte_array ba;
		 * cxx::byte_stream out{ ba };
		 * out << data;
		 *
		 * if (!out) return;
		 *
		 * //...
		 * @endcode
		 */
		byte_stream &operator<<(const byte_array &ba)
		{
			std::copy(ba.cbegin(), ba.cend(), std::back_inserter(m_data));

			return *this;
		}

		/**
		 * @brief Reads and consumes raw bytes into a pre-sized array.
		 * @param[in,out] ba The destination array; its size specifies the number of bytes to read.
		 * @return A reference to this stream.
		 * @note Insufficient bytes trigger an assertion and set the stream failure flag; the destination is then unchanged.
		 *
		 * @code{.cpp}
		 * cxx::byte_array ba;
		 * ba.emplace_back(0x12);
		 * ba.emplace_back(0x34);
		 * ba.emplace_back(0xde);
		 * ba.emplace_back(0xdf);
		 * cxx::byte_stream in{ ba };
		 * cxx::byte_array data(ba.size());
		 * in >> data;
		 *
		 * if (!in) return;
		 * @endcode
		 */
		byte_stream &operator>>(byte_array &ba)
		{
			const auto ba_size{ ba.size() };

			if (m_data.size() < ba_size)
			{
				assert(false);
				ok = false;

				return *this;
			}

			std::copy(m_data.cbegin(), m_data.cbegin() + ba_size, ba.begin());
			m_data.erase(m_data.cbegin(), m_data.cbegin() + ba_size);

			return *this;
		}

		/**
		 * @brief Writes a vector length followed by its serialized elements.
		 * @tparam T The vector element type, which must support the corresponding stream operator.
		 * @tparam SizeType The arithmetic type used for the length prefix.
		 * @param[in] vs The wrapped vector to write.
		 * @return A reference to this stream.
		 *
		 * @code{.cpp}
		 * struct person
		 * {
		 * 	friend cxx::byte_stream &operator<<(cxx::byte_stream &out, const person &rhs)
		 * 	{
		 * 		return out << rhs.id << rhs.age << rhs.name;
		 * 	}
		 *
		 * 	friend cxx::byte_stream &operator>>(cxx::byte_stream &in, person &rhs)
		 * 	{
		 * 		return in >> rhs.id >> rhs.age >> rhs.name;
		 * 	}
		 *
		 * 	person() = default;
		 *
		 * 	person(const uint16_t id, const uint8_t age, const std::string &name)
		 * 	:id{ id }, age{ age }, name{ cxx::basic_string_stream<char>{name} }
		 * 	{
		 * 	}
		 *
		 * 	uint16_t id{};
		 * 	uint8_t age{};
		 * 	cxx::basic_string_stream<char> name;
		 * };
		 *
		 * void stream_struct_vector()
		 * {
		 * 		cxx::byte_array ba;
		 * 		cxx::byte_stream out{ ba };
		 * 		cxx::vector_stream<person> persons{ {{ 1, 30, "mengping" },
		 * 											{ 2, 35, "wangsihui" },
		 * 											{ 3, 29, "guocaolan" }} };
		 * 		out << persons;
		 *
		 * 		if (!out) return;
		 *
		 * 		//...
		 * }
		 * @endcode
		 */
		template <typename T, typename SizeType = stream_size_type>
		byte_stream &operator<<(const vector_stream<T, SizeType> &vs);

		/**
		 * @brief Reads a vector length and appends deserialized elements to the vector.
		 * @tparam T The vector element type, which must support the corresponding stream operator.
		 * @tparam SizeType The arithmetic type used for the length prefix.
		 * @param[in,out] vs The destination vector wrapper.
		 * @return A reference to this stream.
		 * @note A length or element read can set the failure flag; elements already appended are retained.
		 *
		 * @code{.cpp}
		 * struct person
		 * {
		 * 	friend cxx::byte_stream &operator<<(cxx::byte_stream &out, const person &rhs)
		 * 	{
		 * 		return out << rhs.id << rhs.age << rhs.name;
		 * 	}
		 *
		 * 	friend cxx::byte_stream &operator>>(cxx::byte_stream &in, person &rhs)
		 * 	{
		 * 		return in >> rhs.id >> rhs.age >> rhs.name;
		 * 	}
		 *
		 * 	person() = default;
		 *
		 * 	person(const uint16_t id, const uint8_t age, const std::string &name)
		 * 	:id{ id }, age{ age }, name{ cxx::basic_string_stream<char>{name} }
		 * 	{
		 * 	}
		 *
		 * 	uint16_t id{};
		 * 	uint8_t age{};
		 * 	cxx::basic_string_stream<char> name;
		 * };
		 *
		 * void stream_struct_vector()
		 * {
		 * 	cxx::byte_array ba; ///< the source byte array, it can be from the network or others.
		 * 	cxx::byte_stream in{ ba };
		 * 	cxx::vector_stream<person> persons;
		 * 	in >> persons;
		 *
		 * 	if (!in) return;
		 *
		 * 	//...
		 * }
		 * @endcode
		 */
		template <typename T, typename SizeType = stream_size_type>
		byte_stream &operator>>(vector_stream<T, SizeType> &vs);
	private:
		/**
		 * @brief Writes a data wrapper's length and elements.
		 * @tparam T A wrapper exposing size() and data().
		 * @param[in] data The data wrapper to write.
		 */
		template <typename T>
		void stream_data(const T &data);

		byte_array &m_data; ///< Borrowed storage modified by writes and consumed by reads.
		bool m_reverse{}; ///< Enables whole-buffer reversal on arithmetic writes and value-byte reversal on reads.
		bool ok{ true }; ///< Remains false after a checked read fails; subsequent operations do not reset it.
	};

	/**
	 * @brief Owns a string and a length prefix for byte stream serialization.
	 * @tparam CharT The character type.
	 * @tparam SizeType The arithmetic type used to represent the element count.
	 * @author songhuirong
	 * @date 2023/02/08
	 */
	template <typename CharT = char, 
		typename SizeType = stream_size_type>
	class basic_string_stream
	{
		friend class byte_stream;
	public:
        /**
         * @brief Copies a string into the serialization wrapper.
         * @param[in] str The string to copy.
         */
        basic_string_stream(const std::basic_string<CharT> &str = {})
            :m_str{ str }
		{ 
			m_size = static_cast<SizeType>(m_str.size()); 
		}

		/**
		 * @brief Gets the stored string element count.
		 * @return The stored element count.
		 */
		SizeType size() const { return m_size; }
		/**
		 * @brief Gets a pointer to the wrapped string data.
		 * @return A pointer to the stored elements.
		 */
		const CharT *data() const { return m_str.data(); }
		/**
		 * @brief Gets the wrapped string.
		 * @return A constant reference to the stored string.
		 */
		const std::basic_string<CharT> &str() const { return m_str; }
	private:
		SizeType m_size{}; ///< Character count captured at construction or overwritten by the last read prefix.
		std::basic_string<CharT> m_str; ///< Owns characters; reads append without clearing existing contents.
	};

	/**
	 * @brief Owns a byte array and a length prefix for byte stream serialization.
	 * @tparam SizeType The arithmetic type used to represent the element count.
	 * @author songhuirong
	 * @date 2023/02/08
	 */
	template <typename SizeType = stream_size_type>
	class byte_array_stream
	{
		friend class byte_stream;
	public:
        /**
         * @brief Copies a byte array into the serialization wrapper.
         * @param[in] data The byte array to copy.
         */
        byte_array_stream(const byte_array &data = {})
            :m_data{ data }
		{
			m_size = static_cast<SizeType>(m_data.size());
		}

		/**
		 * @brief Gets the stored byte array element count.
		 * @return The stored element count.
		 */
		SizeType size() const { return m_size; }
		/**
		 * @brief Gets a pointer to the wrapped byte array data.
		 * @return A pointer to the stored elements.
		 */
		const byte *data() const { return m_data.data(); }
		/**
		 * @brief Gets the wrapped byte array.
		 * @return A constant reference to the stored byte array.
		 */
		const byte_array &bytes() const { return m_data; }
	private:
		SizeType m_size{}; ///< Byte count captured at construction or overwritten by the last read prefix.
		byte_array m_data; ///< Owns bytes; reads append without clearing existing contents.
	};

	/**
	 * @brief Owns a vector and a length prefix for byte stream serialization.
	 * @tparam T The element type.
	 * @tparam SizeType The arithmetic type used to represent the element count.
	 * @author songhuirong
	 * @date 2023/02/08
	 */
	template <typename T, 
		typename SizeType = stream_size_type>
	class vector_stream
	{
		friend class byte_stream;
	public:
        /**
         * @brief Copies a vector into the serialization wrapper.
         * @param[in] vct The vector to copy.
         */
        vector_stream(const std::vector<T> &vct = {})
            :m_vct{ vct }
		{
			m_size = static_cast<SizeType>(m_vct.size());
		}

		/**
		 * @brief Gets the stored vector element count.
		 * @return The stored element count.
		 */
		SizeType size() const { return m_size; }
		/**
		 * @brief Gets a pointer to the wrapped vector data.
		 * @return A pointer to the stored elements.
		 */
		const T *data() const { return m_vct.data(); }
		/**
		 * @brief Gets the wrapped vector.
		 * @return A constant reference to the stored vector.
		 */
		const std::vector<T> &vct() const { return m_vct; }
	private:
		SizeType m_size{}; ///< Element count captured at construction or overwritten by the last read prefix.
		std::vector<T> m_vct; ///< Owns elements; reads append without clearing existing contents.
	};
}

#endif // BYTE_STREAM_H