#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <Qthreads.h>
#include <QObject>
#include <micThread.h>



int main(int argc, char *argv[])
{
    //Qthreads plot_th;
    qRegisterMetaType<QVector<double>>("QVector<double>");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Qthreads plot_th;
    MicThread mic;
    //plot_th.start();
    mic.start();
//    QObject::connect(&w, &MainWindow::btn_triggered, &plot_th, &Qthreads::chk_trig);
//    QObject::connect(&w, &MainWindow::send_wait, &plot_th, &Qthreads::wait_on);
//    QObject::connect(&w, &MainWindow::send_continue, &plot_th, &Qthreads::wait_off);
//    QObject::connect(&w, &MainWindow::set_sendFlag, &plot_th, &Qthreads::chk_sendFlag);
//    QObject::connect(&w, &MainWindow::is_sendfin, &plot_th, &Qthreads::fin_send);
//    QObject::connect(&plot_th, &Qthreads::send_in, &w, &MainWindow::Plot_FFT);

    QObject::connect(&w, &MainWindow::btn_triggered, &mic, &MicThread::chk_trig);
    QObject::connect(&w, &MainWindow::send_wait, &mic, &MicThread::wait_on);
    QObject::connect(&w, &MainWindow::send_continue, &mic, &MicThread::wait_off);
    QObject::connect(&w, &MainWindow::set_sendFlag, &mic, &MicThread::chk_sendFlag);
    QObject::connect(&w, &MainWindow::is_sendfin, &mic, &MicThread::fin_send);
    QObject::connect(&mic, &MicThread::send_in, &w, &MainWindow::Plot_FFT);
    QObject::connect(&mic, &MicThread::getDeviceInfo, &w, &MainWindow::sendDeviceInfo);

//    QObject::connect(&plot_th, &Qthreads::send_io, &w, &MainWindow::Plot_FFT);
    //connect(plot_th, &Qthreads::finished, plot_th, &QObject::deleteLater);



    return a.exec();
}

