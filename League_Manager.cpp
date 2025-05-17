#include "League_Manager.h"
#include "Tournament.h"
#include "Match.h"
#include <QRandomGenerator>
#include <algorithm>
#include <QtGlobal>

LeagueManager::LeagueManager(const QString &leagueName)
    : m_leagueName(leagueName), m_currentMatchIndex(0)
{
}

LeagueManager::~LeagueManager() {
    qDeleteAll(m_matchHistory);
}

void LeagueManager::addTeam(Team* team) {
    m_teams.append(team);
    m_standings[team] = TournamentRecord{0,0,0,0,0,0,0};
}

QList<Team*> LeagueManager::getTeams() const {
    return m_teams;
}

void LeagueManager::generateSchedule() {
    m_schedule.clear();
    m_currentMatchIndex = 0;
    qDeleteAll(m_matchHistory);
    m_matchHistory.clear();

    for (int i = 0; i < m_teams.size(); ++i) {
        for (int j = i+1; j < m_teams.size(); ++j) {
            ScheduledMatch sch1 { m_teams[i], m_teams[j], false };
            ScheduledMatch sch2 { m_teams[j], m_teams[i], false };
            m_schedule.append(sch1);
            m_schedule.append(sch2);
        }
    }
    std::shuffle(m_schedule.begin(), m_schedule.end(), *QRandomGenerator::global());
}

QString LeagueManager::simulateNextMatch() {
    if(m_currentMatchIndex >= m_schedule.size())
        return "Все матчи сыграны.";

    ScheduledMatch &sch = m_schedule[m_currentMatchIndex];
    Match* match = Match::create(sch.home, sch.away);
    QString events;
    int minutes = 0;
    while(minutes < 90) {
        QString ev = match->simulateStep(minutes);
        if(!ev.isEmpty())
            events.append(ev + "\n");
        minutes += 10;
    }
    m_matchHistory.append(match);

    TournamentRecord recHome = m_standings.value(sch.home);
    TournamentRecord recAway = m_standings.value(sch.away);
    recHome.played++;
    recAway.played++;
    int scoreHome = match->getHomeScore();
    int scoreAway = match->getAwayScore();
    recHome.goalsFor += scoreHome;
    recHome.goalsAgainst += scoreAway;
    recAway.goalsFor += scoreAway;
    recAway.goalsAgainst += scoreHome;
    if(scoreHome > scoreAway) {
        recHome.wins++;
        recHome.points += 3;
        recAway.losses++;
    } else if(scoreHome < scoreAway) {
        recAway.wins++;
        recAway.points += 3;
        recHome.losses++;
    } else {
        recHome.draws++;
        recAway.draws++;
        recHome.points += 1;
        recAway.points += 1;
    }
    m_standings[sch.home] = recHome;
    m_standings[sch.away] = recAway;

    sch.played = true;
    m_currentMatchIndex++;

    return events;
}

QString LeagueManager::getCurrentScheduledMatchDescription() const {
    if(m_currentMatchIndex >= m_schedule.size())
        return "Все матчи сыграны.";
    const ScheduledMatch &sch = m_schedule[m_currentMatchIndex];
    return QString("Следующий матч: %1 (дом) vs %2 (гость)")
        .arg(sch.home->getName()).arg(sch.away->getName());
}

ScheduledMatch LeagueManager::getCurrentScheduledMatch() const {
    if(m_currentMatchIndex < m_schedule.size())
        return m_schedule[m_currentMatchIndex];
    else {
        ScheduledMatch dummy { nullptr, nullptr, true };
        return dummy;
    }
}

QMap<Team*, TournamentRecord> LeagueManager::recalcStandings() const {
    QMap<Team*, TournamentRecord> newStandings;
    for(Team* t : m_teams)
        newStandings[t] = TournamentRecord{0,0,0,0,0,0,0};
    for(Match* match : m_matchHistory) {
        Team* homeTeam = match->getHomeTeam();
        Team* awayTeam = match->getAwayTeam();
        int scoreHome = match->getHomeScore();
        int scoreAway = match->getAwayScore();
        TournamentRecord recHome = newStandings.value(homeTeam);
        TournamentRecord recAway = newStandings.value(awayTeam);
        recHome.played++;
        recAway.played++;
        recHome.goalsFor += scoreHome;
        recHome.goalsAgainst += scoreAway;
        recAway.goalsFor += scoreAway;
        recAway.goalsAgainst += scoreHome;
        if(scoreHome > scoreAway) {
            recHome.wins++;
            recHome.points += 3;
            recAway.losses++;
        } else if(scoreHome < scoreAway) {
            recAway.wins++;
            recAway.points += 3;
            recHome.losses++;
        } else {
            recHome.draws++;
            recAway.draws++;
            recHome.points += 1;
            recAway.points += 1;
        }
        newStandings[homeTeam] = recHome;
        newStandings[awayTeam] = recAway;
    }
    return newStandings;
}

Team* LeagueManager::determineChampion() const {
    Team* champion = nullptr;
    int bestPoints = -1;
    for(Team* t : m_teams) {
        int pts = m_standings.value(t).points;
        if(pts > bestPoints) {
            bestPoints = pts;
            champion = t;
        }
    }
    return champion;
}

bool LeagueManager::allMatchesPlayed() const {
    return m_currentMatchIndex >= m_schedule.size();
}

