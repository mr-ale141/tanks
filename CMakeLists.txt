cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(tanks)

set(SFML_STATIC_LIBRARIES TRUE)

if(WIN32)
    set(SFML_DIR "C:/Program Files (x86)/SFML/")
elseif(UNIX)
    set(SFML_DIR "/home/ale141/SFML-2.6.0/SFML-2.6.0-build/")
else()
    message(CMAKE_ERROR_DEPRECATED "Unknow operetion system")
endif()

find_package(Freetype)
find_package(JPEG)
find_package(SFML 2.6 COMPONENTS window graphics system REQUIRED)

add_executable(
    00
    main.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/headers/wallWood.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/headers/wallMetal.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/headers/wall.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/headers/fire.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/headers/bullet.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/headers/tankUser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/headers/tankEnemy.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/headers/tankEnemyAi.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/headers/tank.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/headers/world.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/wallWood.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/wallMetal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/wall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/fire.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/bullet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/tankUser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/tankEnemy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/tankEnemyAi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/tank.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/world.cpp
)

target_include_directories(00 PRIVATE "${SFML_ROOT}/include")
target_compile_features(00 PUBLIC cxx_std_17)
target_compile_definitions(00 PRIVATE SFML_STATIC)

target_link_libraries(00 sfml-graphics sfml-window sfml-system)