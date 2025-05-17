#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QListWidgetItem>
#include <QTimer>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QTableWidget>
#include "Team.h"
#include "Tournament.h"
#include "League_Manager.h"
#include "Tournament_Manager.h"
#include "Season_Manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct LeagueUI {
    QLabel* labelSchedule;
    QPushButton* matchButton;
    QTextEdit* matchEvents;
    QTableWidget* standingsTable;
    QMap<Team*, TournamentRecord> standings;
    QList<Team*> teams;
    LeagueManager* leagueMgr;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    QList<Team*> teams;
    QMap<QString, Team*> teamMap;
    QTabWidget* dynamicLeagueTabWidget;
    QMap<QString, LeagueUI*> leagueUIs;
    TournamentManager* tournamentManager;
    SeasonManager* seasonManager;
    QTimer* matchTimer;
    bool transferWindowOpen;

    void loadInitialData();
    void createLeagueTabs();
    void updateTeamPlayersList(Team* team);
    void updateTeamDetails(Team* team);
    QList<Player*> getPlayersFromTeam(Team* team);
    void on_comboBoxSellTeam_currentIndexChanged(const QString &teamName);
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void on_btnTransferPlayer_clicked();
    void on_btnConductTraining_clicked();
    void on_btnNextSeason_clicked();
    void updateLeagueStandingsGlobal();
    void on_listWidgetTeamPlayers_itemDoubleClicked(QListWidgetItem* item);
    void onTeamSelectionChanged(const QString &teamName);
    void displaySeasonResults();
};

#endif
