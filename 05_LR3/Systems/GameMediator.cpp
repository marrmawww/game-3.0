#include "GameMediator.h"
#include <iostream>
#include <sstream>

GameMediator::GameMediator(GameController& game, Base& player, Base& enemy)
    : game_(game), player_(player), units_(game, player, enemy), resources_(player),
      combat_(game, units_, enemy), buildings_(player) {}

bool GameMediator::createUnit(UnitKind kind, int x, int y) {
    return static_cast<bool>(units_.create(kind, x, y));
}
bool GameMediator::moveUnit(int id, int x, int y) { return units_.moveTo(id, x, y); }
bool GameMediator::attack(int a, int t) { return combat_.attack(a, t); }
bool GameMediator::attackBase(int id) { return combat_.attackBase(id); }
bool GameMediator::collectResource(int id) { return units_.collectAtPosition(id); }
bool GameMediator::useObject(int id) {
    std::shared_ptr<Unit> u = units_.findPlayerUnit(id);
    return u && game_.interactWithObject(*u);
}
bool GameMediator::buildHospital() { return buildings_.buildHospital(); }
bool GameMediator::upgradeHospital() { return buildings_.upgradeHospital(); }
bool GameMediator::healUnit(int id) { return buildings_.healUnit(id); }
void GameMediator::endTurn() { game_.endTurn(); }
void GameMediator::showMap() const { game_.printMap(std::cout); }
int GameMediator::turn() const { return game_.getTurn(); }

std::string GameMediator::stats(const std::string& section) const {
    std::ostringstream out;
    if (section == "all" || section == "resources") out << resources_.getInfo() << "\n";
    if (section == "all" || section == "buildings") out << buildings_.getInfo() << "\n";
    if (section == "all" || section == "units") out << units_.listUnits();
    out << "Текущий ход: " << game_.getTurn();
    return out.str();
}
