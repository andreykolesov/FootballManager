#ifndef TEAM_H
#define TEAM_H

#include <QString>
#include <QList>
#include "Player.h"

class Team {
private:
    QString m_name;
    int m_mood;
    int m_attack;
    int m_defense;
    QString m_trainer;
    QString m_stadium;
    int m_budget;
    QList<Player*> m_players;
public:
    Team(const QString &name,
         int mood,
         int attack,
         int defense,
         const QString &trainer,
         const QString &stadium,
         int budget);

    QString getName() const;
    int getMood() const;
    int getAttack() const;
    int getDefense() const;
    QString getTrainer() const;
    QString getStadium() const;
    int getBudget() const;

    QList<Player*> getPlayers() const;
    void addPlayer(Player* player);
    void removePlayer(Player* player);

    int getAveragePlayerRating() const;
    QString getTeamDetails() const;

    void increaseBudget(int amount);
    bool decreaseBudget(int amount);
};

#endif
