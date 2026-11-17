#pragma once
#include "Armor.h"


class Attack {
public:
    explicit Attack(int value) {
        value_ = value < 0 ? 0 : value;
    }

    int calculateDamage(const Armor& armor) const {
        return armor.reduceDamage(value_);
    }

    int getValue() const { return value_; }

private:
    int value_;
};
