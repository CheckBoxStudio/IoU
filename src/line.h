/***********************************
 * line.h
 *
 * Line.
 *
 * Author: WeiQM (weiquanmao@hotmail.com)
 * Github: https://github.com/CheckBoxStudio/IoU
 *
 * 2019
 ***********************************/

#ifndef IOU_LINE_H
#define IOU_LINE_H

#include "util.h"
#include "vec2.h"

namespace IOU
{ // Begin of namespace IOU

template <typename T>
class Line {
public:
    typedef Vec2<T> PointType;

public:
    // Constructors
    Line() : m_p1(PointType()), m_p2(PointType()) {}
    Line(const PointType &p1, const PointType &p2) : m_p1(p1), m_p2(p2) {}

    // Methods
    const PointType& p1() const { return m_p1; }
    PointType& p1() { return m_p1; }
    const PointType& p2() const { return m_p2; }
    PointType& p2() { return m_p2; }

    double length() const {return distance(p1, p2); }

private:
    // Members
    PointType m_p1;
    PointType m_p2;
};

template <typename T>
inline double length(Line<T> line) {
    return line.length();
}

template <typename T>
inline bool isDegratedLine(const Line<T> &line) {
    return line.p1() == line.p2();
}
template <typename T>
inline typename Line<T>::PointType midPoint(const Line<T> &line)
{
    return (line.p1() + line.p2())*0.5;
}

template <typename T>
inline bool isOnEdge(const typename Line<T>::PointType &p, const Line<T> &line)
{
    if (isDegratedLine(line))
        return p == midPoint(line);

    typename Line<T>::PointType pp1 = p - line.p1();
    typename Line<T>::PointType pp2 = p - line.p2();

    if (Compare(pp1^pp2, T(0)) &&
        pp1*pp2 < T(0))
        return true;
    else
        return false;
}
template <typename T>
inline bool isOnEdge(const Line<T> &line, const typename Line<T>::PointType &p) {
    return isOnEdge(p, line);
}

template <typename T>
typename Line<T>::PointType intersection(const Line<T> &line1, const Line<T> &line2, bool *bOnEdge = 0)
{
    typename Line<T>::PointType pInter(0,0);
    bool bOn = false;

    bool bIsDegratedLine1 = isDegratedLine(line1);
    bool bIsDegratedLine2 = isDegratedLine(line2);
    if (bIsDegratedLine1 && bIsDegratedLine2) {
        // Both lines are actually points.
        typename Line<T>::PointType midPt1 = midPoint(line1);
        typename Line<T>::PointType midPt2 = midPoint(line2);
        bOn = (midPt1 == midPt2);
        if (bOn)
            pInter = (midPt1+midPt2)*0.5;
    }
    else if (bIsDegratedLine1) {
        // This line1 is actually a point.
        typename Line<T>::PointType midPt = midPoint(line1);
        if (bOn = isOnEdge(midPt, line2))
            pInter = midPt;
    }
    else if (bIsDegratedLine2) {
        // The line2 is actually a point.
        typename Line<T>::PointType midPt = midPoint(line2);
        if (bOn = isOnEdge(midPt, line1))
            pInter = midPt;
    }
    else {
        // Normal cases.
        typename Line<T>::PointType a12 = line1.p2() - line1.p1();
        typename Line<T>::PointType b12 = line2.p2() - line2.p1();
        double ang = angle(a12, b12);
        if (Compare(ang, 0.0) || Compare(ang, Pi))
            bOn = false; // Collinear!!
        else {
            // a1_x + m*a12_x = b1_x + n*b12_x
            // a1_y + m*a12_y = b1_y + n*b12_y
            // n = ( (a1_y-b1_y)*a12_x - (a1_x-b1_x)*a12_y ) / (a12_x*b12_y - b12_x*a12_y)
            // m = ( (a1_y-b1_y)*b12_x - (a1_x-b1_x)*b12_y ) / (a12_x*b12_y - b12_x*a12_y)
            // 0 < m < 1
            // 0 < n < 1
            double abx = line1.p1().x() - line2.p1().x();
            double aby = line1.p1().y() - line2.p1().y();
            double ab = a12.x()*b12.y() - b12.x()*a12.y();
            assert(!Compare(ab, 0.0));
            double n = (aby*a12.x() - abx*a12.y()) / ab;
            double m = (aby*b12.x() - abx*b12.y()) / ab;

            if (n>0.0 && n<1.0 &&
                m>0.0 && m<1.0) {
                typename Line<T>::PointType ip1 = line1.p1() + m*a12;
                typename Line<T>::PointType ip2 = line2.p1() + n*b12;
                pInter = (ip1 + ip2)*0.5;
                bOn = true;
            }
            else
                bOn = false;
        }
    }

    if (bOnEdge != 0)
        *bOnEdge = bOn;

    return pInter;
}

} // End of namespace IOU

#endif // IOU_LINE_H
