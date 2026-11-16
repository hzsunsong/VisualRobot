/**
 * @file simple_vector.h
 * @brief Fixed-dimension vectors and component-wise utilities.
 */

#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

namespace cxx
{
class byte_stream;

template <typename T> struct vector2;
/**
 * @brief Writes a two-dimensional vector in component order.
 * @tparam T The component type.
 * @param[in,out] out The byte stream.
 * @param[in] rhs The vector to serialize.
 * @return A reference to out.
 */
template <typename T>
cxx::byte_stream &operator<<(cxx::byte_stream &out, const vector2<T> &rhs);
/**
 * @brief Reads a two-dimensional vector in component order.
 * @tparam T The component type.
 * @param[in,out] in The byte stream.
 * @param[out] rhs Receives the deserialized components.
 * @return A reference to in.
 */
template <typename T>
cxx::byte_stream &operator>>(cxx::byte_stream &in, vector2<T> &rhs);

/**
 * @brief Stores a two-dimensional vector.
 * @tparam T The component type.
 */
template <typename T>
struct vector2
{
    friend cxx::byte_stream &operator<<<>(cxx::byte_stream &out, const vector2<T> &rhs);
    friend cxx::byte_stream &operator>><>(cxx::byte_stream &in, vector2<T> &rhs);

    T x{}; ///< The x component.
    T y{}; ///< The y component.
};

template <typename T> struct vector3;
/**
 * @brief Writes a three-dimensional vector in component order.
 * @tparam T The component type.
 * @param[in,out] out The byte stream.
 * @param[in] rhs The vector to serialize.
 * @return A reference to out.
 */
template <typename T>
cxx::byte_stream &operator<<(cxx::byte_stream &out, const vector3<T> &rhs);
/**
 * @brief Reads a three-dimensional vector in component order.
 * @tparam T The component type.
 * @param[in,out] in The byte stream.
 * @param[out] rhs Receives the deserialized components.
 * @return A reference to in.
 */
template <typename T>
cxx::byte_stream &operator>>(cxx::byte_stream &in, vector3<T> &rhs);

/**
 * @brief Stores a three-dimensional vector.
 * @tparam T The component type.
 */
template <typename T>
struct vector3
{
    friend cxx::byte_stream &operator<<<>(cxx::byte_stream &out, const vector3<T> &rhs);
    friend cxx::byte_stream &operator>><>(cxx::byte_stream &in, vector3<T> &rhs);

    T x{}; ///< The x component.
    T y{}; ///< The y component.
    T z{}; ///< The z component.
};

template <typename T> struct vector4;
/**
 * @brief Writes a four-dimensional vector in component order.
 * @tparam T The component type.
 * @param[in,out] out The byte stream.
 * @param[in] rhs The vector to serialize.
 * @return A reference to out.
 */
template <typename T>
cxx::byte_stream &operator<<(cxx::byte_stream &out, const vector4<T> &rhs);
/**
 * @brief Reads a four-dimensional vector in component order.
 * @tparam T The component type.
 * @param[in,out] in The byte stream.
 * @param[out] rhs Receives the deserialized components.
 * @return A reference to in.
 */
template <typename T>
cxx::byte_stream &operator>>(cxx::byte_stream &in, vector4<T> &rhs);

/**
 * @brief Stores a four-dimensional vector.
 * @tparam T The component type.
 */
template <typename T>
struct vector4
{
    friend cxx::byte_stream &operator<<<>(cxx::byte_stream &out, const vector4<T> &rhs);
    friend cxx::byte_stream &operator>><>(cxx::byte_stream &in, vector4<T> &rhs);

    T x{}; ///< The x component.
    T y{}; ///< The y component.
    T z{}; ///< The z component.
    T w{}; ///< The w component.
};

/**
 * @brief Checks whether every component of a two-dimensional vector is finite.
 * @tparam T The component type.
 * @param[in] a The vector to inspect.
 * @return True if all components are finite.
 */
template <typename T>
bool isFinityVector2(const vector2<T> &a);

/**
 * @brief Checks whether every component of a three-dimensional vector is finite.
 * @tparam T The component type.
 * @param[in] a The vector to inspect.
 * @return True if all components are finite.
 */
template <typename T>
bool isFinityVector3(const vector3<T> &a);

/**
 * @brief Checks whether every component of a four-dimensional vector is finite.
 * @tparam T The component type.
 * @param[in] a The vector to inspect.
 * @return True if all components are finite.
 */
template <typename T>
bool isFinityVector4(const vector4<T> &a);

/**
 * @brief Gets the component-wise minimum of two two-dimensional vectors.
 * @tparam T The component type.
 * @param[in] a The first vector.
 * @param[in] b The second vector.
 * @return A vector containing the minimum of each corresponding component pair.
 */
template <typename T>
vector2<T> minVector2(const vector2<T> &a, const vector2<T> &b);

/**
 * @brief Gets the component-wise minimum of two three-dimensional vectors.
 * @tparam T The component type.
 * @param[in] a The first vector.
 * @param[in] b The second vector.
 * @return A vector containing the minimum of each corresponding component pair.
 */
template <typename T>
vector3<T> minVector3(const vector3<T> &a, const vector3<T> &b);

/**
 * @brief Gets the component-wise minimum of two four-dimensional vectors.
 * @tparam T The component type.
 * @param[in] a The first vector.
 * @param[in] b The second vector.
 * @return A vector containing the minimum of each corresponding component pair.
 */
template <typename T>
vector4<T> minVector4(const vector4<T> &a, const vector4<T> &b);

/**
 * @brief Gets the component-wise maximum of two two-dimensional vectors.
 * @tparam T The component type.
 * @param[in] a The first vector.
 * @param[in] b The second vector.
 * @return A vector containing the maximum of each corresponding component pair.
 */
template <typename T>
vector2<T> maxVector2(const vector2<T> &a, const vector2<T> &b);

/**
 * @brief Gets the component-wise maximum of two three-dimensional vectors.
 * @tparam T The component type.
 * @param[in] a The first vector.
 * @param[in] b The second vector.
 * @return A vector containing the maximum of each corresponding component pair.
 */
template <typename T>
vector3<T> maxVector3(const vector3<T> &a, const vector3<T> &b);

/**
 * @brief Gets the component-wise maximum of two four-dimensional vectors.
 * @tparam T The component type.
 * @param[in] a The first vector.
 * @param[in] b The second vector.
 * @return A vector containing the maximum of each corresponding component pair.
 */
template <typename T>
vector4<T> maxVector4(const vector4<T> &a, const vector4<T> &b);
}

#endif // SIMPLE_VECTOR_H
