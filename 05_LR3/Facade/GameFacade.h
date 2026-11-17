#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Command.h"

class Handler;
class GameFacade {
public:
    explicit GameFacade(GameMediator& mediator);

    // Фасад владеет цепочкой и историей (unique_ptr) - копировать его нельзя
    GameFacade(const GameFacade&) = delete;
    GameFacade& operator=(const GameFacade&) = delete;
    bool createUnit(UnitKind kind, int x, int y);
    bool moveUnit(int id, int x, int y);
    bool attack(int attacker, int target);
    bool attackBase(int attacker);
    bool collect(int id);
    bool heal(int id);
    bool useObject(int id);
    bool buildHospital();
    bool upgradeHospital();
    bool endTurn();
    std::string stats(const std::string& section = "all") const;
    void showMap() const;
    bool processInput(const std::string& input);
    void showHelp() const;
    void showHistory() const;
private:
    bool run(Command& command);
    GameMediator& mediator_;
    std::shared_ptr<Handler> chain_;
    std::vector<std::string> history_;
};
