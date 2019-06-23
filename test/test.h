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

#ifndef IOU_TEST_H
#define IOU_TEST_H

int testSquare(
        const int N,
        const int width, const int height,
        const bool showup = true,
        const int delay = 100);
int testPlygon(
        const int K, const int N,
        const int width, const int height,
        const bool showup = true,
        const int delay = 100);

#endif // IOU_TEST_H
