#ifndef CCTRLCHART_H
#define CCTRLCHART_H

#include <QLayout>
#include <QChartView>
//#include <QLineSeries>
#include "CMyLinesSeries.h"
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QObject>
#include "chart.h"
#include "chartview.h"
#include "cmyltarchive.h"
#include "MyValueAxis.h"

struct T_Info_Series
{
    QString name, eu;
    QColor color;

    T_Info_Series(QString _name, QString _eu):name(_name), eu(_eu)
    {
    }
};

class CCtrlChart: public QObject
{
      Q_OBJECT
private:

    Chart               *chart ;
    ChartView           *chartView;
    QDateTimeAxis       *axisX;
  //  QValueAxis      *axisY ;
    QList<CMyValueAxis *> listAxisY;
//    static CCtrlChart *instance;

public:

    enum T_HOUR{
        TWO_HOUR,
        EIGHT_HOUR,
        TWENTY_MINUTES,
        ALL_HOUR
    };

public:
    CCtrlChart(QLayout *layout);
    ~CCtrlChart();
 //   static CCtrlChart *GetInst(QLayout *layout);
    void Render();
    void ClearAllSeries();
    void SetSeries(deque<VQT> &arr, T_Info_Series &info);
    void SetTitleChart(const char *title);
    void SetTotalAxisY();
    void SetMultiAxisY();
    void SetHour(T_HOUR hour);
    void SetScrollBar();

    void SetMultiAxisY(bool isMulti);
    void PrintTrends(QPainter &painter);


    ChartView *getChartView() const;
    void removeAllAxis();
protected:
    void CreateAxisX();
};


#endif // CCTRLCHART_H
