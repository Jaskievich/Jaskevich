#ifndef CHARTVIEW_H
#define CHARTVIEW_H


#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QScrollBar>

QT_CHARTS_USE_NAMESPACE


struct T_ScrollCurrDat
{
    int old_value_slide;
    int d_val_begin;
    qreal k;

    QScrollBar *scrollBar;

    T_ScrollCurrDat(): old_value_slide(0), d_val_begin(0), k(1)
    {
        scrollBar = nullptr;
    }

    void Reset()
    {
        old_value_slide = 0;
        d_val_begin = 0;
        k = 1;
    }

    void ResizeScrollBar(qreal _k, qreal dx0, qreal size)
    {
        d_val_begin = static_cast<int>(d_val_begin * _k + dx0 * _k);
        k *= _k;
        int width_range = static_cast<int>(size*(k - 1));
        int right_rng = width_range - d_val_begin;
        int left_rng = -d_val_begin;
        scrollBar->setRange(left_rng,  right_rng);
        old_value_slide = 0;
    }

};


//![1]
class ChartView : public QChartView
//![1]
{
    Q_OBJECT
public:

    ChartView(QChart *chart, QWidget *parent = nullptr);
    ~ChartView();

    void RestorePar_Scroll();
    void SetHour(qreal denominator);

signals:
// Сигнал от точек на графике
 void sendPoint(const QPointF &point, unsigned short quality, int index);
 // Сигнал для кнопок
 void sendResetButton();

public slots:
    void tooltip(const QPointF &point, bool state);
    void moveHorScrollBar(int value);
    void moveVerScrollBar(int value);

//![2]
protected:
    bool viewportEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event) ;

//![2]

private:
    bool m_isTouching;
    QRect frame_mouse;
//    int old_value_slide;
//    int dx_val_begin;
//    qreal kx;

    T_ScrollCurrDat scrollCurrDat_x, scrollCurrDat_y;


    // линии курсора и констнтная линия при двойном щелчке мышью
    QGraphicsLineItem m_lineCursor_v, m_lineCursor_g, m_lineCursor_v_const;
    QGraphicsSimpleTextItem *m_tooltip;     // выводится текст принаведении на элемент графика
    int arr_delta[2];                       // массив для сохранения scroll 0 - Left/Right, 2 - Up/Down

  //  void resizeHorScrollBar(qreal _kx, qreal dx0);

};
#endif // CHARTVIEW_H
