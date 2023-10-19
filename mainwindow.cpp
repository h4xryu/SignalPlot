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
#include <fftw3.h>
#include <fstream>

using namespace std;
class Error;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plot = ui->widget;

    //set_reset();

    // 편집상자, 슬라이드바, 콤보상자를 생성하는 함수 호출
       controlType();
       // 메뉴와 툴바 생성 함수 선언
       menuToolbarCreate();
       //Plot_graph();


       try{
           QVector<double> ve;
           for(int i = 0; i < 100; i++) ve.append(0);
           for(int i = 0; i < 30; i++) ve.append(1);
           for(int i = 0; i < 100; i++) ve.append(0);


           QVector<cpx> v(ve.begin(), ve.end());
           QVector<cpx> tmp;
           QVector<double> tmp_x;
           QVector<double> tmp_y;

           tmp = FFT_vec(ve);

           for(int i = 0; i < 230; i++) {
               cout << i << ": " << tmp[i] << endl;
               tmp_y.append(tmp[i].real());
           }

           for(int i = 0; i < 230; i++) {
               cout << i << ": " << tmp_y[i] << endl;

           }

           plot->addGraph();

           plot->xAxis->setRange(-10,10);
           plot->yAxis->setRange(-10, 10);
           plot->graph(0)->setData(ve,tmp_y);
           plot->replot();
       } catch (std::out_of_range& e) {
           std::cout << "예외 발생 ! " << e.what() << std::endl;
         }


}

// 그래프 그리는 함수
void MainWindow::Plot_graph()
{
    // x축 y축 벡터 정의

    QQueue<QVector<double>> data;
    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");
    plot->xAxis->setRange(-10 - scale ,10 + scale);
    plot->yAxis->setRange(-5 - scale, 5 + scale);


    while (!data.isEmpty()){
        cout << ">?";
        data.append(data.dequeue());
    }
    plot->addGraph();

    //일반 신호일 경우 <- 나중에 switch case 문으로 connect 와 연결해서 번호 부여예정
    generate_signal();


    //wav 파일 받아서 그리기


    plot->graph(0)->setData(vec_x, vec_y);
    plot->replot();

}

void MainWindow::generate_signal(){
    //페이즈는 실시간 이동
    double neg_w = phase;
    double w = phase;

    for(int i = 0; i < 100000 + (int)phase; i = i + 1){
        vec_y.append(sin(neg_w - phase));
        vec_x.append(neg_w);
        neg_w = neg_w - 0.005729;
    }
    for(int i = 0; i < 100000 + (int)phase; i = i + 1){
        vec_y.append(sin(w - phase));
        vec_x.append(w);
        w = w+ 0.005729;
    }

    //푸리에 변환 실험
    /**

    QVector<double> vec_exp;

    for (int i = 0; i < 100000 + (int)phase; i = i +1){
        vec_exp.append(sin(w)cos(w))
        w = w + 0.005729;
    }
    **/


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
    n *= 2;

    v.resize(n);

    QVector<cpx> v_(n);
    for(int i=0; i<n; i++) {
        v_[i] = cpx(v[i], 0);
    }
    cpx unit(cos(2*PI/n), sin(2*PI/n));

    FFT(v_, unit);

    QVector<cpx> w_(n);
    for(int i=0; i<n; i++) w_[i] = v_[i];

    FFT(w_, cpx(1, 0)/unit);
    for(int i=0; i<n; i++) w_[i] /= cpx(n, 0);



    return w_;
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
    bgColor.setRgb(255, 255, 255);
}

// 슬라이드바 처리 함수
void MainWindow::Main_Slider(int value)
{
    str="value:"+QString("%1").arg(value);
    ui->statusbar->showMessage(str);
    // 슬라이드바의 값을 sliderValue에 대입
    sliderValue=value;
    // 위상 조정
    scale = (double)sliderValue;

    set_reset();
    plot->replot();
    Plot_graph();
}


void MainWindow::set_reset(){
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
        // 색상 객체 생성
        QAction *pSlotfgColor = new QAction(fgcolorIcon, tr("&Graph start"), this);
        pSlotfgColor->setShortcut(tr("Alt+C"));
        pSlotfgColor->setStatusTip(tr("그래프를 시작합니다."));
        //  연결 함수 -> 그래프 그리기 시작
        connect(pSlotfgColor, SIGNAL(triggered()), this, SLOT(Plot_graph()));
        // 메뉴바에 "그래프 시작 추가
        pGraphMenu->addAction(pSlotfgColor);
        // 툴바에 색상 아이콘 추가
        GraphToolBar->addAction(pSlotfgColor);

        // 상태바을 연결시킨다.
        pStartusbar = statusBar();
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
