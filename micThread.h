#ifndef MICTHREAD_H
#define MICTHREAD_H
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <QThread>
#include <mainwindow.h>
#include <QTimer>
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <vector>


class Error;

class MicThread : public QThread
{
    Q_OBJECT



public:
    double ff;
    double a;
    bool chk = 0;
    bool w_chk = 0;
    bool s_chk = 0;
    QVector<double> in;
    const char* device;
    void run() override{
        getDeviceInfo(device);
        connect_serial();
    }



    bool isRunning();
    bool isFinished();
    bool wait(QDeadlineTimer deadline = QDeadlineTimer(QDeadlineTimer::Forever));
    bool wait(unsigned long time);




public slots:

    void getDeviceInfo(const char* dev){
        device = dev;
    }

    void chk_trig(bool a){
        if(a){
            std::cout << "checked";
            chk = 1;
        }
    }
    void wait_on(){
        w_chk = 1;
        std::cout << "wait_on"<< std::endl;
    }
    void wait_off(){
        w_chk = 0;
        std::cout << "wait_off"<< std::endl;
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
    void send(QVector<double> results);
    int connect_serial(){
        unsigned char buffer[100];

        int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);

        //O_RDWR : 파일을 읽기/쓰기용으로 연다, O_NDELAY : non-blocking 입추력 옵션 읽고 쓸 데이터 없으면 -1 리턴
            // O_NOCTTY : ctrl + c 무시
            if (fd == -1) {
                fprintf(stderr, "open(%s): %s\n", device, strerror(errno));

            }

            struct termios tty;

            if (tcgetattr(fd, &tty) != 0) //메모리 포인터 주소 확인해서 열려있는지 없는지 속성 값 반환
            {
                fprintf(stderr, "tcgetattr(%s): %s\n", device, strerror(errno));
                return 1;
            }


            //control mode flag

            tty.c_cflag &= ~PARENB; //패리티 없음
            tty.c_cflag &= ~CSTOPB; // 1개의 정지 비트
            tty.c_cflag &= ~CSIZE;
            tty.c_cflag |= CS8; //8비트
            tty.c_cflag &= ~CRTSCTS;
            tty.c_cflag |= CREAD | CLOCAL;

            tty.c_lflag &= ~ICANON;//incannical한 명령어 (\n단위 아님)
            tty.c_lflag &= ~ECHO; //에코설정
            tty.c_lflag &= ~ECHOE;
            tty.c_lflag &= ~ECHONL;
            tty.c_lflag &= ~ISIG;
            tty.c_iflag &= ~(IXON | IXOFF | IXANY); //시작비트 종료비트 미사용
            tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

            tty.c_oflag &= ~OPOST;
            tty.c_oflag &= ~ONLCR;

            tty.c_cc[VTIME] = 0;
            tty.c_cc[VMIN] = 0;

            cfsetispeed(&tty, B115200);
            cfsetospeed(&tty, B115200);

            if (tcsetattr(fd, TCSANOW, &tty) != 0) {
                fprintf(stderr, "tcgetattr(%s): %s\n", device, strerror(errno));
                return 1;
            }

            std::vector<double> dataBuff;
            std::string tmp;
            int tmp2 = 0;
            bool stx = false;
            bool isfull = false;
            bool breakPoint = false;


            while(!chk) chk_trig(0);
            if(chk & !w_chk){ while(1){
                ssize_t num_bytes = read(fd, &buffer, sizeof(buffer));

                if (num_bytes == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        continue;
                    }
                    fprintf(stderr, "read(%s): %s\n", device, strerror(errno));
                    break;
                }

                if (num_bytes > 0) {
                    for (ssize_t i = 0; i < num_bytes; ++i) {
                        if (chk_sendFlag() == 1 && in.size() >= 1000){
                            emit send_in(in);
                            emit fin_send(); // 조건문 걸고 받았다는 신호 받았다는 거 확인할 때 실행.
                            in.clear();
                        }

                        if (buffer[i] == '@'){
                            stx = true;
                            continue;

                        }

                        if (buffer[i] == '#' && stx){
                            std::cout << stoi(tmp);
                            in.append(stoi(tmp)); //여기서 중요함
                            stx = false;
                            tmp.clear();
                            isfull = false;
                            continue;
                            }
                        if (stx && isprint(buffer[i])) {
                            unsigned char ttmp;
                            ttmp = buffer[i];
                            tmp += ttmp;
                            continue;
                        }

                    }

                    if(breakPoint){
                        break;
                    }
                }
            }
    }}

};



#endif // MICTHREAD_H
