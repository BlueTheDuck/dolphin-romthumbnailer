### Find required packages (Qt5 and KDE Framework 5)
# Configure Qt5's CMake
set(CMAKE_AUTOUIC ON)

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

### Configure building sources and components
# ROMData library
add_subdirectory(romdata)

set(romthumbnailer_SRCS
    main.cpp
    romthumbnailer.cpp
    configui.cpp
    configui.h
    configui.ui
)
add_library(romthumbnailer MODULE ${romthumbnailer_SRCS})
kconfig_add_kcfg_files(romthumbnailer ${PROJECT_SOURCE_DIR}/configdata.kcfgc)
target_link_libraries(romthumbnailer PRIVATE
    KF5::KIOWidgets
    KF5::ConfigCore
    KF5::ConfigGui
    Qt5::Widgets
    romdata
)

install(TARGETS romthumbnailer DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES ${PROJECT_SOURCE_DIR}/romthumbnailer.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES ${PROJECT_SOURCE_DIR}/configdata.kcfgc DESTINATION ${KCFG_INSTALL_DIR})
