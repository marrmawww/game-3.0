#pragma once
#include <memory>
#include <string>
#include "GameController.h"

class UnitManager {
public:
    UnitManager(GameController& game, Base& player, Base& enemy);
    std::shared_ptr<Unit> create(UnitKind kind, int x, int y);
    bool moveTo(int id, int x, int y);
    bool collectAtPosition(int id);
    std::shared_ptr<Unit> findPlayerUnit(int id) const;
    std::shared_ptr<Unit> findEnemyUnit(int id) const;
    std::string listUnits() const;
private:
    GameController& game_;
    Base& player_;
    Base& enemy_;
};
