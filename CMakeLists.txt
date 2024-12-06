cmake_minimum_required(VERSION 3.16)

project(coursework VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_PREFIX_PATH "/usr/lib/qt6")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)
qt_standard_project_setup()

qt_add_executable(window
main.cpp
sample.cpp
sample.hpp
dataset.cpp
dataset.hpp
model.cpp
model.hpp
mainWindow.hpp
mainWindow.cpp
tableWindow.hpp
tableWindow.cpp
flourinatedCompounds.hpp
flourinatedCompounds.cpp
csv.hpp
compliance.cpp
compliance.hpp
complianceItem.cpp
complianceItem.hpp
)

target_link_libraries(window PRIVATE Qt6::Widgets Qt6::Charts)

set_target_properties(window PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)