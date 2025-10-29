#pragma once

#include <string>
#include <filesystem>

namespace artic {

/**
 * @brief Compilation mode for Artic
 */
enum class CompilationMode {
    SSG,  // Static Site Generation (Phase 1)
    SSR,  // Server-Side Rendering (Phase 2)
    SPA   // Single Page Application (Phase 3)
};

/**
 * @brief Optimization level
 */
enum class OptimizationLevel {
    None,    // No optimization
    Basic,   // Basic optimization (minify)
    Full     // Full optimization (minify + tree-shaking)
};

/**
 * @brief Compiler options for Artic compilation
 */
struct CompilerOptions {
    // Input/Output
    std::string inputFile;                          // Input .atc file
    std::string outputDir = "dist";                 // Output directory

    // Compilation mode
    CompilationMode mode = CompilationMode::SSG;    // Compilation mode

    // Output options
    bool minifyCss = false;                         // Minify CSS output
    bool minifyHtml = false;                        // Minify HTML output
    bool prettyPrint = true;                        // Pretty print output
    int indentSize = 2;                             // Spaces per indent level

    // Generation options
    bool includeDoctype = true;                     // Include <!DOCTYPE html>
    bool generateSourceMaps = false;                // Generate source maps (future)

    // Optimization
    OptimizationLevel optimization = OptimizationLevel::None;

    // Verbose output
    bool verbose = false;                           // Print verbose output
    bool silent = false;                            // Suppress all output

    /**
     * @brief Get absolute path to input file
     */
    std::filesystem::path getInputPath() const {
        return std::filesystem::absolute(inputFile);
    }

    /**
     * @brief Get absolute path to output directory
     */
    std::filesystem::path getOutputPath() const {
        return std::filesystem::absolute(outputDir);
    }

    /**
     * @brief Get path to output HTML file
     */
    std::filesystem::path getHtmlOutputPath() const {
        return getOutputPath() / "index.html";
    }

    /**
     * @brief Get path to output CSS file
     */
    std::filesystem::path getCssOutputPath() const {
        return getOutputPath() / "style.css";
    }

    /**
     * @brief Validate options
     * @return true if valid, false otherwise
     */
    bool validate() const {
        if (inputFile.empty()) {
            return false;
        }

        if (outputDir.empty()) {
            return false;
        }

        return true;
    }
};

} // namespace artic
