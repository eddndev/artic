# Artic Test Suite

# Manual tests for now (Google Test will be integrated later)

add_executable(artic_tests
    unit/lexer/LexerTest.cpp
)

target_link_libraries(artic_tests PRIVATE
    artic_compiler
)

# Add tests to CTest
add_test(NAME LexerTests COMMAND artic_tests)
