#include "UnitManager.h"
#include <sstream>
#include <cstdlib>
#include "NeutralOperators.h"
#include "TreasureChest.h"

UnitManager::UnitManager(GameController& game, Base& player, Base& enemy)
    : game_(game), player_(player), enemy_(enemy) {}

std::shared_ptr<Unit> UnitManager::create(UnitKind kind, int x, int y) {
    return game_.spawnUnit(player_, kind, x, y);
}

std::shared_ptr<Unit> UnitManager::findPlayerUnit(int id) const {
    const std::vector<std::shared_ptr<Unit> >& units = player_.getUnits();
    for (size_t i = 0; i < units.size(); ++i)
        if (units[i]->getId() == id) return units[i];
    return std::shared_ptr<Unit>();
}

std::shared_ptr<Unit> UnitManager::findEnemyUnit(int id) const {
    const std::vector<std::shared_ptr<Unit> >& units = enemy_.getUnits();
    for (size_t i = 0; i < units.size(); ++i)
        if (units[i]->getId() == id) return units[i];
    return std::shared_ptr<Unit>();
}

bool UnitManager::moveTo(int id, int x, int y) {
    std::shared_ptr<Unit> unit = findPlayerUnit(id);
    if (!unit || !unit->isAlive()) return false;
    int steps = 0;
    while ((unit->getX() != x || unit->getY() != y) && steps < 20) {
        int dx = x > unit->getX() ? 1 : (x < unit->getX() ? -1 : 0);
        int dy = y > unit->getY() ? 1 : (y < unit->getY() ? -1 : 0);
        if (!game_.moveUnit(*unit, dx, dy)) return steps > 0;
        ++steps;
    }
    return unit->getX() == x && unit->getY() == y;
}

bool UnitManager::collectAtPosition(int id) {
    std::shared_ptr<Unit> unit = findPlayerUnit(id);
    if (!unit || !unit->isAlive()) return false;
    NeutralObject* object = game_.map().getNeutralObjectAt(unit->getX(), unit->getY());
    if (dynamic_cast<TreasureChest*>(object) == nullptr) return false;
    *unit + *static_cast<TreasureChest*>(object);
    return true;
}

std::string UnitManager::listUnits() const {
    std::ostringstream out;
    const std::vector<std::shared_ptr<Unit> >& units = player_.getUnits();
    if (units.empty()) return "Юнитов пока нет.";
    for (size_t i = 0; i < units.size(); ++i) {
        const Unit& u = *units[i];
        out << "ID " << u.getId() << ": " << u.getType()
            << " | позиция (" << u.getX() << "," << u.getY() << ")"
            << " | здоровье " << u.getHealth() << "/" << u.getMaxHealth()
            << " | броня " << u.getArmorValue()
            << " | атака " << u.getAttackValue()
            << " | скорость " << u.getSpeed() << "\n";
    }
    return out.str();
}
