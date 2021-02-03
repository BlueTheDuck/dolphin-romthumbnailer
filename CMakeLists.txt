project(romthumbnailer)

set(PROJECT_VERSION "0.1")
set(PROJECT_VERSION_MAJOR 0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(KF5_MIN_VERSION "5.3.0")
set(QT_MIN_VERSION "5.4.0")

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

include(FeatureSummary) # TODO: What's this?

add_subdirectory(src)

# Tests
option(TESTS "Include tests" ON)
if(TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()

