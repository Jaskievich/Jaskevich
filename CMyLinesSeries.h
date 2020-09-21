#ifndef CMYLINESSERIES_H
#define CMYLINESSERIES_H

#include <QChartView>
#include <QLineSeries>

QT_CHARTS_BEGIN_NAMESPACE

class CMyLineSeries: public QLineSeries
{
     Q_OBJECT
public:
    void append(qreal x, qreal y, unsigned short quality){
        QLineSeries::append(x, y);
        qualitys.push_back(quality);
    }

    bool findPointByX(qreal x, QPointF &point, unsigned short &quality)
    {
        int i = 0;
        for( ; i < count() - 1; ++i  )
            if( at(i).x() >= x && x < at(i + 1).x() ) break;
        if( i < count() ){
            point.setX(at(i).x());
            point.setY(at(i).y());
            quality = i < qualitys.size()? qualitys[i] : 0 ;
            return true;
        }
        return false;
    }

private:
    QVector<unsigned short> qualitys;
};

QT_CHARTS_END_NAMESPACE

#endif // CMYLINESSERIES_H
