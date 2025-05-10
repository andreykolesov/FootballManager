#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player {
private:
    QString m_name;
    QString m_position;
    int m_speed;
    int m_shotPrecision;
    int m_endurance;
    int m_dribbling;
    int m_passPrecision;
    int m_defenseSkill;
    int m_physicalStrength;
    int m_currentForm;
    int m_price;
public:
    Player(const QString &name,
           const QString &position,
           int speed,
           int shotPrecision,
           int endurance,
           int dribbling,
           int passPrecision,
           int defenseSkill,
           int physicalStrength,
           int currentForm = 100,
           int price = 100);

    QString getName() const;
    QString getPosition() const;
    int getSpeed() const;
    int getShotPrecision() const;
    int getEndurance() const;
    int getDribbling() const;
    int getPassPrecision() const;
    int getDefenseSkill() const;
    int getPhysicalStrength() const;
    int getCurrentForm() const;
    int getPrice() const;

    void setSpeed(int value);
    void setShotPrecision(int value);
    void setEndurance(int value);
    void setDribbling(int value);
    void setPassPrecision(int value);
    void setDefenseSkill(int value);
    void setPhysicalStrength(int value);
    void setCurrentForm(int value);

    QString getDetails() const;
};

#endif
