/***********************************
 * test.h
 *
 * Test demo of iou.
 *
 * Author: WeiQM (weiquanmao@hotmail.com)
 * Github: https://github.com/CheckBoxStudio/IoU
 *
 * 2019
 ***********************************/

#include "test.h"
#include "src/iou.h"
#include <random>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace IOU;

enum Channel{ C_R=0, C_G=1, C_B=2 };

typedef Plygon<double> PlygonD;

std::default_random_engine _rand_engine(2019);
std::normal_distribution<float> _rand_dis_n(0.0,1.0);
std::uniform_real_distribution<float> _rand_dis_u(0.1,0.9);
float randMeN() {
    float a = _rand_dis_n(_rand_engine);
    if (a>1.0) a = 1.0;
    if (a<-1.0) a = 1.0;
    return a;
}
float randMeU() {
    return _rand_dis_u(_rand_engine);
}
CvScalar colorMe(const Channel c)
{
    CvScalar color=cvScalar(10,10,10);
    switch(c) {
    case C_R: color=cvScalar(255,0,0); break;
    case C_G: color=cvScalar(0,255,0); break;
    case C_B: color=cvScalar(0,0,255); break;
    default: break;
    }
    return color;
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
void genSquare(const int w, const int h, PlygonD &plygon)
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

    plygon.addPt(PlygonD::PointType(w1, h1));
    plygon.addPt(PlygonD::PointType(w1, h2));
    plygon.addPt(PlygonD::PointType(w2, h2));
    plygon.addPt(PlygonD::PointType(w2, h1));
}
void genPlygon(const int w, const int h, int k, PlygonD &plygon)
{
    assert(k>=3);

    while(1) {
        PlygonD tmpPlygon;
        for (int i=0; i<k; ++i)
            tmpPlygon.addPt(PlygonD::PointType(w*randMeU(),h*randMeU()));
        tmpPlygon.sortInSomeWise(ClockWise);
        if (tmpPlygon.whichWise() != NoneWise) {
            plygon = tmpPlygon;
            break;
        }
    }
}

void drawPlygon(IplImage *pImg, const PlygonD &plygon, const Channel channel)
{
    int n[1];
    n[0] = plygon.pointCount();
    CvPoint ** pts = new CvPoint*[1];
    pts[0] = new CvPoint[n[0]];
    for (int i=0; i< n[0]; ++i) {
        pts[0][i].x = plygon[i].x();
        pts[0][i].y = plygon[i].y();
    }
    cvFillPoly(pImg,pts,n,1,colorMe(channel));
    delete[] pts[0];
    delete[] pts;
}

int testSquare(
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
    int notPassed = 0;
    for (int k=0; k<N; ++k) {
        PlygonD plygon1, plygon2;
        genSquare(width,height,plygon1);
        genSquare(width,height,plygon2);
        IplImage *pImg1 = newEmptyImage(width, height);
        IplImage *pImg2 = newEmptyImage(width, height);
        IplImage *pImg3 = newEmptyImage(width, height);
        drawPlygon(pImg1, plygon1, C_R);
        drawPlygon(pImg2, plygon2, C_B);
        cvAdd(pImg1,pImg2,pImg3);
        if (showup) {
            cvShowImage("Test_Square", pImg3);
            cvWaitKey(delay);
        }
        C_iou = countPixel(pImg3, C_R, C_B, C_n1, C_n2, C_i12, C_u12);

        E_n1 = area(plygon1);
        E_n2 = area(plygon2);
        E_i12 = areaIntersection(plygon1,plygon2);
        E_u12 = areaUnion(plygon1,plygon2);
        E_iou = iou(plygon1,plygon2);

        printf("%05d  Count   %6d    %6d     %6d    %6d   %.3f\n",
               k+1, C_n1, C_n2, C_i12, C_u12, C_iou);
        printf("       Calcu   %6d    %6d     %6d    %6d   %.3f\n",
               E_n1, E_n2, E_i12, E_u12, E_iou);
        printf("----\n");
        if (abs(E_iou - E_i12*1.0/E_u12) > 0.05  ) {
            printf("--  Error in IOU, Please Check Me. --\n");
            notPassed ++;
        }

        cvReleaseImage(&pImg1);
        cvReleaseImage(&pImg2);
        cvReleaseImage(&pImg3);
    }

    printf("\n");
    cvDestroyAllWindows();

    return notPassed;
}

int testPlygon(
        const int K, const int N,
        const int width, const int height,
        const bool showup,
        const int delay)
{
    if (K<3) {
        printf("No pliygon with vertexse less than 3.");
        return 0;
    }

    printf("Test Plygon-%d. [%d] Times with Image Size [%dx%d]\n",
           K, N, width, height);
    printf("Method   Area_1   Area_2   Area_1x2  Area_1+2   IOU\n");
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    int C_n1, C_n2, C_i12, C_u12;
    int E_n1, E_n2, E_i12, E_u12;
    double C_iou, E_iou;
    int notPassed = 0;
    for (int k=0; k<N; ++k) {
        PlygonD plygon1, plygon2;
        genPlygon(width,height,K,plygon1);
        genPlygon(width,height,K,plygon2);
        IplImage *pImg1 = newEmptyImage(width, height);
        IplImage *pImg2 = newEmptyImage(width, height);
        IplImage *pImg3 = newEmptyImage(width, height);
        drawPlygon(pImg1, plygon1, C_R);
        drawPlygon(pImg2, plygon2, C_B);
        cvAdd(pImg1,pImg2,pImg3);
        if (showup) {
            cvShowImage("Test_ConQuad", pImg3);
            cvWaitKey(delay);
        }
        C_iou = countPixel(pImg3, C_R, C_B, C_n1, C_n2, C_i12, C_u12);
        E_n1 = area(plygon1);
        E_n2 = area(plygon2);
        E_i12 = areaIntersection(plygon1,plygon2);
        E_u12 = areaUnion(plygon1,plygon2);
        E_iou = iou(plygon1,plygon2);
        printf("%05d  Count   %6d    %6d     %6d    %6d   %.3f\n",
               k+1, C_n1, C_n2, C_i12, C_u12, C_iou);
        printf("       Calcu   %6d    %6d     %6d    %6d   %.3f\n",
               E_n1, E_n2, E_i12, E_u12, E_iou);
        if (abs(E_iou - E_i12*1.0/E_u12) > 0.05  ) {
            printf("--  Error in IOU, Please Check Me. --\n");
            notPassed++;
        }
        printf("----\n");

        cvReleaseImage(&pImg1);
        cvReleaseImage(&pImg2);
        cvReleaseImage(&pImg3);
    }

    printf("\n");
    cvDestroyAllWindows();

    return notPassed;
}
