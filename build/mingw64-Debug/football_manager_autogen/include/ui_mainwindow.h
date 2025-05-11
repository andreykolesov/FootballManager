/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayoutCentral;
    QTabWidget *tabWidgetMain;
    QWidget *tabTeams;
    QVBoxLayout *verticalLayoutTeams;
    QComboBox *teamComboBox;
    QLabel *labelTeamDetails;
    QListWidget *listWidgetTeamPlayers;
    QWidget *tabMatches;
    QVBoxLayout *verticalLayoutMatches;
    QLabel *labelSchedule;
    QWidget *tabTransfers;
    QVBoxLayout *verticalLayoutTransfers;
    QHBoxLayout *horizontalLayoutTransferTeams;
    QComboBox *comboBoxSellTeam;
    QComboBox *comboBoxBuyTeam;
    QListWidget *listWidgetTransferPlayers;
    QPushButton *btnTransferPlayer;
    QWidget *tabSeason;
    QVBoxLayout *verticalLayoutSeason;
    QPushButton *btnNextSeason;
    QListWidget *listWidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutCentral = new QVBoxLayout(centralwidget);
        verticalLayoutCentral->setObjectName("verticalLayoutCentral");
        tabWidgetMain = new QTabWidget(centralwidget);
        tabWidgetMain->setObjectName("tabWidgetMain");
        tabWidgetMain->setTabPosition(QTabWidget::North);
        tabTeams = new QWidget();
        tabTeams->setObjectName("tabTeams");
        verticalLayoutTeams = new QVBoxLayout(tabTeams);
        verticalLayoutTeams->setObjectName("verticalLayoutTeams");
        teamComboBox = new QComboBox(tabTeams);
        teamComboBox->setObjectName("teamComboBox");

        verticalLayoutTeams->addWidget(teamComboBox);

        labelTeamDetails = new QLabel(tabTeams);
        labelTeamDetails->setObjectName("labelTeamDetails");

        verticalLayoutTeams->addWidget(labelTeamDetails);

        listWidgetTeamPlayers = new QListWidget(tabTeams);
        listWidgetTeamPlayers->setObjectName("listWidgetTeamPlayers");

        verticalLayoutTeams->addWidget(listWidgetTeamPlayers);

        tabWidgetMain->addTab(tabTeams, QString());
        tabMatches = new QWidget();
        tabMatches->setObjectName("tabMatches");
        verticalLayoutMatches = new QVBoxLayout(tabMatches);
        verticalLayoutMatches->setObjectName("verticalLayoutMatches");
        labelSchedule = new QLabel(tabMatches);
        labelSchedule->setObjectName("labelSchedule");

        verticalLayoutMatches->addWidget(labelSchedule);

        tabWidgetMain->addTab(tabMatches, QString());
        tabTransfers = new QWidget();
        tabTransfers->setObjectName("tabTransfers");
        verticalLayoutTransfers = new QVBoxLayout(tabTransfers);
        verticalLayoutTransfers->setObjectName("verticalLayoutTransfers");
        horizontalLayoutTransferTeams = new QHBoxLayout();
        horizontalLayoutTransferTeams->setObjectName("horizontalLayoutTransferTeams");
        comboBoxSellTeam = new QComboBox(tabTransfers);
        comboBoxSellTeam->setObjectName("comboBoxSellTeam");

        horizontalLayoutTransferTeams->addWidget(comboBoxSellTeam);

        comboBoxBuyTeam = new QComboBox(tabTransfers);
        comboBoxBuyTeam->setObjectName("comboBoxBuyTeam");

        horizontalLayoutTransferTeams->addWidget(comboBoxBuyTeam);


        verticalLayoutTransfers->addLayout(horizontalLayoutTransferTeams);

        listWidgetTransferPlayers = new QListWidget(tabTransfers);
        listWidgetTransferPlayers->setObjectName("listWidgetTransferPlayers");

        verticalLayoutTransfers->addWidget(listWidgetTransferPlayers);

        btnTransferPlayer = new QPushButton(tabTransfers);
        btnTransferPlayer->setObjectName("btnTransferPlayer");

        verticalLayoutTransfers->addWidget(btnTransferPlayer);

        tabWidgetMain->addTab(tabTransfers, QString());
        tabSeason = new QWidget();
        tabSeason->setObjectName("tabSeason");
        verticalLayoutSeason = new QVBoxLayout(tabSeason);
        verticalLayoutSeason->setObjectName("verticalLayoutSeason");
        btnNextSeason = new QPushButton(tabSeason);
        btnNextSeason->setObjectName("btnNextSeason");

        verticalLayoutSeason->addWidget(btnNextSeason);

        listWidget = new QListWidget(tabSeason);
        listWidget->setObjectName("listWidget");

        verticalLayoutSeason->addWidget(listWidget);

        tabWidgetMain->addTab(tabSeason, QString());

        verticalLayoutCentral->addWidget(tabWidgetMain);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidgetMain->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Football Manager Simulation", nullptr));
        labelTeamDetails->setText(QCoreApplication::translate("MainWindow", "\320\224\320\265\321\202\320\260\320\273\320\270 \320\272\320\276\320\274\320\260\320\275\320\264\321\213 \320\261\321\203\320\264\321\203\321\202 \320\276\321\202\320\276\320\261\321\200\320\260\320\266\320\265\320\275\321\213 \320\267\320\264\320\265\321\201\321\214", nullptr));
        tabWidgetMain->setTabText(tabWidgetMain->indexOf(tabTeams), QCoreApplication::translate("MainWindow", "\320\232\320\276\320\274\320\260\320\275\320\264\321\213", nullptr));
        labelSchedule->setText(QCoreApplication::translate("MainWindow", "\320\241\320\273\320\265\320\264\321\203\321\216\321\211\320\270\320\271 \320\274\320\260\321\202\321\207:", nullptr));
        tabWidgetMain->setTabText(tabWidgetMain->indexOf(tabMatches), QCoreApplication::translate("MainWindow", "\320\234\320\260\321\202\321\207\320\270", nullptr));
        comboBoxSellTeam->setCurrentText(QString());
        comboBoxBuyTeam->setCurrentText(QString());
        btnTransferPlayer->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\200\320\265\320\262\320\265\321\201\321\202\320\270 \320\270\320\263\321\200\320\276\320\272\320\260", nullptr));
        tabWidgetMain->setTabText(tabWidgetMain->indexOf(tabTransfers), QCoreApplication::translate("MainWindow", "\320\242\321\200\320\260\320\275\321\201\321\204\320\265\321\200\321\213", nullptr));
        btnNextSeason->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\262\320\265\321\200\321\210\320\270\321\202\321\214 \321\201\320\265\320\267\320\276\320\275", nullptr));
        tabWidgetMain->setTabText(tabWidgetMain->indexOf(tabSeason), QCoreApplication::translate("MainWindow", "\320\241\320\265\320\267\320\276\320\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
