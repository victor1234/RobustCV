include(CTest)
include(Catch)

add_executable(unit_tests Matrix_tests.cpp io/load_image_tests.cpp)

target_link_libraries(unit_tests PRIVATE ${RCV_TARGET_NAME})
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

# Copy fixtures
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/fixtures/6.jpg
    ${CMAKE_CURRENT_BINARY_DIR}/fixtures/6.jpg
    COPYONLY
)

# Discover tests
catch_discover_tests(unit_tests)
