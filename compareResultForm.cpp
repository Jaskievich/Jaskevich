#include "compareResultForm.h"
#include "ui_compareResultForm.h"
#include <QDateTime>
#include <Utility.h>

CompareResultForm::CompareResultForm(CompareResultData  &_CompareResultData, QWidget *parent) :
    QWidget(parent),    ui(new Ui::CompareResultForm),    m_CompareResultData(_CompareResultData)
{
    ui->setupUi(this);
    ui->compareResult_tableWidget->setColumnCount(5);
    QStringList listLanel;
    listLanel.append("Время 1");
    listLanel.append(m_CompareResultData.name_field[0]);
     listLanel.append("Время 2");
    listLanel.append(m_CompareResultData.name_field[1]);
    listLanel.append("Разница");
    ui->compareResult_tableWidget->setHorizontalHeaderLabels(listLanel);

    SYSTEMTIME st;
    QDateTime tm;
    vector<CompareResultIten> &vResultItem = m_CompareResultData.vResultItem;
    ui->compareResult_tableWidget->setRowCount(static_cast<int>(vResultItem.size()));
    for( unsigned i = 0; i < vResultItem.size(); ++i){

        st = _GetLocalTime(GetFileTime(vResultItem[i].val1.time));
        tm.setDate(QDate(st.wYear, st.wMonth, st.wDay) );
        tm.setTime(QTime(st.wHour, st.wMinute, st.wSecond));
        ui->compareResult_tableWidget->setItem(i, 0, new QTableWidgetItem(tm.toString("dd.MM.yyyy hh:mm:ss")));

        ui->compareResult_tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(vResultItem[i].val1.val)));

        st = _GetLocalTime(GetFileTime(vResultItem[i].val2.time));
        tm.setDate(QDate(st.wYear, st.wMonth, st.wDay) );
        tm.setTime(QTime(st.wHour, st.wMinute, st.wSecond));
        ui->compareResult_tableWidget->setItem(i, 2, new QTableWidgetItem(tm.toString("dd.MM.yyyy hh:mm:ss")));

        ui->compareResult_tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(vResultItem[i].val2.val)));
        ui->compareResult_tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(vResultItem[i].DiffValue())));
    }

     ui->compareResult_tableWidget->setColumnWidth(0, 300);
     ui->compareResult_tableWidget->setColumnWidth(1, 300);
     ui->compareResult_tableWidget->setColumnWidth(2, 300);
     ui->compareResult_tableWidget->setColumnWidth(3, 300);

}

CompareResultForm::~CompareResultForm()
{
    delete ui;
}
