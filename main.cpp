#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <Qthreads.h>
#include <QObject>



int main(int argc, char *argv[])
{
    //Qthreads plot_th;
    qRegisterMetaType<QVector<double>>("QVector<double>");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Qthreads plot_th;
    plot_th.start();
    QObject::connect(&w, &MainWindow::btn_triggered, &plot_th, &Qthreads::chk_trig);
    QObject::connect(&w, &MainWindow::send_wait, &plot_th, &Qthreads::wait_on);
    QObject::connect(&w, &MainWindow::send_continue, &plot_th, &Qthreads::wait_off);
    QObject::connect(&w, &MainWindow::set_sendFlag, &plot_th, &Qthreads::chk_sendFlag);
    QObject::connect(&w, &MainWindow::is_sendfin, &plot_th, &Qthreads::fin_send);
    QObject::connect(&plot_th, &Qthreads::send_in, &w, &MainWindow::Plot_FFT);

//    QObject::connect(&plot_th, &Qthreads::send_io, &w, &MainWindow::Plot_FFT);
    //connect(plot_th, &Qthreads::finished, plot_th, &QObject::deleteLater);



    return a.exec();
}

