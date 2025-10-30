#pragma once

#include <string>
#include <memory>
#include <vector>

#include "compiler/CompilerOptions.h"
#include "compiler/ComponentRegistry.h"
#include "frontend/lexer/Lexer.h"
#include "frontend/parser/Parser.h"
#include "frontend/ast/Component.h"
#include "backend/css/CssGenerator.h"
#include "backend/html/HtmlGenerator.h"

namespace artic {

/**
 * @brief Compilation result
 */
struct CompilationResult {
    bool success = false;
    std::string errorMessage;
    std::string generatedCss;
    std::string generatedHtml;
};

/**
 * @brief Artic Compiler - Main orchestrator
 *
 * Connects the entire compilation pipeline:
 * .atc → Lexer → Parser → CSS Generator → HTML Generator → dist/
 *
 * Example usage:
 * ```cpp
 * CompilerOptions options;
 * options.inputFile = "index.atc";
 * options.outputDir = "dist";
 *
 * Compiler compiler(options);
 * bool success = compiler.compile();
 * ```
 */
class Compiler {
public:
    explicit Compiler(const CompilerOptions& options);

    /**
     * @brief Compile .atc file to HTML and CSS
     * @return true if successful, false otherwise
     */
    bool compile();

    /**
     * @brief Compile and return result (without writing to disk)
     * @return CompilationResult with generated code
     */
    CompilationResult compileToString();

    /**
     * @brief Get last error message
     */
    std::string getLastError() const { return m_lastError; }

    /**
     * @brief Get compilation options
     */
    const CompilerOptions& getOptions() const { return m_options; }

private:
    CompilerOptions m_options;
    std::string m_lastError;
    ComponentRegistry m_registry;

    // Pipeline stages
    bool readSourceFile(std::string& source);
    std::vector<Token> lex(const std::string& source);
    std::unique_ptr<Component> parse(const std::vector<Token>& tokens);
    Component* loadComponentWithRegistry(const std::string& filePath);
    std::string generateCss(Component* component);
    std::string generateHtml(Component* component);
    bool writeOutputFiles(const std::string& css, const std::string& html);

    // Logging
    void log(const std::string& message);
    void logError(const std::string& message);
    void logVerbose(const std::string& message);
};

} // namespace artic
