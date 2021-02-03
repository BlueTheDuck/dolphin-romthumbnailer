project(ROMData)
set(PROJECT_VERSION "0.1")
set(PROJECT_VERSION_MAJOR 0)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Gui
)

add_library(romdata STATIC nds.cpp)
target_link_libraries(romdata Qt5::Widgets)
