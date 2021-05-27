#include "subform.h"
#include "ui_subform.h"


#include <qdatetime.h>
#include <sstream>
#include <QScrollBar>



SubForm::SubForm(CModelLTArchive *_myLTArchive, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SubForm),
    myLTArchive(_myLTArchive)
   /*QMdiSubWindow(parent)*/
{
    ui->setupUi(this);
    proxy = new QSortFilterProxyModel(this);

    proxy->setSourceModel(myLTArchive);
    ui->tableView->setModel(proxy);

     ui->splitter_2->setStretchFactor(0, 1); // 1-ой области максимальный вес
     ui->splitter_2->setStretchFactor(1, 0); // 2-ой области минимальный вес

    ui->tableView->setColumnWidth(0, 200);
    ui->tableView->setColumnWidth(1, 350);
    ui->tableView->setColumnWidth(2, 250);
    ui->tableView->setColumnWidth(3, 150);

    isChangeSeries = isChangeReport = isChangeReportParam = false;
  //  ctrlChat = CCtrlChart::GetInst(ui->horizontalLayout_4);
    ctrlChat = new CCtrlChart(ui->horizontalLayout_4);
    connect(ctrlChat->getChartView(), &ChartView::sendPoint, this, &SubForm::on_get_point);
    connect(ctrlChat->getChartView(), &ChartView::sendResetButton, this, &SubForm::ResetButton);

    p_LTADatarchive = new CModelLTADatarchive();
    ui->tableView_2->setModel(p_LTADatarchive);

    int i = 1;
    for(; i < 5; ++i)
        ui->comboBox->addItem(QString::number( i*10 ) + " мин", i*10);
    for(i = 1; i < 25; ++i)
        ui->comboBox->addItem(QString::number( i ) + " час", i*60);

    ui->comboBox_2->addItem("10 секунд", 10);
    ui->comboBox_2->addItem("1 минута", 60);
    ui->comboBox_2->addItem("2 минуты", 120);
    ui->comboBox_2->addItem("10 минут", 600);
    ui->comboBox_2->addItem("1 час", 3600);

    QHeaderView *header = ui->tableView_2->horizontalHeader();
    header->setStretchLastSection(true);
    header->setSectionResizeMode(0,QHeaderView::ResizeToContents);// ширина столбца по содержимому
    header->setSectionResizeMode(1,QHeaderView::ResizeToContents);// ширина столбца по содержимому
    header->setSectionResizeMode(2,QHeaderView::ResizeToContents);// ширина столбца по содержимому
    header->setSectionResizeMode(3,QHeaderView::ResizeToContents);// ширина столбца по содержимому
    header->setSectionResizeMode(4,QHeaderView::ResizeToContents);// ширина столбца по содержимому

    header->setBackgroundRole(QPalette::Window);
    header->setLineWidth(2);


}

SubForm::~SubForm()
{
    if(ctrlChat) delete ctrlChat;
    delete myLTArchive;
    delete ui;
}


void SubForm::SetDataToWidgetList(const QModelIndex &modelIndex )
{
    QVariant var = modelIndex.data(Qt::UserRole);
    uint index_row = var.toUInt();
    T_LTAHeadRecDispl *lTAHeadRec = myLTArchive->getItem(static_cast<int>(index_row));
    if( ui->listWidget->findItems(lTAHeadRec->TagName, Qt::MatchExactly).size() == 0){
        QListWidgetItem *itm = new QListWidgetItem();
        itm->setData(Qt::UserRole, /*modelIndex.row()*/ index_row);
        itm->setText(lTAHeadRec->TagName);
        ui->listWidget->addItem(itm);
        isChangeSeries = isChangeReport = true;
    }
}

// Выбор тегов
void SubForm::on_toolButton_SelectTag_clicked()
{
    SetDataToWidgetList(ui->tableView->currentIndex());
}

void SubForm::SetItemToWidgetTable(T_LTAHeadRecDispl *item, int index, const QColor &color)
{
    if(ui->tableWidget->columnCount() == 0)   {
        ui->tableWidget->setColumnCount(9);
        ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Тип") );
        ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Наименование") );
        ui->tableWidget->setColumnWidth(1, 250);
        ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Описание") );
        ui->tableWidget->setColumnWidth(2, 250);
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
     ui->tableWidget->setItem(index, 0, new QTableWidgetItem(item->type_IO));
     ui->tableWidget->item(index, 0)->setBackgroundColor(color);
     ui->tableWidget->setItem(index, 1, new QTableWidgetItem( item->TagName ));
     ui->tableWidget->setItem(index, 2, new QTableWidgetItem( item->TagDesc ));
     ui->tableWidget->setItem(index, 4, new QTableWidgetItem(QString::number(item->MinVal)));
     ui->tableWidget->setItem(index, 5, new QTableWidgetItem(QString::number(item->MaxVal)));
     ui->tableWidget->setItem(index, 6, new QTableWidgetItem( item->EU ));
}

// Построить и показать тренды
void SubForm::ShowTrends()
{    
    QString title_chart;
    ctrlChat->ClearAllSeries();
    deque<VQT> arr;
    for(int i = 0, j = 0; i < ui->listWidget->count(); ++i)   {
        QListWidgetItem* currentItem = ui->listWidget->item(i);
        uint index_row = currentItem->data(Qt::UserRole).toUInt();
        if ( myLTArchive->GetDataByIndex(index_row, arr) ){
            T_LTAHeadRecDispl *lTAHeadRec = myLTArchive->getItem(static_cast<int>(index_row) );
            T_Info_Series info( lTAHeadRec->TagName, lTAHeadRec->EU);
            ctrlChat->SetSeries(arr, info);
            title_chart.append(lTAHeadRec->TagName);
            title_chart.append( ", ");
            SetItemToWidgetTable(lTAHeadRec, j, info.color);
            j++;
        }
    }
    int len = title_chart.length();
    if( len && title_chart[len - 2]==',' ) title_chart[len - 2] = '\0';
    ctrlChat->SetTitleChart(title_chart.toStdString().c_str());
    ctrlChat->Render();
}

// Сформировать отчет
void SubForm::ShowRaport()
{
    p_LTADatarchive->clearData();
    QDateTime t0 = myLTArchive->GetFirstTime();
    deque<VQT> arr;
    int period = ui->comboBox->currentData().toInt();
    int dist = ui->comboBox_2->currentData().toInt();
    THeaderParamLtaData *headerPar = p_LTADatarchive->getHeaderParam();
    headerPar->SetParam(t0.time(), period, dist);
    for(int i = 0; i < ui->listWidget->count(); ++i)   {
        QListWidgetItem* currentItem = ui->listWidget->item(i);
        uint index_row = currentItem->data(Qt::UserRole).toUInt();
        if ( myLTArchive->GetDataByIndex(index_row, arr) ){
            T_LTAHeadRecDispl *lTAHeadRec = myLTArchive->getItem(static_cast<int>(index_row));
            T_LTADataRecDispl ltaData;
            ltaData.gid = lTAHeadRec->gid;
            ltaData.TagName = lTAHeadRec->TagName;
            ltaData.TagDesc = lTAHeadRec->TagDesc;
            ltaData.SC_HI = lTAHeadRec->SC_HI;
            ltaData.SC_LO = lTAHeadRec->SC_LO;
            ltaData.addData(arr, headerPar);
            p_LTADatarchive->addData(ltaData);
        }
    }
}


void SubForm::on_tabWidget_currentChanged(int index)
{
    if( index == 1  ){
        if(isChangeSeries) {
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
            ShowRaport();
            isChangeReport = false;
            isChangeReportParam = true;
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

void SubForm::on_get_point(const QPointF &point, unsigned short quality, int index_row)
{
     std::stringstream str;
     ui->tableWidget->setItem(index_row, 3, new QTableWidgetItem(QString::number(point.y())));
     QDateTime tm;
     tm.setMSecsSinceEpoch( static_cast<qint64>(point.x()) );
     ui->tableWidget->setItem(index_row, 7, new QTableWidgetItem(tm.toString("dd.mm.yyyy hh:mm:ss")));
     str << TQuality(quality);
     ui->tableWidget->setItem(index_row, 8, new QTableWidgetItem( RUS( str.str().c_str() ) ));
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


void SubForm::on_comboBox_currentIndexChanged(int index)
{
    if(!isChangeReportParam) return ;
    ShowRaport();
}

void SubForm::on_toolButton_Update_clicked()
{
     ShowRaport();
}
