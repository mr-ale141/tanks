cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(tanks)

set(SFML_STATIC_LIBRARIES TRUE)

if(WIN32)
    set(SFML_DIR "C:/Program Files (x86)/SFML/")
elseif(UNIX)
    set(SFML_DIR "/home/ale141/SFML-2.6.0/SFML-2.6.0-build/")
else()
    message(CMAKE_ERROR_DEPRECATED "Unknown operation system")
endif()

find_package(Freetype)
find_package(JPEG)
find_package(SFML 2.6 COMPONENTS window graphics system REQUIRED)

add_executable(
    00
        main.cpp
        libs/headers/wallWood.hpp
        libs/headers/wallMetal.hpp
        libs/headers/wall.hpp
        libs/headers/fire.hpp
        libs/headers/bullet.hpp
        libs/headers/tankUser.hpp
        libs/headers/tankEnemy.hpp
        libs/headers/tankEnemyAi.hpp
        libs/headers/tank.hpp
        libs/headers/world.hpp
        libs/wallWood.cpp
        libs/wallMetal.cpp
        libs/wall.cpp
        libs/fire.cpp
        libs/bullet.cpp
        libs/tankUser.cpp
        libs/tankEnemy.cpp
        libs/tankEnemyAi.cpp
        libs/tank.cpp
        libs/world.cpp
)

target_include_directories(00 PRIVATE "${SFML_ROOT}/include")
target_compile_features(00 PUBLIC cxx_std_17)
target_compile_definitions(00 PRIVATE SFML_STATIC)

target_link_libraries(00 sfml-graphics sfml-window sfml-system)