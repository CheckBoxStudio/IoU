/***********************************
 * iou.h
 *
 * Calculate the iou (Intersection over Union) ratio.
 * For ONLY Convex Polygons.
 *
 * Author: WeiQM (weiquanmao@hotmail.com)
 * Github: https://github.com/CheckBoxStudio/IoU
 *
 * 2019
 ***********************************/

#ifndef IOU_IOU_H
#define IOU_IOU_H

#include "util.h"
#include "vec2.h"
#include "line.h"
#include "plygon.h"

namespace IOU
{

template<typename T>
int findInterPoints(const Plygon<T> plygon1, const Plygon<T> plygon2, typename Plygon<T>::Vertexes &vert)
{
    typename Plygon<T>::Vertexes _vert;
    const int N = plygon2.pointCount();
    for (int i=0; i<N; ++i) {
        Plygon<T>::Vertexes pts;
        intersections(plygon1,Plygon<T>::LineType(plygon2[i%N],plygon2[(i+1)%N]),pts);
        for (int i = 0; i < pts.size(); ++i)
            _vert.push_back(pts.at(i));
    }
    vert.swap(_vert);
    return vert.size();
}

template<typename T>
int findInnerPoints(const Plygon<T> plygon1, const Plygon<T> plygon2, typename Plygon<T>::Vertexes &vert)
{
    typename Plygon<T>::Vertexes _vert;
    for (int i=0; i<plygon2.pointCount(); ++i) {
        if (location(plygon1, plygon2[i]) != Outside)
            _vert.push_back(plygon2[i]);
    }
    vert.swap(_vert);
    return vert.size();
}

template<typename T>
double areaIntersection(const Plygon<T> plygon1, const Plygon<T> plygon2)
{
    if (whichWise(plygon1) == NoneWise ||
        whichWise(plygon2) == NoneWise )
        return -1.0;

    typename Plygon<T>::Vertexes interVert;
    typename Plygon<T>::Vertexes innerVert12;
    typename Plygon<T>::Vertexes innerVert21;
    typename Plygon<T>::Vertexes allVerts;
    //---------------
    findInterPoints(plygon1, plygon2, interVert);
    findInnerPoints(plygon1, plygon2, innerVert12);
    findInnerPoints(plygon2, plygon1, innerVert21);
    //---------------
    // TODO : Check conditions
    for (unsigned int i = 0; i < interVert.size(); ++i)
        allVerts.push_back(interVert[i]);
    for (unsigned int i = 0; i < innerVert12.size(); ++i)
        allVerts.push_back(innerVert12[i]);
    for (unsigned int i = 0; i < innerVert21.size(); ++i)
        allVerts.push_back(innerVert21[i]);

    if (allVerts.empty())
        return 0.0;
    else {
        assert(allVerts.size() >= 3);
        Plygon<T> insertArea(allVerts);
        insertArea.sortInClockWise();
        if (insertArea.whichWise() == NoneWise)
            return -1.0;
        else
            return area(insertArea);
    }
    return -1.0;
}
template<typename T>
inline double areaUnion(const Plygon<T> plygon1, const Plygon<T> plygon2)
{
    return area(plygon1) + area(plygon2) - areaIntersection(plygon1, plygon2);
}

template<typename T>
inline double iou(const Plygon<T> plygon1, const Plygon<T> plygon2)
{
    return areaIntersection(plygon1, plygon2)/areaUnion(plygon1, plygon2);
}

}
#endif // IOU_IOU_H
