/***********************************
 * vec2.h
 *
 * Two dimension vector.
 *
 * Author: WeiQM (weiquanmao@hotmail.com)
 * Github: https://github.com/CheckBoxStudio/IoU
 *
 * 2019
 ***********************************/

#ifndef IOU_VECTWO_H
#define IOU_VECTWO_H

#include "util.h"
#include <assert.h>

namespace IOU
{ // Begin of namespace IOU

template <typename T>
class Vec2 {
public:
    // Constructors
    Vec2() : m_x(0), m_y(0) {}
    Vec2(T x, T y) : m_x(x), m_y(y) {}

    // Access component
    T& x() { return m_x; }
    T x() const { return m_x; }
    T& y() { return m_y; }
    T y() const { return m_y; }
    T& operator[](unsigned int i) { assert(i < 2); return m_d[i]; }
    T operator[](unsigned int i) const { assert(i < 2); return m_d[i]; }

    // Operations
    bool isZero() { return Compare(m_x,T(0.)) && Compare(m_y,T(0.)); }

    Vec2& operator+=(const Vec2 &p) { m_x+=p.m_x; m_y+=p.m_y; return *this; }
    Vec2& operator-=(const Vec2 &p) { m_x-=p.m_x; m_y-=p.m_y; return *this; }
    template<typename TT>
    Vec2& operator*=(TT t) { m_x*=t; m_y*=t; return *this; }
    template<typename TT>
    Vec2& operator/=(TT t) { m_x/=t; m_y/=t; return *this; }

    double norm() const { return sqrt(m_x*m_x + m_y*m_y); }
    double normSquared() const { return m_x*m_x + m_y*m_y; }
    void normalize() { *this/=this->norm(); }
    const Vec2 normalized() const { return *this/this->norm(); }

    double theta() const { return atan2(m_y, m_x); }

private:
    // Members
    union {
        struct {
            T m_x;
            T m_y;
        };
        T m_d[2];
    };
};

template <typename T>
bool operator==(const Vec2<T> &p1, const Vec2<T> &p2) {
    return Compare(p1.x(), p2.x()) && Compare(p1.y(), p2.y());
}
template<typename T>
inline bool isZero(const Vec2<T> &p) {
    return p.isZero();
}
template<typename T>
inline const Vec2<T> operator+(const Vec2<T> &p1, const Vec2<T> &p2) {
    return Vec2<T>(p1.x()+p2.x(), p1.y()+p2.y());
}
template<typename T>
inline const Vec2<T> operator-(const Vec2<T> &p1, const Vec2<T> &p2) {
    return Vec2<T>(p1.x()-p2.x(), p1.y()-p2.y());
}
template<typename T, typename TT>
inline const Vec2<T> operator*(const Vec2<T> &p, TT t) {
    return Vec2<T>(p.x()*t, p.y()*t);
}
template<typename T, typename TT>
inline const Vec2<T> operator*(TT t, const Vec2<T> &p) {
    return Vec2<T>(p.x()*t, p.y()*t);
}
template<typename T, typename TT>
inline const Vec2<T> operator/(const Vec2<T> &p, TT t) {
    return Vec2<T>(p.x()/t, p.y()/t);
}
template<typename T>
inline const Vec2<T> dmul(const Vec2<T> &p1, const Vec2<T> &p2) {
    return Vec2<T>(p1.x()*p2.x(), p1.y()*p2.y());
}
template<typename T>
inline const Vec2<T> ddiv(const Vec2<T> &p1, const Vec2<T> &p2) {
    return Vec2<T>(p1.x()/p2.x(), p1.y()/p2.y());
}
template<typename T>
inline T dot(const Vec2<T> &p1, const Vec2<T> &p2) {
    return p1.x()*p2.x() + p1.y()*p2.y();
}
template<typename T>
inline T operator*(const Vec2<T> &p1, const Vec2<T> &p2) {
    return dot(p1,p2);
}
template<typename T>
inline T cmul(const Vec2<T> &p1, const Vec2<T> &p2) {
    return p1.x()*p2.y() - p1.y()*p2.x();
}
template<typename T>
inline T operator^(const Vec2<T> &p1, const Vec2<T> &p2) {
    return cmul(p1,p2);
}
template<typename T>
inline double norm(const Vec2<T> p) {
    return p.norm();
}
template<typename T>
inline double normSquared(const Vec2<T> p) {
    return p.normSquared();
}
template<typename T>
inline double distance(const Vec2<T> &p1, const Vec2<T> &p2) {
    return (p1-p2).norm();
}
template<typename T>
inline double squareDistance(const Vec2<T> &p1, const Vec2<T> &p2) {
    return (p1-p2).normSquared();
}
template<typename T>
double angle(const Vec2<T> &p1, const Vec2<T> &p2) {
    return acos( dot(p1,p2) / (p1.norm() * p2.norm()) );
}

typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;

} // End of namespace IOU

#endif // IOU_VECTWO_H
