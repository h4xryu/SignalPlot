#ifndef QTHREADS_H
#define QTHREADS_H
#include <QThread>
#include <mainwindow.h>
#include <QTimer>
#include <QObject>



class Error;

class Qthreads : public QThread
{
    Q_OBJECT



public:
    //Qthreads(QObject *parent = nullptr);
    //~Qthreads();

    QVector<double> in;
    double ff;
    int points = 8192;
    double samp_freq = 44100;
    double a;

    bool chk = 0;
    bool w_chk = 0;
    bool s_chk = 0;



    void run() override{


        QString result;
        while(!chk) chk_trig(0);

        //신호 발생시키고 메인 위젯에 전송

        if(chk & !w_chk){

            while(1){ //while(아두이노가 available)
                a = (double)(rand()%1000)/200;
                in.append(a);
                //ff = (samp_freq * i) / points; //시간 맞춰야함
//                std::cout << "in : "<< in <<std::endl;
//
                if(chk_sendFlag() == 1 && in.size() >= 1000){ // adc 샘플링레이트를 44100이라면 1/44100Hz => 22.6 us * 100 => 22.6ms가 된다. 아두이노가 15000이라면 15000에 맞게 수정해야한다. 33.3us
                    send();

                    emit fin_send(); // 조건문 걸고 받았다는 신호 받았다는 거 확인할 때 실행.

                    in.clear();
                }


            }
        }
    }

    bool isRunning();
    bool isFinished();
    bool wait(QDeadlineTimer deadline = QDeadlineTimer(QDeadlineTimer::Forever));
    bool wait(unsigned long time);

    void startTimer(){

    }
    void send(){
        emit send_in(in);
    }





public slots:
    void chk_trig(bool a){
        if(a){
            //std::cout << "checked";
            chk = 1;
        }
    }
    void wait_on(){
        w_chk = 1;
//        std::cout << "wait_on"<< std::endl;
    }
    void wait_off(){
        w_chk = 0;
//        std::cout << "wait_off"<< std::endl;
    }

    bool chk_sendFlag(){
        if(a){
            s_chk = 1;
            return 1;
        } else {
            chk = 0;
            return 0;
        }
    }



signals:
    void send_in(QVector<double> in);
    void send_f(double f);
    void resultReady(const QString &s);
    void fin_send();

};


#endif // QTHREADS_H
