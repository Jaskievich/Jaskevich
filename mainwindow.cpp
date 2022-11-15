#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <qmessagebox.h>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include "cchildwindow.h"
#include "compareResultForm.h"

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


void GetNameFromPuth(QString path, QString &name)
{
    int index = path.lastIndexOf('/');
    if(index != -1)  name = path.remove(0, index + 1);
    else name = path;
}

bool OpenFile_LoadLibrary(QWidget *parent, std::tuple<CLoaderLibrary* , CLTAReaderLib* , QString > &tp)
{
    std::get<0>(tp) = nullptr;
    std::get<1>(tp) = nullptr;
    std::get<2>(tp) = "";
    QString fileLTA = QFileDialog::getOpenFileName(nullptr, "Open Dialog", "", "*.lta *.alta" );
    if( fileLTA.isEmpty() ) return false;
    // Проверить по расширению файла какую загружать dll
    int indx = fileLTA.lastIndexOf('.');
    if( indx > 0 ){
        CLoaderLibrary  *loaderLibrary = new CLoaderLibrary();
        int len = fileLTA.length() - indx;
        try
        {
            loaderLibrary->Load_library_lta( fileLTA.rightRef(len).toString() ) ;
        }
        catch(QString msg)  {
            QMessageBox::warning(parent, "Ошибка", msg);
            delete  loaderLibrary;
            return false;
        }
        CLTAReaderLib *p_LTArchive = loaderLibrary->CreateReaderInst(nullptr);
        if (p_LTArchive && p_LTArchive->Open(fileLTA.toLocal8Bit().constData()) )    {
            std::get<0>(tp) = loaderLibrary;
            std::get<1>(tp) = p_LTArchive;
            QString name_file;
            GetNameFromPuth(fileLTA, name_file);
            std::get<2>(tp) = name_file;
            return true;
        }
        else {
            delete p_LTArchive;
            delete loaderLibrary;
        }
    }
    QMessageBox::warning(parent, "Ошибка", "файл"+ fileLTA +"не открывается");
    return false;
}


//void MainWindow::on_action_triggered()
//{
//    QString fileLTA = QFileDialog::getOpenFileName(nullptr, "Open Dialog", "", "*.lta *.alta" );
//    if( fileLTA.isEmpty() ) return;
//    // Проверить по расширению файла какую загружать dll
//    int indx = fileLTA.lastIndexOf('.');
//    if( indx > 0 ){
//        CLoaderLibrary  *loaderLibrary = new CLoaderLibrary();
//        int len = fileLTA.length() - indx;
//        try
//        {
//            loaderLibrary->Load_library_lta( fileLTA.rightRef(len).toString() ) ;
//        }
//        catch(QString msg)  {
//            QMessageBox::warning(this, "Ошибка", msg);
//            delete  loaderLibrary;
//            return;
//        }
//        CLTAReaderLib *p_LTArchive = loaderLibrary->CreateReaderInst(nullptr);
//        if (p_LTArchive && p_LTArchive->Open(fileLTA.toLocal8Bit().constData()) )    {
//            ChildWindow *mdiWind = new ChildWindow(p_LTArchive, loaderLibrary, ui->mdiArea);
//            ui->mdiArea->addSubWindow(mdiWind);
//            mdiWind->setAttribute(Qt::WA_DeleteOnClose);
//            QString name_file;
//            GetNameFromPuth(fileLTA, name_file);
//            mdiWind->setWindowTitle(name_file);
//            mdiWind->show();
//            return ;
//        }
//        else {
//            delete p_LTArchive;
//            delete loaderLibrary;
//        }
//        QMessageBox::warning(this, "Ошибка", "файл не открывается");
//    }

//}

void MainWindow::on_action_triggered()
{
    std::tuple<CLoaderLibrary* , CLTAReaderLib* , QString > tp;
    if( OpenFile_LoadLibrary(this, tp) ){
        CLoaderLibrary* loaderLibrary = std::get<0>(tp);
        CLTAReaderLib* p_LTArchive = std::get<1>(tp);
        QString name_file = std::get<2>(tp);
        ChildWindow *mdiWind = new ChildWindow(p_LTArchive, loaderLibrary, ui->mdiArea);
        ui->mdiArea->addSubWindow(mdiWind);
        mdiWind->setAttribute(Qt::WA_DeleteOnClose);
        mdiWind->setWindowTitle(name_file);
        mdiWind->show();
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

            ChildWindow *mdiWind = static_cast<ChildWindow *>(ui->mdiArea->currentSubWindow());
            if( mdiWind ) mdiWind->print_doc( printer);
        }
}

void MainWindow::on_action_connect_triggered()
{
    CLoaderLibrary  *loaderLibrary = new CLoaderLibrary();
    try
    {
        loaderLibrary->Load_library_lta( ".alta" ) ;
    }
    catch(QString msg)  {
        QMessageBox::warning(this, "Ошибка", msg);
        delete  loaderLibrary;
        return;
    }
    CLTAReaderLib *p_LTArchive = loaderLibrary->CreateReaderInst("login");
    if( p_LTArchive ){
        ChildWindow *mdiWind = new ChildWindow(p_LTArchive, loaderLibrary, ui->mdiArea);
        ui->mdiArea->addSubWindow(mdiWind);
        mdiWind->setAttribute(Qt::WA_DeleteOnClose);
        T_InfoArch infoArch;
        if( p_LTArchive->GetInfoArch_utf8(&infoArch) ){
             QString name_file = infoArch.name_plant;
             mdiWind->setWindowTitle(name_file);
        }
        mdiWind->show();
        ui->action_connect->setText("Выбрать установку и дату архива");

        return ;
//        delete p_LTArchive;
    }
   // else   delete loaderLibrary;
    delete loaderLibrary;
}

// Сравнить значения тегов из разных арзивов
void MainWindow::on_action_equal_triggered()
{
    ChildWindow *mdiWind = static_cast<ChildWindow *>(ui->mdiArea->currentSubWindow() );
    if( !mdiWind ) return ;
    const QVector<T_LTADataRecDispl *> &vLTADataSelect = mdiWind->GetLTASelectDataRec();
    if( vLTADataSelect.size() == 0 ){
        QMessageBox::information(this, "Внимание", "Не выбран тег для сравнения");
        return ;
    }
    std::tuple<CLoaderLibrary* , CLTAReaderLib* , QString > tp;
    if( OpenFile_LoadLibrary(this, tp) ){
        CLoaderLibrary* loaderLibrary = std::get<0>(tp);
        CLTAReaderLib* p_LTArchive = std::get<1>(tp);
        QString name_file = std::get<2>(tp);

        T_LTADataRecDispl *LTADataItem = vLTADataSelect[0];

        vector<T_LTAHeadRecDispl> vRecHeadDispl;
        p_LTArchive->GetvRecHeadDisp_utf8(vRecHeadDispl) ;
        // Найти по гиду в vRecHeadDispl
        unsigned index = 0;
        for( ; index < vRecHeadDispl.size(); ++index )
            if( vRecHeadDispl[index].gid == LTADataItem->header->gid ) break;
        if( index < vRecHeadDispl.size() ){
            vector< T_ItemVal > array;
            if( p_LTArchive->GetDataByIndex(index, array) ){
                CompareResultData compareResultData;
                compareResultData.SetField(0, LTADataItem->header->TagName, mdiWind->windowTitle().toStdString().c_str());
                compareResultData.SetField(1, vRecHeadDispl[index].TagName, name_file.toStdString().c_str());
                compareResultData.SetVectorResult(LTADataItem->vVal, static_cast<unsigned long>( mdiWind->GetFirstTime_Step().min_step ), array, p_LTArchive->GetPeriod());
                CompareResultForm *compareResultForm = new CompareResultForm(compareResultData);
                compareResultForm->setWindowTitle("Сравнение тегов");
                ui->mdiArea->addSubWindow(compareResultForm);
                compareResultForm->show();
            }
        }
        else{
           QString msg = "В выбранном архиве тега ";
           msg.append(LTADataItem->header->TagName);
           msg.append(" нет");
           QMessageBox::information(this, "Внимание", msg);
        }

        delete p_LTArchive;
        delete loaderLibrary;

    }
}

