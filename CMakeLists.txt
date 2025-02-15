cmake_minimum_required(VERSION 3.16)

project(coursework VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts LinguistTools)
qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES fr de es it)

qt_add_executable(window
main.cpp
sample.cpp
dataset.cpp
model.cpp
mainWindow.cpp
tableWindow.cpp
flourinatedCompounds.cpp
compliance.cpp
complianceItem.cpp
pollutantOverview.cpp
litterindicator.cpp
dashboard.cpp
)

qt_add_translations(window
    TS_FILES
    translations/coursework_fr.ts
    translations/coursework_de.ts
    translations/coursework_es.ts
    translations/coursework_it.ts
)

target_link_libraries(window PRIVATE Qt6::Widgets Qt6::Charts)

set_target_properties(window PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)
