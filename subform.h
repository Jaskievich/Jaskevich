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
    void ShowTrends(); // Построить и показать тренды
    void ShowRaport(); // Показать отчет

public:
    explicit SubForm( CModelLTArchive *_myLTArchive, QWidget *parent = nullptr);
    ~SubForm();

    void SetDataToWidgetList(const QModelIndex &modelIndex);

    CCtrlChart *getCtrlChat() const;

private slots:

    void on_tabWidget_currentChanged(int index);

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void  on_get_point(const QPointF &point, unsigned short quality, int index_row);

    void on_checkBox_clicked(bool checked);

    void ResetButton();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_comboBox_currentIndexChanged(int index);

    void on_toolButton_SelectTag_clicked();         // Выбор тегов

    void on_toolButton_UnSelectTag_clicked();       // Отмена выбранных тегов

    void on_toolButton_UnSelectAll_clicked();       // Отмена всех выбранных тегов

    void on_toolButton_2hour_clicked();             // Показать 2 часовой интервал

    void on_toolButton_8hour_clicked();             // Показать 8 часовой интервал

    void on_toolButton_20min_clicked();             // Показать 20 минутный интервал

      void on_toolButton_all_clicked();             // Показать весь тренд

private:
    Ui::SubForm *ui;
    CModelLTArchive *myLTArchive;
    CCtrlChart *ctrlChat;
    QSortFilterProxyModel *proxy;
    CModelLTADatarchive *p_LTADatarchive;
    bool isChangeSeries, isChangeReport, isChangeReportParam ;

};

#endif // SUBFORM_H
