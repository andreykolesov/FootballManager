#include "Match.h"
#include <QRandomGenerator>
#include <QString>
#include <QtGlobal>

Match::Match(Team* homeTeam, Team* awayTeam)
    : m_homeTeam(homeTeam),
    m_awayTeam(awayTeam),
    m_homeScore(0),
    m_awayScore(0)
{}

QString Match::simulateStep(int currentTick) {
    QStringList events;
    int baseThreshold = 90;

    int homeAttack = m_homeTeam->getAttackRating() + m_homeTeam->getMood() + m_homeTeam->getTrainerAbility();
    int awayDefense = m_awayTeam->getDefenseRating() + m_awayTeam->getMood() + m_awayTeam->getTrainerAbility();
    int homeDiff = homeAttack - awayDefense;
    int adjustedThresholdHome = qBound(85, baseThreshold - (homeDiff / 6), 98);
    int chanceHome = QRandomGenerator::global()->bounded(100);
    if(chanceHome > adjustedThresholdHome) {
        ++m_homeScore;
        events << QString("Команда \"%1\" забила гол.").arg(m_homeTeam->getName());
    }

    int awayAttack = m_awayTeam->getAttackRating() + m_awayTeam->getMood() + m_awayTeam->getTrainerAbility();
    int homeDefense = m_homeTeam->getDefenseRating() + m_homeTeam->getMood() + m_homeTeam->getTrainerAbility();
    int awayDiff = awayAttack - homeDefense;
    int adjustedThresholdAway = qBound(85, baseThreshold - (awayDiff / 6), 98);
    int chanceAway = QRandomGenerator::global()->bounded(100);
    if(chanceAway > adjustedThresholdAway) {
        ++m_awayScore;
        events << QString("Команда \"%1\" забила гол.").arg(m_awayTeam->getName());
    }

    int extra = QRandomGenerator::global()->bounded(100);
    if(extra < 5) {
        events << "Фол! Желтая карточка.";
    } else if(extra < 10) {
        events << "Угловой удар.";
    } else if(extra < 15) {
        events << "Свободный удар.";
    }
    return events.join("\n");
}


Team* Match::getHomeTeam() const { return m_homeTeam; }
Team* Match::getAwayTeam() const { return m_awayTeam; }
int Match::getHomeScore() const { return m_homeScore; }
int Match::getAwayScore() const { return m_awayScore; }
QStringList Match::getAllEvents() const { return m_events; }
