#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <utility>

// c++17 SFINAE, Point could be instancied with only arithemtic type
// template <std::size_t size, typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
// struct Point

// c++20 concept feature for an explicit error msg if type is not arithmetic
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;
template <std::size_t size, Arithmetic T> struct Point
{
    std::array<T, size> values {};

    Point() = default;

    template <typename... Args>
    Point(T type, Args&&... params) : values { type, static_cast<T>(std::forward<Args>(params))... }
    {
        static_assert(sizeof...(params) + 1 == size);
    }

    T& x() { return values[0]; }
    T x() const { return values[0]; }

    T& y()
    {
        static_assert(size >= 2);
        return values[1];
    }
    T y() const
    {
        static_assert(size >= 2);
        return values[1];
    }

    T& z()
    {
        static_assert(size >= 3);
        return values[2];
    }
    T z() const
    {
        static_assert(size >= 3);
        return values[2];
    }

    Point& operator+=(const Point& other)
    {

        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::plus<T>());

        return *this;
    }

    Point& operator-=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::minus<T>());
        return *this;
    }

    Point& operator*=(const T scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(), [scalar](T v) { return v * scalar; });
        return *this;
    }

    Point operator+(const Point& other) const
    {
        Point result = *this;
        result += other;
        return result;
    }

    Point operator-(const Point& other) const
    {
        Point result = *this;
        result -= other;
        return result;
    }

    Point operator*(const T scalar) const
    {
        Point result = *this;
        result *= scalar;
        return result;
    }

    Point operator-() const { return Point { -x(), -y(), -z() }; }

    Point operator*(const Point& other) const
    {
        Point result = *this;
        result *= other;
        return result;
    }

    Point& operator*=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       [](T v1, T v2) { return v1 * v2; });
        return *this;
    }

    double length() const
    {
        return std::sqrt(std::reduce(values.begin(), values.end(), 0.l,
                                     [](double acc, T cur) { return acc + (cur * cur); }));
    }

    double distance_to(const Point& other) const { return (*this - other).length(); }

    Point& normalize(const double target_len = 1.l)
    {
        const double current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point& cap_length(const double max_len)
    {
        assert(max_len > 0);

        const double current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }
};

using Point2D = Point<2, float>;
using Point3D = Point<3, float>;

// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}
