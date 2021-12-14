#include "subform.h"
#include "ui_subform.h"


#include <qdatetime.h>
#include <QScrollBar>
#include <QMessageBox>
#include <QProgressDialog>
#include <QFile>
#include <QTextStream>



QString get_sec_min_hour_as_str(int tm)
{
    const char *name_time ;
    if( tm < 60  ){
        name_time = " секунд";
    }
    else if( tm < 3600 ){
         tm /= 60;
         if( tm == 1 )  name_time = " минута";
         else if( tm < 5 )  name_time =" минуты";
         else  name_time =" минут";
    }
    else {
        tm /= 3600;
        if( tm == 1)   name_time = " час";
        else if( tm < 5 )  name_time = " часа";
        else  name_time = " часов";
    }
    return QString::number(tm) + name_time ;
}



SubForm::SubForm(CLTAReaderLib *_p_LTArchive, QWidget *parent)
  : QWidget(parent),   ui(new Ui::SubForm),   p_LTArchive(_p_LTArchive)
   /*QMdiSubWindow(parent)*/
{
    ui->setupUi(this);
    proxy = new QSortFilterProxyModel(this);

    CModelLTArchive *myLTArchive = new CModelLTArchive(vRecHeadDispl);

    FillListCtrl();

    proxy->setSourceModel(myLTArchive);
    ui->tableView->setModel(proxy);

     ui->splitter_2->setStretchFactor(0, 3); // 1-ой области максимальный вес
     ui->splitter_2->setStretchFactor(1, 2); // 2-ой области минимальный вес

    ui->tableView->setColumnWidth(0, 200);
    ui->tableView->setColumnWidth(1, 550);
    ui->tableView->setColumnWidth(2, 250);
    ui->tableView->setColumnWidth(3, 150);

    isChangeSeries = isChangeReport = false;
  //  ctrlChat = CCtrlChart::GetInst(ui->horizontalLayout_4);
    ctrlChat = new CCtrlChart(ui->horizontalLayout_4);
    connect(ctrlChat->getChartView(), &ChartView::sendPoint, this, &SubForm::on_get_point);
    connect(ctrlChat->getChartView(), &ChartView::sendResetButton, this, &SubForm::ResetButton);

    int i = 1;
    for(; i < 5; ++i)
        ui->comboBox->addItem(get_sec_min_hour_as_str(i*10*60), i*10);
    for(i = 1; i < 25; ++i)
        ui->comboBox->addItem(get_sec_min_hour_as_str(i*3600), i*60);

    par0 = GetFirstTime_Step();

    FillComboBoxPeriod();

    ui->timeEdit->setTime(par0.t0.time());

}


#define _MSECOND 10000

TBeginParam SubForm::GetFirstTime_Step()
{
    const LONGLONG t1970 = 0x019DB1DED53E8000;
    LONGLONG dt =  p_LTArchive->GetFirstTime() - t1970;
    TBeginParam par;
    par.t0.setMSecsSinceEpoch(dt/_MSECOND);
    par.min_step = static_cast<int>(  p_LTArchive->GetPeriod() );
    return par;
}


// Загрузить расстояние между точками
void SubForm::FillComboBoxPeriod()
{
    const int N = 5;
    const QPair<const QString, int> arr_def[N] =
    {
        {"10 секунд", 10},
        {"1 минута", 60},
        {"2 минуты", 120},
        {"10 минут", 600},
        {"1 час", 3600}
    };
    int i = 0;
    if( par0.min_step > 0  ) {
        for( ; i < N; ++i )
            if( par0.min_step < arr_def[i].second  )  {
                ui->comboBox_2->addItem(get_sec_min_hour_as_str(par0.min_step), par0.min_step);
                break;
            }
        if( i >= N )
            ui->comboBox_2->addItem(get_sec_min_hour_as_str(par0.min_step), par0.min_step);
    }
    for( ; i < N; ++i ) ui->comboBox_2->addItem(arr_def[i].first, arr_def[i].second);
}

void SubForm::FillListCtrl()
{
     p_LTArchive->GetvRecHeadDisp_utf8(vRecHeadDispl) ;
}


SubForm::~SubForm()
{
    if(ctrlChat) delete ctrlChat;
    delete p_LTArchive;
    delete ui;
    for( int i = 0 ; i < vLTAdata_select.size(); ++i ) delete vLTAdata_select[i];
}


void SubForm::SetItemToListWidget( const T_LTAHeadRecDispl *lTAHeadRec, int index_row)
{
    if( ui->listWidget->findItems(lTAHeadRec->TagName, Qt::MatchExactly).size() == 0){
        QListWidgetItem *itm = new QListWidgetItem();
        itm->setData(Qt::UserRole, /*modelIndex.row()*/ index_row);
        itm->setText(lTAHeadRec->TagName );
        ui->listWidget->addItem(itm);
        isChangeSeries = isChangeReport = true;
    }
}

void SubForm::SetDataToWidgetList(const QModelIndex &modelIndex )
{
    QVariant var = modelIndex.data(Qt::UserRole);
    int index_row = var.toInt();
    if(  index_row < vRecHeadDispl.size() )
        SetItemToListWidget( &vRecHeadDispl[index_row], index_row);
}

// Выбор тегов
void SubForm::on_toolButton_SelectTag_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    QModelIndexList modelIndexList = selectionModel->selectedIndexes();
    foreach(QModelIndex item, modelIndexList ) SetDataToWidgetList(item);
   // SetDataToWidgetList(ui->tableView->currentIndex());
}

// Заполнить таблицу под трендами
void SubForm::SetItemToWidgetTable(T_LTAHeadRecDispl *item, int index, const QColor &color)
{
    if(ui->tableWidget->columnCount() == 0)   {
        ui->tableWidget->setColumnCount(9);
        ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Тип") );
        ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Наименование") );
        ui->tableWidget->setColumnWidth(1, 250);
        ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Описание") );
        ui->tableWidget->setColumnWidth(2, 500);
        ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Значение") );
        ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Минимум") );
        ui->tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Максимум") );
        ui->tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Ед.Изм.") );
        ui->tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("Врнмя") );
        ui->tableWidget->setColumnWidth(7, 250);
        ui->tableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("Статус") );
        ui->tableWidget->setColumnWidth(8, 350);
    }
     ui->tableWidget->horizontalHeader()->setVisible(true);
     ui->tableWidget->setRowCount(index + 1);
     ui->tableWidget->setItem(index, 0, new QTableWidgetItem(item->type_IO ));
     ui->tableWidget->item(index, 0)->setBackgroundColor(color);
     ui->tableWidget->setItem(index, 1, new QTableWidgetItem( item->TagName ));
     ui->tableWidget->setItem(index, 2, new QTableWidgetItem( item->TagDesc ));
     ui->tableWidget->setItem(index, 4, new QTableWidgetItem(QString::number(item->MinVal)));
     ui->tableWidget->setItem(index, 5, new QTableWidgetItem(QString::number(item->MaxVal)));
     ui->tableWidget->setItem(index, 6, new QTableWidgetItem( item->EU ));
}

// Сформировать тренды
void SubForm::ShowTrends()
{
    const int MAX_COUNT_TREND = 7; // максимальное количество трендов
    QString title_chart;
    ctrlChat->ClearAllSeries(); // очистить тренды
    int count_trend = vLTAdata_select.size() > MAX_COUNT_TREND ? MAX_COUNT_TREND : ui->listWidget->count();
    for(int i = 0;  i < count_trend  ; ++i)  {
        T_LTADataRecDispl *lTAHeadRec = vLTAdata_select[i];
        T_Info_Series info( lTAHeadRec->header->TagName, lTAHeadRec->header->EU);
        ctrlChat->SetSeries(lTAHeadRec->vVal, info);
        title_chart.append(lTAHeadRec->header->TagName);
        title_chart.append( ", ");
        SetItemToWidgetTable(lTAHeadRec->header, i, info.color);
    }
    int len = title_chart.length();
    if( len && title_chart[len - 2]==',' ) title_chart[len - 2] = '\0';
    ctrlChat->SetTitleChart(title_chart.toStdString().c_str());
    ctrlChat->Render();
}


// Сформировать отчет
void SubForm::ShowRaport()
{
    THeaderParamLtaData headerPar;
    GetParamValLtaDat(headerPar);

    CModelLTADatarchive *p_LTADatarchive = new CModelLTADatarchive(vLTAdata_select);
    p_LTADatarchive->SetHeaderParamLtaData(headerPar);
    ui->tableView_2->setModel(p_LTADatarchive);

    QHeaderView *header = ui->tableView_2->horizontalHeader();
    header->setStretchLastSection(true);
    header->setSectionResizeMode(0,QHeaderView::ResizeToContents);// ширина столбца по содержимому
    header->setSectionResizeMode(1,QHeaderView::ResizeToContents);// ширина столбца по содержимому
    header->setSectionResizeMode(2,QHeaderView::ResizeToContents);// ширина столбца по содержимому
    header->setSectionResizeMode(3,QHeaderView::ResizeToContents);// ширина столбца по содержимому
    header->setSectionResizeMode(4,QHeaderView::ResizeToContents);// ширина столбца по содержимому
}


void SubForm::LoadValFromArch()
{
    for( int i = 0 ; i < vLTAdata_select.size(); ++i ) delete vLTAdata_select[i];
    vLTAdata_select.clear();
    vLTAdata_select.reserve( ui->listWidget->count() );
    for(int i = 0; i < ui->listWidget->count(); ++i)   {
        QListWidgetItem* currentItem = ui->listWidget->item(i);
        uint index_row = currentItem->data(Qt::UserRole).toUInt();
        if (index_row < vRecHeadDispl.size()  ){
            T_LTADataRecDispl *ltaData = new T_LTADataRecDispl(&vRecHeadDispl[index_row]);
            if( p_LTArchive->GetDataByIndex(index_row, ltaData->vVal ) )  vLTAdata_select.push_back(ltaData);
            else delete ltaData;
        }
    }
}

void SubForm::on_tabWidget_currentChanged(int index)
{
    if( index == 1  ){
        if(isChangeSeries) {
            if( isChangeReport ) LoadValFromArch();
            ui->splitter->setStretchFactor(0, 1);  // 1-ой области максимальный вес
            ui->splitter->setStretchFactor(1, 0);  // 2-ой области минимальный вес
            // Показать тренды
            ShowTrends();
            ui->checkBox->setChecked(false);
            ui->checkBox->setDisabled(false);
            isChangeSeries = false;
        }
        return;
    }
    if(index == 2){
        if(isChangeReport) {
            if( isChangeSeries ) LoadValFromArch();
            // Сформировать отчет
            ShowRaport();
            isChangeReport = false;
        }
    }
}

// Отмена выбранных тегов
void SubForm::on_toolButton_UnSelectTag_clicked()
{
    QListWidgetItem* currentItem = ui->listWidget->currentItem();
    if(currentItem) {
        ui->listWidget->removeItemWidget(currentItem);
        delete currentItem;
        isChangeSeries = isChangeReport = true;
    }
}

 // Отмена всех выбранных тегов
void SubForm::on_toolButton_UnSelectAll_clicked()
{
     ui->listWidget->clear();
 //    if(ctrlChat) delete ctrlChat;
     ui->checkBox->setDisabled(true);
     ui->tableWidget->clear();
}

void SubForm::on_listView_doubleClicked(const QModelIndex &index)
{
    SetDataToWidgetList(index);
}

void SubForm::on_lineEdit_2_textChanged(const QString &arg1)
{
    proxy->setFilterFixedString(arg1);
}

void SubForm::on_get_point(const QPointF &point, unsigned int quality, int index_row)
{
     ui->tableWidget->setItem(index_row, 3, new QTableWidgetItem(QString::number(point.y())));
     QDateTime tm;
     tm.setMSecsSinceEpoch( static_cast<qint64>(point.x()) );
     ui->tableWidget->setItem(index_row, 7, new QTableWidgetItem(tm.toString("dd.MM.yyyy hh:mm:ss")));
     char str[1024];
     p_LTArchive->GetStatusAsStr_utf8(quality, str);
     ui->tableWidget->setItem(index_row, 8, new QTableWidgetItem(  str  ));
}

void SubForm::on_checkBox_clicked(bool checked)
{
    ctrlChat->SetMultiAxisY(checked);
}

 // Показать 2 часовой интервал
void SubForm::on_toolButton_2hour_clicked()
{
    ctrlChat->SetHour(CCtrlChart::TWO_HOUR);
    ui->toolButton_all->setChecked(false);
    ui->toolButton_8hour->setChecked(false);
    ui->toolButton_20min->setChecked(false);
}

// Показать 8 часовой интервал
void SubForm::on_toolButton_8hour_clicked()
{
    ctrlChat->SetHour(CCtrlChart::EIGHT_HOUR);
    ui->toolButton_2hour->setChecked(false);
    ui->toolButton_all->setChecked(false);
    ui->toolButton_20min->setChecked(false);
}

 // Показать 20 минутный интервал
void SubForm::on_toolButton_20min_clicked()
{
    ctrlChat->SetHour(CCtrlChart::TWENTY_MINUTES);
    ui->toolButton_2hour->setChecked(false);
    ui->toolButton_8hour->setChecked(false);
    ui->toolButton_all->setChecked(false);
}

// Показать весь тренд
void SubForm::on_toolButton_all_clicked()
{
    ctrlChat->SetHour(CCtrlChart::ALL_HOUR);
    ui->toolButton_2hour->setChecked(false);
    ui->toolButton_8hour->setChecked(false);
    ui->toolButton_20min->setChecked(false);
}

void SubForm::ResetButton()
{
    ui->toolButton_all->setChecked(false);
    ui->toolButton_2hour->setChecked(false);
    ui->toolButton_8hour->setChecked(false);
    ui->toolButton_20min->setChecked(false);
}

CCtrlChart *SubForm::getCtrlChat() const
{
    return ctrlChat;
}

void SubForm::on_tableView_doubleClicked(const QModelIndex &index)
{
    SetDataToWidgetList(index);
}

void SubForm::on_toolButton_Update_clicked()
{
     ShowRaport();
}

 // Выбрать все теги
void SubForm::on_toolButton_SelectAll_clicked()
{
    for(int i = 0; i < vRecHeadDispl.size(); ++i )
       SetItemToListWidget(&vRecHeadDispl[i], i );
}

bool SubForm::SaveToFile(const char *name_file, QProgressDialog *prg)
{
    QFile file(name_file);
    if( file.open(QIODevice::WriteOnly) ) {

        THeaderParamLtaData headerPar;
        GetParamValLtaDat(headerPar);
        prg->setRange(0,  headerPar.valParam.count );
        QTextStream out(&file);
        out.setCodec("windows-1251");
        out << T_LTADataRecDispl::GetHeadreStrCSV();
        out << headerPar.GetStrCSV() << "\n";
        for (int i = 0; i < vLTAdata_select.size(); ++i){
            out << vLTAdata_select[i]->GetStrCSV(headerPar.valParam) << "\n";
            prg->setValue(i);
            if( prg->wasCanceled() ) break;
        }
        file.close();
        prg->setValue(vLTAdata_select.size());
        return true;
    }
    return false;
}

void SubForm::GetParamValLtaDat(THeaderParamLtaData &headerPar)
{
    QTime t0 = ui->timeEdit->time();
    int period = ui->comboBox->currentData().toInt();
    int dist = ui->comboBox_2->currentData().toInt();
    headerPar.SetParam(t0, period, dist, par0);
}

 // Сохранить в csv - файл
void SubForm::on_toolButton_SaveCSV_clicked()
{
    QProgressDialog progress(this);
    progress.setWindowTitle("Подождите");
    progress.setFixedWidth(400);
    progress.setLabelText("Формирование файла...");
    progress.setCancelButtonText("Отмена");
    progress.setWindowModality(Qt::WindowModal);
    QString name_file = ((QMdiSubWindow *)parent())->windowTitle() + ".csv";
    QString txt_msg = "файл " + name_file;
    if( SaveToFile(name_file.toStdString().c_str(), &progress) ) txt_msg.append(" создан");
    else txt_msg.append(" не создан");
    QMessageBox::information(this, "Внимание", txt_msg);
}
