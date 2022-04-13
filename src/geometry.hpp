#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <utility>

template <std::size_t size, typename Ts, typename Done = std::enable_if<std::is_arithmetic_v<Ts>>>
struct Point
// template <std::size_t size, typename Ts> struct Point
{
    std::array<Ts, size> values {};

    Point() = default;

    template <typename... Args>
    Point(Ts type, Args&&... params) : values { type, static_cast<Ts>(std::forward<Args>(params))... }
    {
        static_assert(sizeof...(params) + 1 == size);
        // std::cout << "sizeof...(params) = " << sizeof...(params) << std::endl;
    }

    // Point(Ts x, Ts y) : values { x, y } { static_assert(size == 2); }
    // Point(Ts x, Ts y, Ts z) : values { x, y, z } { static_assert(size == 3); }

    Ts& x() { return values[0]; }
    Ts x() const { return values[0]; }

    Ts& y()
    {
        static_assert(size >= 2);
        return values[1];
    }
    Ts y() const
    {
        static_assert(size >= 2);
        return values[1];
    }

    Ts& z()
    {
        static_assert(size >= 3);
        return values[2];
    }
    Ts z() const
    {
        static_assert(size >= 3);
        return values[2];
    }

    Point& operator+=(const Point& other)
    {

        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::plus<Ts>());

        return *this;
    }

    Point& operator-=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::minus<Ts>());
        return *this;
    }

    Point& operator*=(const Ts scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(), [scalar](Ts v) { return v * scalar; });
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

    Point operator*(const Ts scalar) const
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

    Point<size, Ts>& operator*=(const Point<size, Ts>& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       [](Ts v1, Ts v2) { return v1 * v2; });
        return *this;
    }

    Ts length() const
    {
        return std::sqrt(
            std::reduce(values.begin(), values.end(), 0.0, [](Ts acc, Ts cur) { return acc + (cur * cur); }));
    }
    Ts distance_to(const Point& other) const { return (*this - other).length(); }

    Point& normalize(const Ts target_len = 1.0f)
    {
        const Ts current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point& cap_length(const Ts max_len)
    {
        assert(max_len > 0);

        const Ts current_len = length();
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
