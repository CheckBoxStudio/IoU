TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

OPENCV_DIR = D:/opencv/opencv/build

INCLUDEPATH += \
    src \
    $$OPENCV_DIR/include \
    $$OPENCV_DIR/include/opencv
    $$OPENCV_DIR/include/opencv2

win32:CONFIG(release, debug|release): \
    LIBS += \
    -L$$OPENCV_DIR/x64/vc11/lib/ \
    -lopencv_core244 \
    -lopencv_highgui244  \
    -lopencv_imgproc244
else:win32:CONFIG(debug, debug|release): \
    LIBS += \
    -L$$OPENCV_DIR/x64/vc11/lib/ \
    -lopencv_core244d \
    -lopencv_highgui244d \
    -lopencv_imgproc244d

HEADERS += \
    src/util.h \
    src/vec2.h \
    src/line.h \
    src/plygon.h \
    src/iou.h \
    test/test.h

SOURCES += \
    test/main.cpp \
    test/test.cpp

DISTFILES += \
    ReadMe.md


