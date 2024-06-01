/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QCustomPlot *widget;
    QCustomPlot *widget_2;
    QComboBox *comboBox;
    QPushButton *btnConnect;
    QPushButton *btnDisConnect;
    QPushButton *btnRefresh;
    QCustomPlot *widget_3;
    QRadioButton *hanning;
    QRadioButton *hamming;
    QRadioButton *none;
    QPushButton *apply;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1291, 1024);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        widget = new QCustomPlot(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 40, 631, 441));
        widget_2 = new QCustomPlot(centralwidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(650, 40, 631, 441));
        comboBox = new QComboBox(centralwidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(10, 900, 91, 21));
        btnConnect = new QPushButton(centralwidget);
        btnConnect->setObjectName(QString::fromUtf8("btnConnect"));
        btnConnect->setGeometry(QRect(10, 810, 91, 21));
        btnDisConnect = new QPushButton(centralwidget);
        btnDisConnect->setObjectName(QString::fromUtf8("btnDisConnect"));
        btnDisConnect->setGeometry(QRect(10, 840, 91, 21));
        btnRefresh = new QPushButton(centralwidget);
        btnRefresh->setObjectName(QString::fromUtf8("btnRefresh"));
        btnRefresh->setGeometry(QRect(10, 870, 91, 21));
        widget_3 = new QCustomPlot(centralwidget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(270, 500, 991, 381));
        hanning = new QRadioButton(centralwidget);
        hanning->setObjectName(QString::fromUtf8("hanning"));
        hanning->setGeometry(QRect(10, 670, 112, 23));
        hamming = new QRadioButton(centralwidget);
        hamming->setObjectName(QString::fromUtf8("hamming"));
        hamming->setGeometry(QRect(10, 700, 112, 23));
        none = new QRadioButton(centralwidget);
        none->setObjectName(QString::fromUtf8("none"));
        none->setGeometry(QRect(10, 730, 112, 23));
        apply = new QPushButton(centralwidget);
        apply->setObjectName(QString::fromUtf8("apply"));
        apply->setGeometry(QRect(110, 730, 31, 25));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1291, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnConnect->setText(QCoreApplication::translate("MainWindow", "connect", nullptr));
        btnDisConnect->setText(QCoreApplication::translate("MainWindow", "disconnect", nullptr));
        btnRefresh->setText(QCoreApplication::translate("MainWindow", "refresh", nullptr));
        hanning->setText(QCoreApplication::translate("MainWindow", "hanning", nullptr));
        hamming->setText(QCoreApplication::translate("MainWindow", "hamming", nullptr));
        none->setText(QCoreApplication::translate("MainWindow", "none", nullptr));
        apply->setText(QCoreApplication::translate("MainWindow", "ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
