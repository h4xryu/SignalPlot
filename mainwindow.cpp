#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.cpp"
#include <QQueue>
#include "QMenuBar"
#include "QStatusBar"
#include "QAction"
#include "QToolBar"
#include "QColorDialog"
#include "QPainter"
#include "QPainterPath"
#include "QMouseEvent"
#include "QFontDialog"
#include <iostream>
#include "cmath"
#include "cstring"
#include "iostream"
#include "csignal"
#include <time.h>
#include <fstream>
#include <QInputDialog>
#include <librosa.h>
//#include <Qthreads.h>
#include <QTimer>
#include <QThread>




using namespace std;

class Error;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    timer = new QTimer(this);
    ui->setupUi(this);
    plot = ui->widget;



    // 편집상자, 슬라이드바, 콤보상자를 생성하는 함수 호출
    controlType();
    // 메뉴와 툴바 생성 함수 선언
    menuToolbarCreate();




}

// 그래프 그리는 함수
void MainWindow::Plot_sin_graph()
{
    QPalette pal = QPalette();
    QPen pen;
    pal.setColor(QPalette::Window, Qt::black);
    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");
    plot->xAxis->setRange(-10 - scale ,10 + scale);
    plot->yAxis->setRange(-5 - scale, 5 + scale);
    plot->addGraph();
    //generate_sin(in, ff, points, samp_freq);
    plot->xAxis->setLabelColor(QColor(0,255,0,255));
    plot->yAxis->setLabelColor(QColor(0,255,0,255));
    plot->xAxis->setTickLabelColor(QColor(0,255,0,255));
    plot->yAxis->setTickLabelColor(QColor(0,255,0,255));
    plot->graph(0)->setData(vec_x, vec_y);
    plot->setBackground(QColor(0,0,0,0));
    pen.setColor(QColor(0,255,0,255));
    plot->setAutoFillBackground(true);
    plot->setPalette(pal);
    plot->graph(0)->setPen(pen);
    plot->replot();

}

/*
void MainWindow::Plot_timeGraph(double samp_freq, int points){
     generate_random_signal(in, ff, points, samp_freq,1);
     double t_ = *max_element(time.begin(), time.end());
     plot->xAxis->setRange(0, t_);
     plot->yAxis->setRange(100 + scale, 100 - scale);
}*/
void MainWindow::update_graph(QVector<double>ff, QVector<double> dB){
    //최대 최소값 구하기

    double ff_max = *max_element(ff.begin(), ff.end());
    double ff_min = *min_element(ff.begin(), ff.end());

    double dB_max = *max_element(dB.begin(), dB.end());
    double dB_min = *min_element(dB.begin(), dB.end());
    plot->xAxis->setRange(ff_min - 100 + scale,ff_max + 100 - scale);
    plot->yAxis->setRange(dB_min - 100 + scale, dB_max + 100 - scale);





    //plot->graph(0)->setData(time, in);
    plot->graph(0)->setData(ff, dB);



    plot->replot();
}
void MainWindow::Plot_FFT(QVector<double> in){
    //그래프 색
    QPalette pal = QPalette();
    QPen pen;
    pal.setColor(QPalette::Window, Qt::black);
    pen.setColor(QColor(0,255,0,255));

    plot->setAutoFillBackground(true);
    plot->setPalette(pal);

    plot->xAxis->setLabelColor(QColor(0,255,0,255));
    plot->yAxis->setLabelColor(QColor(0,255,0,255));
    plot->xAxis->setTickLabelColor(QColor(0,255,0,255));
    plot->yAxis->setTickLabelColor(QColor(0,255,0,255));
    plot->addGraph();
    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");
    plot->graph(0)->setPen(pen);
    plot->setBackground(QColor(0,0,0,0));


    /*------신호 세팅 부분 ------*/
    //double y;
    //double dF = samp_freq / points;
    //double T = 1/ samp_freq;
    //double freq = 32000;


    /* 아두이노 포트 혹은 네트워크 연결 되기 전까지는 모든 메뉴 아이콘 비활성화
        이후 연결 확인되면 재생 가능 재생은 무한루프이고 정지를 누르면 while문에서 빠져나옴
        나중에 스레드 사용*/

    /*// "정지" 아이콘 설정
    const QIcon stop = QIcon::fromTheme("start", QIcon(""));
    // 정지 객체 생성
    QAction *pSlotfgColor = new QAction(stop, tr("&Graph stop"), this);
    connect(pSlotfgColor, SIGNAL(triggered()), this, SLOT(Input_dialog()));
    // 메뉴바에 "그래프 시작 추가
    pGraphMenu->addAction(pSlotfgColor);
    // 툴바에 색상 아이콘 추가
    GraphToolBar->addAction(pSlotfgColor);
    // 상태바을 연결시킨다.
    pStartusbar = statusBar();*/





    //신호 종류별
    //generate_singlePulse(in, ff, points, samp_freq);
    //generate_sin(in, ff, points, samp_freq);
    //generate_random_signal(in, ff, points, samp_freq,1);
    //generate_triwave(in, ff, points, samp_freq);
    //generate_reLU_func(in, ff, points, samp_freq);
    //generate_step_func(in, ff, points, samp_freq);


    // 데이터 받아와서 버퍼에 저장 <- 나눠야함

    emit set_sendFlag(1);
    while (!is_sendfin()){
        QThread::msleep(10);
        QTimer* myTimer = new QTimer;
        myTimer->start(200);
        connect(myTimer, SIGNAL(timeout()), this, SLOT(Plot_FFT()));
    }
    emit set_sendFlag(0);



    QVector<double> tmp_ff;
    QVector<double> tmp_in;
    QVector<double> tmp_dB;
    QVector<cpx> tmp_out;



    emit send_wait();
    if(ff.size() < points) {
        //샘플링 주파수는 그대로
        for(int i = -((points/2)-1); i<0; i++){
            tmp_ff.append((samp_freq * i) / points);
        }
        for(int i = 0; i<=((points/2)-1); i++){
            tmp_ff.append((samp_freq * i) / points);
        }

    }
    if(in.size() < points -1){
        // 음수 부분
        for (int i = in.size(); i < points/2 - in.size() -1; i++){
            tmp_in.append(0);
        }
        for (int i = 0; i < in.size(); i++){
            tmp_in.append(in[in.size() - i -1]);
        }


        // 양수 부분
        for (int i = 0; i < in.size(); i++){
            tmp_in.append(in[i]);
        }
        for (int i = ff.size(); i < points/2 - ff.size() -1; i++){
            tmp_in.append(0);
        }


    }
    tmp_out = FFT_vec(tmp_in);
    for(int i = ((points/2)-1); i> 0; i--){
        tmp_dB.append(20*log(sqrt(tmp_out[i].real()*tmp_out[i].real() + tmp_out[i].imag()*tmp_out[i].imag())));
    }
    for(int i = 0; i<= ((points/2)-1); i++){
        tmp_dB.append(20*log(sqrt(tmp_out[i].real()*tmp_out[i].real() + tmp_out[i].imag()*tmp_out[i].imag())));
    }


    emit send_continue();
    update_graph(tmp_ff, tmp_dB);
    tmp_dB.clear();

    //if()




   /* if(in.size() == 5 && ff.size() == 5){
        QVector<double> tmp_ff;
        QVector<double> tmp_in;
        QVector<double> tmp_dB;
        QVector<cpx> tmp_out;
        int start;
        int neg_i;
        emit send_wait();
        if(ff.size() < points) {
            //샘플링 주파수는 그대로
            for(int i = -((points/2)-1); i<0; i++){
                tmp_ff.append((samp_freq * i) / points);
            }
            for(int i = 0; i<=((points/2)-1); i++){
                tmp_ff.append((samp_freq * i) / points);
            }

        }
        if(in.size() < points -1){
            // 음수 부분
            for (int i = in.size(); i < points/2 - in.size() -1; i++){
                tmp_in.append(0);
            }
            for (int i = 0; i < in.size(); i++){
                tmp_in.append(in[in.size() - i -1]);
            }


            // 양수 부분
            for (int i = 0; i < in.size(); i++){
                tmp_in.append(in[i]);
            }
            for (int i = ff.size(); i < points/2 - ff.size() -1; i++){
                tmp_in.append(0);
            }


        }
        tmp_out = FFT_vec(tmp_in);
        for(int i = ((points/2)-1); i> 0; i--){
            tmp_dB.append(20*log(sqrt(tmp_out[i].real()*tmp_out[i].real() + tmp_out[i].imag()*tmp_out[i].imag())));
        }
        for(int i = 0; i<= ((points/2)-1); i++){
            tmp_dB.append(20*log(sqrt(tmp_out[i].real()*tmp_out[i].real() + tmp_out[i].imag()*tmp_out[i].imag())));
        }


        emit send_continue();
        update_graph(tmp_ff, tmp_dB);
    }*/




    /*




    //신호의 세기




    //for(int i = 0; i<dB.size(); i++) std::cout << "x축 : " << ff[i] << "y축 : " << dB[i] << std::endl;


    //scale = get_scale();




    //QThread::msleep(500);*/
}



void MainWindow::Input_dialog(){
    bool ok = false;
    QVector<double> i;
    double f = 0;

    i.append(0);

    double d = QInputDialog::getDouble(this, tr("Generating signal"),
                                   tr("samp_rate:"), 441000, -1280000, 1280000, 1, &ok);
    //여기서 연결
    if (ok)
        samp_freq = d;
        emit btn_triggered(1);
        Plot_FFT(i);


}

void MainWindow::generate_sin(QVector<double> &in, QVector<double> &ff, int points, double samp_freq){

    double neg_w = phase;
    double w = phase;

    for(int i = 0; i < points + (int)phase; i = i + 1){
        in.append(sin(neg_w - phase));
        in.append(neg_w);
        neg_w = neg_w - 0.005729;
    }
    for(int i = 0; i < points + (int)phase; i = i + 1){
        in.append(sin(w - phase));
        in.append(w);
        w = w+ 0.005729;
    }


}

void MainWindow::generate_triwave(QVector<double> &in, QVector<double> &ff, int points, double samp_freq){

    double neg_w = phase;
    double w = phase;

    for(int i = 0; i < points + (int)phase; i = i + 1){
        in.append((40/(PI)*(PI))*cos(neg_w)+(40/(3*PI)*(3*PI))*cos(3*neg_w)+(40/(5*PI)*(5*PI))*cos(5*neg_w));
        in.append(neg_w);
        neg_w = neg_w - 0.005729;
    }
    for(int i = 0; i < points + (int)phase; i = i + 1){
        in.append((40/(PI)*(PI))*cos(w)+(40/(3*PI)*(3*PI))*cos(3*w)+(40/(5*PI)*(5*PI))*cos(5*w));
        in.append(w);
        w = w+ 0.005729;
    }
    for(int i = -((points/2)-1); i<0; i++){
        ff.append((samp_freq * i) / points);
    }
    for(int i = 0; i<=((points/2)-1); i++){
        ff.append((samp_freq * i) / points);
    }

}

void MainWindow::generate_random_signal(QVector<double> &in, QVector<double> &ff, int points, double samp_freq, double t){


    for(int i = 0; i < points + (int)phase; i = i + 1){
        double a = rand()%1000;
        in.append(a);
        ff.append((samp_freq * i) / points);
        /*QThread::sleep(t);
        time.append(t);*/
    }
    /*
    for(int i = -((points/2)-1); i<0; i++){
        ff.append((samp_freq * i) / points);

    }
    for(int i = 0; i<=((points/2)-1); i++){
        ff.append((samp_freq * i) / points);

    }*/

}

void MainWindow::generate_step_func(QVector<double> &in, QVector<double> &ff, int points, double samp_freq){


    for(int i = 0; i < points / 2 + (int)phase; i = i + 1){
        in.append(0);
    }
    for(int i = points / 2; i < points + (int)phase; i = i + 1){
        in.append(1);
    }
    for(int i = -((points/2)-1); i<0; i++){
        ff.append((samp_freq * i) / points);
    }
    for(int i = 0; i<=((points/2)-1); i++){
        ff.append((samp_freq * i) / points);
    }

}

void MainWindow::generate_reLU_func(QVector<double> &in, QVector<double> &ff, int points, double samp_freq){


    for(int i = 0; i < points / 2 + (int)phase; i = i + 1){
        in.append(0);
    }
    for(int i = points / 2; i < points + (int)phase; i = i + 1){
        in.append(i-points);
    }
    for(int i = -((points/2)-1); i<0; i++){
        ff.append((samp_freq * i) / points);
    }
    for(int i = 0; i<=((points/2)-1); i++){
        ff.append((samp_freq * i) / points);
    }

}

void MainWindow::generate_singlePulse(QVector<double> &in, QVector<double> &ff, int points, double samp_freq){
    for (int i = 0; i <100; i++){
        double mul = 0.5*(1-cos(2*PI*i/(points-1)));

        in.append(mul * 0);
    }
    for (int i = 100; i <150; i++){
        double mul = 0.5*(1-cos(2*PI*i/(points-1)));
        in.append(mul *1);
    }
    for (int i = 150; i <points; i++){
        double mul = 0.5*(1-cos(2*PI*i/(points-1)));
        in.append(mul * 0);
    }

    for(int i = -((points/2)-1); i<0; i++){
        ff.append((samp_freq * i) / points);
    }
    for(int i = 0; i<=((points/2)-1); i++){
        ff.append((samp_freq * i) / points);
    }
}


QVector<cpx> MainWindow::DFT_vec(QVector<double> &v) {

    int N = v.size();
    cpx imag(0,1);
    QVector<cpx> result;

    for (int i= 0; i < N; ++i){
        cpx tmp(0,0);

        for (int j = 0; j < N; ++j){
            tmp += v.at(j) * exp(-2*PI*i*j*imag/(double)N);
        }
        result.append(tmp);
    }

    return result;
}

void MainWindow::FFT(QVector<cpx> &v, cpx w) {
    int n = v.size();
    if(n == 1) return;

    QVector<cpx> even(n/2), odd(n/2);
    for(int i=0; i<n; i++) {
        if(i % 2 == 0) even[i/2] = v[i];
        else odd[i/2] = v[i];
    }

    FFT(even, w*w);
    FFT(odd, w*w);

    cpx z(1, 0);
    for(int i=0; i<n/2; i++) {
        v[i] = even[i] + z*odd[i];
        v[i + n/2] = even[i] - z*odd[i];

        z *= w;
    }
}

QVector<cpx> MainWindow::FFT_vec(QVector<double> &v) {
    QVector<double> w(v.size());

    int n = 1;
    while(n <= v.size()) n *= 2;

    v.resize(n);

    QVector<cpx> v_(n);
    for(int i=0; i<n; i++) {
        v_[i] = cpx(v[i], 0);
    }
    cpx unit(cos(2*PI/n), -sin(2*PI/n));

    FFT(v_, unit);


    return v_;
}


void MainWindow::controlType()
{

    // 슬라이드바 객체 생성
    slider=new QSlider(Qt::Horizontal ,this);
    // 슬라이드바 크기 설정
    slider->resize(100, 22);
    // 슬라이드바 위치 설정
    slider->move(380, 60); // move(220, 30);
    // 슬라이드바 드래그시 on_horizontalSlider_valueChanged 함수 호출
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(Main_Slider(int)));
    sliderValue=1;
    //bgColor.setRgb(255, 255, 255);
}

// 슬라이드바 처리 함수
void MainWindow::Main_Slider(int value)
{
    str="value:"+QString("%1\%").arg(value);
    ui->statusbar->showMessage(str);
    // 슬라이드바의 값을 sliderValue에 대입
    sliderValue=value;
    // 위상 조정
    _scale = (double)sliderValue * 100;
}

int MainWindow::get_scale(){
    return _scale;
}


void MainWindow::set_reset(){ //인자로 벡터 받자.
    vec_x.clear();
    vec_y.clear();
}


// 메뉴와 툴바 구성  함수
void MainWindow::menuToolbarCreate()
{
    // 메뉴바에 도형 메뉴 추가작
        pGraphMenu = menuBar()->addMenu(tr("&Graph"));
        // 툴바에 도형 추가
        GraphToolBar = addToolBar(tr("Graph"));
        // "시작" 아이콘 설정
        const QIcon fgcolorIcon = QIcon::fromTheme("start", QIcon("D:/Work/QT Creator/Controls/color2.bmp"));
        // 시작 객체 생성
        QAction *pSlotfgColor = new QAction(fgcolorIcon, tr("&Graph start"), this);


        pSlotfgColor->setShortcut(tr("Alt+C"));
        pSlotfgColor->setStatusTip(tr("그래프를 시작합니다."));
        //  연결 함수 -> 그래프 그리기 시작



//        connect(pSlotfgColor, SIGNAL(triggered()), this, SLOT(Plot_sin_graph()));
        connect(pSlotfgColor, SIGNAL(triggered()), this, SLOT(Input_dialog()));


        // 메뉴바에 "그래프 시작 추가
        pGraphMenu->addAction(pSlotfgColor);
        // 툴바에 색상 아이콘 추가
        GraphToolBar->addAction(pSlotfgColor);

        // 상태바을 연결시킨다.
        pStartusbar = statusBar();
}







void MainWindow::Plot_DFT(double samp_freq, int points){

    QPalette pal = QPalette();
    QPen pen;
    pal.setColor(QPalette::Window, Qt::black);

//    int i;
//    double y;
//    double dF = samp_freq / points;
//    double T = 1/ samp_freq;
//    double freq = 32000;

    QVector<double> in;
    QVector<cpx> out;
    QVector<double> dB;
    QVector<double> time;
    QVector<double> ff;

    generate_singlePulse(in, ff, points,samp_freq);
    out = FFT_vec(in);

    //dB

    for(int i = ((points/2)-1); i>= 0; i--){
       //dB.append(20* log(sqrt(out[i].real()*out[i].real() + out[i].imag()*out[i].imag())));
        dB.append(sqrt(out[i].real()*out[i].real() + out[i].imag()*out[i].imag()));
    }
    for(int i = 0; i<= ((points/2)-1); i++){
       //dB.append(20* log(sqrt(out[i].real()*out[i].real() + out[i].imag()*out[i].imag())));
        dB.append(sqrt(out[i].real()*out[i].real() + out[i].imag()*out[i].imag()));
    }
    //for(int i = 0; i<dB.size(); i++) std::cout << "x축 : " << ff[i] << "y축 : " << dB[i] << std::endl;


    plot->setAutoFillBackground(true);
    plot->setPalette(pal);
    plot->xAxis->setLabelColor(QColor(0,255,0,255));
    plot->yAxis->setLabelColor(QColor(0,255,0,255));
    plot->xAxis->setTickLabelColor(QColor(0,255,0,255));
    plot->yAxis->setTickLabelColor(QColor(0,255,0,255));
    plot->addGraph();
    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");
    plot->xAxis->setRange(-50000 ,50000);
    plot->yAxis->setRange(-2000, 0);
    plot->setBackground(QColor(0,0,0,0));
    pen.setColor(QColor(0,255,0,255));
    plot->graph(0)->setPen(pen);
    plot->graph(0)->setData(ff, dB);


    plot->replot();
}















// 도형 그리기 함수
int MainWindow::draw(QPainter *painter, const QPoint &pos)
{
    // 안티얼라이징 설정
    painter->setRenderHint(QPainter::Antialiasing, true);
    // 펜 전경색, 펜 두께, 라인스타일 설정
    if(lineStyle==0)  // 실선이면
        painter->setPen(QPen(fgColor, sliderValue, Qt::SolidLine));
    else if(lineStyle==1)  // 점선이면
        painter->setPen(QPen(fgColor, sliderValue, Qt::DotLine));
    else if(lineStyle==2)  // 대쉬면
        painter->setPen(QPen(fgColor, sliderValue, Qt::DashLine));
    else if(lineStyle==3) // 대쉬 점선이면
        painter->setPen(QPen(fgColor, sliderValue, Qt::DashDotLine));
    else if(lineStyle==4) // 대쉬 점선 점선이면
        painter->setPen(QPen(fgColor, sliderValue, Qt::DashDotDotLine));
    // 브러쉬 색상 설정
    painter->setBrush(QBrush(bgColor, Qt::SolidPattern));
    if(drawStyle==0)
    {
        // 직선을 그린다.
        int w=200;
        painter->drawLine(x, y, x+w, y);
    }
    else if(drawStyle==1)
    {
        // 사각형을 그린다
        int w=100; int h=100;
        painter->drawRect(x, y, w, h); //80
    }
    else if(drawStyle==2)
    {
        // 원을 그린다
        int w=100; int h=100;
        painter->drawEllipse(x, y, w, h);
    }
    else if(drawStyle==3)
    {
        // 파이를 그린다.
        painter->drawPie(x, y, 100, 40, 60 * 16, 250 * 16);
        //painter.drawPie(80, 80, 400, 240, 60 * 16, 270 * 16);
    }
    else if(drawStyle==4)
    {
       // 베지어 곡선을 그린다
       int w=200; int h=80;
       QPainterPath path;
       path.moveTo(x, y);
       path.cubicTo(x+w, y+h, y+(h-30), y+h, x+(w+100), y+(h-30)); // path.cubicTo(200, 80, 320, 80, 480, 320);
       painter->drawPath(path);
    }
    else if(drawStyle==5)
    {
        float w=100.0; // w=mov_x, h=mov_y (mfc에서)
        float h=100.0;
        // 삼각형을 그린다 , 삼각형의 하단 왼쪽 좌표
        pts[0]=QPointF(x, y); // pts[0].x=x; pts[0].y=y;(mfc에서)
        // 삼각형의 하단 오른쪽 좌표
        pts[1]=QPointF(x+w, y); // pts[1].x=x+mov_x; pts[1].y=y; (mfc에서)
        // 삼각형의 상단 중앙(중심) 좌표
        pts[2]=QPointF((2*x+w)/2, y-h); // pts[2]=(2*x+mov_x)/2; pts[2]=y-mov_y;  (mfc에서)
        painter->drawPolygon(pts, 3);      //dc.Polygon(pts, 3);
     }
     else if(drawStyle==6)
     {
         // 다각형을 그린다
         float w=100.0;float h=100.0;
         // 다각형의 첫 번째 기준 좌표
         polygon[0]=QPointF(x, y);
         // 다각형의 두 번째 좌표
         polygon[1]=QPointF((2*x+w)/2-40.0, y-(h-20.0));
         // 다각형의 세 번째 좌표
         polygon[2]=QPointF((2*x+w)/2+40.0, y-(h-50.0));
         // 다각형의 네 번째(마지막) 좌표
         polygon[3]=QPointF(x+w, y-(h-90.0));
         painter->drawPolygon(polygon, 4);
      }
      else if(drawStyle==7)
      {
          // 화살표를 그린다.
      }
      else if(drawStyle==8)
      {
          editStr=qtextEdit->toPlainText();
          // 폰트 대화상자에서 선택된 글꼴체와 크기로 설정
          painter->setFont(font);
          // 텍스트를 화면에 출력한다
          // 텍스트를 화면에 출력한다
          painter->drawText(x, y, editStr);
      }
      update(); // 화면을 다시 그린다
      return x, y, mov_x, mov_y;
}

// 도형 콤보 상자 항목 선택시 처리 함수
void MainWindow::Main_comboBox(int index)
{

    if(index==0){ // 직선 선택시
        ui->statusbar->showMessage("Selected Shape: Line");
        drawStyle=0;
     }
     else if(index==1){ // 사각형 선택시
           ui->statusbar->showMessage("Selected Shape: Rectangle");
           drawStyle=1;
     }
     else if(index==2){ // 원 선택시
           ui->statusbar->showMessage("Selected Shape: Circle");
           drawStyle=2;
     }
     else if(index==3){ // 부채꼴 선택시
           ui->statusbar->showMessage("Selected Shape: Arc");
           drawStyle=3;
     }
     else if(index==4){ // 베지어 곡선 선택시
           ui->statusbar->showMessage("Selected Shape: Bezier Curves");
           drawStyle=4;
     }
     else if(index==5){ // 삼각형 선택시
           ui->statusbar->showMessage("Selected Shape: Triangle");
           drawStyle=5;
     }
     else if(index==6){ // 다각형 선택시
           ui->statusbar->showMessage("Selected Shape: Polygon");
           drawStyle=6;
     }
     else if(index==7){ // 화살표 선택시
           ui->statusbar->showMessage("Selected Shape: Arrow");
           drawStyle=7;
     }    // 칼라 대화상자에서 얻은 색상 정보를 fgColor에 대입
    fgColor = QColorDialog::getColor();
}

// "도형->배경색" 메뉴 선택 처리 함수
void MainWindow::Main_bgColor()
{
    // 칼라 대화상자에서 얻은 색상 정보를 bgColor에 대입
    bgColor = QColorDialog::getColor();
    if (bgColor.isValid())
    {
        str="Color Name:"+bgColor.name();
        // 상태바의 첫 번째 패널에 칼명 표시
        ui->statusbar->showMessage(str);
    }
}
// "도형->그리기 텍스트" 메뉴 선택시 처리 함수
 void MainWindow::Main_DrawText()
 {
     drawStyle=8;
     bool ok;
     font = QFontDialog::getFont(&ok, QFont("Helvetica [Cronyx]", 10), this);// Helvetica [Cronyx]
     if(ok) // ok 버튼을 누르면
     {
         // 폰트 대화상자에서 선택된 글꼴과 크기가 에디트 상자 입력된 문자열에 반영된다.
         qtextEdit->setFont(font);
     }
     else // cancle 버튼을 누르면 글꼴과 크기는 기본 크기인 10으로 설정
     {
         font.setPointSize(10);
     }
 }


// 라인 스타일 콤보 상자 항목 선택시 처리 함수
void MainWindow::Main_comboBox2(int index)
{
    if(index==0){ // 실선 선택시
        ui->statusbar->showMessage("Selected SolidLine");
        lineStyle=0;
     }
     else if(index==1){ // 점선택시
           ui->statusbar->showMessage("Selected DotLine");
           lineStyle=1;
     }
     else if(index==2){ // 대쉬 선택시
           ui->statusbar->showMessage("Selected DashLine");
           lineStyle=2;
     }
     else if(index==3){ // 대쉬 점선 선택시
           ui->statusbar->showMessage("Selected DashDotLine");
           lineStyle=3;
     }
     else if(index==4){ // 대쉬 점선 점선 선택시
           ui->statusbar->showMessage("Selected DashDotDotLine");
           lineStyle=4;
     }
}
 // 마우스 드래그시 이벤트 함수
 void MainWindow::mouseMoveEvent(QMouseEvent *event)
 {
     if(drawID==true)
        {
            QPainter painter(this);
            draw(&painter, event->pos());
        }
 }
 // 마우스 누름 이벤트 함수
 void MainWindow::mousePressEvent(QMouseEvent *event)
 {
     if(event->button()==Qt::LeftButton) // 마우스 왼족 버튼 클릭시
     {
         // 마우스의 클릭 좌표값을 인수로 onMouseEvent 함수 호출
          onMouseEvent(event->pos());
          drawID=true;
     }
 }
 // 마우스 업 이벤트 함수
 void MainWindow::mouseReleaseEvent(QMouseEvent *event)
 {
     drawID=false;
     event->accept();
 }
 // 페인트 이벤트 함수
 void MainWindow::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);
     draw(&painter, pos());
 }
 // 마우스 처리 함수
 int MainWindow::onMouseEvent(const QPoint &pos)
 {
     str="x: "+QString("%1").arg(pos.x())+",  ";
     str2="y: "+QString("%2").arg(pos.y());
     x=pos.x(); y=pos.y();
     mov_x=x; mov_y=y;
     // 상태바 첫 번째 패널에 마우스 좌표 값 표시
     ui->statusbar->showMessage(str+str2);
     return x, y, mov_x, mov_y;
 }


MainWindow::~MainWindow()
{
    delete ui;
}
