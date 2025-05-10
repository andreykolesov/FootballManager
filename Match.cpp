#include "Match.h"
#include <QRandomGenerator>
#include <QString>

Match::Match(Team* homeTeam, Team* awayTeam)
    : m_homeTeam(homeTeam), m_awayTeam(awayTeam), m_homeScore(0), m_awayScore(0)
{}

QString Match::simulateStep(int currentMinute) {
    QString eventStr;
    int homeRating = m_homeTeam->getAttack() + m_homeTeam->getAveragePlayerRating() + m_homeTeam->getMood();
    int awayRating = m_awayTeam->getDefense() + m_awayTeam->getAveragePlayerRating() + m_awayTeam->getMood();
    int homeChance = QRandomGenerator::global()->bounded(homeRating);
    int awayChance = QRandomGenerator::global()->bounded(awayRating);
    if (homeChance > 50) {
        ++m_homeScore;
        eventStr = QString("Минуты %1: Гол команды \"%2\"").arg(currentMinute).arg(m_homeTeam->getName());
        m_events.append(eventStr);
    }
    if (awayChance > 50) {
        ++m_awayScore;
        eventStr = QString("Минуты %1: Гол команды \"%2\"").arg(currentMinute).arg(m_awayTeam->getName());
        m_events.append(eventStr);
    }
    return eventStr;
}

Team* Match::getHomeTeam() const { return m_homeTeam; }
Team* Match::getAwayTeam() const { return m_awayTeam; }
int Match::getHomeScore() const { return m_homeScore; }
int Match::getAwayScore() const { return m_awayScore; }
QStringList Match::getAllEvents() const { return m_events; }
