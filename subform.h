#ifndef SUBFORM_H
#define SUBFORM_H

#include "cctrlchart.h"
#include "cmyltarchive.h"

#include <QMdiSubWindow>
#include <QSortFilterProxyModel>
#include <QWidget>

namespace Ui {
class SubForm;
}

class SubForm : public QWidget /*QMdiSubWindow*/
{
    Q_OBJECT
private:

    void SetItemToWidgetTable(T_LTAHeadRecDispl *item, int index, const QColor &color);
    // Построить и показать тренды
    void ShowTrends();
    // Сформировать отчет
    void ShowRaport();

    void LoadValFromArch();

    void FillComboBoxPeriod();

    void FillListCtrl();

    void SetItemToListWidget(const T_LTAHeadRecDispl *lTAHeadRec, int index_row);

    TBeginParam GetFirstTime_Step();

    bool SaveToFile(const char *name_file, QProgressDialog *prg);

    void GetParamValLtaDat(THeaderParamLtaData &headerPar);

public:

   // explicit SubForm( CLTArchive *_p_LTArchive, QWidget *parent = nullptr);
    explicit SubForm( CLTAReaderLib *_p_LTArchive, QWidget *parent = nullptr);

    ~SubForm();
    // Заполнить таблицу под трендами
    void SetDataToWidgetList(const QModelIndex &modelIndex);

    CCtrlChart *getCtrlChat() const;

private slots:
    // Сменить закладку
    void on_tabWidget_currentChanged(int index);

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_get_point(const QPointF &point, unsigned int quality, int index_row);

    void on_checkBox_clicked(bool checked);

    void ResetButton();

    void on_tableView_doubleClicked(const QModelIndex &index);
    // Выбор тегов
    void on_toolButton_SelectTag_clicked();
    // Отмена выбранных тегов
    void on_toolButton_UnSelectTag_clicked();
    // Отмена всех выбранных тегов
    void on_toolButton_UnSelectAll_clicked();
    // Показать 2 часовой интервал
    void on_toolButton_2hour_clicked();
    // Показать 8 часовой интервал
    void on_toolButton_8hour_clicked();
    // Показать 20 минутный интервал
    void on_toolButton_20min_clicked();
    // Показать весь тренд
    void on_toolButton_all_clicked();
    // Обновить отчет
    void on_toolButton_Update_clicked();
    // Выбрать все теги
    void on_toolButton_SelectAll_clicked();
    // Сохранить в csv - файл
    void on_toolButton_SaveCSV_clicked();

private:

    Ui::SubForm                     *ui;

    vector<T_LTAHeadRecDispl>       vRecHeadDispl;

    QVector<T_LTADataRecDispl*>     vLTAdata_select;

    CLTAReaderLib                   *p_LTArchive;

    CCtrlChart                      *ctrlChat;

    QSortFilterProxyModel           *proxy;

    TBeginParam                     par0;

    bool                            isChangeSeries, isChangeReport ;

};

#endif // SUBFORM_H
