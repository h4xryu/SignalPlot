 /* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
00002  * Qwt Widget Library
00003  * Copyright (C) 1997   Josef Wilgen
00004  * Copyright (C) 2002   Uwe Rathmann
00005  *
00006  * This library is free software; you can redistribute it and/or
00007  * modify it under the terms of the Qwt License, Version 1.0
00008  *****************************************************************************/
00009 
00010 #ifndef QWT_PLOT_H
00011 #define QWT_PLOT_H
00012 
00013 #include <qframe.h>
00014 #include "qwt_global.h"
00015 #include "qwt_array.h"
00016 #include "qwt_text.h"
00017 #include "qwt_plot_dict.h"
00018 #include "qwt_scale_map.h"
00019 #include "qwt_plot_printfilter.h"
00020 
00021 class QwtPlotLayout;
00022 class QwtLegend;
00023 class QwtScaleWidget;
00024 class QwtScaleEngine;
00025 class QwtScaleDiv;
00026 class QwtScaleDraw;
00027 class QwtTextLabel;
00028 class QwtPlotCanvas;
00029 class QwtPlotPrintFilter;
00030 
00077 class QWT_EXPORT QwtPlot: public QFrame, public QwtPlotDict
00078 {
00079     friend class QwtPlotCanvas;
00080 
00081     Q_OBJECT
00082     Q_PROPERTY( QString propertiesDocument 
00083         READ grabProperties WRITE applyProperties )
00084 
00085 public:
00087     enum Axis 
00088     { 
00089         yLeft, 
00090         yRight, 
00091         xBottom, 
00092         xTop, 
00093 
00094         axisCnt 
00095     };
00096 
00105     enum LegendPosition 
00106     {
00107         LeftLegend,
00108         RightLegend,
00109         BottomLegend,
00110         TopLegend,
00111         
00112         ExternalLegend
00113     };
00114 
00115     explicit QwtPlot(QWidget * = NULL);
00116     explicit QwtPlot(const QwtText &title, QWidget *p = NULL);
00117 #if QT_VERSION < 0x040000
00118     explicit QwtPlot(QWidget *, const char* name);
00119 #endif
00120 
00121     virtual ~QwtPlot();
00122 
00123     void applyProperties(const QString &);
00124     QString grabProperties() const;
00125 
00126     void setAutoReplot(bool tf = true);
00127     bool autoReplot() const;
00128 
00129     void print(QPaintDevice &p,
00130         const QwtPlotPrintFilter & = QwtPlotPrintFilter()) const;
00131     virtual void print(QPainter *, const QRect &rect,
00132         const QwtPlotPrintFilter & = QwtPlotPrintFilter()) const;
00133 
00134     // Layout
00135 
00136     QwtPlotLayout *plotLayout();
00137     const QwtPlotLayout *plotLayout() const;
00138 
00139     void setMargin(int margin);
00140     int margin() const;
00141 
00142     // Title
00143 
00144     void setTitle(const QString &);
00145     void setTitle(const QwtText &t);
00146     QwtText title() const;
00147 
00148     QwtTextLabel *titleLabel();
00149     const QwtTextLabel *titleLabel() const;
00150 
00151     // Canvas
00152 
00153     QwtPlotCanvas *canvas();
00154     const QwtPlotCanvas *canvas() const;
00155 
00156     void setCanvasBackground (const QColor &c);
00157     const QColor& canvasBackground() const;
00158 
00159     void setCanvasLineWidth(int w);
00160     int canvasLineWidth() const;
00161 
00162     virtual QwtScaleMap canvasMap(int axisId) const;
00163 
00164     double invTransform(int axisId, int pos) const;
00165     int transform(int axisId, double value) const;
00166 
00167     // Axes
00168 
00169     QwtScaleEngine *axisScaleEngine(int axisId);
00170     const QwtScaleEngine *axisScaleEngine(int axisId) const;
00171     void setAxisScaleEngine(int axisId, QwtScaleEngine *);
00172 
00173     void setAxisAutoScale(int axisId);
00174     bool axisAutoScale(int axisId) const;
00175 
00176     void enableAxis(int axisId, bool tf = true);
00177     bool axisEnabled(int axisId) const;
00178 
00179     void setAxisFont(int axisId, const QFont &f);
00180     QFont axisFont(int axisId) const;
00181 
00182     void setAxisScale(int axisId, double min, double max, double step = 0);
00183     void setAxisScaleDiv(int axisId, const QwtScaleDiv &);
00184     void setAxisScaleDraw(int axisId, QwtScaleDraw *);
00185 
00186     const QwtScaleDiv *axisScaleDiv(int axisId) const;
00187     QwtScaleDiv *axisScaleDiv(int axisId);
00188 
00189     const QwtScaleDraw *axisScaleDraw(int axisId) const;
00190     QwtScaleDraw *axisScaleDraw(int axisId);
00191 
00192     const QwtScaleWidget *axisWidget(int axisId) const;
00193     QwtScaleWidget *axisWidget(int axisId);
00194 
00195 #if QT_VERSION < 0x040000
00196     void setAxisLabelAlignment(int axisId, int);
00197 #else
00198     void setAxisLabelAlignment(int axisId, Qt::Alignment);
00199 #endif
00200     void setAxisLabelRotation(int axisId, double rotation);
00201 
00202     void setAxisTitle(int axisId, const QString &);
00203     void setAxisTitle(int axisId, const QwtText &);
00204     QwtText axisTitle(int axisId) const;
00205 
00206     void setAxisMaxMinor(int axisId, int maxMinor);
00207     int axisMaxMajor(int axisId) const;
00208     void setAxisMaxMajor(int axisId, int maxMajor);
00209     int axisMaxMinor(int axisId) const;
00210 
00211     // Legend 
00212 
00213     void insertLegend(QwtLegend *, LegendPosition = QwtPlot::RightLegend,
00214         double ratio = -1.0);
00215 
00216     QwtLegend *legend();
00217     const QwtLegend *legend() const;
00218 
00219     // Misc
00220 
00221     virtual void polish();
00222     virtual QSize sizeHint() const;
00223     virtual QSize minimumSizeHint() const;
00224 
00225     virtual void updateLayout();
00226 
00227     virtual bool event(QEvent *);
00228 
00229 signals:
00240     void legendClicked(QwtPlotItem *plotItem);
00241 
00254     void legendChecked(QwtPlotItem *plotItem, bool on);
00255 
00256 public slots:
00257     void clear();
00258 
00259     virtual void replot();
00260     void autoRefresh();
00261 
00262 protected slots:
00263     virtual void legendItemClicked();
00264     virtual void legendItemChecked(bool);
00265 
00266 protected:
00267     static bool axisValid(int axisId);
00268 
00269     virtual void drawCanvas(QPainter *);
00270     virtual void drawItems(QPainter *, const QRect &,
00271         const QwtScaleMap maps[axisCnt],
00272         const QwtPlotPrintFilter &) const;
00273 
00274     virtual void updateTabOrder();
00275 
00276     void updateAxes();
00277 
00278     virtual void resizeEvent(QResizeEvent *e);
00279 
00280     virtual void printLegendItem(QPainter *, 
00281         const QWidget *, const QRect &) const;
00282 
00283     virtual void printTitle(QPainter *, const QRect &) const;
00284     virtual void printScale(QPainter *, int axisId, int startDist, int endDist,
00285         int baseDist, const QRect &) const;
00286     virtual void printCanvas(QPainter *, const QRect &,
00287         const QwtScaleMap maps[axisCnt], const QwtPlotPrintFilter &) const;
00288     virtual void printLegend(QPainter *, const QRect &) const;
00289 
00290 private:
00291     void initAxesData();
00292     void deleteAxesData();
00293     void updateScaleDiv();
00294 
00295     void initPlot(const QwtText &title);
00296 
00297     class AxisData;
00298     AxisData *d_axisData[axisCnt];
00299 
00300     class PrivateData;
00301     PrivateData *d_data;
00302 };
00303 
00304 #endif
