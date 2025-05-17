#include "Tournament.h"
#include <QRandomGenerator>

Tournament::Tournament(const QString &name)
    : m_name(name)
{}

void Tournament::addTeam(Team* team) {
    m_teams.append(team);
}

QList<Team*> Tournament::getTeams() const {
    return m_teams;
}

Team* Tournament::simulateKnockoutTournament() {
    QList<Team*> currentRound = m_teams;
    while (currentRound.size() > 1) {
        QList<Team*> nextRound;
        for (int i = 0; i < currentRound.size(); i += 2) {
            if (i+1 < currentRound.size()) {
                int result = QRandomGenerator::global()->bounded(2);
                if (result == 0)
                    nextRound.append(currentRound.at(i));
                else
                    nextRound.append(currentRound.at(i+1));
            } else {
                nextRound.append(currentRound.at(i));
            }
        }
        currentRound = nextRound;
    }
    return currentRound.isEmpty() ? nullptr : currentRound.first();
}

QMap<Team*, TournamentRecord> Tournament::simulateLeagueSeason(const QList<Team*>& teams) {
    QMap<Team*, TournamentRecord> leagueStandings;
    for (Team* t : teams) {
        TournamentRecord rec = {0,0,0,0,0,0,0};
        leagueStandings[t] = rec;
    }
    for (int i = 0; i < teams.size(); ++i) {
        for (int j = i+1; j < teams.size(); ++j) {
            Team* teamA = teams.at(i);
            Team* teamB = teams.at(j);
            int teamARating = teamA->getAttackRating() + teamA->getMood();
            int teamBRating = teamB->getDefenseRating() + teamB->getMood();
            int scoreA = QRandomGenerator::global()->bounded(0, teamARating / 20 + 2);
            int scoreB = QRandomGenerator::global()->bounded(0, teamBRating / 20 + 2);
            TournamentRecord recA = leagueStandings.value(teamA);
            TournamentRecord recB = leagueStandings.value(teamB);
            recA.played++; recB.played++;
            recA.goalsFor += scoreA; recA.goalsAgainst += scoreB;
            recB.goalsFor += scoreB; recB.goalsAgainst += scoreA;
            if(scoreA > scoreB) { recA.wins++; recA.points += 3; recB.losses++; }
            else if(scoreA < scoreB) { recB.wins++; recB.points += 3; recA.losses++; }
            else { recA.draws++; recB.draws++; recA.points += 1; recB.points += 1; }
            leagueStandings[teamA] = recA;
            leagueStandings[teamB] = recB;
        }
    }
    return leagueStandings;
}
