#ifndef vec2_h
#define vec2_h

#include <iostream>
#include <cmath>
#include <cassert>
#include <compare>

// raylib uses Vector2, so I need to use Vec2 to not trigger redefinition error
class Vec2
{
public:
    float x = .0f;
    float y = .0f;

    constexpr auto operator<=>(const Vec2 &) const = default;

    constexpr Vec2 &operator+=(Vec2 v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    constexpr Vec2 &operator-=(Vec2 v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    constexpr Vec2 &operator*=(float f)
    {
        x *= f;
        y *= f;
        return *this;
    }

    constexpr Vec2 &operator*=(Vec2 v)
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    constexpr Vec2 &operator/=(float f)
    {
        x /= f;
        y /= f;
        return *this;
    }

    constexpr Vec2 &operator/=(Vec2 v)
    {
        x /= v.x;
        y /= v.y;
        return *this;
    }
    float length_squared() const
    {
        return x * x + y * y;
    }

    float length() const
    {
        return std::sqrt(length_squared());
    }

    Vec2 normalized() const
    {
        const float len = this->length();
        if (len == 0.0f)
            return Vec2(0.0f, 0.0f);
        else
            return *this / len;
    }
};

constexpr Vec2 operator+(Vec2 v, Vec2 u)
{
    v += u;
    return v;
}

constexpr Vec2 operator-(Vec2 v, Vec2 u)
{
    v -= u;
    return v;
}

constexpr Vec2 operator*(Vec2 v, Vec2 u)
{
    v *= u;
    return v;
}

constexpr Vec2 operator*(Vec2 v, float f)
{
    v *= f;
    return v;
}

constexpr Vec2 operator*(float f, Vec2 v)
{
    v *= f;
    return v;
}

constexpr Vec2 operator/(Vec2 v, Vec2 u)
{
    v /= u;
    return v;
}

constexpr Vec2 operator/(Vec2 v, float f)
{
    v /= f;
    return v;
}

inline float dot(const Vec2 &u, const Vec2 &v)
{
    return (u.x * v.x) + (u.y * v.y);
}

#endif