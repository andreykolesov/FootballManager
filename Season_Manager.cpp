#include "Season_Manager.h"
#include <QTimer>
#include <QDateTime>
#include <QRandomGenerator>

SeasonManager::SeasonManager(const QStringList &tournaments, QObject *parent)
    : QObject(parent),
    m_currentSeason(1),
    m_seasonStartTime(QDateTime::currentDateTime()),
    m_tournaments(tournaments)
{
}

SeasonManager::~SeasonManager() {}

void SeasonManager::setAllTeams(const QList<Team*> &teams) {
    m_allTeams = teams;
}

QMap<Team*, QStringList> SeasonManager::getTeamTrophies() const {
    return m_teamTrophies;
}

void SeasonManager::recordTournamentResult(const QString &tournamentName, Team* winner) {
    if (!m_tournaments.contains(tournamentName) || m_finishedTournaments.contains(tournamentName))
        return;

    m_finishedTournaments.insert(tournamentName);

    winner->awardTrophy(tournamentName);
    m_teamTrophies[winner].append(tournamentName);

    emit tournamentFinished(tournamentName, winner);

    if (allTournamentsFinished()) {
        endSeason(m_allTeams);
    }
}

bool SeasonManager::allTournamentsFinished() const {
    return m_finishedTournaments.size() == m_tournaments.size();
}

void SeasonManager::endSeason(const QList<Team*>& teams) {
    for (Team* team : teams) {
        for (Player* p : team->getPlayers())
            p->age();
    }
    emit seasonEnded();
}

void SeasonManager::startNextSeason(int delayMilliseconds, std::function<void()> seasonStartedCallback) {
    QTimer::singleShot(delayMilliseconds, [this, seasonStartedCallback]() {
        m_currentSeason++;
        m_finishedTournaments.clear();
        m_teamTrophies.clear();
        m_seasonStartTime = QDateTime::currentDateTime();
        emit seasonStarted();
        seasonStartedCallback();
    });
}

int SeasonManager::currentSeason() const {
    return m_currentSeason;
}
