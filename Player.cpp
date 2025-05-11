#include "Player.h"
#include "Training.h"
#include <QRandomGenerator>

QSet<QString> Player::s_usedSignatures;

Player* Player::create(const QString &name)
{
    return new Player(name);
}

Player::Player(const QString &name)
    : m_name(name)
{
    m_age = QRandomGenerator::global()->bounded(18, 41);
    while (true) {
        int dr = QRandomGenerator::global()->bounded(60, 101);
        int def = QRandomGenerator::global()->bounded(60, 101);
        int sh = QRandomGenerator::global()->bounded(60, 101);
        int sp = QRandomGenerator::global()->bounded(60, 101);
        int pa = QRandomGenerator::global()->bounded(60, 101);
        int en = QRandomGenerator::global()->bounded(60, 101);
        int ps = QRandomGenerator::global()->bounded(60, 101);
        int cf = QRandomGenerator::global()->bounded(60, 101);
        QString sig = makeSignature(dr, def, sh, sp, pa, en, ps, cf);
        if (!s_usedSignatures.contains(sig)) {
            s_usedSignatures.insert(sig);
            m_dribbling = dr;
            m_defense = def;
            m_shot = sh;
            m_speed = sp;
            m_pass = pa;
            m_endurance = en;
            m_physicalStrength = ps;
            m_currentForm = cf;
            break;
        }
    }
    m_price = QRandomGenerator::global()->bounded(100, 301);
}

QString Player::getName() const { return m_name; }
int Player::getAge() const { return m_age; }
int Player::getDribbling() const { return m_dribbling; }
int Player::getDefense() const { return m_defense; }
int Player::getShot() const { return m_shot; }
int Player::getSpeed() const { return m_speed; }
int Player::getPass() const { return m_pass; }
int Player::getEndurance() const { return m_endurance; }
int Player::getPhysicalStrength() const { return m_physicalStrength; }
int Player::getCurrentForm() const { return m_currentForm; }
int Player::getPrice() const { return m_price; }

void Player::setDribbling(int value) { m_dribbling = value; }
void Player::setDefense(int value) { m_defense = value; }
void Player::setShot(int value) { m_shot = value; }
void Player::setSpeed(int value) { m_speed = value; }
void Player::setPass(int value) { m_pass = value; }
void Player::setEndurance(int value) { m_endurance = value; }
void Player::setPhysicalStrength(int value) { m_physicalStrength = value; }
void Player::setCurrentForm(int value) { m_currentForm = value; }

void Player::age() {
    m_age++;
    m_currentForm = qMax(m_currentForm - 2, 50);
    m_dribbling = qMax(m_dribbling - 1, 50);
    m_defense = qMax(m_defense - 1, 50);
    m_shot = qMax(m_shot - 1, 50);
    m_speed = qMax(m_speed - 1, 50);
    m_pass = qMax(m_pass - 1, 50);
    m_endurance = qMax(m_endurance - 1, 50);
    m_physicalStrength = qMax(m_physicalStrength - 1, 50);
}

void Player::train() {
    TrainingSession::conductTraining(this);
}

QString Player::makeSignature(int dr, int def, int sh, int sp, int pa, int en, int ps, int cf) {
    return QString("%1_%2_%3_%4_%5_%6_%7_%8")
    .arg(dr).arg(def).arg(sh).arg(sp)
        .arg(pa).arg(en).arg(ps).arg(cf);
}

QVariantMap Player::getStats() const {
    QVariantMap stats;
    stats["Имя"] = m_name;
    stats["Возраст"] = m_age;
    stats["Дриблинг"] = m_dribbling;
    stats["Защита"] = m_defense;
    stats["Удар"] = m_shot;
    stats["Скорость"] = m_speed;
    stats["Пас"] = m_pass;
    stats["Выносливость"] = m_endurance;
    stats["Физическая сила"] = m_physicalStrength;
    stats["Форма"] = m_currentForm;
    stats["Цена"] = m_price;
    return stats;
}
