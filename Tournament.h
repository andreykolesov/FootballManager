#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "Team.h"
#include "Match.h"
#include <QString>
#include <QList>

struct TournamentRecord {
    int played;
    int wins;
    int draws;
    int losses;
    int goalsFor;
    int goalsAgainst;
    int points;
};

class Tournament {
private:
    QString m_name;
    QList<Team*> m_teams;
public:
    Tournament(const QString &name);
    void addTeam(Team* team);
    Team* simulateKnockoutTournament();
    QList<Team*> getTeams() const;
    static QMap<Team*, TournamentRecord> simulateLeagueSeason(const QList<Team*>& teams);
};

#endif
