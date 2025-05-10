#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Player.h"
#include "Match.h"
#include "Training.h"
#include "Tournament.h"

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QRandomGenerator>
#include <algorithm>
#include <QtGlobal>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    matchTimer(new QTimer(this)),
    currentMinute(0),
    currentMatch(nullptr),
    transferWindowOpen(true),
    knockoutTournament(nullptr)
{
    ui->setupUi(this);
    connect(matchTimer, &QTimer::timeout, this, &MainWindow::simulateMatchStep);
    loadInitialData();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loadInitialData() {
    teams.clear();
    teamMap.clear();
    for (int league = 1; league <= 3; ++league) {
        for (int teamIdx = 1; teamIdx <= 5; ++teamIdx) {
            QString teamName = QString("Лига%1_Команда%2").arg(league).arg(teamIdx);
            Team* team = new Team(teamName,
                                  QRandomGenerator::global()->bounded(60, 90),
                                  QRandomGenerator::global()->bounded(70, 100),
                                  QRandomGenerator::global()->bounded(70, 100),
                                  QString("Тренер %1").arg(teamName),
                                  QString("Стадион %1").arg(teamName),
                                  QRandomGenerator::global()->bounded(400, 800));
            for (int playerIdx = 1; playerIdx <= 11; ++playerIdx) {
                QStringList positions = {"Вратарь", "Защитник", "Защитник", "Защитник", "Защитник",
                                         "Полузащитник", "Полузащитник", "Нападающий", "Нападающий", "Нападающий", "Полузащитник"};
                QString playerName = QString("Player L%1T%2_%3").arg(league).arg(teamIdx).arg(playerIdx);
                Player* player = new Player(playerName,
                                            positions[playerIdx - 1],
                                            QRandomGenerator::global()->bounded(60, 100),
                                            QRandomGenerator::global()->bounded(60, 100),
                                            QRandomGenerator::global()->bounded(60, 100),
                                            QRandomGenerator::global()->bounded(60, 100),
                                            QRandomGenerator::global()->bounded(60, 100),
                                            QRandomGenerator::global()->bounded(60, 100),
                                            QRandomGenerator::global()->bounded(60, 100),
                                            QRandomGenerator::global()->bounded(60, 100),
                                            QRandomGenerator::global()->bounded(100, 300));
                team->addPlayer(player);
            }
            teams.append(team);
            teamMap[team->getName()] = team;
        }
    }
    ui->comboBoxHomeTeam->clear();
    ui->comboBoxAwayTeam->clear();
    ui->teamComboBox->clear();
    ui->comboBoxSellTeam->clear();
    ui->comboBoxBuyTeam->clear();
    for (Team* t : teams) {
        ui->comboBoxHomeTeam->addItem(t->getName());
        ui->comboBoxAwayTeam->addItem(t->getName());
        ui->teamComboBox->addItem(t->getName());
        ui->comboBoxSellTeam->addItem(t->getName());
        ui->comboBoxBuyTeam->addItem(t->getName());
    }
    Team* defaultSellTeam = teamMap.value(ui->comboBoxSellTeam->currentText());
    if (defaultSellTeam) {
        ui->listWidgetTransferPlayers->clear();
        for (Player* p : defaultSellTeam->getPlayers())
            ui->listWidgetTransferPlayers->addItem(QString("%1 (%2)").arg(p->getName()).arg(p->getPosition()));
    }
    if (!teams.isEmpty()) {
        updateTeamDetails(teams.first());
        updateTeamPlayersList(teams.first());
    }
}

QList<Player*> MainWindow::getPlayersFromTeam(Team *team) {
    return team->getPlayers();
}

void MainWindow::updateTeamPlayersList(Team *team) {
    ui->listWidgetTeamPlayers->clear();
    for (Player* p : team->getPlayers())
        ui->listWidgetTeamPlayers->addItem(QString("%1 (%2)").arg(p->getName()).arg(p->getPosition()));
}

void MainWindow::updateTeamDetails(Team *team) {
    if (ui->labelTeamDetails)
        ui->labelTeamDetails->setText(team->getTeamDetails());
}

void MainWindow::simulateMatchStep() {
    currentMinute += 10;
    if (currentMinute > 90) {
        matchTimer->stop();
        ui->textEditMatchEvents->append(QString("Матч завершён. Итоговый счет: %1 - %2")
                                            .arg(currentMatch->getHomeScore())
                                            .arg(currentMatch->getAwayScore()));
        return;
    }
    QString eventMsg = currentMatch->simulateStep(currentMinute);
    if (!eventMsg.isEmpty())
        ui->textEditMatchEvents->append(eventMsg);
}

void MainWindow::on_btnStartMatch_clicked() {
    QString homeName = ui->comboBoxHomeTeam->currentText();
    QString awayName = ui->comboBoxAwayTeam->currentText();
    if (homeName == awayName) {
        QMessageBox::warning(this, "Ошибка", "Выберите разные команды для матча!");
        return;
    }
    Team* homeTeam = teamMap.value(homeName);
    Team* awayTeam = teamMap.value(awayName);
    if (!homeTeam || !awayTeam)
        return;
    if (currentMatch) {
        delete currentMatch;
        currentMatch = nullptr;
    }
    currentMatch = new Match(homeTeam, awayTeam);
    currentMinute = 0;
    ui->textEditMatchEvents->clear();
    matchTimer->start(1000);
}

void MainWindow::on_btnSimulateKnockout_clicked() {
    QList<Team*> selectedTeams;
    int count = qMin(8, teams.size());
    for (int i = 0; i < count; ++i)
        selectedTeams.append(teams.at(i));
    if (knockoutTournament) {
        delete knockoutTournament;
        knockoutTournament = nullptr;
    }
    knockoutTournament = new Tournament("Кубок Чемпионов");
    for (Team* t : selectedTeams)
        knockoutTournament->addTeam(t);
    Team* champ = knockoutTournament->simulateKnockoutTournament();
    QMessageBox::information(this, "Победитель турнира",
                             champ ? QString("Победитель: %1").arg(champ->getName())
                                   : "Нет победителя");
}

void MainWindow::on_teamComboBox_currentIndexChanged(int index) {
    QString teamName = ui->teamComboBox->itemText(index);
    Team* t = teamMap.value(teamName);
    if (t) {
        updateTeamDetails(t);
        updateTeamPlayersList(t);
    }
}

void MainWindow::on_listWidgetTeamPlayers_itemClicked(QListWidgetItem *item) {
    QString teamName = ui->teamComboBox->currentText();
    Team* team = teamMap.value(teamName);
    if (!team)
        return;
    QString displayText = item->text();
    QString playerName = displayText.section(" (", 0, 0);
    for (Player* p : team->getPlayers()) {
        if (p->getName() == playerName) {
            QMessageBox::information(this, "Характеристики игрока", p->getDetails());
            break;
        }
    }
}

void MainWindow::on_btnTransferPlayer_clicked() {
    if (!transferWindowOpen) {
        QMessageBox::warning(this, "Ошибка", "Трансферное окно закрыто на данный сезон!");
        return;
    }
    QString sellTeamName = ui->comboBoxSellTeam->currentText();
    QString buyTeamName = ui->comboBoxBuyTeam->currentText();
    if (sellTeamName == buyTeamName) {
        QMessageBox::warning(this, "Ошибка", "Выберите разные команды для трансфера!");
        return;
    }
    Team* sellTeam = teamMap.value(sellTeamName);
    Team* buyTeam = teamMap.value(buyTeamName);
    if (!sellTeam || !buyTeam)
        return;
    QListWidgetItem* item = ui->listWidgetTransferPlayers->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Ошибка", "Выберите игрока для трансфера!");
        return;
    }
    QString displayText = item->text();
    QString playerName = displayText.section(" (", 0, 0);
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
    if (buyTeam->getBudget() < price) {
        QMessageBox::warning(this, "Ошибка", "У покупателя недостаточно средств!");
        return;
    }
    if (buyTeam->decreaseBudget(price)) {
        sellTeam->increaseBudget(price);
        sellTeam->removePlayer(transferPlayer);
        buyTeam->addPlayer(transferPlayer);
        ui->listWidgetTransferPlayers->clear();
        for (Player* p : sellTeam->getPlayers())
            ui->listWidgetTransferPlayers->addItem(QString("%1 (%2)").arg(p->getName()).arg(p->getPosition()));
        QMessageBox::information(this, "Трансфер",
                                 QString("Игрок \"%1\" переведен из \"%2\" в \"%3\".\nСтоимость трансфера: %4")
                                     .arg(playerName, sellTeamName, buyTeamName)
                                     .arg(price));
    }
}

void MainWindow::on_btnConductTraining_clicked() {
    QListWidgetItem* item = ui->listWidgetTeamPlayers->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Ошибка", "Выберите игрока для тренировки!");
        return;
    }
    QString displayText = item->text();
    QString playerName = displayText.section(" (", 0, 0);
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
        TrainingSession::conductTraining(selectedPlayer);
        QMessageBox::information(this, "Тренировка",
                                 QString("Новые показатели игрока %1:\nФорма: %2")
                                     .arg(selectedPlayer->getName())
                                     .arg(selectedPlayer->getCurrentForm()));
        updateTeamPlayersList(team);
    }
}

void MainWindow::on_btnNextSeason_clicked() {
    transferWindowOpen = false;
    QMessageBox::information(this, "Сезон", "Сезон завершён, трансферное окно закрыто.");
    updateLeagueStandings();
    for (Team* team : teams) {
        for (Player* p : team->getPlayers()) {
            p->setSpeed(qMax(p->getSpeed() - 1, 50));
            p->setShotPrecision(qMax(p->getShotPrecision() - 1, 50));
            p->setEndurance(qMax(p->getEndurance() - 1, 50));
            p->setDribbling(qMax(p->getDribbling() - 1, 50));
            p->setPassPrecision(qMax(p->getPassPrecision() - 1, 50));
            p->setDefenseSkill(qMax(p->getDefenseSkill() - 1, 50));
            p->setPhysicalStrength(qMax(p->getPhysicalStrength() - 1, 50));
            p->setCurrentForm(qMax(p->getCurrentForm() - 2, 50));
        }
    }
    transferWindowOpen = true;
    QMessageBox::information(this, "Новый сезон", "Начало нового сезона. Трансферное окно открыто.");
    updateLeagueStandings();
}

void MainWindow::updateLeagueStandings() {
    QMap<QString, QList<Team*>> leagues;
    for (Team* team : teams) {
        QString leagueId = team->getName().section('_', 0, 0);
        leagues[leagueId].append(team);
    }
    QMap<Team*, TournamentRecord> leagueStandings;
    for (auto leagueId : leagues.keys()) {
        QList<Team*> leagueTeams = leagues.value(leagueId);
        for (Team* t : leagueTeams) {
            TournamentRecord rec = {0, 0, 0, 0, 0, 0, 0};
            leagueStandings[t] = rec;
        }
        for (int i = 0; i < leagueTeams.size(); ++i) {
            for (int j = i+1; j < leagueTeams.size(); ++j) {
                Team* teamA = leagueTeams.at(i);
                Team* teamB = leagueTeams.at(j);
                int teamARating = teamA->getAttack() + teamA->getAveragePlayerRating() + teamA->getMood();
                int teamBRating = teamB->getDefense() + teamB->getAveragePlayerRating() + teamB->getMood();
                int scoreA = QRandomGenerator::global()->bounded(0, teamARating / 20 + 2);
                int scoreB = QRandomGenerator::global()->bounded(0, teamBRating / 20 + 2);
                TournamentRecord recA = leagueStandings.value(teamA);
                TournamentRecord recB = leagueStandings.value(teamB);
                recA.played++;
                recB.played++;
                recA.goalsFor += scoreA;
                recA.goalsAgainst += scoreB;
                recB.goalsFor += scoreB;
                recB.goalsAgainst += scoreA;
                if (scoreA > scoreB) {
                    recA.wins++;
                    recB.losses++;
                    recA.points += 3;
                } else if (scoreA < scoreB) {
                    recB.wins++;
                    recA.losses++;
                    recB.points += 3;
                } else {
                    recA.draws++;
                    recB.draws++;
                    recA.points += 1;
                    recB.points += 1;
                }
                leagueStandings[teamA] = recA;
                leagueStandings[teamB] = recB;
            }
        }
    }
    ui->tableWidgetStandings->clear();
    ui->tableWidgetStandings->setColumnCount(10);
    ui->tableWidgetStandings->setHorizontalHeaderLabels(QStringList() << "Лига" << "Команда" << "Игр"
                                                                      << "Побед" << "Ничьих" << "Поражений"
                                                                      << "Забито" << "Пропущено" << "Очки"
                                                                      << "Бюджет");
    QList<Team*> sortedTeams = leagueStandings.keys();
    std::sort(sortedTeams.begin(), sortedTeams.end(), [leagueStandings](Team* a, Team* b) -> bool {
        QString leagueA = a->getName().section('_', 0, 0);
        QString leagueB = b->getName().section('_', 0, 0);
        if (leagueA == leagueB)
            return leagueStandings.value(a).points > leagueStandings.value(b).points;
        return leagueA < leagueB;
    });
    ui->tableWidgetStandings->setRowCount(sortedTeams.size());
    int row = 0;
    for (Team* team : sortedTeams) {
        QString leagueId = team->getName().section('_', 0, 0);
        TournamentRecord rec = leagueStandings.value(team);
        ui->tableWidgetStandings->setItem(row, 0, new QTableWidgetItem(leagueId));
        ui->tableWidgetStandings->setItem(row, 1, new QTableWidgetItem(team->getName()));
        ui->tableWidgetStandings->setItem(row, 2, new QTableWidgetItem(QString::number(rec.played)));
        ui->tableWidgetStandings->setItem(row, 3, new QTableWidgetItem(QString::number(rec.wins)));
        ui->tableWidgetStandings->setItem(row, 4, new QTableWidgetItem(QString::number(rec.draws)));
        ui->tableWidgetStandings->setItem(row, 5, new QTableWidgetItem(QString::number(rec.losses)));
        ui->tableWidgetStandings->setItem(row, 6, new QTableWidgetItem(QString::number(rec.goalsFor)));
        ui->tableWidgetStandings->setItem(row, 7, new QTableWidgetItem(QString::number(rec.goalsAgainst)));
        ui->tableWidgetStandings->setItem(row, 8, new QTableWidgetItem(QString::number(rec.points)));
        ui->tableWidgetStandings->setItem(row, 9, new QTableWidgetItem(QString::number(team->getBudget())));
        ++row;
    }
}
