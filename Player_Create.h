#ifndef PLAYER_CREATE_H
#define PLAYER_CREATE_H

#include <QWidget>
#include "Player.h"

class PlayerCreate {
public:
    static Player* createNewPlayer(QWidget* parent = nullptr);
};

#endif
