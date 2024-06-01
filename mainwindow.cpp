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
    connect(ui->btnRefresh, SIGNAL(clicked), SLOT(on_btnRefresh_clicked));
    connect(m_serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);
    //connect(m_serialPort, &QSerialPort::errorOccurred, this, &MainWindow::handleError);
    plot = ui->widget;//주파수영역 그래프
    plot_time = ui->widget_2; //시간영역 그래프
    plot_ifft = ui->widget_3; //푸리에 역변환 그래프

    plot_time->hide();
    plot->hide();

    // 편집상자, 슬라이드바, 콤보상자를 생성하는 함수 호출
    //controlType();
    // 메뉴와 툴바 생성 함수 선언
    menuToolbarCreate();
    graphStartBtn->setEnabled(false);


    ui->none->setChecked(true);
    ui->statusbar->showMessage("connect");
    connect(plot_ifft, SIGNAL(mouseMove(QMouseEvent*)), SLOT(showPointToolTip(QMouseEvent*)));
}

void MainWindow::showPointToolTip(QMouseEvent *event)
{

    int x = plot_ifft->xAxis->pixelToCoord(event->pos().x());
    int y = plot_ifft->yAxis->pixelToCoord(event->pos().y());

    setToolTip(QString("%1 , %2").arg(x).arg(y));
    string x_str = to_string(x);
    ui->statusbar->showMessage((x_str+" "+to_string(y)).c_str());

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
    plot->yAxis->setRange(-100, 500);

    plot->graph(0)->setData(ff, dB);

    plot->replot();
}

void MainWindow::update_time_graph(QVector<double> in){
    //최대 최소값 구하기

    double amp_max = *max_element(in.begin(), in.end());
    double amp_min = *min_element(in.begin(), in.end());
    double avg = (amp_max + amp_min) /2;
    double time_max = in.size() * (1/samp_freq) * 500;

    QVector<double> time;
    plot_time->xAxis->setRange(0,time_max);
    plot_time->yAxis->setRange(-30, 30);

    if (time.size() == 0) {
        for(int i = 0; i < in.size(); i++) {
            time.append(i * (1/samp_freq) * 500);
        }
    }


    plot_time->graph(0)->setData(time, in);

    plot_time->replot();
}

void MainWindow::update_ifft_graph(QVector<cpx> in){


    QVector<double> cmp;
    QVector<double> cmp_ff;
    QVector<double> cmp_dB;

    /*여기서 함수적용*/

    if(hanning_active){
        double hann;
        for (int i = 0; i <100; i++){
            hann = 0.5*(1 - cos(2 * PI * i / (points - 1)));
            cmp.append(0*hann);
        }
        for (int i = 100; i <150; i++){ //150 - 100 => 값 50
            hann = 0.5*(1 - cos(2 * PI * i / (points - 1)));
            cmp.append(1*hann);
        }
        for (int i = 150; i <points; i++){
            hann = 0.5*(1 - cos(2 * PI * i / (points - 1)));
            cmp.append( 0*hann);
        }
    }
    else if(hamming_active){
        double hamming;
        for (int i = 0; i <100; i++){
            hamming =  0.54 - 0.46 * cos(2 * PI * i / (points - 1));
            cmp.append(0*hamming);
        }
        for (int i = 100; i <150; i++){ //150 - 100 => 값 50
            hamming =  0.54 - 0.46 * cos(2 * PI * i / (points - 1));
            cmp.append(1*hamming);
        }
        for (int i = 150; i <points; i++){
            hamming =  0.54 - 0.46 * cos(2 * PI * i / (points - 1));
            cmp.append( 0*hamming);
        }
    }
    else {
        for (int i = 0; i <100; i++){
        cmp.append(0);
        }
        for (int i = 100; i <150; i++){ //150 - 100 => 값 50
        cmp.append(1);
        }
        for (int i = 150; i <points; i++){
        cmp.append( 0);
        }
    }





    for(int i = -((points/2)-1); i<0; i++){
        cmp_ff.append((samp_freq * i) / points);
    }
    for(int i = 0; i<=((points/2)-1); i++){
        cmp_ff.append((samp_freq * i) / points);
    }

    QVector<cpx> cmp_out = FFT_vec(cmp);



    for(int i = ((points/2)-1); i> 0; i--){
    cmp_dB.append(20*log(sqrt(cmp_out[i].real()*cmp_out[i].real() + cmp_out[i].imag()*cmp_out[i].imag())));
    }
    for(int i = 0; i<= ((points/2)-1); i++){
    cmp_dB.append(20*log(sqrt(cmp_out[i].real()*cmp_out[i].real() + cmp_out[i].imag()*cmp_out[i].imag())));
    }




    double cmp_ff_max = *max_element(cmp_ff.begin(), cmp_ff.end());
    double cmp_ff_min = *min_element(cmp_ff.begin(), cmp_ff.end());

    double cmp_dB_max = *max_element(cmp_dB.begin(), cmp_dB.end());
    double cmp_dB_min = *min_element(cmp_dB.begin(), cmp_dB.end());
    plot_ifft->xAxis->setRange(cmp_ff_min - 100 + scale, cmp_ff_max + 100 - scale);
    plot_ifft->yAxis->setRange(-100, 100);




    plot_ifft->graph(0)->setData(cmp_ff, cmp_dB);

    plot_ifft->replot();


}

void MainWindow::Plot_FFT(QVector<double> in){

    //그래프1 색
    QPalette pal = QPalette();
    QPen pen;
    pal.setColor(QPalette::Window, Qt::white);
    pen.setColor(QColor(0,0,0,255));

    plot->setAutoFillBackground(true);
    plot->setPalette(pal);

    plot->xAxis->setLabelColor(QColor(0,0,0,255));
    plot->yAxis->setLabelColor(QColor(0,0,0,255));
    plot->xAxis->setTickLabelColor(QColor(0,0,0,255));
    plot->yAxis->setTickLabelColor(QColor(0,0,0,255));
    plot->addGraph();
    plot->xAxis->setLabel("Frequency");
    plot->yAxis->setLabel("Magnitude");
    plot->graph(0)->setPen(pen);
    plot->setBackground(QColor(0,0,0,0));

    //그래프2 색
//    pal.setColor(QPalette::Window, Qt::black);
//    pen.setColor(QColor(0,255,0,255));

    plot_time->setAutoFillBackground(true);
    plot_time->setPalette(pal);

    plot_time->xAxis->setLabelColor(QColor(0,0,0,255));
    plot_time->xAxis->setLabelColor(QColor(0,0,0,255));
    plot_time->xAxis->setTickLabelColor(QColor(0,0,0,255));
    plot_time->yAxis->setTickLabelColor(QColor(0,0,0,255));
    plot_time->addGraph();
    plot_time->xAxis->setLabel("time (msec)");
    plot_time->yAxis->setLabel("Amplitude");
    plot_time->graph(0)->setPen(pen);
    plot_time->setBackground(QColor(0,0,0,0));


    //그래프3 색
//    pal.setColor(QPalette::Window, Qt::black);
//    pen.setColor(QColor(0,255,0,255));

    plot_ifft->setAutoFillBackground(true);
    plot_ifft->setPalette(pal);

    plot_ifft->xAxis->setLabelColor(QColor(0,0,0,255));
    plot_ifft->yAxis->setLabelColor(QColor(0,0,0,255));
    plot_ifft->xAxis->setTickLabelColor(QColor(0,0,0,255));
    plot_ifft->yAxis->setTickLabelColor(QColor(0,0,0,255));
    plot_ifft->addGraph();
    plot_ifft->xAxis->setLabel("Frequency");
    plot_ifft->yAxis->setLabel("Magnitude");
    plot_ifft->graph(0)->setPen(pen);
    plot_ifft->setBackground(QColor(0,0,0,0));


    emit set_sendFlag(1);
    while (!is_sendfin()){

    }
    emit set_sendFlag(0);



    QVector<double> tmp_ff;
    QVector<double> tmp_in;
    QVector<double> tmp_dB;
    QVector<cpx> tmp_out;
    QVector<cpx> ifft_in;
    QVector<cpx> ifft_out;



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
    if(hanning_active){
        double hann;
        if(in.size() < tmp_ff.size()){
            // 음수 부분
            for (int i = in.size(); i < tmp_ff.size()/2 - (in.size()/2-1); i++){ //받은 버퍼절반의 크기 + 0의 개수 = tmp_ff의 절반 사이즈
                tmp_in.append(0);
            }
        }
            for (int i = 0; i < in.size()/2-1; i++){ //받은 버퍼의 크기만큼 채움
                hann = 0.5 * (1 - cos(2 * PI * i / (points - 1)));
                tmp_in.append(hann*in[in.size() - i -1]);
            }


            // 양수 부분
            for (int i = 0; i < in.size()/2-1; i++){ //받은 버퍼의 절반크기만큼 채움
                hann = 0.5 * (1 - cos(2 * PI * i / (points - 1)));
                tmp_in.append(hann*in[i]);
            }
         if(in.size() < tmp_ff.size()){
            for (int i = in.size(); i < tmp_ff.size()/2 - (in.size()/2-1); i++){
                tmp_in.append(0);
            }
         }


    }
    else if(hamming_active){
        double hamming;
        if(in.size() < tmp_ff.size()){
            // 음수 부분
            for (int i = in.size(); i < tmp_ff.size()/2 - (in.size()/2-1); i++){ //받은 버퍼절반의 크기 + 0의 개수 = tmp_ff의 절반 사이즈
                tmp_in.append(0);
            }
        }
            for (int i = 0; i < in.size()/2-1; i++){ //받은 버퍼의 크기만큼 채움
                hamming = 0.54 - 0.46 * cos(2 * PI * i / (points - 1));
                tmp_in.append(hamming*in[in.size() - i -1]);
            }


            // 양수 부분
            for (int i = 0; i < in.size()/2-1; i++){ //받은 버퍼의 절반크기만큼 채움
                hamming = 0.54 - 0.46 * cos(2 * PI * i / (points - 1));
                tmp_in.append(hamming*in[i]);
            }
         if(in.size() < tmp_ff.size()){
            for (int i = in.size(); i < tmp_ff.size()/2 - (in.size()/2-1); i++){
                tmp_in.append(0);
            }
         }

    }
    else {
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

    }






    tmp_out = FFT_vec(tmp_in);




    for(int i = (points/2)-1; i< (points)-2; i++){

            tmp_dB.append(20*log(sqrt(tmp_out[(points)-1-i].real()*tmp_out[(points)-1-i].real()
                                     +tmp_out[(points)-1-i].imag()*tmp_out[(points)-1-i].imag())));

    }
    for(int i = 0; i<= ((points/2)-1); i++){
            tmp_dB.append(20*log(sqrt(tmp_out[i].real()*tmp_out[i].real()
                                     +tmp_out[i].imag()*tmp_out[i].imag())));

    }

    QVector<double> in_amp = in;
    //시간축도 그림


//    for (const auto& value : tmp_dB) {
//        std::cout << value << ", ";
//    }
//    std::cout << std::endl;

    //ifft_out = IFFT_vec(tmp_out);
    emit send_continue();
    update_time_graph(in_amp);
    update_ifft_graph(ifft_out);
    update_graph(tmp_ff, tmp_dB);
    tmp_dB.clear();
    ifft_in.clear();


}

void MainWindow::on_apply_clicked()
{
    if(ui->hamming->isChecked()){
        hamming_active = true;
        hanning_active = false;
    }
    else if (ui->hanning->isChecked()){
        hamming_active = false;
        hanning_active = true;
    }
    else if (ui->none->isChecked()){
        hamming_active = false;
        hanning_active = false;
    }
}


void MainWindow::Input_dialog(){
    bool ok = false;
    QVector<double> i;


    for(int x = 0; x < points * 2; x++) i.append(0);

    double d = QInputDialog::getDouble(this, tr("Set sampling rate."),
                                   tr("samp_rate:"), 1000, -128000, 128000, 1, &ok);
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

void MainWindow::IFFT(QVector<cpx> &v, cpx w) {
    // FFT와 마찬가지로 재귀적으로 IFFT를 수행합니다.
    int n = v.size();
    if(n == 1) return;

    // even과 odd 벡터를 생성합니다.
    QVector<cpx> even(n/2), odd(n/2);
    for(int i=0; i<n/2; i++) {
        even[i] = v[2*i];
        odd[i] = v[2*i + 1];
    }

    // 재귀적으로 IFFT를 수행합니다.
    IFFT(even, w*w);
    IFFT(odd, w*w);

    // 병합 단계에서 계산을 수행합니다.
    cpx z(1, 0);
    for(int i=0; i<n/2; i++) {
        v[i] = even[i] + z*odd[i];
        v[i + n/2] = even[i] - z*odd[i];
        z *= w;
    }

    // 스케일링을 적용합니다.
    for(int i=0; i<n; i++) {
        v[i] /= cpx(n, 0);
    }
}

QVector<cpx> MainWindow::ifft(const QVector<cpx> &inputs){
        // How to calculate ifft by fft?
        // let x[n] denote time domain signal and X[k] denote frequency domain signal, then
        // x[n] = 1/N * sum(k=0..N-1) (X[k] * exp(1j*2*pi/N*k*n))

        // lets denote m = -k, then
        // x[n] = 1/N * sum(m=0..1-N) (X[m] * exp(-1j*2*pi/N*k*n)) == fft(X[m])

        // we know fft is circularly periodic, hence X[m] = X[-k] = X[N-k],
        // therefore we can flip the order of X[k] to get X[m]


        // flip the order of frequency spectrum
        QVector<cpx> reverse_freq_spectrum(inputs);
        std::reverse(std::next(reverse_freq_spectrum.begin()), reverse_freq_spectrum.end());

        // normalization by multiplying 1/N to each element
        const double len = reverse_freq_spectrum.size();
        std::transform(reverse_freq_spectrum.begin(), reverse_freq_spectrum.end(), reverse_freq_spectrum.begin(),
                       [len](const cpx &num) { return num / len; });
        // fft
        return IFFT_vec(reverse_freq_spectrum);
}

QVector<cpx> MainWindow::IFFT_vec(QVector<cpx> &v) {
    int n = v.size();
    // IFFT는 FFT와 마찬가지로 입력 벡터의 길이를 2의 제곱수로 만들어야 합니다.
    int padded_size = 1;
    while(padded_size < n) padded_size *= 2;

    // 입력 벡터를 패딩합니다.
    QVector<cpx> padded_v(padded_size);
    for(int i=0; i<n; i++) {
        padded_v[i] = v[i];
    }
    // 나머지 부분은 0으로 채웁니다.
    for(int i=n; i<padded_size; i++) {
        padded_v[i] = cpx(0, 0);
    }

    // FFT와 마찬가지로 적절한 단위원을 계산합니다.
    cpx unit(cos(2*PI/padded_size), sin(2*PI/padded_size));

    // IFFT를 수행합니다.
    IFFT(padded_v, conj(unit));

    return padded_v;
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

void MainWindow::on_btnRefresh_clicked(){
    ui->comboBox->clear();

    const auto infos = QSerialPortInfo::availablePorts(); // 시리얼 포트 정보 이용가능하도록
    for(const QSerialPortInfo &info : infos) {
        ui->comboBox->addItem(info.portName()); // 콤보박스에 시리얼포트 정보 받아온 이름을 붙여넣는다.
    }
}

void MainWindow::on_btnConnect_clicked(char newParameter)
{

    // 시리얼 설정 코드

    //m_serialPort->setPortName(ui->comboBox->currentText()); // 포트 이름 지정
    //m_serialPort->setBaudRate(QSerialPort::Baud115200); // baud: 초당 신호(siganl) 요소의 수 , 예) 하나의 버드에 2bit 있다면 1Baud 동안 2bit 전송 됨
    //m_serialPort->setDataBits(QSerialPort::Data8); // dataBits
    //m_serialPort->setParity(QSerialPort::NoParity); // 정보 전달 과정에 오류가 생겼는지 검사하기 위한 것
    //m_serialPort->setStopBits(QSerialPort::OneStop); // 포트를 열기전에 set 또는 success 하면 return true로 반환된다.
    //m_serialPort->setFlowControl(QSerialPort::NoFlowControl); // 흐름제어
    //QString password = QInputDialog::getText(, "Insert your password", QLineEdit::Password);
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


