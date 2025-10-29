# Artic Compiler Library
# Phase 1: Only Lexer is implemented
add_library(artic_compiler STATIC
    # Frontend - Lexer
    frontend/lexer/Lexer.cpp
    frontend/lexer/Token.cpp

    # More modules will be added as we implement them
    # frontend/parser/Parser.cpp
    # frontend/ast/Component.cpp
    # backend/css/CssGenerator.cpp
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
