include(CTest)
include(Catch)

add_executable(unit_tests Matrix_tests.cpp)

target_link_libraries(unit_tests PRIVATE ${RCV_TARGET_NAME})
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

# Discover tests
catch_discover_tests(unit_tests)
