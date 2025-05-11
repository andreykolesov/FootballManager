#include "Team.h"
#include <QString>
#include <QTextStream>

Team::Team(const QString &name,
           int mood,
           int trainerAbility,
           int budget)
    : m_name(name),
    m_mood(mood),
    m_trainerAbility(trainerAbility),
    m_budget(budget)
{}

QString Team::getName() const { return m_name; }
int Team::getMood() const { return m_mood; }
int Team::getTrainerAbility() const { return m_trainerAbility; }
int Team::getBudget() const { return m_budget; }

QList<Player*> Team::getPlayers() const { return m_players; }
void Team::addPlayer(Player* player) { m_players.append(player); }
void Team::removePlayer(Player* player) { m_players.removeAll(player); }

int Team::getAttackRating() const {
    if(m_players.isEmpty()) return 0;
    int sum = 0;
    for (Player* p : m_players)
        sum += (p->getDribbling() + p->getShot() + p->getPass()) / 3;
    return sum / m_players.size();
}

int Team::getDefenseRating() const {
    if(m_players.isEmpty()) return 0;
    int sum = 0;
    for (Player* p : m_players)
        sum += (p->getDefense() + p->getEndurance() + p->getPhysicalStrength()) / 3;
    return sum / m_players.size();
}

QString Team::getTeamDetails() const {
    QString details;
    QTextStream stream(&details);
    stream << "Команда: " << m_name << "\nНастрой: " << m_mood
           << "\nАтака: " << getAttackRating()
           << "\nЗащита: " << getDefenseRating()
           << "\nБюджет: " << m_budget
           << "\nТренер: " << m_trainerAbility
           << "\nТрофеи: " << m_trophies;
    return details;
}

void Team::increaseBudget(int amount) { m_budget += amount; }
bool Team::decreaseBudget(int amount) {
    if(m_budget >= amount) {
        m_budget -= amount;
        return true;
    }
    return false;
}

void Team::awardTrophy(const QString &trophy) {
    if(m_trophies.isEmpty())
        m_trophies = trophy;
    else
        m_trophies += ", " + trophy;
}

void Team::resetTrophies() {
    m_trophies.clear();
}

QString Team::getTrophies() const { return m_trophies; }
