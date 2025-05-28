#include "Tournament_Manager.h"
#include "Tournament.h"
#include "Match.h"
#include <QRandomGenerator>
#include <algorithm>

TournamentManager::TournamentManager() {}
TournamentManager::~TournamentManager() {}

QList<QList<Team*>> TournamentManager::startTournament(const QList<Team*>& selectedTeams) {
    QList<Team*> teamsCopy = selectedTeams;
    std::random_shuffle(teamsCopy.begin(), teamsCopy.end());
    int groupSize = 4;
    QList<QList<Team*>> groups;
    for (int i = 0; i < teamsCopy.size(); i += groupSize) {
        QList<Team*> group;
        for (int j = i; j < i + groupSize && j < teamsCopy.size(); ++j)
            group.append(teamsCopy.at(j));
        groups.append(group);
    }
    return groups;
}

QList<Team*> TournamentManager::simulateGroupStage(const QList<Team*>& group) {
    QMap<Team*, TournamentRecord> standings = Tournament::simulateLeagueSeason(group);
    QList<QPair<Team*, TournamentRecord>> ranking;
    for (Team* t : group)
        ranking.append(qMakePair(t, standings[t]));
    std::sort(ranking.begin(), ranking.end(), [](const QPair<Team*, TournamentRecord>& a,
                                                 const QPair<Team*, TournamentRecord>& b) -> bool {
        return a.second.points > b.second.points;
    });
    QList<Team*> sortedTeams;
    for (const auto &pair : ranking)
        sortedTeams.append(pair.first);
    return sortedTeams;
}

QList<Team*> TournamentManager::simulateSemiFinal(const QList<Team*>& semiFinalTeams) {
    QList<Team*> winners;
    for (int i = 0; i < semiFinalTeams.size(); i += 2) {
        if (i + 1 < semiFinalTeams.size()) {
            Tournament temp("Полуфинал");
            temp.addTeam(semiFinalTeams.at(i));
            temp.addTeam(semiFinalTeams.at(i+1));
            Team* winner = temp.simulateKnockoutTournament();
            winners.append(winner);
        } else {
            winners.append(semiFinalTeams.at(i));
        }
    }
    return winners;
}

Team* TournamentManager::simulateFinal(Team* teamA, Team* teamB) {
    Tournament finalTournament("Финал");
    finalTournament.addTeam(teamA);
    finalTournament.addTeam(teamB);
    return finalTournament.simulateKnockoutTournament();
}

Team* TournamentManager::startWorldCupTournament(const QList<Team*>& selectedTeams, QMap<QString, QList<Team*>> &groupResults) {
    if (selectedTeams.size() < 12)
        return nullptr;
    QList<Team*> teamsCopy = selectedTeams;
    std::sort(teamsCopy.begin(), teamsCopy.end(), [](Team* a, Team* b) -> bool {
        int trophyCountA = a->getTrophies().split(", ", Qt::SkipEmptyParts).size();
        int trophyCountB = b->getTrophies().split(", ", Qt::SkipEmptyParts).size();
        if (trophyCountA != trophyCountB)
            return trophyCountA > trophyCountB;
        int perfA = a->getAttackRating() + a->getDefenseRating();
        int perfB = b->getAttackRating() + b->getDefenseRating();
        return perfA > perfB;
    });
    QList<Team*> selected = teamsCopy.mid(0, 12);
    QStringList groupNames = {"Группа A", "Группа B", "Группа C", "Группа D"};
    QMap<QString, QList<Team*>> groups;
    for (int i = 0; i < selected.size(); i++) {
        QString grp = groupNames[i % groupNames.size()];
        groups[grp].append(selected[i]);
    }
    groupResults = groups;

    return nullptr;
}
