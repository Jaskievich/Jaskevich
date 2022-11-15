
#include "chartview.h"
#include <QtGui/QMouseEvent>
#include <QDebug>
#include <QTime>
#include <QTimeZone>
#include <qlineseries.h>
#include <qmessagebox.h>

#include "CMyLinesSeries.h"

ChartView::ChartView(QChart *chart, QWidget *parent) :
    QChartView(chart, parent)
//    m_isTouching(false)

{
  //  setRubberBand(QChartView::RectangleRubberBand);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setRubberBand(QChartView::HorizontalRubberBand);
    m_tooltip = nullptr;
    m_lineCursor_v.setParentItem(chart);
    m_lineCursor_v.setPen(QPen(QColor(Qt::yellow),1, Qt::DashDotLine));
    m_lineCursor_v.setLine(0,-10000,0,10000);

    m_lineCursor_g.setParentItem(chart);
    m_lineCursor_g.setPen(QPen(QColor(Qt::yellow),1, Qt::DashDotLine));
    m_lineCursor_g.setLine(-10000,0,10000, 0);

    m_lineCursor_v_const.setParentItem(chart);
    m_lineCursor_v_const.setPen(QPen(QColor(Qt::yellow),1, Qt::DashDotLine));
    m_lineCursor_v_const.setLine(0,-10000,0,10000);
    arr_delta[0] = arr_delta[1] = 0;

    scrollCurrDat_x.scrollBar = QChartView::horizontalScrollBar();
    scrollCurrDat_y.scrollBar = QChartView::verticalScrollBar();


    QChartView::horizontalScrollBar()->disconnect();
    connect(QChartView::horizontalScrollBar(), &QScrollBar::sliderMoved, this, &ChartView::moveHorScrollBar);
    QChartView::verticalScrollBar()->disconnect();
    connect(QChartView::verticalScrollBar(), &QScrollBar::sliderMoved, this, &ChartView::moveVerScrollBar);

}

ChartView::~ChartView()
{

}

void ChartView::RestorePar_Scroll()
{
    chart()->scroll(arr_delta[0]  , arr_delta[1]);
    arr_delta[0] = arr_delta[1] = 0;
    m_lineCursor_v_const.hide();
    scrollCurrDat_x.Reset();
    scrollCurrDat_y.Reset();
    QChartView::horizontalScrollBar()->setRange(0,  0);
    QChartView::verticalScrollBar()->setRange(0,  0);
    chart()->zoomReset();
}



void ChartView::moveHorScrollBar(int value)
{
    int dx = value - scrollCurrDat_x.old_value_slide;
    scrollCurrDat_x.d_val_begin += dx;
    chart()->scroll(dx, 0);
    arr_delta[0] += dx;
    scrollCurrDat_x.old_value_slide = value;
}

void ChartView::moveVerScrollBar(int value)
{
    int dy =  scrollCurrDat_y.old_value_slide - value;
    scrollCurrDat_y.d_val_begin += dy;
    chart()->scroll(0, dy);
    arr_delta[1] += dy;
    scrollCurrDat_y.old_value_slide = value;
}


//bool ChartView::viewportEvent(QEvent *event)
//{
//    if (event->type() == QEvent::TouchBegin) {
//        // By default touch events are converted to mouse events. So
//        // after this event we will get a mouse event also but we want
//        // to handle touch events as gestures only. So we need this safeguard
//        // to block mouse events that are actually generated from touch.
//        m_isTouching = true;

//        // Turn off animations when handling gestures they
//        // will only slow us down.
//        chart()->setAnimationOptions(QChart::NoAnimation);
//    }
//    return QChartView::viewportEvent(event);
//}

void ChartView::mousePressEvent(QMouseEvent *event)
{
 //   if (m_isTouching)     return;
 //   qDebug() << "There are" << items(event->pos()).size()
 //                << "items at position" << mapToScene(event->pos());
    QChartView::mousePressEvent(event);    
    frame_mouse.setLeft(event->x());
    frame_mouse.setTop(event->y());
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
 //   if (m_isTouching)  return;
    m_lineCursor_v.setPos(event->x(), event->y());
    m_lineCursor_g.setPos(event->x(), event->y());
    QChartView::mouseMoveEvent(event);
}


void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
  //  if (m_isTouching)  m_isTouching = false;
    // Because we disabled animations when touch event was detected
    // we must put them back on.
 //   chart()->setAnimationOptions(QChart::SeriesAnimations);
    frame_mouse.setRight(event->x());
    frame_mouse.setBottom(event->y());
    QChartView::mouseReleaseEvent(event);
    if( frame_mouse.width() == 1 ) return;
    qreal _kx = chart()->plotArea().width()/(frame_mouse.width());
    qreal dx0 = frame_mouse.left() - chart()->plotArea().left();
   // resizeHorScrollBar(_kx, dx0);
    scrollCurrDat_x.ResizeScrollBar(_kx, dx0, chart()->plotArea().width());
    if( frame_mouse.height() == 1 ) return;
    qreal _ky = chart()->plotArea().height()/(frame_mouse.height());
    qreal dy0 = frame_mouse.top() - chart()->plotArea().top();
    scrollCurrDat_y.ResizeScrollBar(_ky, dy0, chart()->plotArea().height());
    emit sendResetButton();

}

void ChartView::wheelEvent(QWheelEvent *event)
{
    m_lineCursor_v_const.hide();
    qreal _kx = 1.0, _ky = 1.0;
    qreal width = chart()->plotArea().width();
    qreal height = chart()->plotArea().height();
    if( event->delta() > 0) {
        chart()->zoomIn();
         if( scrollCurrDat_x.k < 1 ) _kx = 0.5;
         scrollCurrDat_x.ResizeScrollBar(2.0, _kx*width/4.0, width);
         if( scrollCurrDat_y.k < 1 ) _ky = 0.5;
         scrollCurrDat_y.ResizeScrollBar(2.0, _ky*height/4.0, height);

    }else {
        if( scrollCurrDat_x.k > 1 ) _kx = 2.0;
        scrollCurrDat_x.ResizeScrollBar(0.5, -_kx*width/4.0, width);
        if( scrollCurrDat_y.k > 1 ) _ky = 2.0;
        scrollCurrDat_y.ResizeScrollBar(0.5, -_ky*height/4.0, height);
        chart()->zoomOut();
    }
    emit sendResetButton();
}

void ChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
//    if (m_isTouching)  return;
    m_lineCursor_v_const.setPos(event->x(), event->y());
    m_lineCursor_v_const.show();
    // 1. Определить значение функций на графиках по точке x
    QPointF point(event->x(), event->y());
    unsigned int quality = 0;
    QPointF p_screen;
    QList<QAbstractSeries *>  listSeries = chart()->series();
    for( int i = 0; i < listSeries.size(); ++i ){
        CMyLineSeries *series = static_cast<CMyLineSeries *>(listSeries[i]);
        p_screen = chart()->mapToValue(point, series);
        if( series->findPointByX(p_screen.x(), p_screen, quality))
            emit sendPoint(p_screen, quality, i);
    }
    QChartView::mouseDoubleClickEvent(event);
}

//![1]
void ChartView::keyPressEvent(QKeyEvent *event)
{
    m_lineCursor_v_const.hide();
    switch (event->key()) {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    case Qt::Key_Escape:
        RestorePar_Scroll();
        chart()->zoomReset();
        break;
//![1]
    case Qt::Key_Left:
    {
       //   chart()->scroll(-10, 0);
       //   arr_delta[0] += 10;
        int val = QChartView::horizontalScrollBar()->value();
        if( val > QChartView::horizontalScrollBar()->minimum()  ){
            QChartView::horizontalScrollBar()->setValue(val - 10);
            moveHorScrollBar(val - 10);
        }
        else {
            chart()->scroll(-10, 0);
            arr_delta[0] += +10;
        }
        break;
    }
    case Qt::Key_Right:
    {
       //   chart()->scroll(10, 0);
       //   arr_delta[0] += -10;
        int val = QChartView::horizontalScrollBar()->value();
        if( val < QChartView::horizontalScrollBar()->maximum()){
            QChartView::horizontalScrollBar()->setValue(val + 10);
            moveHorScrollBar(val + 10);
        }
        else{
            chart()->scroll(10, 0);
            arr_delta[0] += -10;
        }
        break;
    }
    case Qt::Key_Up:
        chart()->scroll(0, -10);
        arr_delta[1] += 10;
        break;
    case Qt::Key_Down:
        chart()->scroll(0, 10);
        arr_delta[1] += -10;
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
    emit sendResetButton();
}

void ChartView::tooltip(const QPointF &point, bool state)
{
     QAbstractSeries *series = qobject_cast<QAbstractSeries *>(sender());
    if (m_tooltip == nullptr){
        m_tooltip = new QGraphicsSimpleTextItem( chart());
        QPen pen(Qt::white, 1.0, Qt::SolidLine, Qt::SquareCap);
        m_tooltip->setPen(pen);
        m_tooltip->setBrush(QBrush(Qt::white));

    }
    if (state) {
        QDateTime tm;
        tm.setMSecsSinceEpoch(point.x());
        m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(tm.toString("dd.MM.yyyy hh:mm:ss")).arg(point.y()));
        m_tooltip->setZValue(11);
        m_tooltip->setPos( chart()->mapToPosition(point, series) + QPoint(10, -20));
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}

void ChartView::SetHour(qreal denominator)
{
    if( chart()->isZoomed() ){
        RestorePar_Scroll();
        chart()->zoomReset();
    }
    QRectF rc_ar = chart()->plotArea();
    qreal x = rc_ar.x() + (denominator - 1.0)*rc_ar.width()/denominator;
    rc_ar.setRect(x, rc_ar.y(),  rc_ar.width()/denominator, rc_ar.height());
    chart()->zoomIn(rc_ar);
    qreal dx0 = rc_ar.left() - chart()->plotArea().left();
    // resizeHorScrollBar(denominator, dx0);
    scrollCurrDat_x.ResizeScrollBar(denominator, dx0, chart()->plotArea().width());
}


