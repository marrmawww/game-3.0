#pragma once
#include <string>
#include "GameMediator.h"

class Command {
public:
    virtual ~Command() {}
    virtual bool execute() = 0;
    virtual std::string name() const = 0;
};

class CreateUnitCommand : public Command {
public:
    CreateUnitCommand(GameMediator& m, UnitKind k, int x, int y) : m_(m), k_(k), x_(x), y_(y) {}
    bool execute() override { return m_.createUnit(k_, x_, y_); }
    std::string name() const override { return "Создание юнита"; }
private: GameMediator& m_; UnitKind k_; int x_, y_;
};
class MoveUnitCommand : public Command {
public:
    MoveUnitCommand(GameMediator& m, int id, int x, int y) : m_(m), id_(id), x_(x), y_(y) {}
    bool execute() override { return m_.moveUnit(id_, x_, y_); }
    std::string name() const override { return "Перемещение юнита"; }
private: GameMediator& m_; int id_, x_, y_;
};
class AttackCommand : public Command {
public:
    AttackCommand(GameMediator& m, int a, int t) : m_(m), a_(a), t_(t) {}
    bool execute() override { return t_ < 0 ? m_.attackBase(a_) : m_.attack(a_, t_); }
    std::string name() const override { return "Атака"; }
private: GameMediator& m_; int a_, t_;
};
class CollectResourceCommand : public Command {
public:
    CollectResourceCommand(GameMediator& m, int id) : m_(m), id_(id) {}
    bool execute() override { return m_.collectResource(id_); }
    std::string name() const override { return "Сбор ресурсов"; }
private: GameMediator& m_; int id_;
};
class HealCommand : public Command {
public:
    HealCommand(GameMediator& m, int id) : m_(m), id_(id) {}
    bool execute() override { return m_.healUnit(id_); }
    std::string name() const override { return "Лечение"; }
private: GameMediator& m_; int id_;
};
class BuildHospitalCommand : public Command {
public:
    explicit BuildHospitalCommand(GameMediator& m) : m_(m) {}
    bool execute() override { return m_.buildHospital(); }
    std::string name() const override { return "Строительство лечебницы"; }
private: GameMediator& m_;
};
class UpgradeHospitalCommand : public Command {
public:
    explicit UpgradeHospitalCommand(GameMediator& m) : m_(m) {}
    bool execute() override { return m_.upgradeHospital(); }
    std::string name() const override { return "Модернизация лечебницы"; }
private: GameMediator& m_;
};
class UseObjectCommand : public Command {
public:
    UseObjectCommand(GameMediator& m, int id) : m_(m), id_(id) {}
    bool execute() override { return m_.useObject(id_); }
    std::string name() const override { return "Взаимодействие с объектом"; }
private: GameMediator& m_; int id_;
};
class EndTurnCommand : public Command {
public:
    explicit EndTurnCommand(GameMediator& m) : m_(m) {}
    bool execute() override { m_.endTurn(); return true; }
    std::string name() const override { return "Завершение хода"; }
private: GameMediator& m_;
};
