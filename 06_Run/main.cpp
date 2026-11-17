
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

#include "GameController.h"
#include "GameMediator.h"
#include "GameFacade.h"
#include "NeutralOperators.h"
#include "Logger.h"

static std::vector<std::string> makeMap() {
    std::vector<std::string> rows;

    rows.push_back("....................");
    rows.push_back("....***.............");
    rows.push_back("....***......^^.....");
    rows.push_back(".............^^.....");
    rows.push_back("..~~~...............");
    rows.push_back("..~~~....***........");
    rows.push_back("..........***.......");
    rows.push_back("....................");
    rows.push_back("..............~~~...");
    rows.push_back("..............~~~...");
    rows.push_back("....................");
    rows.push_back("....................");

    return rows;
}

void printUnitCatalog() {
    std::cout << "\nДоступные юниты\n\n";

    std::cout << std::left
        << std::setw(17) << "Юнит"
        << std::setw(10) << "Здоровье"
        << std::setw(9) << "Броня"
        << std::setw(9) << "Атака"
        << std::setw(11) << "Скорость"
        << std::setw(10) << "Дальность"
        << "\n";

    std::cout << "\nПехота\n";

    std::cout << std::setw(17) << "Swordsman"
        << std::setw(10) << 100 << std::setw(9) << 5
        << std::setw(9) << 15 << std::setw(11) << 2
        << std::setw(10) << 1 << "\n";

    std::cout << std::setw(17) << "Spearman"
        << std::setw(10) << 90 << std::setw(9) << 6
        << std::setw(9) << 14 << std::setw(11) << 2
        << std::setw(10) << 1 << "\n";

    std::cout << "\nЛучники\n";

    std::cout << std::setw(17) << "LightArcher"
        << std::setw(10) << 60 << std::setw(9) << 1
        << std::setw(9) << 12 << std::setw(11) << 2
        << std::setw(10) << 5 << "\n";

    std::cout << std::setw(17) << "HeavyArcher"
        << std::setw(10) << 80 << std::setw(9) << 3
        << std::setw(9) << 20 << std::setw(11) << 1
        << std::setw(10) << 7 << "\n";

    std::cout << "\nКонница\n";

    std::cout << std::setw(17) << "Cataphract"
        << std::setw(10) << 150 << std::setw(9) << 10
        << std::setw(9) << 25 << std::setw(11) << 3
        << std::setw(10) << 1 << "\n";

    std::cout << std::setw(17) << "HorseArcher"
        << std::setw(10) << 90 << std::setw(9) << 2
        << std::setw(9) << 15 << std::setw(11) << 4
        << std::setw(10) << 1 << "\n";

    std::cout << "\nЭльфы\n";

    std::cout << std::setw(17) << "ElfRanger"
        << std::setw(10) << 85 << std::setw(9) << 3
        << std::setw(9) << 19 << std::setw(11) << 3
        << std::setw(10) << 1 << "\n";

    std::cout << std::setw(17) << "ElfDruid"
        << std::setw(10) << 110 << std::setw(9) << 5
        << std::setw(9) << 16 << std::setw(11) << 2
        << std::setw(10) << 1 << "\n";

    std::cout << std::setw(17) << "GryphonRider"
        << std::setw(10) << 90 << std::setw(9) << 3
        << std::setw(9) << 17 << std::setw(11) << 4
        << std::setw(10) << 1 << "\n";

    std::cout << "\nСоздание юнита: create unit <type> at <x>,<y>\n";
}

static void runDemo(GameFacade& game) {
    const char* scenario1[] = {
        "stats",
        "create unit swordsman at 2,1",
        "create unit lightarcher at 2,2",
        "stats units",
        "map"
    };

    std::cout << "\n создание юнитов и просмотр состояния\n";

    for (size_t i = 0; i < sizeof(scenario1) / sizeof(scenario1[0]); ++i) {
        std::cout << "> " << scenario1[i] << "\n";
        game.processInput(scenario1[i]);
    }

    const char* scenario2[] = {
        "move unit 2 to 4,1",
        "end turn",
        "move unit 2 to 4,1",
        "end turn",
        "collect unit 2",
        "stats resources"
    };

    std::cout << "\n перемещение и сбор ресурсов\n";

    for (size_t i = 0; i < sizeof(scenario2) / sizeof(scenario2[0]); ++i) {
        std::cout << "> " << scenario2[i] << "\n";
        game.processInput(scenario2[i]);
    }

    const char* scenario3[] = {
        "build hospital",
        "stats buildings",
        "end turn",
        "end turn",
        "end turn",
        "end turn",
        "end turn",
        "end turn",
        "end turn",
        "upgrade hospital",
        "stats",
        "history"
    };

    std::cout << "\nстроительство и модернизация лечебницы\n";

    for (size_t i = 0; i < sizeof(scenario3) / sizeof(scenario3[0]); ++i) {
        std::cout << "> " << scenario3[i] << "\n";
        game.processInput(scenario3[i]);
    }
}

int main(int argc, char** argv) {

#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    openLog("game.log");

    printUnitCatalog();

    GameController controller(20, 12);
    controller.map().loadFromText(makeMap());

    controller.map().placeNeutralObject(
        4, 1,
        std::unique_ptr<NeutralObject>(
            new TreasureChest(Resources(50, 20, 10, 0))
        )
    );

    controller.map().placeNeutralObject(
        7, 3,
        std::unique_ptr<NeutralObject>(new Altar())
    );

    controller.map().placeNeutralObject(
        6, 3,
        std::unique_ptr<NeutralObject>(new Trap(20))
    );

    Base& player = controller.addBase(
        "Главная база", 'B', 1, 1, 100, 12, 2
    );

    Base& enemy = controller.addBase(
        "Вражеская база", 'R', 17, 9, 100, 12, 2
    );

    player.addResources(Resources(300, 150, 100, 0));
    enemy.addResources(Resources(100, 50, 30, 0));

    controller.spawnUnit(
        enemy, UnitKind::Swordsman, 5, 4
    );

    GameMediator mediator(controller, player, enemy);
    GameFacade facade(mediator);

    bool demo = argc > 1 && std::string(argv[1]) == "--demo";

    std::cout << "\nКарта местности в начале игры\n";
    controller.printMap(std::cout);

    if (demo) {
        runDemo(facade);
    }
    else {
        std::cout << "\nУправление игровой базой\n";
        std::cout << "Введите help для списка команд, exit для выхода.\n";

        std::string line;

        while (std::cout << "\n> " && std::getline(std::cin, line)) {
            if (line == "exit" || line == "quit") {
                break;
            }

            if (!line.empty()) {
                facade.processInput(line);
            }
        }
    }

    closeLog();

    std::cout << "\nПрограмма завершена. Журнал событий сохранён в game.log.\n";

    return 0;
}
