#include "Building.h"
#include <stdexcept>

Building::Building(const std::string& name, int constructionTurns)
    : name_(name), level_(1), constructionTurnsRemaining_(constructionTurns), upgradeTurnsRemaining_(0) {
    if (constructionTurns < 0) throw std::invalid_argument("Некорректное время строительства");
}

bool Building::startUpgrade(int turns) {
    if (!isBuilt() || isUpgrading() || level_ >= 2 || turns <= 0) return false;
    upgradeTurnsRemaining_ = turns;
    return true;
}

void Building::advanceTurn() {
    if (constructionTurnsRemaining_ > 0) { --constructionTurnsRemaining_; return; }
    if (upgradeTurnsRemaining_ > 0 && --upgradeTurnsRemaining_ == 0) ++level_;
}
