cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

set(CMAKE_TOOLCHAIN_FILE "/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")
set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "" FORCE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# [PROJECT]
project(random-picker
    VERSION 0.0.1
    LANGUAGES CXX
    DESCRIPTION "A Random Picker to divide people to groups"
    )

# [COMPILER]
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++20")


# [SOURCE]
set(UI_LIST
    src/ui/main.ui
    )

set(SRC_LIST
    ${UI_LIST}
    src/randomizer.h
    src/picker.h
    src/main.cc
    src/gui.h
    src/gui.cc
    )
find_package(Eigen3 CONFIG REQUIRED)

# [QT]
set(CMAKE_PREFIX_PATH "/Packages/Qt/current/mingw_64/lib/cmake")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    )

