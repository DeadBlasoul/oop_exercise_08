#pragma once

#include <iostream>
#include <cstddef>
#include <cmath>

template <typename _Type, size_t _Dimensions>
struct point {
    static_assert(_Dimensions != 0, "can not create 0d point");

    using value_type = _Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = pointer;
    using const_iterator = const_pointer;

    value_type dots[_Dimensions];

    [[nodiscard]] value_type& operator[](size_t ix) noexcept {
        return dots[ix];
    }

    [[nodiscard]] const value_type& operator[](size_t ix) const noexcept {
        return const_cast<point&>(*this).operator[](ix);
    }

    [[nodiscard]] iterator begin() noexcept {
        return &dots[0];
    }

    [[nodiscard]] const_iterator begin() const noexcept {
        return const_cast<point&>(*this).begin();
    }

    [[nodiscard]] iterator end() noexcept {
        return &dots[_Dimensions];
    }

    [[nodiscard]] const_iterator end() const noexcept {
        return const_cast<point&>(*this).end();
    }

    [[nodiscard]] static constexpr size_t size() noexcept {
        return _Dimensions;
    }

    [[nodiscard]] point operator+(const point& other) const {
        point result = *this;

        for (size_t i = 0; i < result.size(); i++) {
            result[i] += other[i];
        }

        return result;
    }

    [[nodiscard]] point operator-(const point& other) const {
        point result = *this;

        for (size_t i = 0; i < result.size(); i++) {
            result[i] -= other[i];
        }

        return result;
    }
};

template <typename Type, size_t _Dims>
std::ostream& operator<<(std::ostream& stream, const point<Type, _Dims>& p) {
    stream << "{ ";
    for (const auto& d : p) {
        stream << d << " ";
    }
    stream << "}";

    return stream;
}

template <typename _Type, size_t _Dims>
std::istream& operator>>(std::istream& stream, point<_Type, _Dims>& p) {
    for (auto& d : p) {
        stream >> d;
    }

    return stream;
}

// Examples:
using point2d = point<double, 2>;

inline double distance(const point2d& left, const point2d& right) {
    const double x = left[0] - right[0];
    const double y = left[1] - right[1];
    return std::sqrt((x * x) + (y * y));
}
