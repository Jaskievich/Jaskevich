/********************************************************************************
** Form generated from reading UI file 'subform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUBFORM_H
#define UI_SUBFORM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SubForm
{
public:
    QGridLayout *gridLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab_4;
    QGridLayout *gridLayout_3;
    QSplitter *splitter_2;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *lineEdit_2;
    QTableView *tableView;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout;
    QToolButton *toolButton_SelectTag;
    QToolButton *toolButton_UnSelectTag;
    QToolButton *toolButton_UnSelectAll;
    QSpacerItem *verticalSpacer;
    QToolButton *toolButton_SelectAll;
    QSpacerItem *verticalSpacer_4;
    QListWidget *listWidget;
    QWidget *tab_5;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QCheckBox *checkBox;
    QToolButton *toolButton_all;
    QToolButton *toolButton_2hour;
    QToolButton *toolButton_8hour;
    QToolButton *toolButton_20min;
    QSpacerItem *horizontalSpacer;
    QSplitter *splitter;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_4;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_5;
    QTabWidget *tabWidget_2;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_6;
    QTableWidget *tableWidget;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout_8;
    QTableWidget *tableWidget_2;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QTimeEdit *timeEdit;
    QLabel *label_4;
    QComboBox *comboBox;
    QLabel *label_5;
    QComboBox *comboBox_2;
    QToolButton *toolButton_Update;
    QToolButton *toolButton_SaveCSV;
    QSpacerItem *horizontalSpacer_3;
    QTableView *tableView_2;

    void setupUi(QWidget *SubForm)
    {
        if (SubForm->objectName().isEmpty())
            SubForm->setObjectName(QString::fromUtf8("SubForm"));
        SubForm->resize(801, 381);
        gridLayout_2 = new QGridLayout(SubForm);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(SubForm);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QFont font;
        font.setPointSize(12);
        tabWidget->setFont(font);
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout_3 = new QGridLayout(tab_4);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        splitter_2 = new QSplitter(tab_4);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        verticalLayoutWidget_3 = new QWidget(splitter_2);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(verticalLayoutWidget_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);

        horizontalLayout_2->addWidget(label);

        lineEdit_2 = new QLineEdit(verticalLayoutWidget_3);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        horizontalLayout_2->addWidget(lineEdit_2);


        verticalLayout_5->addLayout(horizontalLayout_2);

        tableView = new QTableView(verticalLayoutWidget_3);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setEnabled(true);
        tableView->setFont(font);
        tableView->setMouseTracking(false);
        tableView->setAcceptDrops(false);
        tableView->setFrameShape(QFrame::StyledPanel);
        tableView->setEditTriggers(QAbstractItemView::CurrentChanged|QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_5->addWidget(tableView);

        splitter_2->addWidget(verticalLayoutWidget_3);
        verticalLayoutWidget = new QWidget(splitter_2);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        verticalLayout_2->addWidget(label_3, 0, Qt::AlignHCenter);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        toolButton_SelectTag = new QToolButton(verticalLayoutWidget);
        toolButton_SelectTag->setObjectName(QString::fromUtf8("toolButton_SelectTag"));
        toolButton_SelectTag->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(toolButton_SelectTag->sizePolicy().hasHeightForWidth());
        toolButton_SelectTag->setSizePolicy(sizePolicy);
        toolButton_SelectTag->setFont(font);
        toolButton_SelectTag->setIconSize(QSize(16, 16));

        verticalLayout->addWidget(toolButton_SelectTag);

        toolButton_UnSelectTag = new QToolButton(verticalLayoutWidget);
        toolButton_UnSelectTag->setObjectName(QString::fromUtf8("toolButton_UnSelectTag"));
        toolButton_UnSelectTag->setFont(font);

        verticalLayout->addWidget(toolButton_UnSelectTag);

        toolButton_UnSelectAll = new QToolButton(verticalLayoutWidget);
        toolButton_UnSelectAll->setObjectName(QString::fromUtf8("toolButton_UnSelectAll"));
        toolButton_UnSelectAll->setEnabled(true);
        sizePolicy.setHeightForWidth(toolButton_UnSelectAll->sizePolicy().hasHeightForWidth());
        toolButton_UnSelectAll->setSizePolicy(sizePolicy);
        toolButton_UnSelectAll->setFont(font);

        verticalLayout->addWidget(toolButton_UnSelectAll);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer);

        toolButton_SelectAll = new QToolButton(verticalLayoutWidget);
        toolButton_SelectAll->setObjectName(QString::fromUtf8("toolButton_SelectAll"));
        sizePolicy.setHeightForWidth(toolButton_SelectAll->sizePolicy().hasHeightForWidth());
        toolButton_SelectAll->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(toolButton_SelectAll);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);


        horizontalLayout_7->addLayout(verticalLayout);

        listWidget = new QListWidget(verticalLayoutWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setFont(font);

        horizontalLayout_7->addWidget(listWidget);


        verticalLayout_2->addLayout(horizontalLayout_7);

        splitter_2->addWidget(verticalLayoutWidget);

        gridLayout_3->addWidget(splitter_2, 0, 0, 1, 1);

        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        gridLayout = new QGridLayout(tab_5);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        checkBox = new QCheckBox(tab_5);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setEnabled(false);

        horizontalLayout->addWidget(checkBox, 0, Qt::AlignTop);

        toolButton_all = new QToolButton(tab_5);
        toolButton_all->setObjectName(QString::fromUtf8("toolButton_all"));
        toolButton_all->setCheckable(true);
        toolButton_all->setChecked(true);

        horizontalLayout->addWidget(toolButton_all, 0, Qt::AlignTop);

        toolButton_2hour = new QToolButton(tab_5);
        toolButton_2hour->setObjectName(QString::fromUtf8("toolButton_2hour"));
        toolButton_2hour->setCheckable(true);
        toolButton_2hour->setToolButtonStyle(Qt::ToolButtonTextOnly);

        horizontalLayout->addWidget(toolButton_2hour, 0, Qt::AlignTop);

        toolButton_8hour = new QToolButton(tab_5);
        toolButton_8hour->setObjectName(QString::fromUtf8("toolButton_8hour"));
        toolButton_8hour->setCheckable(true);

        horizontalLayout->addWidget(toolButton_8hour, 0, Qt::AlignTop);

        toolButton_20min = new QToolButton(tab_5);
        toolButton_20min->setObjectName(QString::fromUtf8("toolButton_20min"));
        toolButton_20min->setCheckable(true);
        toolButton_20min->setChecked(false);

        horizontalLayout->addWidget(toolButton_20min, 0, Qt::AlignTop);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        splitter = new QSplitter(tab_5);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        horizontalLayoutWidget = new QWidget(splitter);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        splitter->addWidget(horizontalLayoutWidget);
        horizontalLayoutWidget_2 = new QWidget(splitter);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        tabWidget_2 = new QTabWidget(horizontalLayoutWidget_2);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayout_6 = new QHBoxLayout(tab_2);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        tableWidget = new QTableWidget(tab_2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setMinimumSize(QSize(587, 50));
        tableWidget->setMaximumSize(QSize(16777215, 16777215));
        tableWidget->setBaseSize(QSize(0, 0));
        tableWidget->setFont(font);
        tableWidget->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        tableWidget->setAutoFillBackground(false);
        tableWidget->setInputMethodHints(Qt::ImhNone);
        tableWidget->setFrameShape(QFrame::StyledPanel);
        tableWidget->setFrameShadow(QFrame::Sunken);
        tableWidget->setLineWidth(2);
        tableWidget->setMidLineWidth(2);
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(0);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_6->addWidget(tableWidget);

        tabWidget_2->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        horizontalLayout_8 = new QHBoxLayout(tab_3);
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        tableWidget_2 = new QTableWidget(tab_3);
        tableWidget_2->setObjectName(QString::fromUtf8("tableWidget_2"));

        horizontalLayout_8->addWidget(tableWidget_2);

        tabWidget_2->addTab(tab_3, QString());

        horizontalLayout_5->addWidget(tabWidget_2);

        splitter->addWidget(horizontalLayoutWidget_2);

        gridLayout->addWidget(splitter, 1, 0, 1, 1);

        tabWidget->addTab(tab_5, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(4);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        timeEdit = new QTimeEdit(tab);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));
        timeEdit->setReadOnly(false);
        timeEdit->setCalendarPopup(false);

        horizontalLayout_3->addWidget(timeEdit);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_3->addWidget(label_4);

        comboBox = new QComboBox(tab);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout_3->addWidget(comboBox);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_3->addWidget(label_5);

        comboBox_2 = new QComboBox(tab);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        horizontalLayout_3->addWidget(comboBox_2);

        toolButton_Update = new QToolButton(tab);
        toolButton_Update->setObjectName(QString::fromUtf8("toolButton_Update"));
        toolButton_Update->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(toolButton_Update->sizePolicy().hasHeightForWidth());
        toolButton_Update->setSizePolicy(sizePolicy1);
        toolButton_Update->setMinimumSize(QSize(45, 0));
        toolButton_Update->setBaseSize(QSize(0, 0));
        toolButton_Update->setToolTipDuration(3);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../../Icons/must_have_icon_set/Refresh/Refresh.ico"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_Update->setIcon(icon);
        toolButton_Update->setCheckable(true);
        toolButton_Update->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolButton_Update->setAutoRaise(true);

        horizontalLayout_3->addWidget(toolButton_Update);

        toolButton_SaveCSV = new QToolButton(tab);
        toolButton_SaveCSV->setObjectName(QString::fromUtf8("toolButton_SaveCSV"));
        sizePolicy1.setHeightForWidth(toolButton_SaveCSV->sizePolicy().hasHeightForWidth());
        toolButton_SaveCSV->setSizePolicy(sizePolicy1);
        toolButton_SaveCSV->setMinimumSize(QSize(45, 0));
        toolButton_SaveCSV->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolButton_SaveCSV->setAutoRaise(true);
        toolButton_SaveCSV->setArrowType(Qt::NoArrow);

        horizontalLayout_3->addWidget(toolButton_SaveCSV);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout_3);

        tableView_2 = new QTableView(tab);
        tableView_2->setObjectName(QString::fromUtf8("tableView_2"));
        tableView_2->setFont(font);
        tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView_2->horizontalHeader()->setCascadingSectionResizes(true);
        tableView_2->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_3->addWidget(tableView_2);

        tabWidget->addTab(tab, QString());

        gridLayout_2->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(SubForm);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SubForm);
    } // setupUi

    void retranslateUi(QWidget *SubForm)
    {
        SubForm->setWindowTitle(QApplication::translate("SubForm", "Form", nullptr));
        label->setText(QApplication::translate("SubForm", "\320\244\320\270\320\273\321\214\321\202\321\200 \320\277\320\276 \320\270\320\274\320\265\320\275\320\270:", nullptr));
        label_3->setText(QApplication::translate("SubForm", "\320\222\321\213\320\261\321\200\320\260\320\275\320\275\321\213\320\265 \321\202\320\265\320\263\320\270 :", nullptr));
        toolButton_SelectTag->setText(QApplication::translate("SubForm", ">>", nullptr));
        toolButton_UnSelectTag->setText(QApplication::translate("SubForm", "<<", nullptr));
        toolButton_UnSelectAll->setText(QApplication::translate("SubForm", "X", nullptr));
        toolButton_SelectAll->setText(QApplication::translate("SubForm", "All", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("SubForm", "\320\241\320\277\320\270\321\201\320\276\320\272 \321\202\320\265\320\263\320\276\320\262", nullptr));
        checkBox->setText(QApplication::translate("SubForm", "\320\236\320\261\321\211\320\260\321\217 \321\210\320\272\320\260\320\273\320\260", nullptr));
        toolButton_all->setText(QApplication::translate("SubForm", "All", nullptr));
        toolButton_2hour->setText(QApplication::translate("SubForm", "2", nullptr));
        toolButton_8hour->setText(QApplication::translate("SubForm", "8", nullptr));
        toolButton_20min->setText(QApplication::translate("SubForm", "20\320\274\320\270\320\275", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QApplication::translate("SubForm", "\320\222\321\213\320\261\321\200\320\260\320\275\320\275\321\213\320\265 \321\202\320\265\320\263\320\270", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("SubForm", "\320\241\320\276\320\261\321\213\321\202\320\270\321\217", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("SubForm", "\320\242\321\200\320\265\320\275\320\264\321\213", nullptr));
        label_2->setText(QApplication::translate("SubForm", "\320\235\320\260\321\207\320\260\320\273\321\214\320\275\320\260\321\217 \321\202\320\276\321\207\320\272\320\260:", nullptr));
        timeEdit->setDisplayFormat(QApplication::translate("SubForm", "H:mm:ss", nullptr));
        label_4->setText(QApplication::translate("SubForm", " \320\237\320\265\321\200\320\270\320\276\320\264:", nullptr));
        label_5->setText(QApplication::translate("SubForm", " \320\240\320\260\321\201\321\201\321\202\320\276\321\217\320\275\320\270\320\265 \320\274\320\265\320\266\320\264\321\203 \321\202\320\276\321\207\320\272\320\260\320\274\320\270:", nullptr));
        toolButton_Update->setText(QString());
        toolButton_SaveCSV->setText(QApplication::translate("SubForm", "CSV", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SubForm", "\320\236\321\202\321\207\320\265\321\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SubForm: public Ui_SubForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBFORM_H
