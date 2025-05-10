#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QListWidgetItem>
#include <QTimer>
#include "Team.h"
#include "Tournament.h"

class Ui_MainWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    void on_btnStartMatch_clicked();
    void simulateMatchStep();
    void on_btnSimulateKnockout_clicked();
    void on_teamComboBox_currentIndexChanged(int index);
    void on_listWidgetTeamPlayers_itemClicked(QListWidgetItem* item);
    void on_btnTransferPlayer_clicked();
    void on_btnConductTraining_clicked();
    void on_btnNextSeason_clicked();
    void updateLeagueStandings();

    Ui_MainWindow *ui;
    QList<Team*> teams;
    QMap<QString, Team*> teamMap;
    Tournament* knockoutTournament;
    QTimer* matchTimer;
    int currentMinute;
    class Match* currentMatch;
    bool transferWindowOpen;

    void loadInitialData();
    void updateTeamPlayersList(Team* team);
    void updateTeamDetails(Team* team);
    QList<Player*> getPlayersFromTeam(Team* team);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif
