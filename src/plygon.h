/***********************************
 * plygon.h
 *
 * Plygon.
 *
 * Author: WeiQM (weiquanmao@hotmail.com)
 * Github: https://github.com/CheckBoxStudio/IoU
 *
 * 2019
 ***********************************/

#ifndef IOU_PLYGON_H
#define IOU_PLYGON_H

#include "util.h"
#include "vec2.h"
#include "line.h"
#include <vector>
#include <algorithm>

namespace IOU
{ // Begin of namespace IOU

enum WiseType
{
    NoneWise,
    ClockWise,
    AntiClockWise
};
enum LocPosition
{
    Outside,
    OnEdge,
    Inside
};

template<typename T>
class Plygon {
public:
    typedef Vec2<T> PointType;
    typedef Line<T> LineType;
    typedef std::vector<PointType> Vertexes;

public:

    // Constructors
    Plygon() {}
    Plygon(const Vertexes &verts) : m_verts(verts) {}

    // Methods
    void addPt(const PointType &p) { m_verts.push_back(p); }
    int pointCount() const { return m_verts.size(); }
    bool isValid() const { return pointCount() >= 3; }
    void flip() { reverse(m_verts.begin(), m_verts.end()); }

    const PointType& operator [](unsigned int idx) const { return m_verts[idx]; }
    PointType& operator [](unsigned int idx) { return m_verts[idx]; }
    const PointType& at(unsigned int idx) const { return m_verts.at(idx); }
    PointType& at(unsigned int idx) { return m_verts.at(idx); }
    const PointType& point(unsigned int idx) const { return m_verts.at(idx); }
    PointType& point(unsigned int idx) { return m_verts.at(idx); }
    const Vertexes& vertexes() const { return m_verts; }
    Vertexes& vertexes() { return m_verts; }

    double area() const;
    WiseType whichWise() const;
    bool isInClockWise() const { return ClockWise == whichWise(); }
    bool isInAntiClockWise() const { return AntiClockWise == whichWise(); }
    void sortInSomeWise(const WiseType wiseType);
    void sortInClockWise() { sortInSomeWise(ClockWise); }
    void sortInAntiClockWise() { sortInSomeWise(AntiClockWise); }

    LocPosition location(const PointType &p) const;
    int intersections(const LineType &line, Vertexes &pts) const;

private:
    Vertexes m_verts;
};

template<typename T>
double Plygon<T>::area() const
{
    if (whichWise() == NoneWise)
        return -1.0;

    double sArea = 0.0;
    const int N = m_verts.size();
    if (N > 2) {
        const PointType &p0 = m_verts[0];
        for (int i = 1; i < N-1; ++i) {
            const PointType &p1 = m_verts[i];
            const PointType &p2 = m_verts[i + 1];
            PointType p01 = p1 - p0;
            PointType p02 = p2 - p0;
            sArea += Abs(p01^p02)*0.5;
        }
    }
    return sArea;
}
template<typename T>
WiseType Plygon<T>::whichWise() const
{
    const int N = m_verts.size();
    if (N > 2) {
        WiseType wiseType = NoneWise;
        for (int i = 0; i < N ; ++i) {
            PointType p0 = m_verts.at(i%N);
            PointType p1 = m_verts.at((i+1)%N);
            PointType p2 = m_verts.at((i+2)%N);
            PointType p01 = p1 - p0;
            PointType p12 = p2 - p1;

            if (Compare(p01^p12, 0.0)) {
                if (p01*p12 < 0.0)
                    return NoneWise;
                else
                    continue;
            }

            WiseType tmpWiseType = (p01^p12) > 0.0 ? AntiClockWise : ClockWise;
            if (wiseType == NoneWise) {
                wiseType = tmpWiseType;
            }
            else if (wiseType != tmpWiseType)
                return NoneWise;
        }
        return wiseType;
    }

    return NoneWise;
}


template<typename T>
bool angIncrease(
        const std::pair<double, typename Plygon<T>::PointType> &p1,
        const std::pair<double, typename Plygon<T>::PointType> &p2)
{
    return p1.first < p2.first;
}
template<typename T>
bool angDecrease(
        const std::pair<double, typename Plygon<T>::PointType> &p1,
        const std::pair<double, typename Plygon<T>::PointType> &p2)
{
    return p1.first > p2.first;
}

template<typename T>
void Plygon<T>::sortInSomeWise(const WiseType wiseType)
{
    if (wiseType != NoneWise) {
        const int N = m_verts.size();
        if (N>2) {
            PointType pO(0.0,0.0);
            for (int i = 0; i < N; ++i)
                pO += m_verts[i];
            pO /= N;
            std::vector< std::pair<double, PointType> > APList;
            for (int i = 0; i < N; ++i) {
                PointType op = m_verts[i] - pO;
                double ang = op.theta();
                APList.push_back(std::pair<double, PointType>(ang, m_verts[i]));
            }
            if (wiseType == AntiClockWise)
                std::sort(APList.begin(), APList.end(), angIncrease<T>);
            else
                std::sort(APList.begin(), APList.end(), angDecrease<T>);
            Vertexes vertTemp;
            for (int i = 0; i < N; ++i)
                vertTemp.push_back(APList.at(i).second);
            m_verts.swap(vertTemp);
        }
    }
}

template<typename T>
LocPosition Plygon<T>::location(const PointType &p) const
{
    const int N = m_verts.size();
    // Special cases.
    if (N == 0)
        return Outside;
    if (N == 1) {
        if (m_verts[0] == p)
            return Inside;
        else
            return Outside;
    }
    if (N == 2) {
        if (isOnEdge(LineType(m_verts[0],m_verts[1]),p))
            return OnEdge;
        else
            return Outside;
    }

    // Normal cases.
    // Check OnEdge.
    for (int i=0; i<N; ++i) {
        if (isOnEdge(LineType(m_verts[i%N],m_verts[(i+1)%N]),p))
            return OnEdge;
    }
    // Check Outside.
    PointType pO(0.0,0.0);
    for (int i=0; i<N; ++i) {
        pO += m_verts[i];
    }
    pO /= N;
    LineType op(pO,p);
    bool bIntersection = true;
    for (int i=0; i<N; ++i) {
        intersection(LineType(m_verts[i%N],m_verts[(i+1)%N]),op,&bIntersection);
        if (bIntersection)
            return Outside;
    }

    return Inside;
}

template<typename T>
int Plygon<T>::intersections(const LineType &line, Vertexes &pts) const
{
    Vertexes vertTemp;
    const int N = m_verts.size();
    bool bIntersection = false;
    for (int i=0; i<N; ++i) {
        PointType p = intersection(LineType(m_verts[i%N],m_verts[(i+1)%N]),line,&bIntersection);
        if (bIntersection)
            vertTemp.push_back(p);
    }
    pts.swap(vertTemp);

    return pts.size();
}



template<typename T>
inline double area(const Plygon<T> &plygon) {
    return plygon.area();
}
template<typename T>
inline WiseType whichWise(const Plygon<T> &plygon) {
    return plygon.whichWise();
}
template<typename T>
inline bool isInClockWise(const Plygon<T> &plygon) {
    return plygon.isInClockWise();
}
template<typename T>
inline bool isInAntiClockWise(const Plygon<T> &plygon) {
    return plygon.isInAntiClockWise(); }
template<typename T>
inline void sortInSomeWise(const Plygon<T> &plygon, const WiseType wiseType) {
    return plygon.sortInSomeWise(wiseType);
}
template<typename T>
inline void sortInClockWise(const Plygon<T> &plygon) {
    plygon.sortInClockWise();
}
template<typename T>
inline void sortInAntiClockWise(const Plygon<T> &plygon) {
    plygon.sortInAntiClockWise();
}

template<typename T>
inline LocPosition location(const Plygon<T> &plygon, const typename Plygon<T>::PointType &p) {
    return plygon.location(p);
}
template<typename T>
inline LocPosition location(const typename Plygon<T>::PointType &p, const Plygon<T> &plygon) {
    return plygon.location(p);
}

template<typename T>
int intersections(const Plygon<T> &plygon, const typename Plygon<T>::LineType &line, typename Plygon<T>::Vertexes &pts) {
    return plygon.intersections(line, pts);
}
template<typename T>
int intersections(const typename Plygon<T>::LineType &line, const Plygon<T> &plygon, typename Plygon<T>::Vertexes &pts) {
    return plygon.intersections(line, pts);
}

} // End of namespace IOU

#endif // IOU_PLYGON_H
