#pragma once
#include "GameController.h"
#include "UnitManager.h"

class CombatSystem {
public:
    CombatSystem(GameController& game, UnitManager& units, Base& enemy)
        : game_(game), units_(units), enemy_(enemy) {}
    bool attack(int attackerId, int targetId);
    bool attackBase(int attackerId);
private:
    GameController& game_;
    UnitManager& units_;
    Base& enemy_;
};
