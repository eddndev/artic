# Artic Compiler Library
# Phase 1: Lexer, Parser, CSS Generator, and HTML Generator implemented
add_library(artic_compiler STATIC
    # Frontend - Lexer
    frontend/lexer/Lexer.cpp
    frontend/lexer/Token.cpp

    # Frontend - Parser
    frontend/parser/Parser.cpp

    # Note: AST classes are header-only (no .cpp files needed)

    # Backend - CSS Generator
    backend/css/UtilityResolver.cpp
    backend/css/CssGenerator.cpp

    # Backend - HTML Generator
    backend/html/HtmlGenerator.cpp

    # More modules will be added as we implement them
    # compiler/Compiler.cpp
    # etc...
)

target_include_directories(artic_compiler PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

# Link dependencies
# target_link_libraries(artic_compiler PUBLIC
#     Boost::system
#     spdlog::spdlog
#     nlohmann_json::nlohmann_json
# )

# CLI Executable
add_executable(artic
    cli/main.cpp
    # More CLI modules will be added later
    # cli/CommandParser.cpp
    # cli/BuildCommand.cpp
    # etc...
)

target_link_libraries(artic PRIVATE artic_compiler)

# target_link_libraries(artic PRIVATE
#     CLI11::CLI11
# )
