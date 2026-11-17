#pragma once
#include <string>

class Building {
public:
    Building(const std::string& name, int constructionTurns);
    const std::string& getName() const { return name_; }
    int getLevel() const { return level_; }
    bool isBuilt() const { return constructionTurnsRemaining_ == 0; }
    bool isUnderConstruction() const { return constructionTurnsRemaining_ > 0; }
    bool isUpgrading() const { return upgradeTurnsRemaining_ > 0; }
    int getConstructionTurnsRemaining() const { return constructionTurnsRemaining_; }
    int getUpgradeTurnsRemaining() const { return upgradeTurnsRemaining_; }
    bool startUpgrade(int turns);
    void advanceTurn();
private:
    std::string name_;
    int level_;
    int constructionTurnsRemaining_;
    int upgradeTurnsRemaining_;
};
