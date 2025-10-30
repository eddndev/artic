#include "compiler/Compiler.h"
#include "shared/utils/FileSystem.h"
#include "frontend/parser/ParseError.h"
#include <iostream>

namespace artic {

Compiler::Compiler(const CompilerOptions& options)
    : m_options(options) {}

bool Compiler::compile() {
    log("Starting compilation...");

    // Validate options
    if (!m_options.validate()) {
        logError("Invalid compiler options");
        return false;
    }

    logVerbose("Input file: " + m_options.inputFile);
    logVerbose("Output directory: " + m_options.outputDir);

    // 1. Load component with registry (this also loads all imports)
    log("Lexing...");
    log("Parsing...");
    Component* component = loadComponentWithRegistry(m_options.inputFile);

    if (!component) {
        logError("Failed to load component");
        return false;
    }

    logVerbose("Component loaded successfully");
    logVerbose("  Utilities: " + std::to_string(component->utilities.size()));
    logVerbose("  Template nodes: " + std::to_string(component->templateNodes.size()));
    logVerbose("  Imported components: " + std::to_string(m_registry.getAllComponents().size() - 1));

    // 2. Generate CSS
    log("Generating CSS...");
    std::string css;
    try {
        css = generateCss(component);
    } catch (const std::exception& e) {
        logError("CSS generation error: " + std::string(e.what()));
        return false;
    }

    logVerbose("Generated " + std::to_string(css.size()) + " bytes of CSS");

    // 3. Generate HTML
    log("Generating HTML...");
    std::string html;
    try {
        html = generateHtml(component);
    } catch (const std::exception& e) {
        logError("HTML generation error: " + std::string(e.what()));
        return false;
    }

    logVerbose("Generated " + std::to_string(html.size()) + " bytes of HTML");

    // 6. Write output files
    log("Writing output files...");
    if (!writeOutputFiles(css, html)) {
        return false;
    }

    log("Compilation successful!");
    log("  CSS: " + m_options.getCssOutputPath().string());
    log("  HTML: " + m_options.getHtmlOutputPath().string());

    return true;
}

CompilationResult Compiler::compileToString() {
    CompilationResult result;

    // Load component with registry
    Component* component = loadComponentWithRegistry(m_options.inputFile);

    if (!component) {
        result.errorMessage = "Failed to load component";
        return result;
    }

    // Generate CSS
    try {
        result.generatedCss = generateCss(component);
    } catch (const std::exception& e) {
        result.errorMessage = "CSS generation error: " + std::string(e.what());
        return result;
    }

    // Generate HTML
    try {
        result.generatedHtml = generateHtml(component);
    } catch (const std::exception& e) {
        result.errorMessage = "HTML generation error: " + std::string(e.what());
        return result;
    }

    result.success = true;
    return result;
}

bool Compiler::readSourceFile(std::string& source) {
    auto inputPath = m_options.getInputPath();

    // Check if file exists
    if (!FileSystem::fileExists(inputPath)) {
        logError("Input file not found: " + inputPath.string());
        return false;
    }

    // Check extension
    if (FileSystem::getExtension(inputPath) != ".atc") {
        logError("Input file must have .atc extension");
        return false;
    }

    // Read file
    auto contents = FileSystem::readFile(inputPath);
    if (!contents.has_value()) {
        logError("Failed to read input file: " + inputPath.string());
        return false;
    }

    source = contents.value();
    return true;
}

std::vector<Token> Compiler::lex(const std::string& source) {
    Lexer lexer(source);
    return lexer.tokenize();
}

std::unique_ptr<Component> Compiler::parse(const std::vector<Token>& tokens) {
    Parser parser(tokens);
    return parser.parse();
}

Component* Compiler::loadComponentWithRegistry(const std::string& filePath) {
    return m_registry.loadComponent(filePath);
}

std::string Compiler::generateCss(Component* component) {
    CssGeneratorOptions cssOptions;
    cssOptions.minify = m_options.minifyCss;
    cssOptions.pretty = m_options.prettyPrint && !m_options.minifyCss;

    CssGenerator generator(cssOptions);
    return generator.generate(component);
}

std::string Compiler::generateHtml(Component* component) {
    HtmlGeneratorOptions htmlOptions;
    htmlOptions.pretty = m_options.prettyPrint && !m_options.minifyHtml;
    htmlOptions.includeDoctype = m_options.includeDoctype;
    htmlOptions.indentSize = m_options.indentSize;
    htmlOptions.minify = m_options.minifyHtml;

    // Pass registry to enable component expansion
    HtmlGenerator generator(htmlOptions, &m_registry);
    return generator.generate(component);
}

bool Compiler::writeOutputFiles(const std::string& css, const std::string& html) {
    auto outputPath = m_options.getOutputPath();

    // Create output directory
    if (!FileSystem::directoryExists(outputPath)) {
        logVerbose("Creating output directory: " + outputPath.string());
        if (!FileSystem::createDirectory(outputPath)) {
            logError("Failed to create output directory: " + outputPath.string());
            return false;
        }
    }

    // Write CSS file
    auto cssPath = m_options.getCssOutputPath();
    logVerbose("Writing CSS to: " + cssPath.string());
    if (!FileSystem::writeFile(cssPath, css)) {
        logError("Failed to write CSS file: " + cssPath.string());
        return false;
    }

    // Write HTML file
    auto htmlPath = m_options.getHtmlOutputPath();
    logVerbose("Writing HTML to: " + htmlPath.string());
    if (!FileSystem::writeFile(htmlPath, html)) {
        logError("Failed to write HTML file: " + htmlPath.string());
        return false;
    }

    return true;
}

void Compiler::log(const std::string& message) {
    if (!m_options.silent) {
        std::cout << message << "\n";
    }
}

void Compiler::logError(const std::string& message) {
    m_lastError = message;
    if (!m_options.silent) {
        std::cerr << "ERROR: " << message << "\n";
    }
}

void Compiler::logVerbose(const std::string& message) {
    if (m_options.verbose && !m_options.silent) {
        std::cout << "  " << message << "\n";
    }
}

} // namespace artic
