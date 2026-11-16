/**
 * @file simple_vector.cpp
 * @brief Component-order vector serialization and component-wise vector operations.
 */

#include "simple_vector.h"
#include "byte_stream.h"
#include "byte_stream.cpp"

template <typename T>
cxx::byte_stream &cxx::operator<<(cxx::byte_stream &out, const cxx::vector2<T> &rhs)
{
    return out << rhs.x << rhs.y;
}

template <typename T>
cxx::byte_stream &cxx::operator>>(cxx::byte_stream &in, cxx::vector2<T> &rhs)
{
    return in >> rhs.x >> rhs.y;
}

template <typename T>
cxx::byte_stream &cxx::operator<<(cxx::byte_stream &out, const cxx::vector3<T> &rhs)
{
    return out << rhs.x << rhs.y << rhs.z;
}

template <typename T>
cxx::byte_stream &cxx::operator>>(cxx::byte_stream &in, cxx::vector3<T> &rhs)
{
    return in >> rhs.x >> rhs.y >> rhs.z;
}

template <typename T>
cxx::byte_stream &cxx::operator<<(cxx::byte_stream &out, const cxx::vector4<T> &rhs)
{
    return out << rhs.x << rhs.y << rhs.z << rhs.w;
}

template <typename T>
cxx::byte_stream &cxx::operator>>(cxx::byte_stream &in, cxx::vector4<T> &rhs)
{
    return in >> rhs.x >> rhs.y >> rhs.z >> rhs.w;
}

template <typename T>
bool cxx::isFinityVector2(const vector2<T> &a)
{
    return std::isfinite(a.x) && std::isfinite(a.y);
}

template <typename T>
bool cxx::isFinityVector3(const vector3<T> &a)
{
    return std::isfinite(a.x) && std::isfinite(a.y) &&
           std::isfinite(a.z);
}

template <typename T>
bool cxx::isFinityVector4(const vector4<T> &a)
{
    return std::isfinite(a.x) && std::isfinite(a.y) &&
           std::isfinite(a.z) && std::isfinite(a.w);
}

template <typename T>
cxx::vector2<T> cxx::minVector2(const vector2<T> &a, const vector2<T> &b)
{
    return vector2<T>{std::min(a.x, b.x), std::min(a.y, b.y)};
}

template <typename T>
cxx::vector3<T> cxx::minVector3(const vector3<T> &a, const vector3<T> &b)
{
    return vector3<T>{std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
}

template <typename T>
cxx::vector4<T> cxx::minVector4(const vector4<T> &a, const vector4<T> &b)
{
    return vector4<T>{std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w)};
}

template <typename T>
cxx::vector2<T> cxx::maxVector2(const vector2<T> &a, const vector2<T> &b)
{
    return vector2<T>{std::max(a.x, b.x), std::max(a.y, b.y)};
}

template <typename T>
cxx::vector3<T> cxx::maxVector3(const vector3<T> &a, const vector3<T> &b)
{
    return vector3<T>{std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
}

template <typename T>
cxx::vector4<T> cxx::maxVector4(const vector4<T> &a, const vector4<T> &b)
{
    return vector4<T>{std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w)};
}