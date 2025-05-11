#ifndef TEAM_H
#define TEAM_H

#include <QString>
#include <QList>
#include "Player.h"

class Team {
private:
    QString m_name;
    int m_mood;
    int m_trainerAbility;
    int m_budget;
    QList<Player*> m_players;
    QString m_trophies;
public:
    Team(const QString &name, int mood, int trainerAbility, int budget);

    QString getName() const;
    int getMood() const;
    int getTrainerAbility() const;
    int getBudget() const;

    QList<Player*> getPlayers() const;
    void addPlayer(Player* player);
    void removePlayer(Player* player);

    int getAttackRating() const;
    int getDefenseRating() const;

    QString getTeamDetails() const;

    void increaseBudget(int amount);
    bool decreaseBudget(int amount);

    void awardTrophy(const QString &trophy);
    QString getTrophies() const;

    void resetTrophies();
};

#endif
