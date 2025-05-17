#ifndef SEASON_MANAGER_H
#define SEASON_MANAGER_H

#include <QObject>
#include <functional>
#include <QList>
#include <QDateTime>
#include <QSet>
#include <QMap>
#include <QStringList>
#include "Team.h"

class SeasonManager : public QObject {
    Q_OBJECT
private:
    int m_currentSeason;
    QDateTime m_seasonStartTime;
    QStringList m_tournaments;
    QSet<QString> m_finishedTournaments;
    QMap<Team*, QStringList> m_teamTrophies;
    QList<Team*> m_allTeams;
public:
    explicit SeasonManager(const QStringList &tournaments, QObject *parent = nullptr);
    ~SeasonManager();

    void setAllTeams(const QList<Team*>& teams);

    QMap<Team*, QStringList> getTeamTrophies() const;

    void recordTournamentResult(const QString &tournamentName, Team* winner);
    bool allTournamentsFinished() const;
    void endSeason(const QList<Team*>& teams);
    void startNextSeason(int delayMilliseconds, std::function<void()> seasonStartedCallback);

    int currentSeason() const;

signals:
    void tournamentFinished(const QString &tournamentName, Team* winner);
    void seasonEnded();
    void seasonStarted();
};

#endif
