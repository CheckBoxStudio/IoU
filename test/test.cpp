/***********************************
 * test.h
 *
 * Test demo of iou.
 *
 * Author: WeiQM (weiquanmao@hotmail.com)
 * Github: https://github.com/CheckBoxStudio/IoU
 *
 * 2018
 ***********************************/

#include "test.h"

#include <random>

std::default_random_engine _rand_engine(2018);
std::normal_distribution<float> _rand_dis_n(0.0,1.0);
std::uniform_real_distribution<float> _rand_dis_u(0.1,0.9);
float randMeN(){
    float a = _rand_dis_n(_rand_engine);
    if (a>1.0)
        a = 1.0;
    if (a<-1.0)
        a = 1.0;
    return a;
}
float randMeU(){
    return _rand_dis_u(_rand_engine);
}
CvScalar colorMe(const Channel c)
{
    CvScalar color=cvScalar(10,10,10);
    switch(c) {
    case C_R:
        color=cvScalar(255,0,0);
        break;
    case C_G:
        color=cvScalar(0,255,0);
        break;
    case C_B:
        color=cvScalar(0,0,255);
        break;
    }
    return color;
}

void squareVertex(const int w, const int h, Vertexes &vert)
{
    int h1 = randMeN()*0.2*h + 0.25*h;
    int h2 = randMeN()*0.2*h + 0.75*h;
    if (h1<0) h1 = 0;
    if (h1>=h) h1 = h-1;
    if (h2<0) h2 = 0;
    if (h2>=h) h2 = h-1;
    if (h1>h2) {
        int temp = h1;
        h1 = h2;
        h2 = temp;
    }
    int w1 = randMeN()*0.2*w +0.25*w;
    int w2 = randMeN()*0.2*w +0.75*w;
    if (w1<0) w1 = 0;
    if (w1>=w) w1 = w-1;
    if (w2<0) w2 = 0;
    if (w2>=w) w2 = w-1;
    if (w1>w2) {
        int temp = w1;
        w1 = w2;
        w2 = temp;
    }

    Vertexes _vert;
    _vert.reserve(4);
    _vert.push_back(IOU::Point(w1,h1));
    _vert.push_back(IOU::Point(w1,h2));
    _vert.push_back(IOU::Point(w2,h2));
    _vert.push_back(IOU::Point(w2,h1));
    vert.swap(_vert);
}
void conQuadVertex(const int w, const int h, Vertexes &vert)
{
    while(1) {
        Vertexes _vert;
        _vert.reserve(4);
        _vert.push_back(IOU::Point(w*randMeU(),h*randMeU()));
        _vert.push_back(IOU::Point(w*randMeU(),h*randMeU()));
        _vert.push_back(IOU::Point(w*randMeU(),h*randMeU()));
        _vert.push_back(IOU::Point(w*randMeU(),h*randMeU()));
        beInSomeWiseEx(_vert,ClockWise);
        if (whichWiseEx(_vert) != NoneWise) {
            vert.swap(_vert);
            break;
        }
    }
}

IplImage* newEmptyImage(const int width, const int height)
{
    if (width<=0 || height<=0)
        return 0;
    IplImage *pImg = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 3);
    int s = pImg->widthStep;
    uchar *pData = (uchar *)pImg->imageData;
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j) {
            pData[j*3+0] = 0;
            pData[j*3+1] = 0;
            pData[j*3+2] = 0;
        }
        pData += s;
    }
    return pImg;
}
void drawSquare(IplImage *pImg, const Vertexes &vert, const Channel channel)
{
    if (pImg != 0) {
       cvRectangle(pImg,
                   cvPoint(vert[0].x,vert[0].y),
                   cvPoint(vert[2].x,vert[2].y),
                   colorMe(channel),CV_FILLED);
    }
}
void drawConvexQuad(IplImage *pImg, const Vertexes &vert, const Channel channel)
{
    int n[1];
    n[0] = vert.size();
    CvPoint ** pts = new CvPoint*[1];
    pts[0] = new CvPoint[n[0]];
    for (int i=0; i< n[0]; ++i) {
        pts[0][i].x = vert[i].x;
        pts[0][i].y = vert[i].y;
    }
    cvFillPoly(pImg,pts,n,1,colorMe(channel));
    delete[] pts[0];
    delete[] pts;
}
double countPixel(
        const IplImage *pImg,
        const Channel channel1, const Channel channel2,
        int &n1, int &n2, int &i12, int &u12)
{
    int k1 = channel1;
    int k2 = channel2;

    int _n1=0;
    int _n2=0;
    int _i12=0;
    int _u12=0;
    int w = pImg->width;
    int h = pImg->height;
    int s = pImg->widthStep;
    uchar *pData = (uchar *)pImg->imageData;
    for (int i=0; i<h; ++i) {
        for (int j=0; j<w; ++j) {
            bool b1 = pData[3*j+k1] > 128;
            bool b2 = pData[3*j+k2] > 128;
            if (b1) _n1++;
            if (b2) _n2++;
            if (b1||b2) _u12++;
            if (b1&&b2) _i12++;
        }
        pData += s;
    }
    n1 = _n1;
    n2 = _n2;
    i12 = _i12;
    u12 = _u12;

    return (i12*1.0)/(u12*1.0);
}

void testSquare(
        const int N,
        const int width, const int height,
        const bool showup,
        const int delay)
{
    printf("Test Squres [%d] Times with Image Size [%dx%d]\n",
           N, width, height);
    printf("No.    Method   Area_1   Area_2   Area_1x2  Area_1+2   IOU\n");
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    int C_n1, C_n2, C_i12, C_u12;
    int E_n1, E_n2, E_i12, E_u12;
    double C_iou, E_iou;
    for (int k=0; k<N; ++k) {
        Vertexes vert1, vert2;
        squareVertex(width,height,vert1);
        squareVertex(width,height,vert2);
        IplImage *pImg1 = newEmptyImage(width, height);
        IplImage *pImg2 = newEmptyImage(width, height);
        IplImage *pImg3 = newEmptyImage(width, height);
        drawSquare(pImg1, vert1, C_R);
        drawSquare(pImg2, vert2, C_B);
        cvAdd(pImg1,pImg2,pImg3);
        if (showup) {
            cvShowImage("Test_Square", pImg3);
            cvWaitKey(delay);
        }
        C_iou = countPixel(pImg3, C_R, C_B, C_n1, C_n2, C_i12, C_u12);
        E_n1 = areaEx(vert1);
        E_n2 = areaEx(vert2);
        E_i12 = areaIntersectionEx(vert1,vert2);
        E_u12 = areaUnionEx(vert1,vert2);
        E_iou = iouEx(vert1,vert2);

        printf("%05d  Count   %6d    %6d     %6d    %6d   %.3f\n",
               k+1, C_n1, C_n2, C_i12, C_u12, C_iou);
        printf("       Calcu   %6d    %6d     %6d    %6d   %.3f\n",
               E_n1, E_n2, E_i12, E_u12, E_iou);
        printf("----\n");
        if (abs(E_iou - E_i12*1.0/E_u12) > 0.05  ) {
            printf("--  Error in IOU, Please Check Me. --\n");
        }

        cvReleaseImage(&pImg1);
        cvReleaseImage(&pImg2);
        cvReleaseImage(&pImg3);
    }

    printf("\n");
    cvDestroyAllWindows();
}

void testConvexQuad(
        const int N,
        const int width, const int height,
        const bool showup,
        const int delay)
{
    printf("Test Convex Quad. [%d] Times with Image Size [%dx%d]\n",
           N, width, height);
    printf("Method   Area_1   Area_2   Area_1x2  Area_1+2   IOU\n");
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    int C_n1, C_n2, C_i12, C_u12;
    int E_n1, E_n2, E_i12, E_u12;
    double C_iou, E_iou;
    for (int k=0; k<N; ++k) {
        Vertexes vert1, vert2;
        conQuadVertex(width,height,vert1);
        conQuadVertex(width,height,vert2);
        IplImage *pImg1 = newEmptyImage(width, height);
        IplImage *pImg2 = newEmptyImage(width, height);
        IplImage *pImg3 = newEmptyImage(width, height);
        drawConvexQuad(pImg1, vert1, C_R);
        drawConvexQuad(pImg2, vert2, C_B);
        cvAdd(pImg1,pImg2,pImg3);
        if (showup) {
            cvShowImage("Test_ConQuad", pImg3);
            cvWaitKey(delay);
        }
        C_iou = countPixel(pImg3, C_R, C_B, C_n1, C_n2, C_i12, C_u12);
        E_n1 = areaEx(vert1);
        E_n2 = areaEx(vert2);
        E_i12 = areaIntersectionEx(vert1,vert2);
        E_u12 = areaUnionEx(vert1,vert2);
        E_iou = iouEx(vert1,vert2);
        printf("%05d  Count   %6d    %6d     %6d    %6d   %.3f\n",
               k+1, C_n1, C_n2, C_i12, C_u12, C_iou);
        printf("       Calcu   %6d    %6d     %6d    %6d   %.3f\n",
               E_n1, E_n2, E_i12, E_u12, E_iou);
        if (abs(E_iou - E_i12*1.0/E_u12) > 0.05  ) {
            printf("--  Error in IOU, Please Check Me. --\n");
        }
        printf("----\n");

        cvReleaseImage(&pImg1);
        cvReleaseImage(&pImg2);
        cvReleaseImage(&pImg3);
    }

    printf("\n");
    cvDestroyAllWindows();
}

