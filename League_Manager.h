#ifndef LEAGUE_MANAGER_H
#define LEAGUE_MANAGER_H

#include <QMap>
#include <QList>
#include "Team.h"
#include "Match.h"
#include "Tournament.h"

struct ScheduledMatch {
    Team* home;
    Team* away;
    bool played;
};

class LeagueManager {
private:
    QString m_leagueName;
    QList<Team*> m_teams;
    QMap<Team*, TournamentRecord> m_standings;
    QList<Match*> m_matchHistory;
    QList<ScheduledMatch> m_schedule;
    int m_currentMatchIndex;
public:
    LeagueManager(const QString &leagueName);
    ~LeagueManager();

    void addTeam(Team* team);
    void generateSchedule();
    QString simulateNextMatch();
    bool allMatchesPlayed() const;
    QString getCurrentScheduledMatchDescription() const;
    ScheduledMatch getCurrentScheduledMatch() const;
    QMap<Team*, TournamentRecord> recalcStandings() const;
    Team* determineChampion() const;
    QList<Team*> getTeams() const;
};

#endif
