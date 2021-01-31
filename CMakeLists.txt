project(romthumbnailer)

set(PROJECT_VERSION "0.1")
set(PROJECT_VERSION_MAJOR 0)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

include(FeatureSummary) # TODO: What's this?

set(QT_MIN_VERSION "5.4.0")
set(KF5_MIN_VERSION "5.3.0")

# Find required packages (Qt5 and KDE Framework 5)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Gui
)

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    KIO
    Config
)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary) # TODO: Really, what's this?
include_directories(${KF5_INCLUDES})

# Configuration UI
set (Settings_src ${PROJECT_SOURCE_DIR}/src/configui.cpp)
qt5_wrap_cpp(Settings_hdr_moc ${PROJECT_SOURCE_DIR}/src/configui.h)
qt5_wrap_ui (Settings_ui_moc  ${PROJECT_SOURCE_DIR}/src/configui.ui)

set(romthumbnailer_SRCS
    src/main.cpp
    src/romthumbnailer.cpp
    src/nds.cpp
    ${Settings_src}
    ${Settings_hdr_moc}
    ${Settings_ui_moc}
)
kconfig_add_kcfg_files(romthumbnailer_SRCS configdata.kcfgc)

add_library(romthumbnailer MODULE ${romthumbnailer_SRCS})
target_link_libraries(romthumbnailer PRIVATE
    KF5::KIOWidgets
    KF5::ConfigCore
    KF5::ConfigGui
    Qt5::Widgets
)

# Tests
option(TESTS "Include tests" ON)
if(TESTS)
    include(GoogleTest)
    add_subdirectory(tests)
    #include(tests)
    #find_package(GTest)
    #add_executable(FooTest test/tests.cpp)
    #gtest_add_tests(FooTest "" AUTO)
    #target_link_libraries(FooTest gtest gtest_main Qt5::Widgets)
endif()
    
install(TARGETS romthumbnailer DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES romthumbnailer.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES configdata.kcfgc DESTINATION ${KCFG_INSTALL_DIR})
