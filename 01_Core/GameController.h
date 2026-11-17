#pragma once
#include <memory>
#include <ostream>
#include <vector>
#include "Base.h"
#include "TerrainMap.h"
#include "Unit.h"

class GameController {
public:
    GameController(int width, int height, int seed = 1);

    TerrainMap& map() { return map_; }
    const TerrainMap& map() const { return map_; }

    Base& addBase(const std::string& name, char symbol, int x, int y,
                  int health, size_t maxUnits, int defenseLevel);

 
    std::shared_ptr<Unit> spawnUnit(Base& base, UnitType type, int x, int y);

    bool moveUnit(Unit& unit, int dx, int dy);

    bool attackUnit(Unit& attacker, Unit& target);
    bool attackBase(Unit& attacker, Base& base);

   
    bool interactWithObject(Unit& unit);

    void endTurn();
    int getTurn() const { return turn_; }


    void printMap(std::ostream& os) const;


private:
    char getCellSymbol(int x, int y) const;
    bool isCellFree(int x, int y) const;
    int nextRandom();   

    TerrainMap map_;
    std::vector<std::unique_ptr<Base> > bases_;
    long long seed_;
    int turn_;
};
