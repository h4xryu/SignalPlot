QT       += core gui \
serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport serialport

TEMPLATE += app

CONFIG += c++11 \
serialport



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp\
    qcustomplot.cpp\

HEADERS += \
    mainwindow.h \
    qcustomplot.h \
    micThread.h \
    micThread.h \


FORMS += \
    mainwindow.ui



