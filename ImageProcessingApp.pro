QMAKE_PROJECT_DEPTH = 0

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DenoiseFilters.cpp \
    FloatSlider.cpp \
    FrequencyDomainFilters.cpp \
    Image.cpp \
    ImageListItemContainerWidget.cpp \
    ImageListWidget.cpp \
    ImageLoader.cpp \
    ImageManager.cpp \
    ImageOperation.cpp \
    ImageTab.cpp \
    ImageTabManager.cpp \
    ImageView.cpp \
    MatUtils.cpp \
    NewImageDialog.cpp \
    Opeartions.cpp \
    Singleton.cpp \
    SpaceFilters.cpp \
    ToolBarManager.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    DenoiseFilters.h \
    FloatSlider.h \
    FrequencyDomainFilters.h \
    Image.h \
    ImageListItemContainerWidget.h \
    ImageListWidget.h \
    ImageLoader.h \
    ImageManager.h \
    ImageOperation.h \
    ImageTab.h \
    ImageTabManager.h \
    ImageView.h \
    MatUtils.h \
    NewImageDialog.h \
    Opeartions.h \
    Singleton.h \
    SpaceFilters.h \
    ToolBarManager.h \
    mainwindow.h \
    ui_mainwindow.h

FORMS += \
    mainwindow.ui


INCLUDEPATH += "D:/opencv/opencv_build/install/include"

LIBS += \
    -LD:/opencv/opencv_build/install/x64/mingw/lib \
    -lopencv_calib3d4100 \
    -lopencv_core4100 \
    -lopencv_dnn4100 \
    -lopencv_features2d4100 \
    -lopencv_flann4100 \
    -lopencv_gapi4100 \
    -lopencv_highgui4100 \
    -lopencv_imgcodecs4100 \
    -lopencv_imgproc4100 \
    -lopencv_ml4100 \
    -lopencv_objdetect4100 \
    -lopencv_photo4100 \
    -lopencv_stitching4100 \
    -lopencv_video4100

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
