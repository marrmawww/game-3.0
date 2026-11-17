#include <iostream>
#include <sstream>
#include "GameController.h"
#include "GameMediator.h"
#include "GameFacade.h"

int main() {
    GameController game(12, 8);
    Base& player = game.addBase("Тестовая база", 'B', 1, 1, 100, 10, 2);
    Base& enemy = game.addBase("Вражеская база", 'R', 10, 6, 100, 10, 2);
    player.addResources(Resources(300, 150, 100, 0));
    game.spawnUnit(enemy, UnitKind::Swordsman, 8, 6);
    GameMediator mediator(game, player, enemy);
    GameFacade facade(mediator);

    int passed = 0;
    if (facade.processInput("create unit swordsman at 2,1")) ++passed;
    if (facade.processInput("stats units")) ++passed;
    if (facade.processInput("build hospital")) ++passed;
    if (player.getResources().gold == 200 && player.getResources().wood == 100 && player.getResources().metal == 70) ++passed;
    if (facade.processInput("end turn")) ++passed;
    if (facade.processInput("history")) ++passed;

    std::cout << "Проверок ЛР3: 6, успешно: " << passed << ", провалено: " << (6 - passed) << "\n";
    return passed == 6 ? 0 : 1;
}
