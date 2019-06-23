/***********************************
 * util.h
 *
 * Math utility for iou calculation.
 *
 * Author: WeiQM (weiquanmao@hotmail.com)
 * Github: https://github.com/CheckBoxStudio/IoU
 *
 * 2019
 ***********************************/

#ifndef IOU_UTIL_H
#define IOU_UTIL_H

#include "math.h"

namespace IOU
{ // Begin of namespace IOU

#ifndef Pi
#define Pi 3.141592653589793
#endif // Pi

template <typename T>
inline T Abs(const T &t) { return t >= 0 ? t : -t; }

template <typename T>
inline const T& Min(const T &a, const T &b) { return (a < b) ? a : b; }

template <typename T>
inline const T& Max(const T &a, const T &b) { return (a < b) ? b : a; }

template <typename T>
inline bool Compare(T a, T b) { return a == b; }

inline bool Compare(double p1, double p2) {
    return (Abs(p1 - p2) * 1000000000000. <= Min(Abs(p1), Abs(p2)));
}
inline bool Compare(float p1, float p2) {
    return (Abs(p1 - p2) * 100000.f <= Min(Abs(p1), Abs(p2)));
}

} // End of namespace IOU

#endif // IOU_UTIL_H
