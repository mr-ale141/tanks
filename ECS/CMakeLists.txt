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
    ${PROJECT_NAME}
    main.cpp
        init/initWorld.h
        systems/control/control.h
        systems/moving/moving.h
        systems/collision/collision.h
        systems/render/render.h
        init/utils.h
        init/components.h
        init/config.h)

target_link_libraries(${PROJECT_NAME} flecs::flecs_static)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

#set(CMAKE_BUILD_TYPE Debug)
#or
#set(CMAKE_BUILD_TYPE Release)

#or
#file(GLOB CPPS "*.cpp" CS "*.c")
#add_executable(${PROJECT_NAME} ${CPPS} ${CS})
#or
#set(SOURCES main.cpp)
#add_executable(${PROJECT_NAME} ${SOURCES})

#set(CMAKE_CXX_STANDARD 17)
#or
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y") #last version
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")