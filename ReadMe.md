# IOU

> Template-base version is available in branch **Featured**.

This is a C++ implement of intersection over union (IoU) ratio calculation, which requires no third-party libraries. And the IoU calculation method is applicable to rectangles, rotated rectangles and arbitrary convex polygons. A test demo is also included in this project, which can verify the validity and accuracy of the IoU calculation method.

---

## About the IoU calculation method

Given two convex polygons `Q1` and `Q2`, the IoU ration of `Q1` and `Q2` is estimated as:

```
Area(Q1+Q2) = Area(Q1) + Area(Q2) - Area(Q1*Q2)
IoU         = Area(Q1*Q2) / Area(Q1+Q2)
'+' means union;
'*' means intersection.
```

### Area of a convex polygon

The convex polygon is decomposed into a group of triangles, which are non-overlapping and complementary. The area of the convex polygon is equal to the sum of all the triangular areas.

![triangles](https://github.com/CheckBoxStudio/IoU/blob/master/images/triangles.png)

### Intersection of two convex polygons

The intersection of two convex polygons is also a convex polygon, which consists of two types of vertexes, i.e., a) the intersection points of edges of the two convex polygons and b) the vertices of the polygon which are located inside the other convex polygon. Once all these points are found, the intersection of two convex polygons is obtained. And the area of the intersection, treated as a convex polygon, can then be calculated.

![iou](https://github.com/CheckBoxStudio/IoU/blob/master/images/iou.png)

### Area of union of two polygons

The area of the union of two polygons is equal to the sum of the areas of the polygon minus the area of its intersection.

```
Area(Q1+Q2) = Area(Q1) + Area(Q2) - Area(Q1*Q2)

'+' means union;
'*' means intersection.
```

---

## About the test demo

In the test demo, a black background image `I` is first generated, and two convex polygons `Q1` and `Q2` are then randomly generated. `Q1` is drawn on `I` in Red-channel, and `Q2` is drawn in Blue-channel. Namely, the region of `Q1` is in Red, and the region of `Q2` is in Blue, while the intersection region of `Q1` and `Q2` is in Magenta. The areas of `Q1`, `Q2`, intersection of `Q1` and `Q2`, union of `Q1` and `Q2` can be calculated by directly counting the number of pixels with different colors, and the IoU ratio is further calculated. Such values, calculated by counting, is compared to the values that are given by the IoU calculation method.

Noted that [OpenCV](https://opencv.org/) is required for dealing with the images in the test demo.

---
By [WeiQM](https://weiquanmao.github.io) at D409.IPC.BUAA.
