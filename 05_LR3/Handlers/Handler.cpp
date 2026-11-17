
#include "Handler.h"
#include "GameFacade.h"

#include <iostream>
#include <sstream>
#include <cstdlib>

bool Handler::handle(const std::string& input, GameFacade& facade) {
    if (next_) {
        return next_->handle(input, facade);
    }

    std::cout << "Неизвестная команда. Введите help.\n";
    return false;
}

static bool readXY(const std::string& text, int& x, int& y) {
    char comma = 0;
    std::istringstream in(text);

    return static_cast<bool>(in >> x >> comma >> y) && comma == ',';
}

static bool parseKind(const std::string& s, UnitKind& k) {
    if (s == "swordsman" || s == "мечник")
        k = UnitKind::Swordsman;
    else if (s == "spearman" || s == "копейщик")
        k = UnitKind::Spearman;
    else if (s == "lightarcher" || s == "легкий_лучник")
        k = UnitKind::LightArcher;
    else if (s == "heavyarcher" || s == "тяжелый_лучник")
        k = UnitKind::HeavyArcher;
    else if (s == "cataphract" || s == "катафракт")
        k = UnitKind::Cataphract;
    else if (s == "horsearcher" || s == "конный_лучник")
        k = UnitKind::HorseArcher;
    else if (s == "elfranger" || s == "следопыт")
        k = UnitKind::ElfRanger;
    else if (s == "elfdruid" || s == "друид")
        k = UnitKind::ElfDruid;
    else if (s == "gryphonrider" || s == "грифон")
        k = UnitKind::GryphonRider;
    else
        return false;

    return true;
}

bool CreateHandler::handle(const std::string& input, GameFacade& facade) {
    if (input.compare(0, 11, "create unit") != 0 &&
        input.compare(0, 13, "создать юнита") != 0) {
        return Handler::handle(input, facade);
    }

    std::istringstream in(input);

    std::string a, b, type, at, xy;
    int x = 2, y = 1;

    in >> a >> b >> type;

    UnitKind kind;

    if (!parseKind(type, kind)) {
        std::cout << "Неизвестный тип юнита. Введите help.\n";
        return false;
    }

    if (in >> at >> xy) {
        if (at != "at" && at != "в") {
            std::cout << "Формат: create unit <type> at x,y\n";
            return false;
        }

        if (!readXY(xy, x, y)) {
            return false;
        }
    }

    return facade.createUnit(kind, x, y);
}

bool MoveHandler::handle(const std::string& input, GameFacade& facade) {
    if (input.compare(0, 9, "move unit") != 0) {
        return Handler::handle(input, facade);
    }

    std::istringstream in(input);

    std::string a, b, to, xy;
    int id, x, y;

    in >> a >> b >> id >> to >> xy;

    if (!in || to != "to" || !readXY(xy, x, y)) {
        std::cout << "Формат: move unit <id> to x,y\n";
        return false;
    }

    bool moved = facade.moveUnit(id, x, y);

    if (moved) {
        std::cout << "\nКарта после перемещения\n";
        facade.showMap();
    }

    return moved;
}

bool AttackHandler::handle(const std::string& input, GameFacade& facade) {
    if (input.compare(0, 11, "attack unit") != 0) {
        return Handler::handle(input, facade);
    }

    std::istringstream in(input);

    std::string a, b, target;
    int id;

    if (!(in >> a >> b >> id >> target)) {
        std::cout << "Формат: attack unit <id> target <enemyId> или attack unit <id> base\n";
        return false;
    }

    if (target == "base") {
        return facade.attackBase(id);
    }

    if (target == "target") {
        int targetId;

        if (!(in >> targetId)) {
            std::cout << "Укажите ID вражеского юнита.\n";
            return false;
        }

        return facade.attack(id, targetId);
    }

    std::cout << "Формат: attack unit <id> target <enemyId> или attack unit <id> base\n";
    return false;
}

bool CollectHandler::handle(const std::string& input, GameFacade& facade) {
    if (input.compare(0, 12, "collect unit") != 0) {
        return Handler::handle(input, facade);
    }

    std::istringstream in(input);

    std::string a, b;
    int id;

    in >> a >> b >> id;

    return in && facade.collect(id);
}

bool HealHandler::handle(const std::string& input, GameFacade& facade) {
    if (input.compare(0, 9, "heal unit") != 0) {
        return Handler::handle(input, facade);
    }

    std::istringstream in(input);

    std::string a, b;
    int id;

    in >> a >> b >> id;

    return in && facade.heal(id);
}

bool UseHandler::handle(const std::string& input, GameFacade& facade) {
    if (input.compare(0, 8, "use unit") != 0) {
        return Handler::handle(input, facade);
    }

    std::istringstream in(input);

    std::string a, b;
    int id;

    in >> a >> b >> id;

    return in && facade.useObject(id);
}

bool BuildHandler::handle(const std::string& input, GameFacade& facade) {
    if (input == "build hospital" || input == "построить лечебницу") {
        return facade.buildHospital();
    }

    return Handler::handle(input, facade);
}

bool UpgradeHandler::handle(const std::string& input, GameFacade& facade) {
    if (input == "upgrade hospital" || input == "улучшить лечебницу") {
        return facade.upgradeHospital();
    }

    return Handler::handle(input, facade);
}

bool StatsHandler::handle(const std::string& input, GameFacade& facade) {
    if (input == "stats") {
        std::cout << facade.stats() << "\n";
        return true;
    }

    if (input == "stats units") {
        std::cout << facade.stats("units") << "\n";
        return true;
    }

    if (input == "stats resources") {
        std::cout << facade.stats("resources") << "\n";
        return true;
    }

    if (input == "stats buildings") {
        std::cout << facade.stats("buildings") << "\n";
        return true;
    }

    return Handler::handle(input, facade);
}

bool MapHandler::handle(const std::string& input, GameFacade& facade) {
    if (input == "map") {
        facade.showMap();
        return true;
    }

    return Handler::handle(input, facade);
}

bool TurnHandler::handle(const std::string& input, GameFacade& facade) {
    if (input == "end turn") {
        return facade.endTurn();
    }

    return Handler::handle(input, facade);
}

bool HelpHandler::handle(const std::string& input, GameFacade& facade) {
    if (input == "help") {
        facade.showHelp();
        return true;
    }

    if (input == "history") {
        facade.showHistory();
        return true;
    }

    return Handler::handle(input, facade);
}
