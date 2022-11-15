cmake_minimum_required(VERSION 3.23)
project(Hanjie)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        Hanjie
        main.cpp
        Button.cpp
        Entry.cpp
        GameBoard.cpp
        Resources.cpp
        OpenSans.cpp
        Cross.cpp
        Rules.cpp
        Sprite.cpp
        TextField.cpp
)

target_link_libraries(Hanjie sfml-graphics sfml-window sfml-system)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    #add_definitions(-DBUILD_SHARED_LIBS=OFF  -DSFML_STATIC)
    #target_link_libraries(stayNormal sfml-graphics sfml-window-s-d sfml-system-s-d -static)
ELSE()
    #target_link_libraries(stayNormal sfml-graphics-s sfml-window-s sfml-system-s -static -s)
ENDIF()
