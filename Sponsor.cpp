#include "Sponsor.h"

Sponsor* Sponsor::getInstance() {
    static Sponsor instance;
    return &instance;
}

Sponsor::Sponsor() {
    QStringList sponsorNames = {"Nike", "Adidas", "Puma", "Reebok", "UnderArmour"};
    int index = QRandomGenerator::global()->bounded(sponsorNames.size());
    m_name = sponsorNames.at(index);

    m_bonusBudget = QRandomGenerator::global()->bounded(100, 501);
}

QString Sponsor::getName() const {
    return m_name;
}

int Sponsor::getBonusBudget() const {
    return m_bonusBudget;
}

void Sponsor::sponsorTeam(Team* team) const {
    team->increaseBudget(m_bonusBudget);
}
