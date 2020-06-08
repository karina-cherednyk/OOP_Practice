QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canvas.cpp \
    colorpicker/colorpicker.cpp \
    filterform.cpp \
    filters/afilter.cpp \
    filters/blackandwhitefilter.cpp \
    filters/brightnessfilter.cpp \
    filters/contrastfilter.cpp \
    filters/gaussianblur.cpp \
    filters/grayfilter.cpp \
    filters/medianfilter.cpp \
    filters/negativefilter.cpp \
    filters/noisefilter.cpp \
    filters/peronmalikblur.cpp \
    filters/rgbafilter.cpp \
    filters/sepiafilter.cpp \
    layerdelegate.cpp \
    layermodel.cpp \
    main.cpp \
    mainwindow.cpp \
    selectevent.cpp

HEADERS += \
    Layer.h \
    canvas.h \
    colorpicker/colorpicker.h \
    colorpicker/colorspectrum.h \
    filterform.h \
    filters/afilter.h \
    filters/blackandwhitefilter.h \
    filters/brightnessfilter.h \
    filters/contrastfilter.h \
    filters/gaussianblur.h \
    filters/grayfilter.h \
    filters/medianfilter.h \
    filters/negativefilter.h \
    filters/noisefilter.h \
    filters/peronmalikblur.h \
    filters/rgbafilter.h \
    filters/sepiafilter.h \
    layerdelegate.h \
    layermodel.h \
    mainwindow.h \
    resizable_inner_widget.h \
    rotateevent.h \
    samplewidget.h \
    selectevent.h \
    shapeevent.h

FORMS += \
    canvas.ui \
    colorpicker/colorpicker.ui \
    filterform.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    filters/test.jpg \
    filters/test.png
