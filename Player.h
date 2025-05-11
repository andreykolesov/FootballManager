#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QVariantMap>
#include <QSet>

class Player {
private:
    Player(const QString &name);

    QString m_name;
    int m_age;
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
    static Player* create(const QString &name);

    QString getName() const;
    int getAge() const;
    int getDribbling() const;
    int getDefense() const;
    int getShot() const;
    int getSpeed() const;
    int getPass() const;
    int getEndurance() const;
    int getPhysicalStrength() const;
    int getCurrentForm() const;
    int getPrice() const;

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
