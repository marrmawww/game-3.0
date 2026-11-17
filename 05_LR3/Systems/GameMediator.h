#pragma once
#include <string>
#include "GameController.h"
#include "UnitManager.h"
#include "ResourceSystem.h"
#include "CombatSystem.h"
#include "BuildingManager.h"

class GameMediator {
public:
    GameMediator(GameController& game, Base& player, Base& enemy);

    // Копировать посредника нельзя: системы хранят указатель на "этого" посредника
    GameMediator(const GameMediator&) = delete;
    GameMediator& operator=(const GameMediator&) = delete;
    bool createUnit(UnitKind kind, int x, int y);
    bool moveUnit(int id, int x, int y);
    bool attack(int attackerId, int targetId);
    bool attackBase(int attackerId);
    bool collectResource(int id);
    bool useObject(int id);
    bool buildHospital();
    bool upgradeHospital();
    bool healUnit(int id);
    void endTurn();
    void showMap() const;
    std::string stats(const std::string& section = "all") const;
    int turn() const;
private:
    GameController& game_;
    Base& player_;
    UnitManager units_;
    ResourceSystem resources_;
    CombatSystem combat_;
    BuildingManager buildings_;
};
