#pragma once
#include <string>
#include "Base.h"

class BuildingManager {
public:
    explicit BuildingManager(Base& base) : base_(base) {}
    bool buildHospital() { return base_.startHospitalConstruction(); }
    bool upgradeHospital() { return base_.startHospitalUpgrade(); }
    bool healUnit(int id) { return base_.healUnitAtBase(id); }
    std::string getInfo() const { return base_.buildingsInfo(); }
private:
    Base& base_;
};
