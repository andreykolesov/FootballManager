#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QVariantMap>
#include <QSet>

class Player {
private:
    QString m_name;
    int m_age;
    QString m_position;
    int m_dribbling;
    int m_defense;
    int m_shot;
    int m_speed;
    int m_pass;
    int m_endurance;
    int m_physicalStrength;
    int m_currentForm;
    int m_price;

    static QSet<QString> s_usedSignatures;
    static QString makeSignature(int dr, int def, int sh, int sp, int pa, int en, int ps, int cf);
public:
    Player(const QString &name);

    Player(const QString &name, int age, const QString &position,
           int dribbling, int defense, int shot, int speed, int pass,
           int endurance, int physicalStrength, int currentForm, int price);

    QString getName() const;
    int getAge() const;
    QString getPosition() const;
    int getDribbling() const;
    int getDefense() const;
    int getShot() const;
    int getSpeed() const;
    int getPass() const;
    int getEndurance() const;
    int getPhysicalStrength() const;
    int getCurrentForm() const;
    int getPrice() const;

    void setAge(int age);
    void setPosition(const QString &pos);
    void setDribbling(int value);
    void setDefense(int value);
    void setShot(int value);
    void setSpeed(int value);
    void setPass(int value);
    void setEndurance(int value);
    void setPhysicalStrength(int value);
    void setCurrentForm(int value);

    void age();
    void train();

    QVariantMap getStats() const;
};

#endif
