#include "Base.h"
#include <cstdlib>
#include <stdexcept>
#include "Logger.h"
#include "UnitFactory.h"

Base::Base(const std::string& name, char symbol, int x, int y,
           int health, size_t maxUnits, int defenseLevel, const Resources& resources) {
    if (health <= 0) throw std::invalid_argument("База: здоровье должно быть больше 0");
    name_ = name;
    symbol_ = symbol;
    x_ = x;
    y_ = y;
    health_ = health;
    maxHealth_ = health;
    maxUnits_ = maxUnits;
    defenseLevel_ = defenseLevel;
    resources_ = resources;
    aliveCount_ = 0;
}

Base::~Base() {

    for (size_t i = 0; i < units_.size(); ++i) {
        units_[i]->unsubscribe(this);
        units_[i]->setOwner(nullptr);
    }
}

std::shared_ptr<Unit> Base::createUnit(UnitType type, int x, int y) {
    if (isDestroyed()) {
        logMessage("Base", name_ + ": база уничтожена, создание невозможно");
        return std::shared_ptr<Unit>();
    }
    if (aliveCount_ >= maxUnits_) {
        logMessage("Base", name_ + ": достигнут лимит юнитов (" + std::to_string(maxUnits_) + ")");
        return std::shared_ptr<Unit>();
    }
    
    std::shared_ptr<Unit> unit(factoryFor(type).createUnit(type, x, y));
    unit->setOwner(this);
    unit->subscribe(this);      
    units_.push_back(unit);
    unit->announceCreated();    
    return unit;
}

bool Base::removeUnit(int unitId) {
    for (size_t i = 0; i < units_.size(); ++i) {
        if (units_[i]->getId() == unitId) {
            if (units_[i]->isAlive() && aliveCount_ > 0) aliveCount_ = aliveCount_ - 1;
            units_[i]->unsubscribe(this);
            units_[i]->setOwner(nullptr);
            units_.erase(units_.begin() + i);
            return true;
        }
    }
    return false;
}

// Убираем погибших из списка. Делаем это не сразу при гибели, а в конце хода:
// иначе юнит мог бы удалиться, пока он ещё отправляет уведомление.
void Base::purgeDead() {
    size_t i = 0;
    while (i < units_.size()) {
        if (!units_[i]->isAlive()) {
            units_[i]->unsubscribe(this);
            units_[i]->setOwner(nullptr);
            units_.erase(units_.begin() + i);
        } else {
            ++i;
        }
    }
}

void Base::onUnitEvent(const UnitEvent& event) {
    std::string unit = event.unitName + "#" + std::to_string(event.unitId);
    if (event.type == UnitEventType::Created) {
        aliveCount_ = aliveCount_ + 1;
        logMessage("Observer", name_ + ": получил уведомление Created от " + unit +
            ", живых юнитов " + std::to_string(aliveCount_) + "/" + std::to_string(maxUnits_));
    } else if (event.type == UnitEventType::Damaged) {
        logMessage("Observer", name_ + ": " + unit + " получил урон " + std::to_string(event.damage));
    } else {   // Destroyed
        if (aliveCount_ > 0) aliveCount_ = aliveCount_ - 1;
        DestroyedRecord record;
        record.unitId = event.unitId;
        record.unitName = event.unitName;
        record.timestamp = event.timestamp;
        history_.push_back(record);
        logMessage("Observer", name_ + ": получил уведомление Destroyed от " + unit +
            ", живых юнитов " + std::to_string(aliveCount_) + "/" + std::to_string(maxUnits_));
    }
}

void Base::takeDamage(int rawDamage) {
    int damage = rawDamage - defenseLevel_;   // защита базы уменьшает урон
    if (damage < 0) damage = 0;
    health_ = health_ - damage;
    if (health_ < 0) health_ = 0;
    logMessage("Base", name_ + ": удар " + std::to_string(rawDamage) + ", защита " +
        std::to_string(defenseLevel_) + " -> урон " + std::to_string(damage) +
        ", здоровье базы " + std::to_string(health_) + "/" + std::to_string(maxHealth_));
    if (isDestroyed()) logMessage("Base", name_ + ": БАЗА УНИЧТОЖЕНА");
}

// Юниты рядом с базой (до 2 клеток) получают бонус защиты.
int Base::defenseBonusAt(int x, int y) const {
    if (isDestroyed()) return 0;
    int dx = std::abs(x - x_);
    int dy = std::abs(y - y_);
    int distance = dx;
    if (dy > distance) distance = dy;
    if (distance <= 2) return defenseLevel_;
    return 0;
}


bool Base::startHospitalConstruction() {
    if (activeBuildingProcess_) return false;
    if (resources_.gold < 100 || resources_.wood < 50 || resources_.metal < 30) return false;
    for (size_t i = 0; i < builtBuildings_.size(); ++i)
        if (builtBuildings_[i].getName() == "Лечебница") return false;
    resources_.gold -= 100; resources_.wood -= 50; resources_.metal -= 30;
    buildingQueue_.push_back(Building("Лечебница", 7));
    activeBuildingProcess_ = true;
    return true;
}

bool Base::startHospitalUpgrade() {
    if (activeBuildingProcess_) return false;
    if (resources_.gold < 50 || resources_.wood < 25 || resources_.metal < 15) return false;
    for (size_t i = 0; i < builtBuildings_.size(); ++i) {
        if (builtBuildings_[i].getName() == "Лечебница" && builtBuildings_[i].getLevel() == 1) {
            resources_.gold -= 50; resources_.wood -= 25; resources_.metal -= 15;
            if (builtBuildings_[i].startUpgrade(3)) { activeBuildingProcess_ = true; return true; }
            resources_.gold += 50; resources_.wood += 25; resources_.metal += 15;
            return false;
        }
    }
    return false;
}

void Base::advanceBuildingTurn() {
    healedThisTurn_ = 0;
    if (!activeBuildingProcess_) return;
    if (!buildingQueue_.empty()) {
        buildingQueue_[0].advanceTurn();
        if (buildingQueue_[0].isBuilt()) {
            builtBuildings_.push_back(buildingQueue_[0]);
            buildingQueue_.clear();
            activeBuildingProcess_ = false;
        }
        return;
    }
    for (size_t i = 0; i < builtBuildings_.size(); ++i) {
        if (builtBuildings_[i].isUpgrading()) {
            builtBuildings_[i].advanceTurn();
            if (!builtBuildings_[i].isUpgrading()) activeBuildingProcess_ = false;
            return;
        }
    }
    activeBuildingProcess_ = false;
}

bool Base::healUnitAtBase(int unitId) {
    int capacity = 0;
    for (size_t i = 0; i < builtBuildings_.size(); ++i) {
        if (builtBuildings_[i].getName() == "Лечебница" && builtBuildings_[i].isBuilt()) {
            capacity = builtBuildings_[i].getLevel();
            break;
        }
    }
    if (capacity == 0 || healedThisTurn_ >= capacity) return false;
    for (size_t i = 0; i < units_.size(); ++i) {
        int dx = std::abs(units_[i]->getX() - x_);
        int dy = std::abs(units_[i]->getY() - y_);
        if (units_[i]->getId() == unitId && units_[i]->isAlive() && dx <= 1 && dy <= 1) {
            units_[i]->heal(units_[i]->getMaxHealth());
            ++healedThisTurn_;
            return true;
        }
    }
    return false;
}

std::string Base::resourcesInfo() const {
    return "Золото: " + std::to_string(resources_.gold) +
        "\nДерево: " + std::to_string(resources_.wood) +
        "\nМеталл: " + std::to_string(resources_.metal);
}

std::string Base::buildingsInfo() const {
    std::string result = "Построенные здания:\n";
    if (builtBuildings_.empty()) result += "- Нет\n";
    for (size_t i = 0; i < builtBuildings_.size(); ++i)
        result += "- " + builtBuildings_[i].getName() + " (уровень " +
            std::to_string(builtBuildings_[i].getLevel()) + ")\n";
    result += "Строятся:\n";
    if (buildingQueue_.empty() && !activeBuildingProcess_) result += "- Нет\n";
    for (size_t i = 0; i < buildingQueue_.size(); ++i)
        result += "- " + buildingQueue_[i].getName() + " — осталось " +
            std::to_string(buildingQueue_[i].getConstructionTurnsRemaining()) + " ходов\n";
    for (size_t i = 0; i < builtBuildings_.size(); ++i)
        if (builtBuildings_[i].isUpgrading()) result += "- Модернизация " + builtBuildings_[i].getName() +
            " — осталось " + std::to_string(builtBuildings_[i].getUpgradeTurnsRemaining()) + " ходов\n";
    return result;
}
