# Artic Compiler Library
add_library(artic_compiler STATIC
    # Frontend - Lexer
    frontend/lexer/Lexer.cpp
    frontend/lexer/Token.cpp

    # Frontend - Parser
    frontend/parser/Parser.cpp
    frontend/parser/ParseError.cpp

    # Frontend - AST
    frontend/ast/Component.cpp
    frontend/ast/Decorator.cpp
    frontend/ast/Import.cpp
    frontend/ast/Props.cpp
    frontend/ast/Statement.cpp
    frontend/ast/Expression.cpp
    frontend/ast/Template.cpp
    frontend/ast/Attribute.cpp
    frontend/ast/Directive.cpp
    frontend/ast/Utility.cpp

    # Frontend - Semantic
    frontend/semantic/SemanticAnalyzer.cpp
    frontend/semantic/ScopeBuilder.cpp
    frontend/semantic/TypeInference.cpp

    # Backend - CSS
    backend/css/CssGenerator.cpp
    backend/css/UtilityResolver.cpp
    backend/css/CssOptimizer.cpp

    # Backend - Client
    backend/client/ClientGenerator.cpp
    backend/client/JsEmitter.cpp
    backend/client/ReactivityTransformer.cpp
    backend/client/TemplateCompiler.cpp
    backend/client/RpcStubGenerator.cpp

    # Backend - Server
    backend/server/ServerGenerator.cpp
    backend/server/CppEmitter.cpp
    backend/server/RouteGenerator.cpp
    backend/server/RpcHandlerGenerator.cpp
    backend/server/SsrGenerator.cpp

    # Shared - Symbols
    shared/symbols/SymbolTable.cpp
    shared/symbols/Scope.cpp
    shared/symbols/Symbol.cpp

    # Shared - Types
    shared/types/PrimitiveType.cpp
    shared/types/ArrayType.cpp
    shared/types/ObjectType.cpp
    shared/types/FunctionType.cpp
    shared/types/UnionType.cpp
    shared/types/TypeChecker.cpp

    # Shared - Diagnostics
    shared/diagnostics/DiagnosticEngine.cpp
    shared/diagnostics/Diagnostic.cpp
    shared/diagnostics/DiagnosticRenderer.cpp

    # Shared - Utils
    shared/utils/FileSystem.cpp
    shared/utils/StringUtils.cpp
    shared/utils/Logger.cpp

    # Compiler
    compiler/Compiler.cpp
    compiler/CompilerOptions.cpp
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
    cli/CommandParser.cpp
    cli/BuildCommand.cpp
    cli/DevCommand.cpp
    cli/CreateCommand.cpp
)

target_link_libraries(artic PRIVATE artic_compiler)

# target_link_libraries(artic PRIVATE
#     CLI11::CLI11
# )
