#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Observer.h"
#include "Resources.h"
#include "Unit.h"
#include "UnitKind.h"
#include "Building.h"

struct DestroyedRecord {
    int unitId;
    std::string unitName;
    long long timestamp;
};


class Base : public Observer {
public:
    Base(const std::string& name, char symbol, int x, int y,
         int health, size_t maxUnits, int defenseLevel,
         const Resources& resources = Resources());
    ~Base();


    Base(const Base&) = delete;
    Base& operator=(const Base&) = delete;
    Base(Base&&) = delete;
    Base& operator=(Base&&) = delete;

  
    std::shared_ptr<Unit> createUnit(UnitType type, int x, int y);
    bool removeUnit(int unitId);
    void purgeDead();   

  
    void onUnitEvent(const UnitEvent& event) override;

    
    void takeDamage(int rawDamage);
    
    int defenseBonusAt(int x, int y) const;

    void addResources(const Resources& r) { resources_ += r; }
    bool startHospitalConstruction();
    bool startHospitalUpgrade();
    void advanceBuildingTurn();
    bool healUnitAtBase(int unitId);
    std::string buildingsInfo() const;
    std::string resourcesInfo() const;
    bool hasBuildingProcess() const { return activeBuildingProcess_; }

    const std::string& getName() const { return name_; }
    char getSymbol() const { return symbol_; }
    int getX() const { return x_; }
    int getY() const { return y_; }
    int getHealth() const { return health_; }
    int getMaxHealth() const { return maxHealth_; }
    size_t getMaxUnits() const { return maxUnits_; }
    int getDefenseLevel() const { return defenseLevel_; }
    const Resources& getResources() const { return resources_; }
    bool isDestroyed() const { return health_ <= 0; }

    size_t getAliveCount() const { return aliveCount_; }
    const std::vector<std::shared_ptr<Unit> >& getUnits() const { return units_; }
    const std::vector<DestroyedRecord>& getDestroyedHistory() const { return history_; }

private:
    std::string name_;
    char symbol_;
    int x_;
    int y_;
    int health_;
    int maxHealth_;
    size_t maxUnits_;
    int defenseLevel_;
    Resources resources_;

    std::vector<std::shared_ptr<Unit> > units_;  
    std::vector<DestroyedRecord> history_;
    size_t aliveCount_;
    std::vector<Building> builtBuildings_;
    std::vector<Building> buildingQueue_;
    bool activeBuildingProcess_ = false;
    int healedThisTurn_ = 0;                          
};
