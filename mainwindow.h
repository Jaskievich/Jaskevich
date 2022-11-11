#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QtWidgets>


#include "cmyltarchive.h"


//Q_DECLARE_METATYPE(LTAHeadRec)
//Q_DECLARE_METATYPE(LTAHeadRec*)


#include "cctrlchart.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:

    void on_action_triggered();

    void on_action_cascad_triggered();

    void on_action_mozaika_triggered();

    void on_action_exit_triggered();

    void on_action_print_triggered();

    void on_action_connect_triggered();

    void on_action_equal_triggered();

private:


    Ui::MainWindow *ui;
  //  CModelLTArchive *myLTArchive;
};

#endif // MAINWINDOW_H
