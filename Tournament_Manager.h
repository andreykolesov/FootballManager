#ifndef TOURNAMENT_MANAGER_H
#define TOURNAMENT_MANAGER_H

#include <QList>
#include <QMap>
#include "Team.h"

class TournamentManager {
public:
    TournamentManager();
    ~TournamentManager();

    QList<QList<Team*>> startTournament(const QList<Team*>& selectedTeams);
    QList<Team*> simulateGroupStage(const QList<Team*>& group);
    QList<Team*> simulateSemiFinal(const QList<Team*>& semiFinalTeams);
    Team* simulateFinal(Team* teamA, Team* teamB);

    Team* startWorldCupTournament(const QList<Team*>& selectedTeams, QMap<QString, QList<Team*>> &groupResults);
};

#endif
