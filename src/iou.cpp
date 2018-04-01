#include "IOU.h"
#include <algorithm>

using namespace IOU;

double Quad::area() const
{
    std::vector<Point> verts;
    verts.push_back(p1);
    verts.push_back(p2);
    verts.push_back(p3);
    verts.push_back(p4);

    return AreaOfCovex(verts);
}
void Quad::vertList(std::vector<Point> &_vert) const
{
    std::vector<Point> vertTemp;
    vertTemp.push_back(p1);
    vertTemp.push_back(p2);
    vertTemp.push_back(p3);
    vertTemp.push_back(p4);
    _vert.swap(vertTemp);
}
bool Quad::isClockWiseConvex() const
{
    std::vector<Point> vertTemp;
    vertList(vertTemp);

    return checkWiseCovex(vertTemp, clockWise);
}
bool Quad::isAntiClockWiseConvex() const
{
    std::vector<Point> vertTemp;
    vertList(vertTemp);

    return checkWiseCovex(vertTemp, antiClockWise);
}
bool Quad::hasRepeatVert() const
{
    bool bRep = (
        p1 == p2 || p1 == p3 || p1 == p4 ||
        p2 == p3 || p2 == p4 ||
        p3 == p4
        );
    return bRep;
}
Quad::LocPosition Quad::loc(const Point p) const
{
    if (onLine(p1, p2, p) ||
        onLine(p2, p3, p) ||
        onLine(p3, p4, p) ||
        onLine(p4, p1, p))
        return OnLine;

    const Point pO = (p1 + p2 + p3 + p4) / 4.0;
    Point _p;
    if (cross(p1, p2, pO, p, _p) ||
        cross(p2, p3, pO, p, _p) ||
        cross(p3, p4, pO, p, _p) ||
        cross(p4, p1, pO, p, _p))
        return OutSide;
    return InSide;
}
int Quad::inter(const Point _p1, const Point _p2, std::vector<Point> &interPts) const
{
    std::vector<Point> _interPts;
    if (loc(_p1) == OnLine && loc(_p2) == OnLine) {
        _interPts.push_back(_p1);
        _interPts.push_back((_p1+_p2)/2.0);
        _interPts.push_back(_p2);
    }
    else{
        Point ip;
        if (cross(_p1, _p2, p1, p2, ip)) _interPts.push_back(ip);
        if (cross(_p1, _p2, p2, p3, ip)) _interPts.push_back(ip);
        if (cross(_p1, _p2, p3, p4, ip)) _interPts.push_back(ip);
        if (cross(_p1, _p2, p4, p1, ip)) _interPts.push_back(ip);
    }

    interPts.swap(_interPts);
    return interPts.size();
}
bool onLine(const Point &a1, const Point &a2, const Point &p)
{
    if (a1 == a2)
        return (p == (a1 + a2) / 2.0);
    
    Point pa1 = p - a1;
    Point pa2 = p - a2;

    if (abs(pa1^pa2) < ZERO &&
        pa1*pa2 < ZERO)
        return true;
    else
        return false;
}
bool cross(
    const Point &a1, const Point &a2,
    const Point &b1, const Point &b2,
    Point &ip)
{
    if (a1 == a2 && b1 == b2)
        return ((a1 + a2) / 2.0 == (b1 + b2) / 2.0);
    else if (a1 == a2)
        return onLine(b1, b2, (a1 + a2) / 2.0);
    else if (b1 == b2)
        return onLine(a1, a2, (b1 + b2) / 2.0);
        
    Point a12 = a2 - a1;
    Point b12 = b2 - b1;
    double ang = angle(a12, b12);
    if (ang < ZERO || abs(3.141592653 - ang) < ZERO)
        return false;

    // a1_x + m*a12_x = b1_x + n*b12_x
    // a1_y + m*a12_y = b1_y + n*b12_y
    // n = ( (a1_y-b1_y)*a12_x - (a1_x-b1_x)*a12_y ) / (a12_x*b12_y - b12_x*a12_y)
    // m = ( (a1_y-b1_y)*b12_x - (a1_x-b1_x)*b12_y ) / (a12_x*b12_y - b12_x*a12_y)
    // 0 < m < 1
    // 0 < n < 1
    double abx = a1.x - b1.x;
    double aby = a1.y - b1.y;
    double ab = a12.x*b12.y - b12.x*a12.y;
    assert(abs(ab) > ZERO);
    double n = (aby*a12.x - abx*a12.y) / ab;
    double m = (aby*b12.x - abx*b12.y) / ab;

    if (n > 0 && n < 1 &&
        m > 0 && m < 1) {
        Point ip1 = a1 + m*a12;
        Point ip2 = b1 + n*b12;
        ip = (ip1 + ip2) / 2.0;
        return true;
    }
    else
        return false;

}
bool checkWiseCovex(const std::vector<Point> &vert, const WiseType wiseType)
{
    bool bWise = true;
    const int N = vert.size();
    const double flip = (wiseType == clockWise) ? 1.0 : -1.0;

    if (N > 2) {
        Point p0 = vert.at(N - 1);
        Point p1 = vert.at(0);
        Point p2 = vert.at(1);
        Point p01 = p1 - p0;
        Point p12 = p2 - p1;
        if ((p01^p12)*flip > 0.0 ||
            ((abs(p01^p12) <= ZERO) && p01*p12 < 0.0)) {
            return false;
        }
        p0 = vert.at(N - 2);
        p1 = vert.at(N - 1);
        p2 = vert.at(0);
        p01 = p1 - p0;
        p12 = p2 - p1;
        if ((p01^p12)*flip > 0.0 ||
            ((abs(p01^p12) <= ZERO) && p01*p12 < 0.0)) {
            return false;
        }

        for (int i = 1; i < N - 1; ++i) {
            p0 = vert.at(i - 1);
            p1 = vert.at(i);
            p2 = vert.at(i + 1);
            p01 = p1 - p0;
            p12 = p2 - p1;
            if ((p01^p12)*flip > 0.0 ||
                ((abs(p01^p12) <= ZERO) && p01*p12 < 0.0)) {
                bWise = false;
                break;
            }
        }
    }
    return bWise;
}

typedef std::pair<double, Point> AngPoint;
bool angIncrease(const AngPoint &p1, const AngPoint &p2)
{
    return p1.first < p2.first;
}
bool angDecrease(const AngPoint &p1, const AngPoint &p2)
{
    return p1.first > p2.first;
}
bool makeWiseCovex(std::vector<Point> &vert, const WiseType wiseType)
{
    const int N = vert.size();
    bool bRet = false;
    if (N>2) {
        Point pO;
        for (int i = 0; i < N; ++i)
            pO += vert.at(i);
        pO /= N;
        std::vector<AngPoint> APList;
        for (int i = 0; i < N; ++i) {
            Point op = vert.at(i) - pO;
            double ang = op.theta();
            APList.push_back(AngPoint(ang, vert.at(i)));
        }
        if (wiseType == antiClockWise)
            std::sort(APList.begin(), APList.end(), angIncrease);
        else
            std::sort(APList.begin(), APList.end(), angDecrease);
        std::vector<Point> vertTemp;
        for (int i = 0; i < N; ++i)
            vertTemp.push_back(APList.at(i).second);
        vert.swap(vertTemp);
        bRet = checkWiseCovex(vert, wiseType);
    }
    return bRet;
}
double AreaOfCovex(const std::vector<Point> &vert)
{
    if (!checkWiseCovex(vert, clockWise) && !checkWiseCovex(vert, antiClockWise))
        return -1.0;

    double sArea = 0.0;
    const int N = vert.size();
    if (N > 2) {
        const Point &p0 = vert.at(0);
        for (int i = 1; i < N-1; ++i) {
            const Point &p1 = vert.at(i);
            const Point &p2 = vert.at(i + 1);
            Point p01 = p1 - p0;
            Point p02 = p2 - p0;
            sArea += abs(p01^p02)*0.5;
        }
    }
    return sArea;
}
int FindInterPoints(const Quad& P1, const Quad& P2, std::vector<Point> &vert)
{
    std::vector<IOU::Point> _vert;
    std::vector<IOU::Point> inter;
    if (P1.inter(P2.p1, P2.p2, inter))
        for (int i = 0; i < inter.size(); ++i)
            _vert.push_back(inter.at(i));
    if (P1.inter(P2.p2, P2.p3, inter))
        for (int i = 0; i < inter.size(); ++i)
            _vert.push_back(inter.at(i));
    if (P1.inter(P2.p3, P2.p4, inter))
        for (int i = 0; i < inter.size(); ++i)
            _vert.push_back(inter.at(i));
    if (P1.inter(P2.p4, P2.p1, inter))
        for (int i = 0; i < inter.size(); ++i)
            _vert.push_back(inter.at(i));
    vert.swap(_vert);
    return vert.size();
}
int FindInnerPoints(const Quad& P1, const Quad& P2, std::vector<Point> &vert)
{
    std::vector<IOU::Point> _vert;
    if (P1.loc(P2.p1) != Quad::OutSide) _vert.push_back(P2.p1);
    if (P1.loc(P2.p2) != Quad::OutSide) _vert.push_back(P2.p2);
    if (P1.loc(P2.p3) != Quad::OutSide) _vert.push_back(P2.p3);
    if (P1.loc(P2.p4) != Quad::OutSide) _vert.push_back(P2.p4);

    vert.swap(_vert);
    return vert.size();       
}
double CalInterArea(const Quad& P1, const Quad& P2)
{
    if (!P1.isClockWiseConvex() ||
        !P2.isClockWiseConvex() )
        return -1.0;

    std::vector<Point> interVert;
    std::vector<Point> innerVert12;
    std::vector<Point> innerVert21;
    std::vector<Point> verts;
    //---------------
    FindInterPoints(P1, P2, interVert);
    FindInnerPoints(P1, P2, innerVert12);
    FindInnerPoints(P2, P1, innerVert21);
    //---------------
    // TODO : Check conditions
    for (int i = 0; i < interVert.size(); ++i)
        verts.push_back(interVert.at(i));
    for (int i = 0; i < innerVert12.size(); ++i)
        verts.push_back(innerVert12.at(i));
    for (int i = 0; i < innerVert21.size(); ++i)
        verts.push_back(innerVert21.at(i));

    if (verts.empty())
        return 0.0;
    else {
        assert(verts.size() >= 3);
        if (!makeWiseCovex(verts, clockWise))
            return -1.0;
        return AreaOfCovex(verts);
    }
    return -1.0;
}