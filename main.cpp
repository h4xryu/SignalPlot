#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <Qthreads.h>



int main(int argc, char *argv[])
{
    //Qthreads plot_th;



    QApplication a(argc, argv);
    MainWindow w;
    //connect(plot_th, &Qthreads::resultReady, w, &MainWindow::controlType);
    //connect(plot_th, &Qthreads::finished, plot_th, &QObject::deleteLater);
    //plot_th.start();
    w.show();
    return a.exec();
}

