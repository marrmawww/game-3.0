#include "GameFacade.h"
#include "Handler.h"
#include <iostream>

GameFacade::GameFacade(GameMediator& mediator) : mediator_(mediator) {
    std::shared_ptr<Handler> create(new CreateHandler());
    std::shared_ptr<Handler> move(new MoveHandler());
    std::shared_ptr<Handler> attack(new AttackHandler());
    std::shared_ptr<Handler> collect(new CollectHandler());
    std::shared_ptr<Handler> heal(new HealHandler());
    std::shared_ptr<Handler> use(new UseHandler());
    std::shared_ptr<Handler> build(new BuildHandler());
    std::shared_ptr<Handler> upgrade(new UpgradeHandler());
    std::shared_ptr<Handler> statsHandler(new StatsHandler());
    std::shared_ptr<Handler> map(new MapHandler());
    std::shared_ptr<Handler> turn(new TurnHandler());
    std::shared_ptr<Handler> help(new HelpHandler());
    create->setNext(move); move->setNext(attack); attack->setNext(collect);
    collect->setNext(heal); heal->setNext(use); use->setNext(build);
    build->setNext(upgrade); upgrade->setNext(statsHandler); statsHandler->setNext(map);
    map->setNext(turn); turn->setNext(help); chain_ = create;
}

bool GameFacade::run(Command& command) {
    bool result = command.execute();
    if (result) history_.push_back(command.name());
    std::cout << (result ? "Выполнено: " : "Не удалось выполнить: ") << command.name() << "\n";
    return result;
}
bool GameFacade::createUnit(UnitKind k, int x, int y) { CreateUnitCommand c(mediator_, k, x, y); return run(c); }
bool GameFacade::moveUnit(int id, int x, int y) { MoveUnitCommand c(mediator_, id, x, y); return run(c); }
bool GameFacade::attack(int a, int t) { AttackCommand c(mediator_, a, t); return run(c); }
bool GameFacade::attackBase(int a) { AttackCommand c(mediator_, a, -1); return run(c); }
bool GameFacade::collect(int id) { CollectResourceCommand c(mediator_, id); return run(c); }
bool GameFacade::heal(int id) { HealCommand c(mediator_, id); return run(c); }
bool GameFacade::useObject(int id) { UseObjectCommand c(mediator_, id); return run(c); }
bool GameFacade::buildHospital() { BuildHospitalCommand c(mediator_); return run(c); }
bool GameFacade::upgradeHospital() { UpgradeHospitalCommand c(mediator_); return run(c); }
bool GameFacade::endTurn() { EndTurnCommand c(mediator_); return run(c); }
std::string GameFacade::stats(const std::string& section) const { return mediator_.stats(section); }
void GameFacade::showMap() const { mediator_.showMap(); }
bool GameFacade::processInput(const std::string& input) { return chain_->handle(input, *this); }
void GameFacade::showHelp() const {
    std::cout << "Команды:\n"
      << " create unit <type> at <x>,<y>\n move unit <id> to <x>,<y>\n"
      << " attack unit <id> target <enemyId> | attack unit <id> base\n"
      << " collect unit <id>  (юнит должен стоять на сундуке)\n"
      << " heal unit <id> | use unit <id>\n build hospital | upgrade hospital\n"
      << " stats [units|resources|buildings] | map | end turn | history | help | exit\n"
      << "Типы: swordsman, spearman, lightarcher, heavyarcher, cataphract, horsearcher, elfranger, elfdruid, gryphonrider\n";
}
void GameFacade::showHistory() const {
    if (history_.empty()) { std::cout << "История пока пуста.\n"; return; }
    for (size_t i = 0; i < history_.size(); ++i) std::cout << i + 1 << ". " << history_[i] << "\n";
}
