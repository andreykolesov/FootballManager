#ifndef MATCH_H
#define MATCH_H

#include "Team.h"
#include <QStringList>

class Match {
private:
    Team* m_homeTeam;
    Team* m_awayTeam;
    int m_homeScore;
    int m_awayScore;
    QStringList m_events;
public:
    Match(Team* homeTeam, Team* awayTeam);
    QString simulateStep(int currentMinute);

    Team* getHomeTeam() const;
    Team* getAwayTeam() const;
    int getHomeScore() const;
    int getAwayScore() const;
    QStringList getAllEvents() const;
};

#endif
