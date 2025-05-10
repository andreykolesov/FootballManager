#include "Training.h"
#include <QRandomGenerator>

static int clamp(int value, int minVal, int maxVal) {
    return (value < minVal) ? minVal : (value > maxVal ? maxVal : value);
}

void TrainingSession::conductTraining(Player* player) {
    int deltaSpeed = QRandomGenerator::global()->bounded(-2, 4);
    player->setSpeed(clamp(player->getSpeed() + deltaSpeed, 50, 100));

    int deltaShot = QRandomGenerator::global()->bounded(-2, 4);
    player->setShotPrecision(clamp(player->getShotPrecision() + deltaShot, 50, 100));

    int deltaEndurance = QRandomGenerator::global()->bounded(-2, 4);
    player->setEndurance(clamp(player->getEndurance() + deltaEndurance, 50, 100));

    int deltaDribbling = QRandomGenerator::global()->bounded(-2, 4);
    player->setDribbling(clamp(player->getDribbling() + deltaDribbling, 50, 100));

    int deltaPass = QRandomGenerator::global()->bounded(-2, 4);
    player->setPassPrecision(clamp(player->getPassPrecision() + deltaPass, 50, 100));

    int deltaDefense = QRandomGenerator::global()->bounded(-2, 4);
    player->setDefenseSkill(clamp(player->getDefenseSkill() + deltaDefense, 50, 100));

    int deltaPhysical = QRandomGenerator::global()->bounded(-2, 4);
    player->setPhysicalStrength(clamp(player->getPhysicalStrength() + deltaPhysical, 50, 100));

    int deltaForm = QRandomGenerator::global()->bounded(-3, 6);
    player->setCurrentForm(clamp(player->getCurrentForm() + deltaForm, 50, 100));
}
