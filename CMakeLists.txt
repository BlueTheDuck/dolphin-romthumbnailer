cmake_minimum_required(VERSION 3.31)
project(ndsromthumbnail)

set(KF_MIN_VERSION "6.0.0")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMOptionalAddSubdirectory)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(ECMDeprecationSettings)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Gui)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS KIO)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS Archive)


kcoreaddons_add_plugin(ndsromthumbnail INSTALL_NAMESPACE "kf6/thumbcreator")

target_sources(ndsromthumbnail PRIVATE
src/nds.cpp
    src/thumbnailer.cpp
)

include(ECMQtDeclareLoggingCategory)

ecm_qt_declare_logging_category(ndsromthumbnail
    HEADER debug.h
    IDENTIFIER LOG_NDSROMTHUMBNAILER
    CATEGORY_NAME org.ndsromthumbnailer.thumbnail
    DESCRIPTION "ndsromthumbnailer thumbnail"
    EXPORT ndsromthumbnail
)

target_link_libraries(ndsromthumbnail
    KF6::KIOCore
    KF6::KIOWidgets
    KF6::Archive
    Qt::Core
)

# kconfig_add_kcfg_files(ndsromthumbnail ndsthumbnailersettings.kcfgc)
# install(FILES ndsthumbnailersettings.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})

# ecm_qt_install_logging_categories(
#   EXPORT ndsromthumbnail
#   DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
# )

install(FILES org.ndsromthumbnailer.metainfo.xml
        DESTINATION ${KDE_INSTALL_METAINFODIR})
        
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
