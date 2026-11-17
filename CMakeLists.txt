cmake_minimum_required(VERSION 3.12)
project(LR3_GameProject LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    01_Core 02_Units 03_Terrain 04_NeutralObjects
    05_LR3/Buildings 05_LR3/Systems 05_LR3/Commands
    05_LR3/Facade 05_LR3/Handlers
)

set(COMMON_SOURCES
    01_Core/Base.cpp 01_Core/GameController.cpp 01_Core/Logger.cpp
    02_Units/Unit.cpp 02_Units/UnitFactory.cpp
    03_Terrain/TerrainMap.cpp 03_Terrain/TerrainProxy.cpp
    04_NeutralObjects/Strategies.cpp
    05_LR3/Buildings/Building.cpp
    05_LR3/Systems/UnitManager.cpp 05_LR3/Systems/ResourceSystem.cpp
    05_LR3/Systems/CombatSystem.cpp 05_LR3/Systems/BuildingManager.cpp
    05_LR3/Systems/GameMediator.cpp
    05_LR3/Commands/Command.cpp
    05_LR3/Facade/GameFacade.cpp
    05_LR3/Handlers/Handler.cpp
)

add_executable(game 06_Run/main.cpp ${COMMON_SOURCES})
add_executable(tests_lab2 07_Tests/tests_lab2.cpp ${COMMON_SOURCES})
add_executable(tests_lab3 07_Tests/tests_lab3.cpp ${COMMON_SOURCES})
