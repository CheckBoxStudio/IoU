#ifndef _IOU_H_FILE_
#define _IOU_H_FILE_

#include "math.h"
#include <assert.h>
#include <vector>

namespace IOU
{
    const double ZERO = 1e-12;
    template <typename T>
    struct Vec2 {
        // Members.
        union {
            struct 
            {
                T x;
                T y;
            };
            T D[2];
        };

        inline bool isZero() { return (abs(x) <= ZERO && abs(y) <= ZERO); }
        inline bool nonZero() { return !isZero(); }

        // Constructors.
        Vec2() : x(0), y(0) {}
        Vec2(T _x, T _y) : x(_x), y(_y) {}
        Vec2(const Vec2& p) : x(p.x), y(p.y) {}

        // Access component.
        inline T& operator[](unsigned int i) { assert(i < 2); return D[i]; }
        inline const T& operator[](unsigned int i) const { assert(i < 2); return D[i]; }

        // Operations.
        inline Vec2& operator=(const Vec2& p) { x = p.x; y = p.y; return *this; }
        inline bool operator==(const Vec2& p) const { return (abs(x - p.x) <= ZERO && abs(y - p.y) <= ZERO); }
        inline Vec2 operator*(T t) const { return Vec2(x * t, y * t); }
        inline Vec2 operator/(T t) const { return Vec2(x / t, y / t); }       
        inline Vec2& operator*=(T t) { x *= t; y *= t; return *this; }
        inline Vec2& operator/=(T t) { x /= t; y /= t; return *this; }

        inline Vec2 operator+(const Vec2& p) const { return Vec2(x + p.x, y + p.y); }
        inline Vec2 operator-(const Vec2& p) const { return Vec2(x - p.x, y - p.y); }
        inline Vec2& operator+=(const Vec2& p) { x += p.x; y += p.y; return *this; }
        inline Vec2& operator-=(const Vec2& p) { x -= p.x; y -= p.y; return *this; }

        inline Vec2 dmul(const Vec2& p) const { return Vec2(x * p.x, y * p.y); }
        inline Vec2 ddiv(const Vec2& p) const { return Vec2(x / p.x, y / p.y); }
        
        inline T dot(const Vec2& p) const { return x * p.x + y * p.y; }       
        inline T operator*(const Vec2& p) const { return x * p.x + y * p.y; }

        inline T cmul(const Vec2& p) const { return x * p.y - y * p.x; }
        inline T operator^(const Vec2& p) const { return x * p.y - y * p.x; }

        inline T norm() const { return std::sqrt(x*x + y*y); }
        inline T normSquared() const { return x*x + y*y; }
        inline void normalize() { *this /= norm(); }
        inline Vec2 normalized() const { return *this / norm(); }

        inline T distance(const Vec2& r) const { return (*this - r).norm(); }
        inline T squareDistance(const Vec2& r) const { return (*this - r).normSquared(); }
        inline double angle(const Vec2& r) const { return acos( dot(r) / (norm() * r.norm()) ); }
        inline double theta() const {
            Vec2 ax(1.0, 0.0);
            double a = angle(ax);
            if (cmul(ax) > 0.0)
                a = 3.141592653 * 2 - a;
            return a;
        }
     
        void max(const Vec2& p)
        {
            x = p.x > x ? p.x : x;
            y = p.y > y ? p.y : y;
        }

        void min(const Vec2& p)
        {
            x = p.x < x ? p.x : x;
            y = p.y < y ? p.y : y;
        }
    };

    template <typename T>
    inline Vec2<T> operator*(T t, const Vec2<T>& v) {
        return Vec2<T>(v.x * t, v.y * t);
    }
    template <typename T>
    inline T distance(const Vec2<T>& p1, const Vec2<T>& p2) { return p1.distance(p2); }
    template <typename T>
    inline T squareDistance(const Vec2<T>& p1, const Vec2<T>& p2) { return p1.squareDistance(p2); }
    template <typename T>
    inline double angle(const Vec2<T>& p1, const Vec2<T>& p2) { return acos( p1*p2 / (p1.norm() * p2.norm()) ); }
    template <typename T>
    void swap(Vec2<T>& p1, Vec2<T>& p2) { Vec2<T> temp = p1;  p1 = p2; p2 = temp; }

    typedef Vec2<int> Vec2i;
    typedef Vec2<float> Vec2f;
    typedef Vec2<double> Vec2d;

    typedef Vec2d Point;
    
    class Quad {
    public:
        // clockwise
        union {
            struct
            {
                Vec2d p1;
                Vec2d p2;
                Vec2d p3;
                Vec2d p4;
            };
            Vec2d vert[4];
        };
        enum LocPosition
        {
            OutSide,
            OnLine,
            InSide
        };

        Quad() : p1(Vec2d()), p2(Vec2d()), p3(Vec2d()), p4(Vec2d()) {}
        Quad(const Vec2d _p1, const Vec2d _p2, const Vec2d _p3, const Vec2d _p4)
            : p1(_p1), p2(_p2), p3(_p3), p4(_p4) {}
        Quad(const Vec2d _vert[4]) 
            : p1(_vert[0]), p2(_vert[1]), p3(_vert[2]), p4(_vert[3]) {}
        Quad(const Quad &quad)
            : p1(quad.p1), p2(quad.p2), p3(quad.p3), p4(quad.p4) {}
        void flipBack() { swap(p2, p4); }
        double area() const;
        void vertList(std::vector<Point> &_vert) const;
        bool isClockWiseConvex() const;
        bool isAntiClockWiseConvex() const;
        bool hasRepeatVert() const;
        LocPosition loc(const Point p) const;
        int inter(const Point _p1, const Point _p2, std::vector<Point> &interPts) const;
    };
}
enum WiseType
{
    clockWise,
    antiClockWise
};
bool checkWiseCovex(const std::vector<IOU::Point> &vert, const WiseType wiseType);
bool makeWiseCovex(std::vector<IOU::Point> &vert, const WiseType wiseType);
double AreaOfCovex(const std::vector<IOU::Point> &vert);
bool onLine(const IOU::Point &a1, const IOU::Point &a2, const IOU::Point &p);
bool cross(
    const IOU::Point &a1, const IOU::Point &a2, 
    const IOU::Point &b1, const IOU::Point &b2, 
    IOU::Point &ip);
int FindInterPoints(const IOU::Quad& P1, const IOU::Quad& P2, std::vector<IOU::Point> &vert); // P2 inter P1
int FindInnerPoints(const IOU::Quad& P1, const IOU::Quad& P2, std::vector<IOU::Point> &vert); // P2 in P1
double CalInterArea(const IOU::Quad& P1, const IOU::Quad& P2);

#endif // !_IOU_H_FILE_