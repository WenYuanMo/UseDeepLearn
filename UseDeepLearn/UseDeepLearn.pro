QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH+=\
    install/include\
    install/include/opencv2\
    ShowImageWidget\
    onnxruntimeWin/include\

# $$PWD代表.pro目录

LIBS+=\
$$PWD\install/x64/vc16/lib/opencv_world480d.lib \
$$PWD\onnxruntimeWin/lib/onnxruntime.lib\
$$PWD\onnxruntimeWin/lib/onnxruntime_providers_shared.lib\

SOURCES += \
    DealWithImage/dealwithimage.cpp \
    ShowImageWidget/showimagewidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    DealWithImage/dealwithimage.h \
    ShowImageWidget/showimagewidget.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
