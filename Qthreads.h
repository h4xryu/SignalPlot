#ifndef QTHREADS_H
#define QTHREADS_H
#include <QThread>
#include <mainwindow.h>


class Qthreads : public QThread
{
    Q_OBJECT

public:
    explicit Qthreads(QObject *parent = nullptr);
    ~Qthreads(){

    };

    void run() override{
        while(1){
            QString result;

            //신호 발생시키고 메인 위젯에 전송

            emit resultReady(result);
            sleep(1);
        }
    }

    bool isRunning();
    bool isFinished();
    bool wait(QDeadlineTimer deadline = QDeadlineTimer(QDeadlineTimer::Forever));
    bool wait(unsigned long time);

    void generate_random_signal(QVector<double> &in, QVector<double> &ff, int points, double samp_freq, double t){


        for(int i = 0; i < points ; i = i + 1){
            double a = rand()%1000;
            in.append(a);
            ff.append((samp_freq * i) / points);
            QThread::sleep(t);

        }


    }




public slots:
    void exit(int returnCode = 0);
    void quit();



signals:
    void started();
    void finished();
    void resultReady(const QString &s);

};


#endif // QTHREADS_H
