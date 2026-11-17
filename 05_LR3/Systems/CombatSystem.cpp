#include "CombatSystem.h"

bool CombatSystem::attack(int attackerId, int targetId) {
    std::shared_ptr<Unit> attacker = units_.findPlayerUnit(attackerId);
    std::shared_ptr<Unit> target = units_.findEnemyUnit(targetId);
    if (!attacker || !target) return false;
    return game_.attackUnit(*attacker, *target);
}

bool CombatSystem::attackBase(int attackerId) {
    std::shared_ptr<Unit> attacker = units_.findPlayerUnit(attackerId);
    if (!attacker) return false;
    return game_.attackBase(*attacker, enemy_);
}
