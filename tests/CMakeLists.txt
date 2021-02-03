find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Gui
)

add_executable(NDS_TEST nds.cpp)
target_link_libraries(NDS_TEST PUBLIC
    gtest
    gtest_main
    Qt5::Gui
    romdata
)
# gtest_discover_tests replaces gtest_add_tests,
# see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
gtest_discover_tests(NDS_TEST
    # set a working directory so your project root so that you can find test data via paths relative to the project root
    WORKING_DIRECTORY ${PROJECT_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
)
set_target_properties(NDS_TEST PROPERTIES FOLDER tests)
