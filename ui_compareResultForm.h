/********************************************************************************
** Form generated from reading UI file 'compareResultForm.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPARERESULTFORM_H
#define UI_COMPARERESULTFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CompareResultForm
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *compareResult_tableWidget;

    void setupUi(QWidget *CompareResultForm)
    {
        if (CompareResultForm->objectName().isEmpty())
            CompareResultForm->setObjectName(QString::fromUtf8("CompareResultForm"));
        CompareResultForm->resize(400, 300);
        verticalLayout = new QVBoxLayout(CompareResultForm);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        compareResult_tableWidget = new QTableWidget(CompareResultForm);
        compareResult_tableWidget->setObjectName(QString::fromUtf8("compareResult_tableWidget"));

        verticalLayout->addWidget(compareResult_tableWidget);


        retranslateUi(CompareResultForm);

        QMetaObject::connectSlotsByName(CompareResultForm);
    } // setupUi

    void retranslateUi(QWidget *CompareResultForm)
    {
        CompareResultForm->setWindowTitle(QApplication::translate("CompareResultForm", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CompareResultForm: public Ui_CompareResultForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPARERESULTFORM_H
