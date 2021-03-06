#include "cchildwindow.h"
#include <QMessageBox>

ChildWindow::ChildWindow(CModelLTArchive *myLTArchive, QWidget *parent ):QMdiSubWindow(parent)
{
    myWidget = new SubForm(myLTArchive, this);
    this->setWidget(myWidget);
}

ChildWindow::~ChildWindow( )
{
    myWidget->~SubForm();
}

void ChildWindow::print_doc( QPrinter &printer)
{
    QMessageBox::warning(this, "Внимание", windowTitle());

    QPainter painter;
    if (! painter.begin(&printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        return ;
    }

    ChartView *chtw = myWidget->getCtrlChat()->getChartView();

    QRect rect = printer.paperRect();

    QSize size = chtw->size();

    painter.translate(0, abs(rect.height() - size.height())/2);

    painter.setRenderHint(QPainter::Antialiasing);

    myWidget->getCtrlChat()->PrintTrends(painter);

    painter.end();
}
