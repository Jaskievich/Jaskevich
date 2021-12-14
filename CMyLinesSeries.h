#ifndef CMYLINESSERIES_H
#define CMYLINESSERIES_H

#include <QChartView>
#include <QLineSeries>

QT_CHARTS_BEGIN_NAMESPACE

class CMyLineSeries: public QLineSeries
{
     Q_OBJECT
public:
    void append(qreal x, qreal y, unsigned int quality){
        QLineSeries::append(x, y);
        qualitys.push_back(quality);
    }

    bool findPointByX(qreal x, QPointF &point, unsigned int &quality)
    {
        int i = 0, n = count();
        for( ; i < n - 1; ++i  )
            if( at(i).x() >= x && x < at(i + 1).x() ) break;
        if( i < n ){
            point.setX(at(i).x());
            point.setY(at(i).y());
            quality = i < qualitys.size()? qualitys[i] : 0 ;
            return true;
        }
        return false;
    }

private:
    QVector<unsigned int> qualitys;
};

QT_CHARTS_END_NAMESPACE

#endif // CMYLINESSERIES_H
