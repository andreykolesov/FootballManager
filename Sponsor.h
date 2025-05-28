#ifndef SPONSOR_H
#define SPONSOR_H

#include <QString>
#include <QStringList>
#include <QRandomGenerator>
#include "Team.h"

class Sponsor {
private:
    QString m_name;
    int m_bonusBudget;

    Sponsor();
public:
    Sponsor(const Sponsor&) = delete;
    Sponsor& operator=(const Sponsor&) = delete;

    static Sponsor* getInstance();

    QString getName() const;
    int getBonusBudget() const;
    void sponsorTeam(Team* team) const;
};

#endif
