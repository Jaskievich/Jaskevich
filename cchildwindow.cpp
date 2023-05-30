#include "cchildwindow.h"
#include <QMessageBox>

//ChildWindow::ChildWindow(CModelLTArchive *myLTArchive, QWidget *parent ):QMdiSubWindow(parent)
//{
//    myWidget = new SubForm(myLTArchive, this);
//    this->setWidget(myWidget);
//}

ChildWindow::ChildWindow(CLTAReaderLib *_p_LTArchive,  CLoaderLibrary  *_loaderLibrary, QWidget *parent ):QMdiSubWindow(parent)
{
    loaderLibrary = _loaderLibrary;
    myWidget = new SubForm(_p_LTArchive, this);
    this->setWidget(myWidget);
}

ChildWindow::~ChildWindow( )
{
    myWidget->~SubForm();
    delete loaderLibrary;
}

void ChildWindow::print_doc( QPrinter &printer)
{
    ChartView *chtw = myWidget->getCtrlChat()->getChartView();
    if( chtw == NULL || chtw->chart()->series().size() == 0 ) {
        QMessageBox::warning(this, "Внимание", "Тренд не сформирован");
        return;
    }
    QMessageBox::warning(this, "Внимание", windowTitle());
    QPainter painter;
    if (!painter.begin(&printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        return ;
    }
    //    ChartView *chtw = myWidget->getCtrlChat()->getChartView();
    //    QRect rect = printer.pageRect();

    //    QSize size = chtw->size();

    //    double xscale = printer.pageRect().width()/double(chtw->width());
    //    double yscale = printer.pageRect().height()/double(chtw->height());
    //    double scale = qMin(xscale, yscale);

    //    double yscale_2 = printer.pageRect().height()/double(chtw->height()*scale);

    //    painter.scale(1.0, yscale_2);

    painter.setRenderHint(QPainter::Antialiasing);
    myWidget->getCtrlChat()->PrintTrends(painter);
    painter.end();
}




const QVector<T_LTADataRecDispl *> ChildWindow::GetLTASelectDataRec()
{
    myWidget->LoadValFromArch();
    return myWidget->vLTAdata_select;
}

TBeginParam ChildWindow::GetFirstTime_Step()
{
    return myWidget->GetFirstTime_Step();
}
