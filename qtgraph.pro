QT       += core gui \
serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qcustomplot
TEMPLATE += app

CONFIG += c++11 \
serialport

PKGCONFIG += opencv4
INCLUDEPATH += /usr/include/opencv4 \
/home/qtgraph/eigen3 \


LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp\
    qcustomplot.cpp

HEADERS += \
    mainwindow.h \
    micThread.h \
    qcustomplot.h \


FORMS += \
    mainwindow.ui



