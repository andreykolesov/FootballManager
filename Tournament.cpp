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
