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

/*
 * Структура содержащая данные о серии
 */
struct T_Info_Series
{
    QString name, eu;

    QColor color;

    T_Info_Series(QString _name, QString _eu):name(_name), eu(_eu)
    {
    }
};

/*
 * Класс служит для управления трендами
 */
class CCtrlChart: public QObject
{
      Q_OBJECT
private:

    //Chart               *chart ;
    QChart               *chart ;

    ChartView           *chartView;

    QDateTimeAxis       *axisX;

    QList<CMyValueAxis *> listAxisY;

public:

    enum T_HOUR{
        TWO_HOUR,
        EIGHT_HOUR,
        TWENTY_MINUTES,
        ALL_HOUR
    };

public:

    CCtrlChart(QLayout *layout);

    ~CCtrlChart() override;

    void Render();

    void ClearAllSeries();

  //  void SetSeries(deque<VQT> &arr, T_Info_Series &info);
    void SetSeries(vector<T_ItemVal> &arr, T_Info_Series &info);

    void SetTitleChart(const char *title);

    void SetTotalAxisY();   // Установить общую ось Y

    void SetMultiAxisY();    // Установить разные оси Y

    void SetHour(T_HOUR hour);

    void SetScrollBar();

    void SetMultiAxisY(bool isMulti);

    void PrintTrends(QPainter &painter);

    ChartView *getChartView() const;

    void removeAllAxis();

    QDateTimeAxis *GetAxisX();

protected:

    void CreateAxisX();
};


#endif // CCTRLCHART_H
