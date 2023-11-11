#ifndef MICTHREAD_H
#define MICTHREAD_H
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <QVector>
#include <QThread>
#include <QTimer>
#include <QObject>
#include <termios.h>
#include <unistd.h>


class Error;
class MicThread : public QThread
{
    Q_OBJECT



public:
    bool chk_dinfo = 0;
    double ff;
    double a;
    bool chk = 0;
    bool w_chk = 0;
    bool s_chk = 0;
    QVector<double> in;
    char* device = nullptr;
    double avg = 0;

    void run() override{

        connect_serial();
    }

    int connect_serial(){
        unsigned char buffer[1000];
        std::cout << "Waiting for device..."<< std::endl;

        while(!chk_dinfo) chk_gotdev(0);

        //getDeviceInfo(device);
        std::cout << "Port name : " << device << std::endl;
        if (device == NULL) {
            printf("Port error\n");
            return 1;

        }
        std::string str = device;
        std::system(("echo \"20181657\" | sudo -S chmod 777 "+str+"").c_str());


        int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);

        //O_RDWR : 파일을 읽기/쓰기용으로 연다, O_NDELAY : non-blocking 입출력 옵션 읽고 쓸 데이터 없으면 -1 리턴
            // O_NOCTTY : ctrl + c 무시
            if (fd == -1) {
                fprintf(stderr, "open(%s): %s\n", device, strerror(errno));
                getDeviceInfo(device);
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

            cfsetispeed(&tty, B230400);
            cfsetospeed(&tty, B230400);

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

            try{
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

                            while(!s_chk) chk_sendFlag(0);
                            if (in.size() >= 400 && !(in.size() <= 10)){
                                double amp_max = *std::max_element(in.begin(), in.end());
                                double amp_min = *std::min_element(in.begin(), in.end());
                                avg = (amp_max + amp_min) / 2;
                                for(int i = 0; i < in.size(); i++) in[i] = (in[i] - avg);
                                emit send_in(in);
                                emit fin_send(); // 조건문 걸고 받았다는 신호 받았다는 거 확인할 때 실행.

                                in.clear();
                                s_chk = 0;
                            }

                            if (buffer[i] == char(0x02)){
                                stx = true;
                                continue;

                            }

                            if (buffer[i] == char(0x03) && stx){
                                //std::cout << stoi(tmp);
                                try{in.append(stoi(tmp)/10);} //except
                                catch(const std::invalid_argument& e){}
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
        }
            }//try block endpoint
            catch (std::out_of_range& e){

            }
    }



    bool isRunning();
    bool isFinished();
    bool wait(QDeadlineTimer deadline = QDeadlineTimer(QDeadlineTimer::Forever));
    bool wait(unsigned long time);




public slots:

    void getDeviceInfo(char* dev){
        device = dev;
    }
    void chk_gotdev(bool a){
        if(a) {
            chk_dinfo = 1;
        }

    }

    void chk_trig(bool a){
        if(a){

            chk = 1;
        }
    }
    void wait_on(){
        w_chk = 1;

    }
    void wait_off(){
        w_chk = 0;

    }

    bool chk_sendFlag(bool a){
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


};





#endif // MICTHREAD_H
