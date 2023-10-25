#ifndef QTHREADS_H
#define QTHREADS_H
#include <QThread>
#include <mainwindow.h>

class Qthreads : public QThread
{
    Q_OBJECT

public:
    //Qthreads(QObject *parent = nullptr);
    //~Qthreads();

    double in;
    double ff;
    int points = 30000;
    double samp_freq = 441000;
    double a;
    bool chk = 0;
    bool w_chk = 0;

    void run() override{


        QString result;
        while(!chk) chk_trig(0);

        //신호 발생시키고 메인 위젯에 전송
        if(chk & !w_chk){
            for(int i = 0; i < points ; i = i + 1){
                a = rand()%1000;
                in = a;
                ff = (samp_freq * i) / points; //시간 맞춰야함
                std::cout << "in : "<< in <<std::endl;
                std::cout << "ff : "<< ff <<std::endl;
                emit send_io(in, ff);

                usleep(1);
            }

        }


    }

    bool isRunning();
    bool isFinished();
    bool wait(QDeadlineTimer deadline = QDeadlineTimer(QDeadlineTimer::Forever));
    bool wait(unsigned long time);



public slots:
    void chk_trig(bool a){
        if(a){
            std::cout << "checked";
            chk = 1;
        }
    }
    void wait_on(){
        w_chk = 1;
    }
    void wait_off(){
        w_chk = 0;
    }



signals:
    void send_io(double i, double f);
    void send_f(double f);
    void resultReady(const QString &s);

};


#endif // QTHREADS_H
