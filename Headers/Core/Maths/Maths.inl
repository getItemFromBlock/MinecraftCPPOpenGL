#include <cstdio>

#include "Maths.hpp"
#include "Core/Debug/Assert.hpp"

#include <corecrt_math_defines.h>

namespace Core::Maths
{

    // -----------------------   IVec2    -----------------------

    inline IVec2::IVec2(const Vec2& in) : x((int)in.x), y((int)in.y) {}

    inline IVec2 IVec2::operator+(const IVec2& a) const
    {
        IVec2 res = IVec2(a.x + this->x, a.y + this->y);
        return res;
    }

    inline IVec2 IVec2::operator-(const IVec2& a) const
    {
        return IVec2(x - a.x, y - a.y);
    }

    inline IVec2 IVec2::operator-() const
    {
        return IVec2(-x, -y);
    }

    inline IVec2 IVec2::operator*(const IVec2& a) const
    {
        IVec2 res = IVec2(this->x * a.x, this->y * a.y);
        return res;
    }

    inline IVec2 IVec2::operator*(const float& a) const
    {
        IVec2 res = IVec2(this->x * (int)a, this->y * (int)a);
        return res;
    }

    inline IVec2 IVec2::operator/(const float& a) const
    {
        if ((int)a == 0)
            return IVec2(_CRT_INT_MAX, _CRT_INT_MAX);
        IVec2 res = IVec2(x / (int)a, y / (int)a);
        return res;
    }

    inline bool IVec2::operator==(const IVec2& b) const
    {
        return (x == b.x && y == b.y);
    }

    // -----------------------   Vec2    -----------------------

    inline float Vec2::lengthSquared() const
    {
        return (x * x + y * y);
    }

    inline float Vec2::getLength() const
    {
        return sqrtf(lengthSquared());
    }

    inline Vec2 Vec2::operator+(const Vec2& a) const
    {
        Vec2 res = Vec2(a.x + this->x, a.y + this->y);
        return res;
    }

    inline Vec2 Vec2::operator-(const Vec2& a) const
    {
        Vec2 res = Vec2(this->x - a.x, this->y - a.y);
        return res;
    }

    inline Vec2 Vec2::operator-() const
    {
        return this->negate();
    }

    inline Vec2 Vec2::operator*(const Vec2& a) const
    {
        Vec2 res = Vec2(this->x * a.x, this->y * a.y);
        return res;
    }

    inline Vec2 Vec2::operator*(const float& a) const
    {
        Vec2 res = Vec2(this->x * a, this->y * a);
        return res;
    }

    inline Vec2 Vec2::operator/(const float& a) const
    {
        if (a == 0.0)
            return operator*(VEC_HIGH_VALUE);
        Vec2 res = operator*(1 / a);
        return res;
    }

    inline bool Vec2::operator==(const Vec2& b) const
    {
        return (x == b.x && y == b.y);
    }

    inline float& Vec2::operator[](const size_t a)
    {
        return *((&x) + a);
    }

    inline const float& Vec2::operator[](const size_t a) const
    {
        return *((&x) + a);
    }

    inline bool Vec2::isCollinearWith(Vec2 a) const
    {
        float res = a.x * y - a.y * x;
        return (res < VEC_COLLINEAR_PRECISION);
    }

    inline float Vec2::dotProduct(Vec2 a) const
    {
        return (a.x * x + a.y * y);
    }

    inline float Vec2::crossProduct(Vec2 a) const
    {
        return (x * a.y - y * a.x);
    }

    inline Vec2 Vec2::unitVector() const
    {
        return operator/(getLength());
    }

    inline Vec2 Vec2::negate() const
    {
        return operator*(-1);
    }

    inline Vec2 Vec2::getNormal() const
    {
        return Vec2(-y, x);
    }

    inline bool Vec2::isIntEquivalent(Vec2 a) const
    {
        return ((int)x == (int)a.x && (int)y == a.y);
    }

    inline float Vec2::getDistanceFromPoint(Vec2 a) const
    {
        float i = a.x - x;
        float j = a.y - y;
        return sqrtf(i * i + j * j);
    }

    inline float Core::Maths::Vec2::GetAngle() const
    {
        if (lengthSquared() == 0.0f) return 0.0f;
        Vec2 tmp = unitVector();
        float outValue;
        if (tmp.x == 0)
        {
            if (tmp.y > 0)
                outValue = (float)M_PI / 2.0f;
            else
                outValue = -(float)M_PI / 2.0f;
        }
        else
        {
            outValue = (atanf(tmp.y / tmp.x));
            if (tmp.x < 0)
            {
                outValue += (float)M_PI;
            }
        }
        return Util::toDegrees(outValue);
    }

    inline bool Vec2::isNearlyEqual(Vec2 a, float prec)
    {
        return (fabsf(x-a.x) < prec) && (fabsf(y - a.y) < prec);
    }

    // -----------------------   IVec3    -----------------------

    inline IVec3::IVec3(const Vec3& in) : x((int)in.x), y((int)in.y), z((int)in.z) {}

    inline IVec3 IVec3::operator+(const IVec3& a) const
    {
        return IVec3(a.x + this->x, a.y + this->y, a.z + this->z);
    }

    inline IVec3 IVec3::operator-(const IVec3& a) const
    {
        return IVec3(x - a.x, y - a.y, z - a.z);
    }

    inline IVec3 IVec3::operator*(const IVec3& a) const
    {
        IVec3 res = IVec3(this->x * a.x, this->y * a.y, this->z * a.z);
        return res;
    }

    inline IVec3 IVec3::operator*(const float& a) const
    {
        IVec3 res = IVec3(this->x * (int)a, this->y * (int)a, this->z * (int)a);
        return res;
    }

    inline IVec3 IVec3::operator/(const float& a) const
    {
        if ((int)a == 0)
            return IVec3(_CRT_INT_MAX, _CRT_INT_MAX, _CRT_INT_MAX);
        IVec3 res = IVec3(x / (int)a, y / (int)a, z / (int)a);
        return res;
    }

    inline bool IVec3::operator==(const IVec3& b) const
    {
        return (x == b.x && y == b.y && z == b.z);
    }

    inline const int& IVec3::operator[](const size_t a) const
    {
        return *((&x) + a);
    }

    inline int& IVec3::operator[](const size_t a)
    {
        return *((&x) + a);
    }

    // -----------------------   Vec3    -----------------------

    inline float Vec3::lengthSquared() const
    {
        return (x * x + y * y + z * z);
    }

    inline float Vec3::getLength() const
    {
        return sqrtf(lengthSquared());
    }

    inline Vec3 Vec3::operator+(const Vec3& a) const
    {
        Vec3 res = Vec3(a.x + this->x, a.y + this->y, a.z + this->z);
        return res;
    }

    inline Vec3 Vec3::operator-(const Vec3& a) const
    {
        Vec3 res = Vec3(this->x - a.x, this->y - a.y, this->z - a.z);
        return res;
    }

    inline Vec3 Vec3::operator-() const
    {
        return this->negate();
    }

    inline Vec3 Vec3::operator*(const Vec3& a) const
    {
        Vec3 res = Vec3(this->x * a.x, this->y * a.y, this->z * a.z);
        return res;
    }

    inline Vec3 Vec3::operator*(const float& a) const
    {
        Vec3 res = Vec3(this->x * a, this->y * a, this->z * a);
        return res;
    }

    inline Vec3 Vec3::operator/(const float& a) const
    {
        if (a == 0.0f)
            return operator*(VEC_HIGH_VALUE);
        Vec3 res = operator*(1 / a);
        return res;
    }

    inline bool Vec3::operator==(const Vec3& b) const
    {
        return (x == b.x && y == b.y && z == b.z);
    }

    inline float& Vec3::operator[](const size_t a)
    {
        return *((&x) + a);
    }

    inline const float& Vec3::operator[](const size_t a) const
    {
        return *((&x) + a);
    }

    inline float Vec3::dotProduct(Vec3 a) const
    {
        return (a.x * x + a.y * y + a.z * z);
    }

    inline bool Vec3::isCollinearWith(Vec3 a) const
    {
        float res = this->dotProduct(a);
        return (res < VEC_COLLINEAR_PRECISION);
    }

    inline Vec3 Vec3::crossProduct(Vec3 a) const
    {
        return Vec3((y * a.z) - (z * a.y), (z * a.x) - (x * a.z), (x * a.y) - (y * a.x));
    }

    inline Vec3 Vec3::unitVector() const
    {
        return operator/(getLength());
    }

    inline Vec3 Vec3::negate() const
    {
        return operator*(-1);
    }

    inline bool Vec3::isIntEquivalent(Vec3 a) const
    {
        return ((int)x == (int)a.x && (int)y == a.y && (int)z == (int)a.z);
    }

    inline bool Vec3::isNearlyEqual(Vec3 a, float prec)
    {
        return (fabsf(x - a.x) < prec) && (fabsf(y - a.y) < prec) && (fabsf(z - a.z) < prec);
    }

    // -----------------------   Vec4    -----------------------

    inline Vec3 Vec4::getVector() const
    {
        return Vec3(x, y, z);
    }

    inline Vec4 Vec4::homogenize() const
    {
        return Vec4(getVector() / w);
    }

    inline Vec4 Vec4::unitVector() const
    {
        Vec4 homogenized = homogenize();
        return homogenized / homogenized.getVector().getLength();
    }

    inline float Vec4::lengthSquared() const
    {
        return homogenize().getVector().lengthSquared();
    }

    inline float Vec4::getLength() const
    {
        return sqrtf(lengthSquared());
    }

    inline Vec4 Vec4::operator+(const Vec4& a) const
    {
        return Vec4(x + a.x, y + a.y, z + a.z, w + a.w);
    }

    inline Vec4 Vec4::operator-(const Vec4& a) const
    {
        return Vec4(x - a.x, y - a.y, z - a.z, w - a.w);
    }

    inline Vec4 Vec4::operator-() const
    {
        return this->negate();
    }

    inline Vec4 Vec4::operator*(const Vec4& a) const
    {
        return Vec4(x * a.x, y * a.y, z * a.z, w * a.w);
    }

    inline Vec4 Vec4::operator*(const float& a) const
    {
        return Vec4(x * a, y * a, z * a, w * a);
    }

    inline Vec4 Vec4::operator/(const float& b) const
    {
        if (b == 0.0f)
            return operator*(VEC_HIGH_VALUE);
        Vec4 res = operator*(1 / b);
        return res;
    }

    inline bool Vec4::operator==(const Vec4& b) const
    {
        return (x == b.x && y == b.y && z == b.z && w == b.w);
    }

    inline float& Vec4::operator[](const size_t a)
    {
        return *((&x) + a);
    }

    inline const float& Vec4::operator[](const size_t a) const
    {
        return *((&x) + a);
    }

    inline bool Vec4::isCollinearWith(Vec4 a) const
    {
        float res = dotProduct(a);
        return (res < VEC_COLLINEAR_PRECISION);
    }

    inline float Vec4::dotProduct(Vec4 a) const
    {
        return (a.x * x + a.y * y + a.z * z);
    }

    inline Vec4 Vec4::crossProduct(Vec4 a) const
    {
        return Vec4((y * a.z) - (z * a.y), (z * a.x) - (x * a.z), (x * a.y) - (y * a.x), 1.0f);
    }

    inline Vec4 Vec4::negate() const
    {
        return operator*(-1);
    }

    inline bool Vec4::isIntEquivalent(Vec4 a) const
    {
        return ((int)x == (int)a.x && (int)y == a.y && (int)z == (int)a.z && (int)w == (int)a.w);
    }

    inline bool Vec4::isNearlyEqual(Vec4 a, float prec)
    {
        return (fabsf(x - a.x) < prec) && (fabsf(y - a.y) < prec) && (fabsf(z - a.z) < prec) && (fabsf(w - a.w) < prec);
    }

    // -----------------------   Color4    -----------------------

    inline Color4::Color4(const float* in)
    {
        r = (unsigned char)(in[0] * 255);
        g = (unsigned char)(in[1] * 255);
        b = (unsigned char)(in[2] * 255);
        a = (unsigned char)(in[3] * 255);
    }

    inline Color4::Color4(const Vec4& in)
    {
        r = (unsigned char)(Util::cut(in[0], 0, 1) * 255);
        g = (unsigned char)(Util::cut(in[1], 0, 1) * 255);
        b = (unsigned char)(Util::cut(in[2], 0, 1) * 255);
        a = (unsigned char)(Util::cut(in[3], 0, 1) * 255);
    }

    inline Color4 Color4::operator*(const float& in) const
    {
        return Color4(r * (int)in, g * (int)in, b * (int)in, a);
    }

    inline Color4 Color4::operator+(const Color4& in) const
    {
        return Color4(r + in.r, g + in.g, b + in.b, a);
    }

    // -----------------------   Mat4    -----------------------

    inline float& Mat4::operator[](const size_t in)
    {
        Assert(in < 16);
        return content[in];
    }

    inline const float& Mat4::operator[](const size_t in) const
    {
        Assert(in < 16);
        return content[in];
    }

    inline float& Mat4::at(const unsigned char x, const unsigned char y)
    {
        Assert(x < 4 && y < 4);
        return content[x*4+y];
    }

    inline float& Mat3::operator[](const size_t in)
    {
        Assert(in < 9);
        return content[in];
    }

    inline float& Mat3::at(const unsigned char x, const unsigned char y)
    {
        Assert(x < 3 && y < 3);
        return content[x * 3 + y];
    }

    // ----------------------- Math Utils -----------------------

    inline float Util::toRadians(float in)
    {
        return in / 180.0f * (float)M_PI;
    }

    inline float Util::toDegrees(float in)
    {
        return in * 180.0f / (float)M_PI;
    }

    inline float Util::cut(float in, float min, float max)
    {
        if (in < min)
            in = min;
        if (in > max)
            in = max;
        return in;
    }

    inline int Util::cutInt(int in, int min, int max)
    {
        if (in < min)
            in = min;
        if (in > max)
            in = max;
        return in;
    }

    inline float Util::lerp(float delta, float min, float max)
    {
        return min + delta * (max - min);
    }

    inline float Util::mod(float in, float value)
    {
        return in - value * floorf(in / value);
    }

    inline int Util::imod(int in, int value)
    {
        int tmp = in % value;
        if (tmp < 0) tmp += value;
        return tmp;
    }

    inline float Util::minF(float a, float b)
    {
        if (a > b)
            return b;
        return a;
    }

    inline float Util::maxF(float a, float b)
    {
        if (a > b)
            return a;
        return b;
    }

    inline int Util::minI(int a, int b)
    {
        if (a > b)
            return b;
        return a;
    }

    inline int Util::maxI(int a, int b)
    {
        if (a > b)
            return a;
        return b;
    }

    inline bool Util::isEqual(float a, float b, float prec)
    {
        return (a - prec < b&& a + prec > b);
    }

    inline bool Util::isEqualVec4(Vec4 a, Vec4 b, float prec)
    {
        return (isEqual(b.x, a.x, prec) && isEqual(b.y, a.y, prec) && isEqual(b.z, a.z, prec) && isEqual(b.w, a.w, prec));
    }
}