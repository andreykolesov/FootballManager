#include "Team.h"

Team::Team(const QString &name,
           int mood,
           int attack,
           int defense,
           const QString &trainer,
           const QString &stadium,
           int budget)
    : m_name(name),
    m_mood(mood),
    m_attack(attack),
    m_defense(defense),
    m_trainer(trainer),
    m_stadium(stadium),
    m_budget(budget)
{}

QString Team::getName() const { return m_name; }
int Team::getMood() const { return m_mood; }
int Team::getAttack() const { return m_attack; }
int Team::getDefense() const { return m_defense; }
QString Team::getTrainer() const { return m_trainer; }
QString Team::getStadium() const { return m_stadium; }
int Team::getBudget() const { return m_budget; }

QList<Player*> Team::getPlayers() const { return m_players; }
void Team::addPlayer(Player* player) { m_players.append(player); }
void Team::removePlayer(Player* player) { m_players.removeAll(player); }

int Team::getAveragePlayerRating() const {
    if (m_players.isEmpty())
        return 0;
    int total = 0;
    for (Player* p : m_players)
        total += (p->getSpeed() + p->getShotPrecision() + p->getEndurance() +
                  p->getDribbling() + p->getPassPrecision() +
                  p->getDefenseSkill() + p->getPhysicalStrength() +
                  p->getCurrentForm()) / 8;
    return total / m_players.size();
}

QString Team::getTeamDetails() const {
    return QString("Команда: %1\nНастрой: %2\nАтака: %3\nЗащита: %4\nБюджет: %5\nТренер: %6\nСтадион: %7")
        .arg(m_name)
        .arg(m_mood)
        .arg(m_attack)
        .arg(m_defense)
        .arg(m_budget)
        .arg(m_trainer)
        .arg(m_stadium);
}

void Team::increaseBudget(int amount) { m_budget += amount; }

bool Team::decreaseBudget(int amount) {
    if (m_budget >= amount) {
        m_budget -= amount;
        return true;
    }
    return false;
}
