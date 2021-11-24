#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <qmessagebox.h>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include "cchildwindow.h"

QT_CHARTS_USE_NAMESPACE



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
 //   myLTArchive = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::GetNameFromPuth(QString path, QString &name)
{
    int index = path.lastIndexOf('/');
    if(index != -1)  name = path.remove(0, index + 1);
    else name = path;
}


void MainWindow::on_action_triggered()
{
    QString fileLTA = QFileDialog::getOpenFileName(nullptr, "Open Dialog", "", "*.lta *.alta" );
    if( fileLTA.isEmpty() ) return;
 //  CLTArchive *p_LTArchive = new CLTArchive();
 //   CLTAReaderLib *p_LTArchive = CLTAReaderLib::CreateReaderInst();
    CLTAReaderLib *p_LTArchive = CreateReaderInst();

    if (p_LTArchive && p_LTArchive->Open(fileLTA.toLocal8Bit().constData()) )    {
        ChildWindow *mdiWind = new ChildWindow(p_LTArchive, ui->mdiArea);
        ui->mdiArea->addSubWindow(mdiWind);
        mdiWind->setAttribute(Qt::WA_DeleteOnClose);
        QString name_file;
        GetNameFromPuth(fileLTA, name_file);
        mdiWind->setWindowTitle(name_file);
        mdiWind->show();
    }
    else {
        QMessageBox::warning(this, "Ошибка", "файл не открывается");
        delete p_LTArchive;
    }
}

void MainWindow::on_action_cascad_triggered()
{
     ui->mdiArea->cascadeSubWindows();
}

void MainWindow::on_action_mozaika_triggered()
{
    ui->mdiArea->tileSubWindows();
}

void MainWindow::on_action_exit_triggered()
{
    close();
}

void MainWindow::on_action_print_triggered()
{
        QPrinter printer;
        QPrintDialog *dialog = new QPrintDialog(&printer, this);
        dialog->setWindowTitle(tr("Print Document"));
        if (dialog->exec() == QDialog::Accepted){
//            printer.setOutputFormat(QPrinter::PdfFormat);
//            printer.setOutputFileName("nonwritable.pdf");
//            printer.setOrientation(QPrinter::Landscape);

            ChildWindow *mdiWind = (ChildWindow *)ui->mdiArea->currentSubWindow();
            if( mdiWind ) mdiWind->print_doc( printer);
        }
}
