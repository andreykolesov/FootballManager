#include "Player.h"

Player::Player(const QString &name,
               const QString &position,
               int speed,
               int shotPrecision,
               int endurance,
               int dribbling,
               int passPrecision,
               int defenseSkill,
               int physicalStrength,
               int currentForm,
               int price)
    : m_name(name),
    m_position(position),
    m_speed(speed),
    m_shotPrecision(shotPrecision),
    m_endurance(endurance),
    m_dribbling(dribbling),
    m_passPrecision(passPrecision),
    m_defenseSkill(defenseSkill),
    m_physicalStrength(physicalStrength),
    m_currentForm(currentForm),
    m_price(price)
{}

QString Player::getName() const { return m_name; }
QString Player::getPosition() const { return m_position; }
int Player::getSpeed() const { return m_speed; }
int Player::getShotPrecision() const { return m_shotPrecision; }
int Player::getEndurance() const { return m_endurance; }
int Player::getDribbling() const { return m_dribbling; }
int Player::getPassPrecision() const { return m_passPrecision; }
int Player::getDefenseSkill() const { return m_defenseSkill; }
int Player::getPhysicalStrength() const { return m_physicalStrength; }
int Player::getCurrentForm() const { return m_currentForm; }
int Player::getPrice() const { return m_price; }

void Player::setSpeed(int value) { m_speed = value; }
void Player::setShotPrecision(int value) { m_shotPrecision = value; }
void Player::setEndurance(int value) { m_endurance = value; }
void Player::setDribbling(int value) { m_dribbling = value; }
void Player::setPassPrecision(int value) { m_passPrecision = value; }
void Player::setDefenseSkill(int value) { m_defenseSkill = value; }
void Player::setPhysicalStrength(int value) { m_physicalStrength = value; }
void Player::setCurrentForm(int value) { m_currentForm = value; }

QString Player::getDetails() const {
    return QString("Имя: %1\nПозиция: %2\nСкорость: %3\nТочность удара: %4\nВыносливость: %5\n"
                   "Дриблинг: %6\nТочность паса: %7\nОборонительные умения: %8\nФизическая сила: %9\n"
                   "Форма: %10\nСтоимость: %11")
        .arg(m_name)
        .arg(m_position)
        .arg(m_speed)
        .arg(m_shotPrecision)
        .arg(m_endurance)
        .arg(m_dribbling)
        .arg(m_passPrecision)
        .arg(m_defenseSkill)
        .arg(m_physicalStrength)
        .arg(m_currentForm)
        .arg(m_price);
}
