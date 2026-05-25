cmake_minimum_required(VERSION 3.10)
project(DungeonGame)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dungeon_game
    main.cpp
    game.cpp
    room.cpp
    person.cpp
    resource.cpp
    parser.cpp
    DefaultAlgorithm.cpp
)

target_sources(dungeon_game PRIVATE
    game.h room.h person.h resource.h parser.h IAlgorithm.h DefaultAlgorithm.h OutputRedirector.h
)

# Копируем in.txt в папку с исполняемым файлом после сборки
add_custom_command(TARGET dungeon_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/in.txt
        $<TARGET_FILE_DIR:dungeon_game>/in.txt
)