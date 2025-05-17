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
    m_groups = groups;

    for (int i = 0; i < groups.size(); ++i) {
        QMap<Team*, TournamentRecord> standings = Tournament::simulateLeagueSeason(groups.at(i));
        m_groupStandings[i] = standings;
    }
    return groups;
}

QList<Team*> TournamentManager::simulateGroupStage(const QList<Team*>& group) {
    QMap<Team*, TournamentRecord> standings = Tournament::simulateLeagueSeason(group);
    int groupIndex = m_groups.indexOf(group);
    if (groupIndex != -1)
        m_groupStandings[groupIndex] = standings;

    QList<QPair<Team*, TournamentRecord>> teamRecords;
    for (Team* t : group)
        teamRecords.append(qMakePair(t, standings[t]));
    std::sort(teamRecords.begin(), teamRecords.end(), [](const QPair<Team*, TournamentRecord>& a,
                                                         const QPair<Team*, TournamentRecord>& b) -> bool {
        return a.second.points > b.second.points;
    });

    QList<Team*> sortedTeams;
    for (const auto &pair : teamRecords)
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
