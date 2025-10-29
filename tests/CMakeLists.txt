# Artic Test Suite

# Google Test will be added later
# For now, create placeholder

add_executable(artic_tests
    unit/lexer/LexerTest.cpp
    # More tests will be added as we implement modules
)

target_link_libraries(artic_tests PRIVATE
    artic_compiler
    # gtest
    # gtest_main
)

# Add tests to CTest
add_test(NAME artic_tests COMMAND artic_tests)

# Copy test fixtures
file(COPY integration/fixtures DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/integration)
