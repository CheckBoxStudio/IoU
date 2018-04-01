#ifndef _IOU_TEST_H_FILE_
#define _IOU_TEST_H_FILE_

#include <cv.h>
#include <highgui.h>
#include "../src/iou.h"

using namespace cv;
using namespace IOU;

enum Channel{
    C_R = 0,
    C_G = 1,
    C_B = 2
};

IplImage* newEmptyImage(const int width, const int height);
void darwSquare(IplImage *pImg, const Vertexes &vert, const Channel channel);
void darwConvexQuad(IplImage *pImg, const Vertexes &vert, const Channel channel);
double countPixel(
        const IplImage *pImg,
        const Channel channel1, const Channel channel2,
        int &n1, int &n2, int &i12, int &u12);

void testSquare(
        const int N,
        const int width, const int height,
        const bool showup = true,
        const int delay = 500);
void testConvexQuad(
        const int N,
        const int width, const int height,
        const bool showup = true,
        const int delay = 500);

#endif // !_IOU_TEST_H_FILE_
