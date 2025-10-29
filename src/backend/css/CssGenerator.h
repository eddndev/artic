#pragma once

#include <string>
#include <memory>
#include <vector>
#include "frontend/ast/Component.h"
#include "backend/css/UtilityResolver.h"

namespace artic {

/**
 * @brief Options for CSS generation
 */
struct CssGeneratorOptions {
    bool minify = false;        // Minify output CSS
    bool sourceMaps = false;    // Generate source maps (future)
    bool pretty = true;         // Pretty print (indentation)
};

/**
 * @brief CSS Generator - Converts AST utilities to CSS
 *
 * Takes @utility declarations from the AST and generates CSS:
 *
 * @utility
 * btn {
 *     px:4 py:2
 *     bg:blue-500
 *     hover:bg:blue-700
 * }
 *
 * →
 *
 * .btn {
 *     padding-left: 1rem;
 *     padding-right: 1rem;
 *     padding-top: 0.5rem;
 *     padding-bottom: 0.5rem;
 *     background-color: #3b82f6;
 * }
 * .btn:hover {
 *     background-color: #1d4ed8;
 * }
 */
class CssGenerator {
public:
    CssGenerator();
    explicit CssGenerator(const CssGeneratorOptions& options);

    /**
     * @brief Generate CSS from Component AST
     * @param component Component with @utility declarations
     * @return Generated CSS string
     */
    std::string generate(Component* component);

    /**
     * @brief Generate CSS for a single utility declaration
     * @param utility Utility declaration
     * @return Generated CSS for this utility
     */
    std::string generateUtility(UtilityDeclaration* utility);

private:
    CssGeneratorOptions m_options;
    UtilityResolver m_resolver;

    // Generate CSS for a single token
    std::string generateToken(UtilityToken* token, const std::string& className);

    // Handle prefixes (hover:, focus:, sm:, md:, lg:, xl:, dark:)
    std::string wrapWithPrefix(
        const std::string& className,
        const std::string& prefix,
        const std::vector<std::string>& declarations
    );

    // Get CSS selector for a prefix
    std::string getPrefixSelector(const std::string& className, const std::string& prefix);

    // Format CSS rule
    std::string formatRule(const std::string& selector, const std::vector<std::string>& declarations);

    // Format CSS declarations
    std::string formatDeclarations(const std::vector<std::string>& declarations);
};

} // namespace artic
