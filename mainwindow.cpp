#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Player.h"
#include "Match.h"
#include "Training.h"
#include "League_Manager.h"
#include "Tournament_Manager.h"
#include "Season_Manager.h"
#include "Player_Create.h"
#include "League_Manager.h"
#include "Sponsor.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QRandomGenerator>
#include <QTimer>
#include <functional>
#include <QtGlobal>
#include <QGroupBox>

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
    createWorldCupTab();

    connect(ui->btnAddNewPlayer, &QPushButton::clicked,
            this, &MainWindow::on_btnAddNewPlayer_clicked);

    connect(ui->sponsorButton, &QPushButton::clicked, this, &MainWindow::on_sponsorButton_clicked);

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
                Player* player = new Player(playerName);
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

void MainWindow::updateTeamPlayersList(Team *team) {
    ui->listWidgetTeamPlayers->clear();
    for (Player* p : team->getPlayers())
        ui->listWidgetTeamPlayers->addItem(p->getName());
}

void MainWindow::updateTeamDetails(Team *team) {
    if (ui->labelTeamDetails) {
        QString details = team->getTeamDetails();
        ui->labelTeamDetails->setText(details);
        ui->labelTeamDetails->setStyleSheet("background-color: #4E4E4E; color: #FFD700;");
        QTimer::singleShot(1500, [this]() {
            ui->labelTeamDetails->setStyleSheet("");
        });
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
        scheduleLabel->setText("Следующий матч:");
        vLayout->addWidget(scheduleLabel);

        QPushButton* matchButton = new QPushButton("Сыграть матч");
        vLayout->addWidget(matchButton);

        QPushButton* resultButton = new QPushButton("Результаты матча");
        vLayout->addWidget(resultButton);

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
        lUI->resultButton = resultButton;
        lUI->matchEvents = matchEvents;
        lUI->standingsTable = standingsTable;
        lUI->teams = leagueTeams;
        for (Team* t : leagueTeams)
            lUI->standings[t] = TournamentRecord{0, 0, 0, 0, 0, 0, 0};
        lUI->leagueMgr = new LeagueManager(leagueName);
        for (Team* t : leagueTeams)
            lUI->leagueMgr->addTeam(t);
        lUI->leagueMgr->generateSchedule();

        lUI->simTimer = nullptr;
        lUI->simTick = 0;
        lUI->currentMatch = nullptr;

        scheduleLabel->setText(lUI->leagueMgr->getCurrentScheduledMatchDescription());

        connect(matchButton, &QPushButton::clicked, this,
                [this, lUI, scheduleLabel, leagueName]() {
            QString schDesc = lUI->leagueMgr->getCurrentScheduledMatchDescription();
            if (schDesc.startsWith("Все матчи")) {
                Team* champion = lUI->leagueMgr->determineChampion();
                if (champion) {
                    champion->awardTrophy("Кубок Лиги");
                    QMessageBox::information(this, "Лига завершена",
                                             QString("Чемпион лиги %1: %2")
                                                 .arg(leagueName)
                                                 .arg(champion->getName()));
                }
                return;
            } else {
                lUI->matchButton->setEnabled(false);
                ScheduledMatch currSch = lUI->leagueMgr->getCurrentScheduledMatch();
                lUI->currentMatch = new Match(currSch.home, currSch.away);
                lUI->matchEvents->clear();

                lUI->simTimer = new QTimer(this);
                lUI->simTimer->setInterval(1000);
                lUI->simTick = 0;

                connect(lUI->simTimer, &QTimer::timeout, this, [this, lUI, scheduleLabel]() mutable {
                    int matchMinute = 0;
                    if (lUI->simTick < 30) {
                        matchMinute = qRound(lUI->simTick * 1.5);
                    } else {
                        matchMinute = 45 + qRound((lUI->simTick - 30) * 1.5);
                    }
                    if (lUI->simTick == 30) {
                        lUI->matchEvents->append("Перерыв! Начинается второй тайм.");
                    }
                    if (lUI->simTick >= 60) {
                        lUI->simTimer->stop();
                        lUI->matchButton->setEnabled(true);
                        lUI->matchEvents->append(QString("Матч завершён. Счёт: %1 : %2")
                                                 .arg(lUI->currentMatch->getHomeScore())
                                                 .arg(lUI->currentMatch->getAwayScore()));
                        lUI->leagueMgr->appendMatchResult(lUI->currentMatch);
                        lUI->leagueMgr->markMatchPlayed();
                        scheduleLabel->setText(lUI->leagueMgr->getCurrentScheduledMatchDescription());
                        updateLeagueStandingsGlobal();
                        lUI->simTimer->deleteLater();
                        lUI->simTimer = nullptr;
                        lUI->currentMatch = nullptr;
                        return;
                    } else {
                        QString event = lUI->currentMatch->simulateStep(lUI->simTick);
                        if (!event.isEmpty()) {
                            lUI->matchEvents->append(QString("[%1 мин] %2")
                                                     .arg(matchMinute)
                                                     .arg(event));
                        }
                    }
                    lUI->simTick++;
                });
                lUI->simTimer->start();
            }
        });

        connect(resultButton, &QPushButton::clicked, this,
                [this, lUI, scheduleLabel, leagueName]() {
            QString schDesc = lUI->leagueMgr->getCurrentScheduledMatchDescription();
            if (schDesc.startsWith("Все матчи")) {
                Team* champion = lUI->leagueMgr->determineChampion();
                if (champion) {
                    champion->awardTrophy("Кубок Лиги");
                    QMessageBox::information(this, "Лига завершена",
                                             QString("Чемпион лиги %1: %2")
                                                 .arg(leagueName)
                                                 .arg(champion->getName()));
                }
                return;
            }
            if (lUI->simTimer && lUI->simTimer->isActive()) {
                int startTick = lUI->simTick;
                for (int tick = startTick; tick < 60; tick++) {
                    int matchMinute = (tick < 30) ? qRound(tick * 1.5)
                                                  : 45 + qRound((tick - 30) * 1.5);
                    if (tick == 30) {
                        lUI->matchEvents->append("Перерыв! Начинается второй тайм.");
                    }
                    QString event = lUI->currentMatch->simulateStep(tick);
                    if (!event.isEmpty()) {
                        lUI->matchEvents->append(QString("[%1 мин] %2")
                                                 .arg(matchMinute)
                                                 .arg(event));
                    }
                }
                lUI->simTimer->stop();
                lUI->matchButton->setEnabled(true);
                lUI->matchEvents->append(QString("Матч завершён. Счёт: %1 : %2")
                                         .arg(lUI->currentMatch->getHomeScore())
                                         .arg(lUI->currentMatch->getAwayScore()));
                lUI->leagueMgr->appendMatchResult(lUI->currentMatch);
                lUI->leagueMgr->markMatchPlayed();
                scheduleLabel->setText(lUI->leagueMgr->getCurrentScheduledMatchDescription());
                updateLeagueStandingsGlobal();
                lUI->simTimer->deleteLater();
                lUI->simTimer = nullptr;
                lUI->simTick = 60;
                lUI->currentMatch = nullptr;
            }
            else {
                if (lUI->currentMatch == nullptr) {
                    ScheduledMatch currSch = lUI->leagueMgr->getCurrentScheduledMatch();
                    lUI->currentMatch = new Match(currSch.home, currSch.away);
                    lUI->matchEvents->clear();
                }
                for (int tick = 0; tick < 60; tick++) {
                    int matchMinute = (tick < 30) ? qRound(tick * 1.5)
                                                  : 45 + qRound((tick - 30) * 1.5);
                    if (tick == 30)
                        lUI->matchEvents->append("Перерыв! Начинается второй тайм.");
                    QString event = lUI->currentMatch->simulateStep(tick);
                    if (!event.isEmpty()) {
                        lUI->matchEvents->append(QString("[%1 мин] %2")
                                                 .arg(matchMinute)
                                                 .arg(event));
                    }
                }
                lUI->matchEvents->append(QString("Матч завершён. Счёт: %1 : %2")
                                         .arg(lUI->currentMatch->getHomeScore())
                                         .arg(lUI->currentMatch->getAwayScore()));
                lUI->leagueMgr->appendMatchResult(lUI->currentMatch);
                lUI->leagueMgr->markMatchPlayed();
                scheduleLabel->setText(lUI->leagueMgr->getCurrentScheduledMatchDescription());
                updateLeagueStandingsGlobal();
                lUI->currentMatch = nullptr;
            }
        });

        leagueUIs[leagueName] = lUI;
    }
    ui->verticalLayoutMatches->addWidget(dynamicLeagueTabWidget);
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
                QMessageBox::warning(this, "Ошибка",
                                     "Невозможно завершить сезон: не сыграны все матчи лиг!");
                return;
            }
        }
        if (!worldCupFinished) {
            QMessageBox::warning(this, "Ошибка",
                                 "Невозможно завершить сезон: Чемпионат Мира не завершён!");
            return;
        }
        transferWindowOpen = false;
        ui->listWidget->addItem("Сезон завершён, трансферное окно закрыто.");

        seasonManager->endSeason(teams);

        seasonManager->startNextSeason(0, [this]() {
            ui->btnNextSeason->setText("Открыть сезон");
            ui->listWidget->addItem("Новый сезон готов к открытию. Нажмите 'Открыть сезон'.");

            for (int i = ui->tabWidgetMain->count() - 1; i >= 0; i--) {
                if (ui->tabWidgetMain->tabText(i) == "Чемпионат Мира") {
                    QWidget *tab = ui->tabWidgetMain->widget(i);
                    ui->tabWidgetMain->removeTab(i);
                    tab->deleteLater();
                }
            }
            worldChampion = nullptr;
            quarterFinalWinners.clear();
            semifinalWinners.clear();
            qDeleteAll(worldCupGroupUIs);
            worldCupGroupUIs.clear();
            worldCupFinished = false;

            createWorldCupTab();
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
        for (int i = ui->tabWidgetMain->count() - 1; i >= 0; i--) {
            if (ui->tabWidgetMain->tabText(i) == "Чемпионат Мира") {
                QWidget* tab = ui->tabWidgetMain->widget(i);
                ui->tabWidgetMain->removeTab(i);
                tab->deleteLater();
            }
        }
        worldChampion = nullptr;
        quarterFinalWinners.clear();
        semifinalWinners.clear();
        qDeleteAll(worldCupGroupUIs);
        worldCupGroupUIs.clear();
        worldCupFinished = false;

        createWorldCupTab();

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

void MainWindow::on_btnConductTraining_clicked() {
    QListWidgetItem* item = ui->listWidgetTeamPlayers->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Ошибка", "Выберите игрока для тренировки!");
        return;
    }
    QString playerName = item->text();
    QString teamName = ui->teamComboBox->currentText();
    Team* team = teamMap.value(teamName);
    if (!team)
        return;
    Player* selectedPlayer = nullptr;
    for (Player* p : team->getPlayers()) {
        if (p->getName() == playerName) {
            selectedPlayer = p;
            break;
        }
    }
    if (selectedPlayer) {
        int oldForm = selectedPlayer->getCurrentForm();
        selectedPlayer->train();
        int newForm = selectedPlayer->getCurrentForm();

        QMessageBox::information(this, "Тренировка",
                                 QString("Новые показатели игрока %1:\nФорма: %2 -> %3")
                                     .arg(selectedPlayer->getName())
                                     .arg(oldForm)
                                     .arg(newForm));
        updateTeamPlayersList(team);
        updateTeamDetails(team);
    }
}

void MainWindow::createWorldCupTab() {
    QWidget *worldCupTab = new QWidget;
    QVBoxLayout *wcLayout = new QVBoxLayout(worldCupTab);

    QTabWidget *wcStagesTabWidget = new QTabWidget;
    wcLayout->addWidget(wcStagesTabWidget);

    QWidget *groupStageWidget = new QWidget;
    QVBoxLayout *groupStageLayout = new QVBoxLayout(groupStageWidget);

    QTabWidget *groupTabWidget = new QTabWidget;
    groupStageLayout->addWidget(groupTabWidget);

    QStringList groupNames = {"Группа A", "Группа B", "Группа C", "Группа D"};
    QMap<QString, QList<Team*>> groups;
    {
        QList<Team*> teamsCopy = teams;
        std::sort(teamsCopy.begin(), teamsCopy.end(), [](Team* a, Team* b) -> bool {
            int trophyCountA = a->getTrophies().split(", ", Qt::SkipEmptyParts).size();
            int trophyCountB = b->getTrophies().split(", ", Qt::SkipEmptyParts).size();
            if (trophyCountA != trophyCountB)
                return trophyCountA > trophyCountB;
            int perfA = a->getAttackRating() + a->getDefenseRating();
            int perfB = b->getAttackRating() + b->getDefenseRating();
            return perfA > perfB;
        });
        QList<Team*> selectedTeams = teamsCopy.mid(0, 12);
        for (int i = 0; i < selectedTeams.size(); i++) {
            QString grp = groupNames[i % groupNames.size()];
            groups[grp].append(selectedTeams[i]);
        }
    }

    for (const QString &grpName : groupNames) {
        QWidget *grpWidget = new QWidget;
        QVBoxLayout *grpLayout = new QVBoxLayout(grpWidget);

        QLabel *labSchedule = new QLabel("Следующий матч:");
        grpLayout->addWidget(labSchedule);

        QPushButton *btnPlayMatch = new QPushButton("Сыграть матч");
        grpLayout->addWidget(btnPlayMatch);
        QPushButton *btnMatchResults = new QPushButton("Результат матча");
        grpLayout->addWidget(btnMatchResults);

        QTextEdit *txtEvents = new QTextEdit;
        txtEvents->setReadOnly(true);
        grpLayout->addWidget(txtEvents);

        QTableWidget *tableStandings = new QTableWidget;
        tableStandings->setColumnCount(7);
        tableStandings->setHorizontalHeaderLabels(QStringList() << "Игр" << "Побед" << "Ничьих"
                                                                << "Поражений" << "Забито" << "Пропущено" << "Очки");
        grpLayout->addWidget(tableStandings);

        LeagueManager *grpMgr = new LeagueManager(grpName);
        for (Team* t : groups.value(grpName))
            grpMgr->addTeam(t);
        grpMgr->generateSchedule();

        WorldCupGroupUI *wcu = new WorldCupGroupUI;
        wcu->labelSchedule = labSchedule;
        wcu->matchButton = btnPlayMatch;
        wcu->resultButton = btnMatchResults;
        wcu->matchEvents = txtEvents;
        wcu->standingsTable = tableStandings;
        wcu->teams = groups.value(grpName);
        wcu->groupMgr = grpMgr;
        wcu->simTimer = nullptr;
        wcu->simTick = 0;
        wcu->currentMatch = nullptr;

        worldCupGroupUIs.insert(grpName, wcu);

        auto updateGroupStandingsTable = [this](WorldCupGroupUI* wcu) {
            QMap<Team*, TournamentRecord> newStandings = wcu->groupMgr->recalcStandings();

            QList<Team*> sortedTeams = wcu->teams;
            std::sort(sortedTeams.begin(), sortedTeams.end(), [newStandings](Team* a, Team* b) {
                return newStandings.value(a).points > newStandings.value(b).points;
            });

            wcu->standingsTable->clearContents();
            wcu->standingsTable->setRowCount(sortedTeams.size());

            QStringList verticalHeaders;
            int row = 0;
            const int qualifiedCount = 2;

            for (Team* t : sortedTeams) {
                TournamentRecord rec = newStandings.value(t);
                QString teamName = t->getName();
                if (row < qualifiedCount) {
                    teamName += " *";
                }
                verticalHeaders << teamName;

                QTableWidgetItem *itemPlayed  = new QTableWidgetItem(QString::number(rec.played));
                QTableWidgetItem *itemWins    = new QTableWidgetItem(QString::number(rec.wins));
                QTableWidgetItem *itemDraws   = new QTableWidgetItem(QString::number(rec.draws));
                QTableWidgetItem *itemLosses  = new QTableWidgetItem(QString::number(rec.losses));
                QTableWidgetItem *itemGF      = new QTableWidgetItem(QString::number(rec.goalsFor));
                QTableWidgetItem *itemGA      = new QTableWidgetItem(QString::number(rec.goalsAgainst));
                QTableWidgetItem *itemPoints  = new QTableWidgetItem(QString::number(rec.points));

                if (row < qualifiedCount) {
                    QBrush brush(Qt::green);
                    itemPlayed->setBackground(brush);
                    itemWins->setBackground(brush);
                    itemDraws->setBackground(brush);
                    itemLosses->setBackground(brush);
                    itemGF->setBackground(brush);
                    itemGA->setBackground(brush);
                    itemPoints->setBackground(brush);
                }

                wcu->standingsTable->setItem(row, 0, itemPlayed);
                wcu->standingsTable->setItem(row, 1, itemWins);
                wcu->standingsTable->setItem(row, 2, itemDraws);
                wcu->standingsTable->setItem(row, 3, itemLosses);
                wcu->standingsTable->setItem(row, 4, itemGF);
                wcu->standingsTable->setItem(row, 5, itemGA);
                wcu->standingsTable->setItem(row, 6, itemPoints);
                row++;
            }

            wcu->standingsTable->setVerticalHeaderLabels(verticalHeaders);
        };

        connect(btnPlayMatch, &QPushButton::clicked, this, [this, wcu, updateGroupStandingsTable]() {
            bool leaguesPlayed = true;
            for (auto lUI : leagueUIs) {
                if (!lUI->leagueMgr->allMatchesPlayed()) {
                    leaguesPlayed = false;
                    break;
                }
            }
            if (!leaguesPlayed) {
                QMessageBox::warning(this, "Ошибка", "Нельзя играть чемпионат мира, пока не сыграны все игры лиг!");
                return;
            }
            QString schDesc = wcu->groupMgr->getCurrentScheduledMatchDescription();
            if (schDesc.startsWith("Все матчи"))
                return;
            wcu->matchButton->setEnabled(false);
            ScheduledMatch currSch = wcu->groupMgr->getCurrentScheduledMatch();
            wcu->currentMatch = new Match(currSch.home, currSch.away);
            wcu->matchEvents->clear();
            wcu->simTimer = new QTimer(this);
            wcu->simTick = 0;
            connect(wcu->simTimer, &QTimer::timeout, this, [this, wcu, updateGroupStandingsTable]() {
                int matchMinute = (wcu->simTick < 30)
                ? qRound(wcu->simTick * 1.5)
                : 45 + qRound((wcu->simTick - 30) * 1.5);
                if (wcu->simTick == 30)
                    wcu->matchEvents->append("Перерыв! Второй тайм начался.");
                if (wcu->simTick >= 60) {
                    wcu->simTimer->stop();
                    disconnect(wcu->simTimer, nullptr, this, nullptr);
                    wcu->matchButton->setEnabled(true);
                    wcu->matchEvents->append(QString("Матч завершён. Счёт: %1 : %2")
                                                 .arg(wcu->currentMatch->getHomeScore())
                                                 .arg(wcu->currentMatch->getAwayScore()));
                    wcu->groupMgr->appendMatchResult(wcu->currentMatch);
                    wcu->groupMgr->markMatchPlayed();
                    wcu->labelSchedule->setText(wcu->groupMgr->getCurrentScheduledMatchDescription());
                    updateGroupStandingsTable(wcu);
                    wcu->simTimer->deleteLater();
                    wcu->simTimer = nullptr;
                    wcu->currentMatch = nullptr;
                    return;
                } else {
                    QString event = wcu->currentMatch->simulateStep(wcu->simTick);
                    if (!event.isEmpty())
                        wcu->matchEvents->append(QString("[%1 мин] %2").arg(matchMinute).arg(event));
                }
                wcu->simTick++;
            });
            wcu->simTimer->start(1000);
        });

        connect(btnMatchResults, &QPushButton::clicked, this, [this, wcu, updateGroupStandingsTable]() {
            bool leaguesPlayed = true;
            for (auto lUI : leagueUIs) {
                if (!lUI->leagueMgr->allMatchesPlayed()) {
                    leaguesPlayed = false;
                    break;
                }
            }
            if (!leaguesPlayed) {
                QMessageBox::warning(this, "Ошибка", "Нельзя играть чемпионат мира, пока не сыграны все игры лиг!");
                return;
            }
            QString schDesc = wcu->groupMgr->getCurrentScheduledMatchDescription();
            if (schDesc.startsWith("Все матчи")) {
                updateGroupStandingsTable(wcu);
                return;
            }
            if (wcu->simTimer && wcu->simTimer->isActive()) {
                for (int tick = wcu->simTick; tick < 60; tick++) {
                    int matchMinute = (tick < 30)
                    ? qRound(tick * 1.5)
                    : 45 + qRound((tick - 30) * 1.5);
                    if (tick == 30)
                        wcu->matchEvents->append("Перерыв! Второй тайм начался.");
                    QString event = wcu->currentMatch->simulateStep(tick);
                    if (!event.isEmpty())
                        wcu->matchEvents->append(QString("[%1 мин] %2").arg(matchMinute).arg(event));
                }
                wcu->simTimer->stop();
                disconnect(wcu->simTimer, nullptr, this, nullptr);
                wcu->matchButton->setEnabled(true);
                wcu->matchEvents->append(QString("Матч завершён. Счёт: %1 : %2")
                                             .arg(wcu->currentMatch->getHomeScore())
                                             .arg(wcu->currentMatch->getAwayScore()));
                wcu->groupMgr->appendMatchResult(wcu->currentMatch);
                wcu->groupMgr->markMatchPlayed();
                wcu->labelSchedule->setText(wcu->groupMgr->getCurrentScheduledMatchDescription());
                updateGroupStandingsTable(wcu);
                wcu->simTimer->deleteLater();
                wcu->simTimer = nullptr;
                wcu->simTick = 60;
                wcu->currentMatch = nullptr;
            }
            else {
                if (wcu->currentMatch == nullptr) {
                    ScheduledMatch currSch = wcu->groupMgr->getCurrentScheduledMatch();
                    wcu->currentMatch = new Match(currSch.home, currSch.away);
                    wcu->matchEvents->clear();
                }
                for (int tick = 0; tick < 60; tick++) {
                    int matchMinute = (tick < 30)
                    ? qRound(tick * 1.5)
                    : 45 + qRound((tick - 30) * 1.5);
                    if (tick == 30)
                        wcu->matchEvents->append("Перерыв! Второй тайм начался.");
                    QString event = wcu->currentMatch->simulateStep(tick);
                    if (!event.isEmpty())
                        wcu->matchEvents->append(QString("[%1 мин] %2").arg(matchMinute).arg(event));
                }
                wcu->matchEvents->append(QString("Матч завершён. Счёт: %1 : %2")
                                             .arg(wcu->currentMatch->getHomeScore())
                                             .arg(wcu->currentMatch->getAwayScore()));
                wcu->groupMgr->appendMatchResult(wcu->currentMatch);
                wcu->groupMgr->markMatchPlayed();
                wcu->labelSchedule->setText(wcu->groupMgr->getCurrentScheduledMatchDescription());
                updateGroupStandingsTable(wcu);
                wcu->currentMatch = nullptr;
            }
        });

        groupTabWidget->addTab(grpWidget, grpName);
    }
    wcStagesTabWidget->addTab(groupStageWidget, "Группы");

    static QList<Team*> quarterFinalWinners;
    static QList<Team*> semifinalWinners;
    static Team* worldChampion = nullptr;

    auto simulateKnockoutMatchInstant = [this](Team* teamA, Team* teamB, QTextEdit* eventsArea) -> Team* {
        Match* match = new Match(teamA, teamB);
        for (int tick = 0; tick < 60; tick++) {
            int matchMinute = (tick < 30)
            ? qRound(tick * 1.5)
            : 45 + qRound((tick - 30) * 1.5);
            if (tick == 30)
                eventsArea->append("Перерыв! Второй тайм начался.");
            QString event = match->simulateStep(tick);
            if (!event.isEmpty())
                eventsArea->append(QString("[%1 мин] %2").arg(matchMinute).arg(event));
        }
        eventsArea->append(QString("Матч завершён. Счёт: %1 : %2")
                               .arg(match->getHomeScore()).arg(match->getAwayScore()));
        Team* win = nullptr;
        if (match->getHomeScore() == match->getAwayScore()) {
            int r = QRandomGenerator::global()->bounded(2);
            win = (r == 0) ? teamA : teamB;
        } else {
            win = (match->getHomeScore() > match->getAwayScore()) ? teamA : teamB;
        }
        eventsArea->append(QString("Победитель: %1").arg(win->getName()));
        eventsArea->append("=======================================");
        delete match;
        return win;
    };

    {
        QWidget *quarterTab = new QWidget;
        QVBoxLayout *qLayout = new QVBoxLayout(quarterTab);
        QLabel *qLabel = new QLabel("Четвертьфинальные матчи");
        qLayout->addWidget(qLabel);

        QPushButton *qSimButton = new QPushButton("Сыграть матч");
        QPushButton *qInstantButton = new QPushButton("Результат матча");
        qLayout->addWidget(qSimButton);
        qLayout->addWidget(qInstantButton);

        QTextEdit *qEvents = new QTextEdit;
        qEvents->setReadOnly(true);
        qLayout->addWidget(qEvents);

        qSimButton->setEnabled(true);
        qInstantButton->setEnabled(true);

        connect(qInstantButton, &QPushButton::clicked, this,
                [this, qEvents, qSimButton, qInstantButton, simulateKnockoutMatchInstant]() {
                    qEvents->clear();
                    QList<Team*> qTeams;
                    for (const QString &grp : QStringList() << "Группа A" << "Группа B" << "Группа C" << "Группа D") {
                        if (!worldCupGroupUIs.contains(grp))
                            continue;
                        WorldCupGroupUI* wcu = worldCupGroupUIs.value(grp);
                        QMap<Team*, TournamentRecord> standings = wcu->groupMgr->recalcStandings();
                        QList<QPair<Team*, TournamentRecord>> ranking;
                        for (Team* t : wcu->teams)
                            ranking.append(qMakePair(t, standings.value(t)));
                        std::sort(ranking.begin(), ranking.end(), [](const QPair<Team*, TournamentRecord> &a,
                                                                     const QPair<Team*, TournamentRecord> &b) {
                            return a.second.points > b.second.points;
                        });
                        if (!ranking.isEmpty()){
                            qTeams.append(ranking.first().first);
                            if (ranking.size() > 1)
                                qTeams.append(ranking.at(1).first);
                        }
                    }
                    if(qTeams.size() < 8) {
                        QMessageBox::warning(this, "Ошибка", "Недостаточно команд для четвертьфинала!");
                        return;
                    }
                    quarterFinalWinners.clear();
                    for (int i = 0; i < qTeams.size(); i += 2) {
                        Team* teamA = qTeams.at(i);
                        Team* teamB = qTeams.at(i+1);
                        qEvents->append(QString("Матч: %1 vs %2").arg(teamA->getName()).arg(teamB->getName()));
                        Team* winner = simulateKnockoutMatchInstant(teamA, teamB, qEvents);
                        quarterFinalWinners.append(winner);
                    }
                    qEvents->setProperty("played", true);
                    qSimButton->setEnabled(false);
                    qInstantButton->setEnabled(false);
                });

        connect(qSimButton, &QPushButton::clicked, this,
                [this, qEvents, qSimButton, qInstantButton, simulateKnockoutMatchInstant]() {
                    qEvents->clear();
                    QList<Team*> qTeams;
                    for (const QString &grp : QStringList() << "Группа A" << "Группа B" << "Группа C" << "Группа D") {
                        if (!worldCupGroupUIs.contains(grp))
                            continue;
                        WorldCupGroupUI* wcu = worldCupGroupUIs.value(grp);
                        QMap<Team*, TournamentRecord> standings = wcu->groupMgr->recalcStandings();
                        QList<QPair<Team*, TournamentRecord>> ranking;
                        for (Team* t : wcu->teams)
                            ranking.append(qMakePair(t, standings.value(t)));
                        std::sort(ranking.begin(), ranking.end(), [](const QPair<Team*, TournamentRecord> &a,
                                                                     const QPair<Team*, TournamentRecord> &b) {
                            return a.second.points > b.second.points;
                        });
                        if (!ranking.isEmpty()){
                            qTeams.append(ranking.first().first);
                            if (ranking.size() > 1)
                                qTeams.append(ranking.at(1).first);
                        }
                    }
                    if(qTeams.size() < 8) {
                        QMessageBox::warning(this, "Ошибка", "Недостаточно команд для четвертьфинала!");
                        return;
                    }
                    QList<QPair<Team*, Team*>> matchPairs;
                    for (int i = 0; i < qTeams.size(); i += 2) {
                        matchPairs.append(qMakePair(qTeams.at(i), qTeams.at(i+1)));
                    }
                    quarterFinalWinners.clear();
                    auto *data = new KnockoutSimulationData;
                    data->matchPairs = matchPairs;
                    data->currentMatchIndex = 0;
                    data->currentTick = 0;
                    data->currentMatch = nullptr;
                    data->timer = new QTimer(this);
                    data->timer->setInterval(1000);
                    connect(data->timer, &QTimer::timeout, this,
                            [this, qEvents, data, qSimButton, qInstantButton, simulateKnockoutMatchInstant]() mutable {
                                if (data->currentMatch == nullptr) {
                                    if (data->currentMatchIndex >= data->matchPairs.size()) {
                                        data->timer->stop();
                                        data->timer->deleteLater();
                                        qEvents->append("Все матчи четвертьфинала сыграны.");
                                        qEvents->setProperty("played", true);
                                        qSimButton->setEnabled(false);
                                        qInstantButton->setEnabled(false);
                                        delete data;
                                        return;
                                    }
                                    QPair<Team*, Team*> pair = data->matchPairs.at(data->currentMatchIndex);
                                    qEvents->append(QString("Матч: %1 vs %2").arg(pair.first->getName()).arg(pair.second->getName()));
                                    data->currentMatch = new Match(pair.first, pair.second);
                                    data->currentTick = 0;
                                }
                                int matchMinute = (data->currentTick < 30)
                                                      ? qRound(data->currentTick * 1.5)
                                                      : 45 + qRound((data->currentTick - 30) * 1.5);
                                if (data->currentTick == 30)
                                    qEvents->append("Перерыв! Второй тайм начался.");
                                if (data->currentTick >= 60) {
                                    qEvents->append(QString("Матч завершён. Счёт: %1 : %2")
                                                        .arg(data->currentMatch->getHomeScore())
                                                        .arg(data->currentMatch->getAwayScore()));
                                    Team* winner = nullptr;
                                    if (data->currentMatch->getHomeScore() == data->currentMatch->getAwayScore()) {
                                        int r = QRandomGenerator::global()->bounded(2);
                                        winner = (r == 0) ? data->matchPairs.at(data->currentMatchIndex).first
                                                          : data->matchPairs.at(data->currentMatchIndex).second;
                                    } else {
                                        winner = (data->currentMatch->getHomeScore() > data->currentMatch->getAwayScore())
                                        ? data->matchPairs.at(data->currentMatchIndex).first
                                        : data->matchPairs.at(data->currentMatchIndex).second;
                                    }
                                    quarterFinalWinners.append(winner);
                                    delete data->currentMatch;
                                    data->currentMatch = nullptr;
                                    data->currentMatchIndex++;
                                    return;
                                } else {
                                    QString event = data->currentMatch->simulateStep(data->currentTick);
                                    if (!event.isEmpty())
                                        qEvents->append(QString("[%1 мин] %2").arg(matchMinute).arg(event));
                                }
                                data->currentTick++;
                            });
                    data->timer->start();
                });
        wcStagesTabWidget->addTab(quarterTab, "Четверть финал");
    }

    {
        QWidget *semiTab = new QWidget;
        QVBoxLayout *sLayout = new QVBoxLayout(semiTab);
        QLabel *sLabel = new QLabel("Полуфинальные матчи");
        sLayout->addWidget(sLabel);
        QPushButton *sSimButton = new QPushButton("Сыграть матч");
        QPushButton *sInstantButton = new QPushButton("Результат матча");
        sLayout->addWidget(sSimButton);
        sLayout->addWidget(sInstantButton);
        QTextEdit *sEvents = new QTextEdit;
        sEvents->setReadOnly(true);
        sLayout->addWidget(sEvents);
        sSimButton->setEnabled(true);
        sInstantButton->setEnabled(true);

        connect(sInstantButton, &QPushButton::clicked, this,
                [this, sEvents, sSimButton, sInstantButton, simulateKnockoutMatchInstant]() {
                    sEvents->clear();
                    if (quarterFinalWinners.size() != 4) {
                        QMessageBox::warning(this, "Ошибка", "Четвертьфинал ещё не завершён!");
                        return;
                    }
                    semifinalWinners.clear();
                    for (int i = 0; i < quarterFinalWinners.size(); i += 2) {
                        Team* teamA = quarterFinalWinners.at(i);
                        Team* teamB = quarterFinalWinners.at(i+1);
                        sEvents->append(QString("Матч: %1 vs %2").arg(teamA->getName()).arg(teamB->getName()));
                        Team* winner = simulateKnockoutMatchInstant(teamA, teamB, sEvents);
                        semifinalWinners.append(winner);
                    }
                    sEvents->setProperty("played", true);
                    sSimButton->setEnabled(false);
                    sInstantButton->setEnabled(false);
                });

        connect(sSimButton, &QPushButton::clicked, this,
                [this, sEvents, sSimButton, sInstantButton, simulateKnockoutMatchInstant]() {
                    sEvents->clear();
                    if (quarterFinalWinners.size() != 4) {
                        QMessageBox::warning(this, "Ошибка", "Четвертьфинал ещё не завершён!");
                        return;
                    }
                    QList<QPair<Team*, Team*>> matchPairs;
                    for (int i = 0; i < quarterFinalWinners.size(); i += 2) {
                        matchPairs.append(qMakePair(quarterFinalWinners.at(i), quarterFinalWinners.at(i+1)));
                    }
                    semifinalWinners.clear();
                    auto *data = new KnockoutSimulationData;
                    data->matchPairs = matchPairs;
                    data->currentMatchIndex = 0;
                    data->currentTick = 0;
                    data->currentMatch = nullptr;
                    data->timer = new QTimer(this);
                    data->timer->setInterval(1000);
                    connect(data->timer, &QTimer::timeout, this,
                            [this, sEvents, data, sSimButton, sInstantButton, simulateKnockoutMatchInstant]() mutable {
                                if (data->currentMatch == nullptr) {
                                    if (data->currentMatchIndex >= data->matchPairs.size()) {
                                        data->timer->stop();
                                        data->timer->deleteLater();
                                        sEvents->append("Все матчи полуфинала сыграны.");
                                        sEvents->setProperty("played", true);
                                        sSimButton->setEnabled(false);
                                        sInstantButton->setEnabled(false);
                                        delete data;
                                        return;
                                    }
                                    QPair<Team*, Team*> pair = data->matchPairs.at(data->currentMatchIndex);
                                    sEvents->append(QString("Матч: %1 vs %2").arg(pair.first->getName()).arg(pair.second->getName()));
                                    data->currentMatch = new Match(pair.first, pair.second);
                                    data->currentTick = 0;
                                }
                                int matchMinute = (data->currentTick < 30)
                                                      ? qRound(data->currentTick * 1.5)
                                                      : 45 + qRound((data->currentTick - 30) * 1.5);
                                if (data->currentTick == 30)
                                    sEvents->append("Перерыв! Второй тайм начался.");
                                if (data->currentTick >= 60) {
                                    sEvents->append(QString("Матч завершён. Счёт: %1 : %2")
                                                        .arg(data->currentMatch->getHomeScore())
                                                        .arg(data->currentMatch->getAwayScore()));
                                    Team* winner = nullptr;
                                    if (data->currentMatch->getHomeScore() == data->currentMatch->getAwayScore()) {
                                        int r = QRandomGenerator::global()->bounded(2);
                                        winner = (r == 0) ? data->matchPairs.at(data->currentMatchIndex).first
                                                          : data->matchPairs.at(data->currentMatchIndex).second;
                                    } else {
                                        winner = (data->currentMatch->getHomeScore() > data->currentMatch->getAwayScore())
                                        ? data->matchPairs.at(data->currentMatchIndex).first
                                        : data->matchPairs.at(data->currentMatchIndex).second;
                                    }
                                    semifinalWinners.append(winner);
                                    delete data->currentMatch;
                                    data->currentMatch = nullptr;
                                    data->currentMatchIndex++;
                                    return;
                                } else {
                                    QString event = data->currentMatch->simulateStep(data->currentTick);
                                    if (!event.isEmpty())
                                        sEvents->append(QString("[%1 мин] %2").arg(matchMinute).arg(event));
                                }
                                data->currentTick++;
                            });
                    data->timer->start();
                });
        wcStagesTabWidget->addTab(semiTab, "Полуфинал");
    }

    {
        QWidget *finalTab = new QWidget;
        QVBoxLayout *fLayout = new QVBoxLayout(finalTab);
        QLabel *fLabel = new QLabel("Финальный матч");
        fLayout->addWidget(fLabel);
        QPushButton *fSimButton = new QPushButton("Сыграть матч");
        QPushButton *fInstantButton = new QPushButton("Результат матча");
        fLayout->addWidget(fSimButton);
        fLayout->addWidget(fInstantButton);
        QTextEdit *fEvents = new QTextEdit;
        fEvents->setReadOnly(true);
        fLayout->addWidget(fEvents);
        fSimButton->setEnabled(true);
        fInstantButton->setEnabled(true);

        connect(fInstantButton, &QPushButton::clicked, this,
                [this, fEvents, fSimButton, fInstantButton, simulateKnockoutMatchInstant]() {
                    fEvents->clear();
                    if (semifinalWinners.size() != 2) {
                        QMessageBox::warning(this, "Ошибка", "Полуфинал ещё не завершён!");
                        return;
                    }
                    // Симуляция финального матча
                    Team* winner = simulateKnockoutMatchInstant(semifinalWinners.at(0), semifinalWinners.at(1), fEvents);
                    // Присваиваем победителя и устанавливаем флаг завершения чемпионата мира
                    worldChampion = winner;
                    worldCupFinished = true;
                    fEvents->setProperty("played", true);
                    fSimButton->setEnabled(false);
                    fInstantButton->setEnabled(false);
                    QMessageBox::information(this, "Чемпионат Мира",
                                             QString("Победитель чемпионата мира: %1").arg(winner->getName()));
                    winner->awardTrophy("Чемпионат мира");
                });

        connect(fSimButton, &QPushButton::clicked, this,
                [this, fEvents, fSimButton, fInstantButton, simulateKnockoutMatchInstant]() {
                    fEvents->clear();
                    if (semifinalWinners.size() != 2) {
                        QMessageBox::warning(this, "Ошибка", "Полуфинал ещё не завершён!");
                        return;
                    }
                    QList<QPair<Team*, Team*>> matchPairs;
                    matchPairs.append(qMakePair(semifinalWinners.at(0), semifinalWinners.at(1)));
                    auto *data = new KnockoutSimulationData;
                    data->matchPairs = matchPairs;
                    data->currentMatchIndex = 0;
                    data->currentTick = 0;
                    data->currentMatch = nullptr;
                    data->timer = new QTimer(this);
                    data->timer->setInterval(1000);
                    connect(data->timer, &QTimer::timeout, this,
                            [this, fEvents, data, fSimButton, fInstantButton, simulateKnockoutMatchInstant]() mutable {
                                if (data->currentMatch == nullptr) {
                                    if (data->currentMatchIndex >= data->matchPairs.size()) {
                                        data->timer->stop();
                                        data->timer->deleteLater();
                                        fEvents->append("Финальный матч сыгран.");
                                        fEvents->setProperty("played", true);
                                        fSimButton->setEnabled(false);
                                        fInstantButton->setEnabled(false);
                                        if (worldChampion == nullptr) {
                                            Team* finalWinner = simulateKnockoutMatchInstant(
                                                data->matchPairs.at(0).first, data->matchPairs.at(0).second, fEvents);
                                            worldChampion = finalWinner;
                                        }
                                        worldCupFinished = true;
                                        delete data;
                                        return;
                                    }
                                    QPair<Team*, Team*> pair = data->matchPairs.at(data->currentMatchIndex);
                                    fEvents->append(QString("Матч: %1 vs %2").arg(pair.first->getName()).arg(pair.second->getName()));
                                    data->currentMatch = new Match(pair.first, pair.second);
                                    data->currentTick = 0;
                                }
                                int matchMinute = (data->currentTick < 30)
                                                      ? qRound(data->currentTick * 1.5)
                                                      : 45 + qRound((data->currentTick - 30) * 1.5);
                                if (data->currentTick == 30)
                                    fEvents->append("Перерыв! Второй тайм начался.");
                                if (data->currentTick >= 60) {
                                    fEvents->append(QString("Матч завершён. Счёт: %1 : %2")
                                                        .arg(data->currentMatch->getHomeScore())
                                                        .arg(data->currentMatch->getAwayScore()));
                                    Team* winner = nullptr;
                                    if (data->currentMatch->getHomeScore() == data->currentMatch->getAwayScore()) {
                                        int r = QRandomGenerator::global()->bounded(2);
                                        winner = (r == 0) ? data->matchPairs.at(data->currentMatchIndex).first
                                                          : data->matchPairs.at(data->currentMatchIndex).second;
                                    } else {
                                        winner = (data->currentMatch->getHomeScore() > data->currentMatch->getAwayScore())
                                        ? data->matchPairs.at(data->currentMatchIndex).first
                                        : data->matchPairs.at(data->currentMatchIndex).second;
                                    }
                                    worldChampion = winner;
                                    fEvents->append(QString("Победитель: %1").arg(winner->getName()));
                                    fEvents->append("=======================================");
                                    delete data->currentMatch;
                                    data->currentMatch = nullptr;
                                    data->currentMatchIndex++;
                                    worldCupFinished = true;
                                    return;
                                } else {
                                    QString event = data->currentMatch->simulateStep(data->currentTick);
                                    if (!event.isEmpty())
                                        fEvents->append(QString("[%1 мин] %2").arg(matchMinute).arg(event));
                                }
                                data->currentTick++;
                            });
                    data->timer->start();
                });
        wcStagesTabWidget->addTab(finalTab, "Финал");
    }

    ui->tabWidgetMain->addTab(worldCupTab, "Чемпионат Мира");
}

void MainWindow::on_btnAddNewPlayer_clicked() {
    QString teamName = ui->teamComboBox->currentText();
    Team* team = teamMap.value(teamName);
    if (!team) {
        QMessageBox::warning(this, "Ошибка", "Не выбрана команда для добавления игрока!");
        return;
    }

    Player* newPlayer = PlayerCreate::createNewPlayer(this);
    if (newPlayer) {
        team->addPlayer(newPlayer);
        updateTeamPlayersList(team);
        updateTeamDetails(team);
    }
}

void MainWindow::on_sponsorButton_clicked() {
    Team* team = teamMap.value(ui->teamComboBox->currentText());
    if (!team) {
        QMessageBox::warning(this, "Ошибка", "Выберите команду для спонсорской поддержки!");
        return;
    }

    Sponsor* sponsor = Sponsor::getInstance();
    sponsor->sponsorTeam(team);
    QMessageBox::information(this, "Спонсорская поддержка",
                             QString("Команда \"%1\" получила спонсорский бонус от %2.\n"
                                     "Бонус: %3\nНовый бюджет команды: %4")
                                 .arg(team->getName())
                                 .arg(sponsor->getName())
                                 .arg(sponsor->getBonusBudget())
                                 .arg(team->getBudget()));

    updateTeamDetails(team);
}
