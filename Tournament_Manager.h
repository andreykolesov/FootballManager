#ifndef TOURNAMENT_MANAGER_H
#define TOURNAMENT_MANAGER_H

#include <QList>
#include <QMap>
#include "Team.h"
#include "Tournament.h"

class TournamentManager {
private:
    QList<QList<Team*>> m_groups;
    QMap<int, QMap<Team*, TournamentRecord>> m_groupStandings;
public:
    TournamentManager();
    ~TournamentManager();

    QList<QList<Team*>> startTournament(const QList<Team*>& selectedTeams);
    QList<Team*> simulateGroupStage(const QList<Team*>& group);
    QList<Team*> simulateSemiFinal(const QList<Team*>& semiFinalTeams);
    Team* simulateFinal(Team* teamA, Team* teamB);
};

#endif
