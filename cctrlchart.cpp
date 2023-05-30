#include "cctrlchart.h"
#include "Utility.h"
#include <QTime>
#include <QTimeZone>
#include <QScrollBar>
#include <QLocale>

ChartView *CCtrlChart::getChartView() const
{
    return chartView;
}

void CCtrlChart::CreateAxisX()
{
    axisX = new QDateTimeAxis;
    axisX->setTickCount(24);
    axisX->setFormat("h:mm:ss");
   // axisX->setFormat("dd-MM-yyyy h:mm:ss");
    axisX->setTitleText("Время");
//    QFont fnt("Arial", 15, QFont::Bold);
//    axisX->setTitleFont(fnt);
}

CCtrlChart::CCtrlChart(QLayout *layout):
    /*series(NULL),*/ chart(nullptr), chartView(nullptr)
{
    chart = new QChart();
    chart->setTheme(QChart::ChartThemeDark);
    chart->setAnimationOptions(QChart::AllAnimations);
    chartView = new ChartView(chart, layout->parentWidget());
 //   chartView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    if(layout) layout->addWidget(chartView); 
    CreateAxisX();
}

CCtrlChart::~CCtrlChart()
{
    if( chartView ) delete chartView;
}

//CCtrlChart *CCtrlChart::GetInst(QLayout *layout)
//{
//    if(!instance) instance = new CCtrlChart(layout);
//    return instance;
//}

void CCtrlChart::Render()
{
    chartView->setRenderHint(QPainter::Antialiasing);  
}

void CCtrlChart::PrintTrends(QPainter &painter)
{
     chartView->setRenderHint(QPainter::Antialiasing);
     chart->setTheme(QChart::ChartThemeLight);
     chartView->render(&painter);
     chart->setTheme(QChart::ChartThemeDark);
}

void CCtrlChart::removeAllAxis()
{
    QList<QAbstractAxis*> listAxis = chart->axes();
    QList<QAbstractAxis*>::iterator it;
    for(it = listAxis.begin(); it != listAxis.end(); it++)  chart->removeAxis(*it);
}

QDateTimeAxis *CCtrlChart::GetAxisX()
{
    return axisX;
}

void CCtrlChart::ClearAllSeries()
{
      chartView->RestorePar_Scroll();
      chart->removeAllSeries();    
      removeAllAxis();
      listAxisY.clear();      
}



void CCtrlChart::SetSeries(vector<T_ItemVal> &arr, T_Info_Series &info)
{
    CMyLineSeries *series = new CMyLineSeries();
    vector<T_ItemVal>::iterator it;
    int i = 0;
    for(it = arr.begin(); it != arr.end(); ++it, ++i){
        SYSTEMTIME st = _GetLocalTime(GetFileTime(it->time));
        QDateTime tm(QDate(st.wYear, st.wMonth, st.wDay), QTime(st.wHour, st.wMinute, st.wSecond));
        series->append(tm.toMSecsSinceEpoch(), it->val, it->ev);
    }
    chart->addSeries(series);

    chart->legend()->hide();
    chart->addAxis(axisX, Qt::AlignBottom);

    CMyValueAxis *axisY = new CMyValueAxis;
    chart->addAxis(axisY, Qt::AlignLeft);

    axisY->setLinePenColor(series->pen().color());
    info.color = series->pen().color();
    series->attachAxis(axisX);
    axisY->setLabelFormat("%.2f");
    axisY->setTitleText(info.name + " " + info.eu);
    series->attachAxis(axisY);
    connect(series,  &CMyLineSeries::hovered, chartView, &ChartView::tooltip);
}



void CCtrlChart::SetTitleChart(const char *title)
{
//    QFont fnt("Arial", 15, QFont::Bold);
//    chart->setTitleFont(fnt);
    chart->setTitle(title);
}

void CCtrlChart::SetTotalAxisY()
{
    listAxisY.clear();
    QList<QAbstractAxis*> listAxis = chart->axes(Qt::Vertical);
    QList<QAbstractAxis*>::iterator it;
    for(it = listAxis.begin(); it != listAxis.end(); it++){
        CMyValueAxis *axisY = new CMyValueAxis;
        CMyValueAxis *axisY_old = (CMyValueAxis *)(*it);
        *axisY = *axisY_old;
        listAxisY.push_back(axisY);
    }
    chart->createDefaultAxes();
    QAbstractAxis *axisX_old = chart->axisX();
    chart->removeAxis(axisX_old);

    CreateAxisX();
    chart->addAxis(axisX, Qt::AlignBottom);

    QList<QAbstractSeries *> list = chart->series();
    QList<QAbstractSeries *>::iterator jt;
    for(jt = list.begin(); jt != list.end(); ++jt ){
        (*jt)->attachAxis(axisX);
    }

}

void CCtrlChart::SetMultiAxisY()
{
    // Удалить старые оси
    removeAllAxis();
    axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("h:mm:ss");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    QList<QAbstractSeries *> list = chart->series();
    for(int i = 0; i < list.size(); ++i ){
        QLineSeries *series = ( QLineSeries *)list.at(i);
        CMyValueAxis *axisY = listAxisY.at(i);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
        series->attachAxis(axisX);
    }

}

void CCtrlChart::SetHour(T_HOUR hour)
{
    switch (hour) {
        case TWO_HOUR:
            chartView->SetHour(24/2);
            break;
        case EIGHT_HOUR:
            chartView->SetHour(24/8);
            break;
        case TWENTY_MINUTES:
            chartView->SetHour(24*5.0);
            break;
        case ALL_HOUR:
            chartView->RestorePar_Scroll();
         //   chart->zoomReset();
            break;
    }

}

void CCtrlChart::SetMultiAxisY(bool isMulti)
{
    if( chart->isZoomed() )  chartView->RestorePar_Scroll();
    if( isMulti ) SetTotalAxisY(); else SetMultiAxisY();
}



//CCtrlChart *CCtrlChart::instance = nullptr;
