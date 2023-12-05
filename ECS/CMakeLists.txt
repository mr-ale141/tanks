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

add_subdirectory(flecs-master)

add_executable(
    Tanks
        main.cpp
        utils/initWorld.h
        systems/moving/moving.h
        systems/render/render.h
        systems/control/control.h)

target_include_directories(Tanks PRIVATE "${SFML_ROOT}/include")
target_compile_features(Tanks PUBLIC cxx_std_17)
target_compile_definitions(Tanks PRIVATE SFML_STATIC)

target_link_libraries(Tanks flecs::flecs_static)
target_link_libraries(Tanks sfml-graphics sfml-window sfml-system)