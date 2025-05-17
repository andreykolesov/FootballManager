#include "Training.h"
#include <QRandomGenerator>

static int clamp(int value, int minVal, int maxVal) {
    return (value < minVal) ? minVal : (value > maxVal ? maxVal : value);
}

void TrainingSession::conductTraining(Player* player) {
    int deltaDribbling = QRandomGenerator::global()->bounded(-2, 5);
    player->setDribbling(clamp(player->getDribbling() + deltaDribbling, 50, 100));

    int deltaDefense = QRandomGenerator::global()->bounded(-2, 5);
    player->setDefense(clamp(player->getDefense() + deltaDefense, 50, 100));

    int deltaShot = QRandomGenerator::global()->bounded(-2, 5);
    player->setShot(clamp(player->getShot() + deltaShot, 50, 100));

    int deltaSpeed = QRandomGenerator::global()->bounded(-2, 5);
    player->setSpeed(clamp(player->getSpeed() + deltaSpeed, 50, 100));

    int deltaPass = QRandomGenerator::global()->bounded(-2, 5);
    player->setPass(clamp(player->getPass() + deltaPass, 50, 100));

    int deltaEndurance = QRandomGenerator::global()->bounded(-2, 5);
    player->setEndurance(clamp(player->getEndurance() + deltaEndurance, 50, 100));

    int deltaPhysical = QRandomGenerator::global()->bounded(-2, 5);
    player->setPhysicalStrength(clamp(player->getPhysicalStrength() + deltaPhysical, 50, 100));

    int deltaForm = QRandomGenerator::global()->bounded(-3, 6);
    player->setCurrentForm(clamp(player->getCurrentForm() + deltaForm, 50, 100));
}
