#ifndef CCHILDWINDOW_H
#define CCHILDWINDOW_H

#include <QMdiSubwindow>
#include <QtPrintSupport/QPrinter>
#include <QObject>
#include "subform.h"

namespace Ui {
    class ChildWindow;
}

class ChildWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
//    explicit ChildWindow( CLTArchive *_p_LTArchive, QWidget *parent = nullptr);
      explicit ChildWindow( CLTAReaderLib *_p_LTArchive, CLoaderLibrary  *_loaderLibrary, QWidget *parent = nullptr);
    ~ChildWindow() override;

    void print_doc(QPrinter &printer);

    const QVector<T_LTADataRecDispl *> GetLTASelectDataRec();

    TBeginParam GetFirstTime_Step();

private:

    SubForm *myWidget;

    CLoaderLibrary  *loaderLibrary;
};


#endif // CCHILDWINDOW_H
