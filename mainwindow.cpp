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
#include <QTimer>
#include <QThread>
#include <micThread.h>



using namespace std;

class Error;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_serialPort(new QSerialPort(this))
    , mic(new MicThread)
{

    timer = new QTimer(this);
    ui->setupUi(this);

    mic->start();
    device = (char*)malloc(sizeof(char)*20);
    ui->btnDisConnect->setEnabled(false);

    QObject::connect(this, &MainWindow::btn_triggered, mic, &MicThread::chk_trig);
    QObject::connect(this, &MainWindow::send_wait, mic, &MicThread::wait_on);
    QObject::connect(this, &MainWindow::send_continue, mic, &MicThread::wait_off);
    QObject::connect(this, &MainWindow::set_sendFlag, mic, &MicThread::chk_sendFlag);
    QObject::connect(this, &MainWindow::is_sendfin, mic, &MicThread::fin_send);
    QObject::connect(mic, &MicThread::send_in, this, &MainWindow::Plot_FFT);
    QObject::connect(mic, &MicThread::getDeviceInfo, this, &MainWindow::sendDeviceInfo);
    QObject::connect(this, &MainWindow::sendDinfoReady, mic, &MicThread::chk_gotdev);



    fillPortsInfo();
    connect(ui->btnConnect, SIGNAL(clicked), SLOT(on_btnConnect_clicked));
    connect(m_serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);
    //connect(m_serialPort, &QSerialPort::errorOccurred, this, &MainWindow::handleError);
    plot = ui->widget;//주파수영역 그래프
    plot_time = ui->widget_2; //시간영역 그래프

    plot_time->hide();
    plot->hide();

    // 편집상자, 슬라이드바, 콤보상자를 생성하는 함수 호출
    //controlType();
    // 메뉴와 툴바 생성 함수 선언
    menuToolbarCreate();
    graphStartBtn->setEnabled(false);


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

void MainWindow::update_graph(QVector<double>ff, QVector<double> dB){
    //최대 최소값 구하기

    double ff_max = *max_element(ff.begin(), ff.end());
    double ff_min = *min_element(ff.begin(), ff.end());

    double dB_max = *max_element(dB.begin(), dB.end());
    double dB_min = *min_element(dB.begin(), dB.end());
    plot->xAxis->setRange(ff_min - 100 + scale,ff_max + 100 - scale);
    plot->yAxis->setRange(-50 - scale, 1500 + scale);

    plot->graph(0)->setData(ff, dB);


    plot->replot();
}

void MainWindow::update_time_graph(QVector<double> in){
    //최대 최소값 구하기

    double amp_max = *max_element(in.begin(), in.end());
    double amp_min = *min_element(in.begin(), in.end());
    double avg = (amp_max + amp_min) /2;
    double time_max = in.size() * (1/samp_freq) * 150;

    QVector<double> time;
    plot_time->xAxis->setRange(0,time_max);
    plot_time->yAxis->setRange(-25, 25);

    if (time.size() == 0) {
        for(int i = 0; i < in.size(); i++) {
            time.append(i * (1/samp_freq) * 150);
        }
    }


    plot_time->graph(0)->setData(time, in);

    plot_time->replot();
}

void MainWindow::Plot_FFT(QVector<double> in){

    //그래프1 색
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
    plot->xAxis->setLabel("Frequency");
    plot->yAxis->setLabel("y");
    plot->graph(0)->setPen(pen);
    plot->setBackground(QColor(0,0,0,0));

    //그래프2 색
    pal.setColor(QPalette::Window, Qt::black);
    pen.setColor(QColor(0,255,0,255));

    plot_time->setAutoFillBackground(true);
    plot_time->setPalette(pal);

    plot_time->xAxis->setLabelColor(QColor(0,255,0,255));
    plot_time->yAxis->setLabelColor(QColor(0,255,0,255));
    plot_time->xAxis->setTickLabelColor(QColor(0,255,0,255));
    plot_time->yAxis->setTickLabelColor(QColor(0,255,0,255));
    plot_time->addGraph();
    plot_time->xAxis->setLabel("time (ms)");
    plot_time->yAxis->setLabel("Amplitude");
    plot_time->graph(0)->setPen(pen);
    plot_time->setBackground(QColor(0,0,0,0));


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

    // 데이터 받아와서 버퍼에 저장하는 개념

    //데이터를 보냈는지 확인
    emit set_sendFlag(1);
    while (!is_sendfin()){

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
    if(in.size() < tmp_ff.size()){
        // 음수 부분
        for (int i = in.size(); i < tmp_ff.size()/2 - (in.size()/2-1); i++){ //받은 버퍼절반의 크기 + 0의 개수 = tmp_ff의 절반 사이즈
            tmp_in.append(0);
        }
    }
        for (int i = 0; i < in.size()/2-1; i++){ //받은 버퍼의 크기만큼 채움
            tmp_in.append(in[in.size() - i -1]);
        }


        // 양수 부분
        for (int i = 0; i < in.size()/2-1; i++){ //받은 버퍼의 절반크기만큼 채움
            tmp_in.append(in[i]);
        }
     if(in.size() < tmp_ff.size()){
        for (int i = in.size(); i < tmp_ff.size()/2 - (in.size()/2-1); i++){
            tmp_in.append(0);
        }
     }


    tmp_out = FFT_vec(tmp_in);
//    for(int i = (points/2)-1; i< (points)-2; i++){
//        tmp_dB.append(20*log(sqrt(tmp_out[(points)-1-i].real()*tmp_out[(points)-1-i].real() + tmp_out[(points)-1-i].imag()*tmp_out[(points)-1-i].imag())));
//    }
//    for(int i = 0; i<= ((points/2)-1); i++){
//        tmp_dB.append(20*log(sqrt(tmp_out[i].real()*tmp_out[i].real() + tmp_out[i].imag()*tmp_out[i].imag())));
//    }

    for(int i = (points/2)-1; i< (points)-2; i++){
        tmp_dB.append((0.5)*sqrt(tmp_out[(points)-1-i].real()*tmp_out[(points)-1-i].real() + tmp_out[(points)-1-i].imag()*tmp_out[(points)-1-i].imag()*(1-cos(2*PI*((points)-1-i))/(points-1))));
    }
    for(int i = 0; i<= ((points/2)-1); i++){
        tmp_dB.append((0.5)*sqrt(tmp_out[i].real()*tmp_out[i].real() + tmp_out[i].imag()*tmp_out[i].imag()*(1-cos(2*PI*i/(points-1)))));
    }

    QVector<double> in_amp = in;
    //시간축도 그림
    update_time_graph(in_amp);


    emit send_continue();
    update_graph(tmp_ff, tmp_dB);
    tmp_dB.clear();



}



void MainWindow::Input_dialog(){
    bool ok = false;
    QVector<double> i;


    for(int x = 0; x < points * 2; x++) i.append(0);

    double d = QInputDialog::getDouble(this, tr("Generating signal"),
                                   tr("samp_rate:"), 4410, -128000, 128000, 1, &ok);
    //여기서 연결
    if (ok)
        samp_freq = d;
        emit btn_triggered(1);
        Plot_FFT(i);


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
        const QIcon graphIcon = QIcon::fromTheme("start", QIcon("D:/Work/QT Creator/Controls/color2.bmp"));
        // 시작 객체 생성
        graphStartBtn = new QAction(graphIcon, tr("&Graph start"), this);


        graphStartBtn->setShortcut(tr("Alt+C"));
        graphStartBtn->setStatusTip(tr("그래프를 시작합니다."));
        //  연결 함수 -> 그래프 그리기 시작



//        connect(pSlotfgColor, SIGNAL(triggered()), this, SLOT(Plot_sin_graph()));
        connect(graphStartBtn, SIGNAL(triggered()), this, SLOT(Input_dialog()));


        // 메뉴바에 "그래프 시작 추가
        pGraphMenu->addAction(graphStartBtn);
        // 툴바에 색상 아이콘 추가
        GraphToolBar->addAction(graphStartBtn);

        // 상태바을 연결시킨다.
        pStartusbar = statusBar();

}








void MainWindow::fillPortsInfo()
{
    ui->comboBox->clear();

    const auto infos = QSerialPortInfo::availablePorts(); // 시리얼 포트 정보 이용가능하도록
    for(const QSerialPortInfo &info : infos) {
        ui->comboBox->addItem(info.portName()); // 콤보박스에 시리얼포트 정보 받아온 이름을 붙여넣는다.
    }
}

void MainWindow::on_btnConnect_clicked()
{

    // 시리얼 설정 코드

    //m_serialPort->setPortName(ui->comboBox->currentText()); // 포트 이름 지정
    //m_serialPort->setBaudRate(QSerialPort::Baud115200); // baud: 초당 신호(siganl) 요소의 수 , 예) 하나의 버드에 2bit 있다면 1Baud 동안 2bit 전송 됨
    //m_serialPort->setDataBits(QSerialPort::Data8); // dataBits
    //m_serialPort->setParity(QSerialPort::NoParity); // 정보 전달 과정에 오류가 생겼는지 검사하기 위한 것
    //m_serialPort->setStopBits(QSerialPort::OneStop); // 포트를 열기전에 set 또는 success 하면 return true로 반환된다.
    //m_serialPort->setFlowControl(QSerialPort::NoFlowControl); // 흐름제어
    portName ="/dev/" + ui->comboBox->currentText();
    ba = portName.toLocal8Bit();
    device = ba.data();
    std::cout << "send test : " << device << std::endl;
    emit sendDinfoReady(1);
    mic->device = device;
    //readData();
    ui->btnConnect->setEnabled(false);
    ui->btnDisConnect->setEnabled(true);

    plot_time->show();
    plot->show();
    graphStartBtn->setEnabled(true);


}

void MainWindow::on_btnDisConnect_clicked()
{
    emit send_wait();
    fillPortsInfo();
    mic->exit();
    mic->start();
    if (m_serialPort->isOpen()) // 시리얼 포트 열렸으면
        m_serialPort->close(); // 시리얼 닫아라
    ui->btnConnect->setEnabled(true);
    ui->btnDisConnect->setEnabled(false);
    plot_time->hide();
    plot->hide();

}



int MainWindow::readData()
{




//    qDebug() << data.size() << "/" << (int)data[0] << "/" << (int)data[1];
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serialPort->errorString());
        on_btnDisConnect_clicked();
    }
}











// 도형 그리기 함수
int MainWindow::draw(QPainter *painter, const QPoint &pos)
{

}

// 도형 콤보 상자 항목 선택시 처리 함수
void MainWindow::Main_comboBox(int index)
{

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
 }


// 라인 스타일 콤보 상자 항목 선택시 처리 함수
void MainWindow::Main_comboBox2(int index)
{

}
 // 마우스 드래그시 이벤트 함수
 void MainWindow::mouseMoveEvent(QMouseEvent *event)
 {

 }
 // 마우스 누름 이벤트 함수
 void MainWindow::mousePressEvent(QMouseEvent *event)
 {

 }
 // 마우스 업 이벤트 함수
 void MainWindow::mouseReleaseEvent(QMouseEvent *event)
 {

 }
 // 페인트 이벤트 함수
 void MainWindow::paintEvent(QPaintEvent *event)
 {

 }
 // 마우스 처리 함수
 int MainWindow::onMouseEvent(const QPoint &pos)
 {

 }


MainWindow::~MainWindow()
{
    delete ui;
}
