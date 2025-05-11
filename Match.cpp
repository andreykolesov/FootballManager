#include "Match.h"
#include <QRandomGenerator>
#include <QString>
#include <QtGlobal>

Match* Match::create(Team* homeTeam, Team* awayTeam) {
    return new Match(homeTeam, awayTeam);
}

Match::Match(Team* homeTeam, Team* awayTeam)
    : m_homeTeam(homeTeam),
    m_awayTeam(awayTeam),
    m_homeScore(0),
    m_awayScore(0)
{}

QString Match::simulateStep(int currentMinute) {
    QString eventStr;
    int homeRating = m_homeTeam->getAttackRating() + m_homeTeam->getMood() + m_homeTeam->getTrainerAbility();
    int awayRating = m_awayTeam->getDefenseRating() + m_awayTeam->getMood() + m_awayTeam->getTrainerAbility();
    int randomVal = QRandomGenerator::global()->bounded(100);

    if(randomVal < 5) {
        eventStr = QString("На %1-й минуте произошло несчастье.").arg(currentMinute);
        m_events.append(eventStr);
        return eventStr;
    }

    int threshold = (homeRating > awayRating) ? 65 : 75;
    int chance = QRandomGenerator::global()->bounded(100);
    if(chance > threshold) {
        ++m_homeScore;
        eventStr = QString("На %1-й минуте команда %2 забила гол.")
                       .arg(currentMinute)
                       .arg(m_homeTeam->getName());
        m_events.append(eventStr);
        return eventStr;
    }
    return "";
}

Team* Match::getHomeTeam() const { return m_homeTeam; }
Team* Match::getAwayTeam() const { return m_awayTeam; }
int Match::getHomeScore() const { return m_homeScore; }
int Match::getAwayScore() const { return m_awayScore; }
QStringList Match::getAllEvents() const { return m_events; }
