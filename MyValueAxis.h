#ifndef MYVALUEAXIS_H
#define MYVALUEAXIS_H

#include <qvalueaxis.h>

QT_CHARTS_BEGIN_NAMESPACE

class CMyValueAxis: public QValueAxis
{
    Q_OBJECT
public:
    CMyValueAxis(QObject *parent = nullptr):QValueAxis(parent){   }
    ~CMyValueAxis(){}

    CMyValueAxis& operator=(const CMyValueAxis& right)
    {
           //проверка на самоприсваивание
           if (this == &right) {
               return *this;
           }
           setTickType(right.tickType());
           setTickCount(right.tickCount());
           setTitleText(right.titleText());
           setMax(right.max());
           setMin(right.min());
           setLabelFormat(right.labelFormat());
           setLinePen(right.linePen());
           setLinePenColor(right.linePenColor());
           return *this;
       }
};

QT_CHARTS_END_NAMESPACE

#endif // MYVALUEAXIS_H
