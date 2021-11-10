

#include "mainwindow.h"
#include <QApplication>
//#define NO_VINCADB



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    MainWindow w;
    w.show();

    return a.exec();

}
