
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(dsa-tests ${TEST_SOURCES})

target_link_libraries(dsa-tests PRIVATE Catch2::Catch2WithMain)

target_include_directories(dsa-tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

include(CTest)
include(Catch)
catch_discover_tests(dsa-tests)

