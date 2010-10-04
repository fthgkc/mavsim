/********************************************************************************
** Form generated from reading ui file 'MainWindow.ui'
**
** Created: Mon Oct 4 19:20:07 2010
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include "QOSGAdapterWidget.hpp"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_Model;
    QAction *actionLoad_Map;
    QAction *actionGhgh;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGroupBox *vehicleData;
    QGridLayout *gridLayout_2;
    QGroupBox *state;
    QGridLayout *gridLayout_4;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QGroupBox *autoPilot;
    QGridLayout *gridLayout_5;
    QLabel *label_9;
    QLabel *label_10;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    ViewerQT *map;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(1020, 693);
    actionLoad_Model = new QAction(MainWindow);
    actionLoad_Model->setObjectName(QString::fromUtf8("actionLoad_Model"));
    actionLoad_Map = new QAction(MainWindow);
    actionLoad_Map->setObjectName(QString::fromUtf8("actionLoad_Map"));
    actionGhgh = new QAction(MainWindow);
    actionGhgh->setObjectName(QString::fromUtf8("actionGhgh"));
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    vehicleData = new QGroupBox(centralwidget);
    vehicleData->setObjectName(QString::fromUtf8("vehicleData"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(vehicleData->sizePolicy().hasHeightForWidth());
    vehicleData->setSizePolicy(sizePolicy);
    gridLayout_2 = new QGridLayout(vehicleData);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    state = new QGroupBox(vehicleData);
    state->setObjectName(QString::fromUtf8("state"));
    QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(state->sizePolicy().hasHeightForWidth());
    state->setSizePolicy(sizePolicy1);
    gridLayout_4 = new QGridLayout(state);
    gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
    label = new QLabel(state);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout_4->addWidget(label, 0, 0, 1, 1);

    label_2 = new QLabel(state);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout_4->addWidget(label_2, 1, 0, 1, 1);

    label_3 = new QLabel(state);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setMinimumSize(QSize(0, 0));

    gridLayout_4->addWidget(label_3, 0, 1, 1, 1);

    label_4 = new QLabel(state);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setMinimumSize(QSize(0, 0));

    gridLayout_4->addWidget(label_4, 1, 1, 1, 1);

    label_5 = new QLabel(state);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    gridLayout_4->addWidget(label_5, 2, 0, 1, 1);

    label_6 = new QLabel(state);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    gridLayout_4->addWidget(label_6, 2, 1, 1, 1);

    label_7 = new QLabel(state);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout_4->addWidget(label_7, 3, 0, 1, 1);

    label_8 = new QLabel(state);
    label_8->setObjectName(QString::fromUtf8("label_8"));

    gridLayout_4->addWidget(label_8, 3, 1, 1, 1);

    label_11 = new QLabel(state);
    label_11->setObjectName(QString::fromUtf8("label_11"));

    gridLayout_4->addWidget(label_11, 4, 0, 1, 1);

    label_12 = new QLabel(state);
    label_12->setObjectName(QString::fromUtf8("label_12"));

    gridLayout_4->addWidget(label_12, 4, 1, 1, 1);

    label_13 = new QLabel(state);
    label_13->setObjectName(QString::fromUtf8("label_13"));

    gridLayout_4->addWidget(label_13, 5, 0, 1, 1);

    label_14 = new QLabel(state);
    label_14->setObjectName(QString::fromUtf8("label_14"));

    gridLayout_4->addWidget(label_14, 5, 1, 1, 1);

    label_15 = new QLabel(state);
    label_15->setObjectName(QString::fromUtf8("label_15"));

    gridLayout_4->addWidget(label_15, 6, 0, 1, 1);

    label_16 = new QLabel(state);
    label_16->setObjectName(QString::fromUtf8("label_16"));

    gridLayout_4->addWidget(label_16, 6, 1, 1, 1);


    gridLayout_2->addWidget(state, 0, 0, 1, 1);

    autoPilot = new QGroupBox(vehicleData);
    autoPilot->setObjectName(QString::fromUtf8("autoPilot"));
    sizePolicy1.setHeightForWidth(autoPilot->sizePolicy().hasHeightForWidth());
    autoPilot->setSizePolicy(sizePolicy1);
    gridLayout_5 = new QGridLayout(autoPilot);
    gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
    label_9 = new QLabel(autoPilot);
    label_9->setObjectName(QString::fromUtf8("label_9"));

    gridLayout_5->addWidget(label_9, 0, 0, 1, 1);

    label_10 = new QLabel(autoPilot);
    label_10->setObjectName(QString::fromUtf8("label_10"));

    gridLayout_5->addWidget(label_10, 0, 1, 1, 1);


    gridLayout_2->addWidget(autoPilot, 0, 1, 1, 1);

    groupBox = new QGroupBox(vehicleData);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy);
    gridLayout_3 = new QGridLayout(groupBox);
    gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
    lineEdit = new QLineEdit(groupBox);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

    gridLayout_3->addWidget(lineEdit, 2, 0, 1, 1);

    pushButton = new QPushButton(groupBox);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));

    gridLayout_3->addWidget(pushButton, 2, 1, 1, 1);

    scrollArea = new QScrollArea(groupBox);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 731, 192));
    scrollArea->setWidget(scrollAreaWidgetContents);

    gridLayout_3->addWidget(scrollArea, 1, 0, 1, 2);


    gridLayout_2->addWidget(groupBox, 0, 2, 1, 1);


    gridLayout->addWidget(vehicleData, 1, 0, 1, 1);

    map = new ViewerQT(centralwidget);
    map->setObjectName(QString::fromUtf8("map"));
    sizePolicy.setHeightForWidth(map->sizePolicy().hasHeightForWidth());
    map->setSizePolicy(sizePolicy);

    gridLayout->addWidget(map, 0, 0, 1, 1);

    MainWindow->setCentralWidget(centralwidget);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 1020, 26));
    menuFile = new QMenu(menubar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    MainWindow->setMenuBar(menubar);

    menubar->addAction(menuFile->menuAction());
    menuFile->addAction(actionLoad_Model);
    menuFile->addAction(actionLoad_Map);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
    actionLoad_Model->setText(QApplication::translate("MainWindow", "Load Vehicle", 0, QApplication::UnicodeUTF8));
    actionLoad_Map->setText(QApplication::translate("MainWindow", "Load Map", 0, QApplication::UnicodeUTF8));
    actionGhgh->setText(QApplication::translate("MainWindow", "ghgh", 0, QApplication::UnicodeUTF8));
    vehicleData->setTitle(QApplication::translate("MainWindow", "Vehicle Data", 0, QApplication::UnicodeUTF8));
    state->setTitle(QApplication::translate("MainWindow", "State", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("MainWindow", "lat", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("MainWindow", "lon", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("MainWindow", "alt", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("MainWindow", "ground speed", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("MainWindow", "roll", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("MainWindow", "pitch", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
    label_15->setText(QApplication::translate("MainWindow", "yaw", 0, QApplication::UnicodeUTF8));
    label_16->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
    autoPilot->setTitle(QApplication::translate("MainWindow", "AutoPilot", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("MainWindow", "mode", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("MainWindow", "manual", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("MainWindow", "Terminal", 0, QApplication::UnicodeUTF8));
    pushButton->setText(QApplication::translate("MainWindow", "Send", 0, QApplication::UnicodeUTF8));
    menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
