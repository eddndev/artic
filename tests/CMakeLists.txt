# Artic Test Suite

# Manual tests for now (Google Test will be integrated later)

# Lexer tests
add_executable(lexer_tests
    unit/lexer/LexerTest.cpp
)

target_link_libraries(lexer_tests PRIVATE
    artic_compiler
)

# Parser tests
add_executable(parser_tests
    unit/parser/ParserTest.cpp
)

target_link_libraries(parser_tests PRIVATE
    artic_compiler
)

# CSS Generator tests
add_executable(css_generator_tests
    unit/backend/CssGeneratorTest.cpp
)

target_link_libraries(css_generator_tests PRIVATE
    artic_compiler
)

# HTML Generator tests
add_executable(html_generator_tests
    unit/backend/HtmlGeneratorTest.cpp
)

target_link_libraries(html_generator_tests PRIVATE
    artic_compiler
)

# Compiler tests
add_executable(compiler_tests
    unit/compiler/CompilerTest.cpp
)

target_link_libraries(compiler_tests PRIVATE
    artic_compiler
)

# Add tests to CTest
add_test(NAME LexerTests COMMAND lexer_tests)
add_test(NAME ParserTests COMMAND parser_tests)
add_test(NAME CssGeneratorTests COMMAND css_generator_tests)
add_test(NAME HtmlGeneratorTests COMMAND html_generator_tests)
add_test(NAME CompilerTests COMMAND compiler_tests)
