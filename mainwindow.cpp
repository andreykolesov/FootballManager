#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Player.h"
#include "Match.h"
#include "Training.h"
#include "League_Manager.h"
#include "Tournament_Manager.h"
#include "Season_Manager.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QRandomGenerator>
#include <QTimer>
#include <functional>
#include <QtGlobal>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    dynamicLeagueTabWidget(new QTabWidget),
    tournamentManager(new TournamentManager),
    seasonManager(new SeasonManager(QStringList{"Лига1", "Лига2", "Лига3", "БольшойТурнир"}, this)),
    matchTimer(new QTimer(this)),
    transferWindowOpen(true)
{
    ui->setupUi(this);

    connect(ui->comboBoxSellTeam, &QComboBox::currentTextChanged, this, &MainWindow::on_comboBoxSellTeam_currentIndexChanged);
    connect(ui->btnNextSeason, &QPushButton::clicked, this, &MainWindow::on_btnNextSeason_clicked);
    connect(ui->btnTransferPlayer, &QPushButton::clicked, this, &MainWindow::on_btnTransferPlayer_clicked);

    connect(seasonManager, &SeasonManager::tournamentFinished,
            this, [&](const QString &tournamentName, Team* winner){
                ui->listWidget->addItem(
                    QString("%1 победила в %2").arg(winner->getName(), tournamentName)
                    );
            });

    connect(seasonManager, &SeasonManager::tournamentFinished,
            this, [&](const QString&, Team*){
                ui->btnNextSeason->setEnabled(seasonManager->allTournamentsFinished());
            });

    connect(seasonManager, &SeasonManager::seasonEnded,
            this, [&](){
                ui->listWidget->addItem("Сезон завершён.");
                ui->btnNextSeason->setEnabled(false);
                displaySeasonResults();
            });

    connect(seasonManager, &SeasonManager::seasonStarted,
            this, [&](){
                ui->listWidget->addItem("Новый сезон начался.");
                ui->btnNextSeason->setEnabled(true);
            });

    connect(ui->teamComboBox, &QComboBox::currentTextChanged,
            this, &MainWindow::onTeamSelectionChanged);

    connect(ui->listWidgetTeamPlayers, &QListWidget::itemDoubleClicked,
            this, &MainWindow::on_listWidgetTeamPlayers_itemDoubleClicked);
    loadInitialData();
    createLeagueTabs();
}

MainWindow::~MainWindow() {
    delete ui;
    qDeleteAll(leagueUIs);
    delete dynamicLeagueTabWidget;
    delete tournamentManager;
}

void MainWindow::loadInitialData() {
    teams.clear();
    teamMap.clear();
    for (int league = 1; league <= 3; ++league) {
        for (int teamIdx = 1; teamIdx <= 5; ++teamIdx) {
            QString teamName = QString("Лига%1_Команда%2").arg(league).arg(teamIdx);
            Team* team = new Team(teamName,
                                  QRandomGenerator::global()->bounded(60, 91),
                                  QRandomGenerator::global()->bounded(60, 91),
                                  QRandomGenerator::global()->bounded(400, 801));
            for (int playerIdx = 1; playerIdx <= 11; ++playerIdx) {
                QString playerName = QString("Player L%1T%2_%3")
                .arg(league).arg(teamIdx).arg(playerIdx);
                Player* player = Player::create(playerName);
                team->addPlayer(player);
            }
            teams.append(team);
            teamMap[team->getName()] = team;
        }
    }

    seasonManager->setAllTeams(teams);

    ui->comboBoxSellTeam->clear();
    ui->comboBoxBuyTeam->clear();
    ui->teamComboBox->clear();
    for (Team* t : teams) {
        ui->teamComboBox->addItem(t->getName());
        ui->comboBoxSellTeam->addItem(t->getName());
        ui->comboBoxBuyTeam->addItem(t->getName());
    }

    Team* defaultSellTeam = teamMap.value(ui->comboBoxSellTeam->currentText());
    if (defaultSellTeam) {
        ui->listWidgetTransferPlayers->clear();
        for (Player* p : defaultSellTeam->getPlayers())
            ui->listWidgetTransferPlayers->addItem(p->getName());
    }

    if (!teams.isEmpty()) {
        updateTeamDetails(teams.first());
        updateTeamPlayersList(teams.first());
    }
}

void MainWindow::displaySeasonResults() {
    QMap<Team*, TournamentRecord> seasonResults;
    for (auto leagueName : leagueUIs.keys()) {
        LeagueUI* lUI = leagueUIs.value(leagueName);
        QMap<Team*, TournamentRecord> leagueStandings = lUI->leagueMgr->recalcStandings();
        for (Team* t : leagueStandings.keys()) {
            seasonResults[t] = leagueStandings[t];
        }
    }
    QString summary;
    summary.append("=== Итоги сезона ===\n\n");
    for (Team* t : seasonResults.keys()) {
        TournamentRecord rec = seasonResults[t];
        summary.append(QString("Команда: %1\n  Игр: %2, Побед: %3, Ничьих: %4, Поражений: %5,\n  Забито: %6, Пропущено: %7, Очки: %8\n")
                           .arg(t->getName())
                           .arg(rec.played)
                           .arg(rec.wins)
                           .arg(rec.draws)
                           .arg(rec.losses)
                           .arg(rec.goalsFor)
                           .arg(rec.goalsAgainst)
                           .arg(rec.points));
        QString trophies = t->getTrophies();
        if (!trophies.isEmpty()) {
            summary.append("  Трофеи: " + trophies + "\n");
        }
        summary.append("\n");
    }
    QStringList lines = summary.split("\n", Qt::SkipEmptyParts);
    for (const QString &line : lines) {
        ui->listWidget->addItem(line);
    }
}

void MainWindow::createLeagueTabs() {
    dynamicLeagueTabWidget->setTabPosition(QTabWidget::North);
    leagueUIs.clear();

    QMap<QString, QList<Team*>> leagueGroups;
    for (Team* team : teams) {
        QString leagueKey = team->getName().section('_', 0, 0);
        leagueGroups[leagueKey].append(team);
    }

    for (auto it = leagueGroups.begin(); it != leagueGroups.end(); ++it) {
        QString leagueName = it.key();
        QList<Team*> leagueTeams = it.value();

        QWidget* leagueTab = new QWidget;
        QVBoxLayout* vLayout = new QVBoxLayout(leagueTab);

        QLabel* scheduleLabel = new QLabel;
        scheduleLabel->setText("Следующий матч: ");
        vLayout->addWidget(scheduleLabel);

        QPushButton* matchButton = new QPushButton("Сыграть матч");
        vLayout->addWidget(matchButton);

        QTextEdit* matchEvents = new QTextEdit;
        matchEvents->setReadOnly(true);
        vLayout->addWidget(matchEvents);

        QTableWidget* standingsTable = new QTableWidget;
        standingsTable->setColumnCount(7);
        standingsTable->setHorizontalHeaderLabels(QStringList() << "Игр" << "Побед" << "Ничьих"
                                                                << "Поражений" << "Забито" << "Пропущено" << "Очки");
        vLayout->addWidget(standingsTable);

        leagueTab->setLayout(vLayout);
        dynamicLeagueTabWidget->addTab(leagueTab, leagueName);

        LeagueUI* lUI = new LeagueUI;
        lUI->labelSchedule = scheduleLabel;
        lUI->matchButton = matchButton;
        lUI->matchEvents = matchEvents;
        lUI->standingsTable = standingsTable;
        lUI->teams = leagueTeams;
        for (Team* t : leagueTeams)
            lUI->standings[t] = TournamentRecord{0,0,0,0,0,0,0};
        lUI->leagueMgr = new LeagueManager(leagueName);
        for (Team* t : leagueTeams)
            lUI->leagueMgr->addTeam(t);
        lUI->leagueMgr->generateSchedule();

        scheduleLabel->setText(lUI->leagueMgr->getCurrentScheduledMatchDescription());

        connect(matchButton, &QPushButton::clicked, [this, lUI, scheduleLabel, leagueName]() {
            QString schDesc = lUI->leagueMgr->getCurrentScheduledMatchDescription();
            if (schDesc.startsWith("Все матчи")) {
                Team* champion = lUI->leagueMgr->determineChampion();
                if (champion) {
                    champion->awardTrophy("Кубок Лиги");
                    if (ui->teamComboBox->currentText() == champion->getName()) {
                        updateTeamDetails(champion);
                    }
                    QMessageBox::information(this, "Лига завершена",
                                             QString("Чемпион лиги %1: %2")
                                                 .arg(leagueName)
                                                 .arg(champion->getName()));
                }
            } else {
                QString events = lUI->leagueMgr->simulateNextMatch();
                lUI->matchEvents->append(events);
                scheduleLabel->setText(lUI->leagueMgr->getCurrentScheduledMatchDescription());
                QMap<Team*, TournamentRecord> newStandings = lUI->leagueMgr->recalcStandings();
                lUI->standings = newStandings;
                lUI->standingsTable->clearContents();
                lUI->standingsTable->setRowCount(lUI->teams.size());
                int row = 0;
                for (Team* t : lUI->teams) {
                    TournamentRecord rec = newStandings.value(t);
                    lUI->standingsTable->setItem(row, 0, new QTableWidgetItem(QString::number(rec.played)));
                    lUI->standingsTable->setItem(row, 1, new QTableWidgetItem(QString::number(rec.wins)));
                    lUI->standingsTable->setItem(row, 2, new QTableWidgetItem(QString::number(rec.draws)));
                    lUI->standingsTable->setItem(row, 3, new QTableWidgetItem(QString::number(rec.losses)));
                    lUI->standingsTable->setItem(row, 4, new QTableWidgetItem(QString::number(rec.goalsFor)));
                    lUI->standingsTable->setItem(row, 5, new QTableWidgetItem(QString::number(rec.goalsAgainst)));
                    lUI->standingsTable->setItem(row, 6, new QTableWidgetItem(QString::number(rec.points)));
                    row++;
                }
            }
        });

        leagueUIs[leagueName] = lUI;
    }
    ui->verticalLayoutMatches->addWidget(dynamicLeagueTabWidget);
}

void MainWindow::updateTeamPlayersList(Team *team) {
    ui->listWidgetTeamPlayers->clear();
    for (Player* p : team->getPlayers())
        ui->listWidgetTeamPlayers->addItem(p->getName());
}

void MainWindow::updateTeamDetails(Team *team) {
    if (ui->labelTeamDetails)
        ui->labelTeamDetails->setText(team->getTeamDetails());
}

QList<Player*> MainWindow::getPlayersFromTeam(Team *team) {
    return team->getPlayers();
}

void MainWindow::on_listWidgetTeamPlayers_itemDoubleClicked(QListWidgetItem* item) {
    if (!item) return;
    QString playerName = item->text();
    Team* team = teamMap.value(ui->teamComboBox->currentText());
    if (!team) return;

    Player* selectedPlayer = nullptr;
    for (Player* p : team->getPlayers()) {
        if (p->getName() == playerName) {
            selectedPlayer = p;
            break;
        }
    }
    if (!selectedPlayer) return;

    QDialog dlg(this);
    dlg.setWindowTitle("Статистика игрока и тренировка");
    QVBoxLayout* vbox = new QVBoxLayout(&dlg);

    QList<QLabel*> labels;
    QVariantMap stats = selectedPlayer->getStats();
    for (auto key : stats.keys()) {
        QLabel* lbl = new QLabel(QString("%1: %2").arg(key, stats[key].toString()), &dlg);
        vbox->addWidget(lbl);
        labels << lbl;
    }

    QPushButton* btnTrain = new QPushButton("Провести тренировку", &dlg);
    vbox->addWidget(btnTrain);

    connect(btnTrain, &QPushButton::clicked, [&]() {
        selectedPlayer->train();
        QVariantMap newStats = selectedPlayer->getStats();
        for (QLabel* lbl : labels) {
            QString key = lbl->text().section(':', 0, 0);
            lbl->setText(QString("%1: %2").arg(key, newStats[key].toString()));
        }
    });

    dlg.exec();
}

void MainWindow::on_btnTransferPlayer_clicked() {
    QString sellTeamName = ui->comboBoxSellTeam->currentText();
    QString buyTeamName = ui->comboBoxBuyTeam->currentText();

    Team* sellTeam = teamMap.value(sellTeamName);
    Team* buyTeam = teamMap.value(buyTeamName);

    if (!sellTeam || !buyTeam)
        return;

    for (auto lUI : leagueUIs) {
        if (lUI->leagueMgr->getTeams().contains(sellTeam) &&
            !lUI->leagueMgr->allMatchesPlayed()) {
            QMessageBox::warning(this, "Ошибка", "Невозможно проводить трансфер: продающая команда играет турнир!");
            return;
        }

        if (lUI->leagueMgr->getTeams().contains(buyTeam) &&
            !lUI->leagueMgr->allMatchesPlayed()) {
            QMessageBox::warning(this, "Ошибка", "Невозможно проводить трансфер: покупающая команда играет турнир!");
            return;
        }
    }

    if (sellTeamName == buyTeamName) {
        QMessageBox::warning(this, "Ошибка", "Выберите разные команды для трансфера!");
        return;
    }

    QListWidgetItem* item = ui->listWidgetTransferPlayers->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Ошибка", "Выберите игрока для трансфера!");
        return;
    }
    QString playerName = item->text();
    Player* transferPlayer = nullptr;
    for (Player* p : sellTeam->getPlayers()) {
        if (p->getName() == playerName) {
            transferPlayer = p;
            break;
        }
    }
    if (!transferPlayer)
        return;
    int price = transferPlayer->getPrice();
    if (buyTeam->getBudget() < price || QRandomGenerator::global()->bounded(100) > 65) {
        QMessageBox::warning(this, "Ошибка", "Трансфер не состоялся: либо недостаточно средств, либо игрок не хочет переходить.");
        return;
    }
    if (buyTeam->decreaseBudget(price)) {
        sellTeam->increaseBudget(price);
        sellTeam->removePlayer(transferPlayer);
        buyTeam->addPlayer(transferPlayer);
        ui->listWidgetTransferPlayers->clear();
        for (Player* p : sellTeam->getPlayers())
            ui->listWidgetTransferPlayers->addItem(p->getName());
        QMessageBox::information(this, "Трансфер",
                                 QString("Игрок \"%1\" переведен из \"%2\" в \"%3\".\nСтоимость трансфера: %4")
                                     .arg(playerName, sellTeamName, buyTeamName)
                                     .arg(price));
    }
}

void MainWindow::on_btnNextSeason_clicked() {
    if (ui->btnNextSeason->text() == "Завершить сезон") {
        for (auto lUI : leagueUIs) {
            if (lUI->leagueMgr->getCurrentScheduledMatchDescription() != "Все матчи сыграны.") {
                QMessageBox::warning(this, "Ошибка", "Невозможно завершить сезон: не сыграны все матчи лиг!");
                return;
            }
        }
        transferWindowOpen = false;
        ui->listWidget->addItem("Сезон завершён, трансферное окно закрыто.");

        seasonManager->endSeason(teams);

        seasonManager->startNextSeason(0, [this]() {
            ui->btnNextSeason->setText("Открыть сезон");
            ui->listWidget->addItem("Новый сезон готов к открытию. Нажмите 'Открыть сезон'.");
        });
    }
    else if (ui->btnNextSeason->text() == "Открыть сезон") {
        transferWindowOpen = true;

        ui->listWidget->clear();

        for (Team* team : teams) {
            team->resetTrophies();
        }

        for (auto lUI : leagueUIs) {
            lUI->leagueMgr->generateSchedule();
            lUI->labelSchedule->setText(lUI->leagueMgr->getCurrentScheduledMatchDescription());

            lUI->matchEvents->clear();

            QMap<Team*, TournamentRecord> newStandings = lUI->leagueMgr->recalcStandings();
            lUI->standings = newStandings;
            lUI->standingsTable->clearContents();
            lUI->standingsTable->setRowCount(lUI->teams.size());
            int row = 0;
            for (Team* t : lUI->teams) {
                TournamentRecord rec = newStandings.value(t);
                lUI->standingsTable->setItem(row, 0, new QTableWidgetItem(QString::number(rec.played)));
                lUI->standingsTable->setItem(row, 1, new QTableWidgetItem(QString::number(rec.wins)));
                lUI->standingsTable->setItem(row, 2, new QTableWidgetItem(QString::number(rec.draws)));
                lUI->standingsTable->setItem(row, 3, new QTableWidgetItem(QString::number(rec.losses)));
                lUI->standingsTable->setItem(row, 4, new QTableWidgetItem(QString::number(rec.goalsFor)));
                lUI->standingsTable->setItem(row, 5, new QTableWidgetItem(QString::number(rec.goalsAgainst)));
                lUI->standingsTable->setItem(row, 6, new QTableWidgetItem(QString::number(rec.points)));
                row++;
            }
        }

        if (ui->teamComboBox->currentIndex() >= 0) {
            Team* team = teamMap.value(ui->teamComboBox->currentText());
            if (team)
                updateTeamDetails(team);
        }

        ui->listWidget->addItem("Новый сезон открыт.");
        ui->btnNextSeason->setText("Завершить сезон");
    }
}


void MainWindow::updateLeagueStandingsGlobal() {
    for (auto leagueName : leagueUIs.keys()) {
        LeagueUI* lUI = leagueUIs.value(leagueName);
        QMap<Team*, TournamentRecord> newStandings = lUI->leagueMgr->recalcStandings();
        lUI->standings = newStandings;
        lUI->standingsTable->clearContents();
        lUI->standingsTable->setRowCount(lUI->teams.size());
        int row = 0;
        for (Team* t : lUI->teams) {
            TournamentRecord rec = newStandings.value(t);
            lUI->standingsTable->setItem(row, 0, new QTableWidgetItem(QString::number(rec.played)));
            lUI->standingsTable->setItem(row, 1, new QTableWidgetItem(QString::number(rec.wins)));
            lUI->standingsTable->setItem(row, 2, new QTableWidgetItem(QString::number(rec.draws)));
            lUI->standingsTable->setItem(row, 3, new QTableWidgetItem(QString::number(rec.losses)));
            lUI->standingsTable->setItem(row, 4, new QTableWidgetItem(QString::number(rec.goalsFor)));
            lUI->standingsTable->setItem(row, 5, new QTableWidgetItem(QString::number(rec.goalsAgainst)));
            lUI->standingsTable->setItem(row, 6, new QTableWidgetItem(QString::number(rec.points)));
            row++;
        }
    }
}

void MainWindow::onTeamSelectionChanged(const QString &teamName) {
    Team* team = teamMap.value(teamName);
    if (team) {
        updateTeamDetails(team);
        updateTeamPlayersList(team);
    }
}

void MainWindow::on_comboBoxSellTeam_currentIndexChanged(const QString &teamName) {
    Team* team = teamMap.value(teamName);
    ui->listWidgetTransferPlayers->clear();
    if (team) {
        for (Player* p : team->getPlayers()) {
            ui->listWidgetTransferPlayers->addItem(p->getName());
        }
    }
}
