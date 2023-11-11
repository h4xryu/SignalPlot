#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include "QTextEdit"
#include "QComboBox"
#include "QSlider"
#include "qcustomplot.h"
#include <bits/stdc++.h>
#include "iterator"
#include <iostream>
#include <complex>
#include <cmath>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <micThread.h>

const double PI = acos(-1);
typedef std::complex<double> cpx;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MicThread *mic;

    QCustomPlot *plot;
    QCustomPlot *plot_time;
    // x축 y축 벡터 정의
    QVector<double> vec_x,vec_y;
    QVector<double> mul_x;
    QQueue<QVector<double>> data;
    QVector<double> time;
    double scale;
    //발생시킬 신호의 위상
    double phase = 0;
    //슬라이더 전용 변수
    int _scale;
    int get_scale();
    QVector<double> amp;
    QAction *graphStartBtn;



    // 푸리에 변환 관련
    void FFT(QVector<cpx> &v, cpx w);
    QVector<cpx> DFT_vec(QVector<double> &v);
    QVector<cpx> FFT_vec(QVector<double> &v);
    QTimer *timer;
    QVector<double> in;
    QVector<cpx> out;
    QVector<double> dB;
    QVector<double> ff;
    double samp_freq;
    int points = 8192;

    // 문자열 객체 선언
     QString str, str2, editStr;
     // 포인터 메뉴 객체 선언
     QMenu *pGraphMenu;
     // 툴바 객체 선언
     QToolBar *GraphToolBar;
     // 포인터 상태바 객체 선언
     QStatusBar *pStartusbar;
     // 전경색, 배경색 객체 선언
     QColor fgColor, bgColor;
      // 텍스트 상자 객체 선언
     QTextEdit *qtextEdit;
      // 콤보상자 객체 선언
     QComboBox *comboBox, *comboBox2;
      // 슬라이드바 객체 선언
     QSlider *slider;
      // 마우스 좌표를 저장하기 위한 변수
     int x, y, mov_x, mov_y;
      // 도형 스타일 지정 변수
     int drawStyle;
    // 라인 스타일 지정 변수
     int lineStyle;
      // 스라이드바 값을 저장하기 위한 변수
     int sliderValue;
      // 그리기 상태를 나타내는 논리 변수
     bool drawID=false;
      // 폰트 객체 변수 선언
     QFont font;
      // 삼각형과 다각형을 그리기 위한 포인트형 배열변수
     QPointF pts[3], polygon[4];
      // 편집상자, 슬라이드바, 콤보상자를 생성하는 함수 선언
     void controlType();
      // 메뉴와 툴바 생성 함수 선언
     void menuToolbarCreate();
     // 그리기 함수 선언
     int draw(QPainter *painter, const QPoint &pos);
     //일반신호발생 함수
     void generate_sin(QVector<double> &in, QVector<double> &ff, int points, double samp_freq);
     void generate_singlePulse(QVector<double> &in, QVector<double> &ff, int points, double samp_freq);
     void set_reset();
     void generate_random_signal(QVector<double> &in, QVector<double> &ff, int points, double samp_freq, double time);
     void generate_triwave(QVector<double> &in, QVector<double> &ff, int points, double samp_freq);
     void generate_step_func(QVector<double> &in, QVector<double> &ff, int points, double samp_freq);
     void generate_reLU_func(QVector<double> &in, QVector<double> &ff, int points, double samp_freq);


     QString portName;
     QByteArray ba;
     char *device;

    ~MainWindow();

signals:
     QVector<double> get_in();
     QVector<double> get_out();


private:
    Ui::MainWindow *ui;

    void fillPortsInfo();
    QSerialPort* m_serialPort = nullptr;

      // 도형 콤보상자 항목 선택시 처리 함수 선언
     void Main_comboBox(int index);
      // 라인 스타일 콤보상자 항목 선택시 처리 함수 선언
     void Main_comboBox2(int index);
     // "도형->전경색" 함수 선언
     void Main_fgColor();
      // "도형->배경색" 함수 선언
     void Main_bgColor();
      // 그리기 텍스트 함수 선언
     void Main_DrawText();

      // 마우스 이벤트 처리 함수 선언
     int onMouseEvent(const QPoint &pos);

signals:
     void btn_triggered(bool b);
     void send_wait();
     void send_continue();
     void set_sendFlag(bool b);
     void sendDeviceInfo(char *device);
     void sendDinfoReady(bool b);
//슬롯 함수들
public slots:
     void Main_Slider(int value);
     void Plot_sin_graph();
     void Plot_FFT(QVector<double> i);
     //void Plot_Wave(double samp_freq, int points);
     void Input_dialog();

     void get_i(double i){
         std::cout << i ;
         in.append(i);
     }
     void get_f(double f){
         ff.append(f);
     }
     void update_graph(QVector<double>ff, QVector<double> dB);
     void update_time_graph(QVector<double>in);
     bool is_sendfin(){
         return 1;
     };

     //시리얼 관련
     int readData();
     void handleError(QSerialPort::SerialPortError error);
     void on_btnConnect_clicked();
     void on_btnDisConnect_clicked();


protected:
    // 마우스 드래그 이벤트 함수 선언
    void mouseMoveEvent(QMouseEvent *event) override;
    // 마우스 클릭 이벤트 함수 선언
    void mousePressEvent(QMouseEvent *event) override;
    // 마우스 업 이벤트 함수 선언
    void mouseReleaseEvent(QMouseEvent *event) override;
    // 페인트 이벤트 함수 선언
    void paintEvent(QPaintEvent *event);
};
#endif // MAINWINDOW_H
