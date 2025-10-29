#include "backend/css/CssGenerator.h"
#include <sstream>
#include <unordered_map>

namespace artic {

CssGenerator::CssGenerator() : m_options{} {}

CssGenerator::CssGenerator(const CssGeneratorOptions& options)
    : m_options(options) {}

std::string CssGenerator::generate(Component* component) {
    if (!component) {
        return "";
    }

    std::ostringstream css;
    bool first = true;

    // Generate CSS for each @utility declaration
    for (const auto& utility : component->utilities) {
        if (!first && m_options.pretty) {
            css << "\n";
        }
        css << generateUtility(utility.get());
        first = false;
    }

    return css.str();
}

std::string CssGenerator::generateUtility(UtilityDeclaration* utility) {
    if (!utility || utility->tokens.empty()) {
        return "";
    }

    std::string className = utility->name;

    // Group tokens by prefix
    // base: tokens without prefix
    // hover: tokens with hover: prefix
    // focus: tokens with focus: prefix, etc.
    std::unordered_map<std::string, std::vector<std::string>> groupedDeclarations;

    for (const auto& token : utility->tokens) {
        // Resolve token to CSS declarations
        auto declarations = m_resolver.resolve(token->property, token->value);

        if (declarations.empty()) {
            continue;
        }

        // Group by prefix (empty string = base)
        std::string prefix = token->prefix.empty() ? "" : token->prefix;

        for (const auto& decl : declarations) {
            groupedDeclarations[prefix].push_back(decl);
        }
    }

    // Generate CSS rules
    std::ostringstream css;

    // 1. Base rule (no prefix)
    if (groupedDeclarations.count("")) {
        std::string selector = "." + className;
        css << formatRule(selector, groupedDeclarations[""]);
    }

    // 2. Rules with prefixes
    for (const auto& [prefix, declarations] : groupedDeclarations) {
        if (prefix.empty()) {
            continue; // Skip base (already handled)
        }

        std::string selector = getPrefixSelector(className, prefix);

        if (m_options.pretty) {
            css << "\n";
        }

        css << formatRule(selector, declarations);
    }

    return css.str();
}

std::string CssGenerator::getPrefixSelector(const std::string& className, const std::string& prefix) {
    // Map prefix to CSS selector
    static const std::unordered_map<std::string, std::string> prefixMap = {
        // Pseudo-classes
        {"hover", ":hover"},
        {"focus", ":focus"},
        {"active", ":active"},
        {"visited", ":visited"},
        {"disabled", ":disabled"},
        {"checked", ":checked"},

        // Pseudo-elements
        {"before", "::before"},
        {"after", "::after"},
        {"placeholder", "::placeholder"},

        // Responsive breakpoints (mobile-first)
        {"sm", "@media (min-width: 640px)"},   // Small devices
        {"md", "@media (min-width: 768px)"},   // Medium devices
        {"lg", "@media (min-width: 1024px)"},  // Large devices
        {"xl", "@media (min-width: 1280px)"},  // Extra large devices
        {"2xl", "@media (min-width: 1536px)"}, // 2X Extra large

        // Dark mode
        {"dark", "@media (prefers-color-scheme: dark)"},
    };

    auto it = prefixMap.find(prefix);
    if (it == prefixMap.end()) {
        // Unknown prefix, return as-is
        return "." + className + ":" + prefix;
    }

    std::string selector = it->second;

    // Handle media queries differently
    if (selector.find("@media") == 0) {
        return selector + " { ." + className;
    }

    // Handle pseudo-classes/elements
    return "." + className + selector;
}

std::string CssGenerator::formatRule(const std::string& selector, const std::vector<std::string>& declarations) {
    if (declarations.empty()) {
        return "";
    }

    std::ostringstream css;

    // Check if selector is media query
    bool isMediaQuery = selector.find("@media") == 0;

    if (isMediaQuery) {
        // Media query format:
        // @media (min-width: 768px) {
        //   .btn { ... }
        // }
        css << selector << " {\n";
        css << formatDeclarations(declarations);
        css << "  }\n";
        css << "}";
    } else {
        // Regular rule:
        // .btn { ... }
        css << selector << " {\n";
        css << formatDeclarations(declarations);
        css << "}";
    }

    return css.str();
}

std::string CssGenerator::formatDeclarations(const std::vector<std::string>& declarations) {
    std::ostringstream css;

    for (const auto& decl : declarations) {
        if (m_options.minify) {
            css << decl << ";";
        } else if (m_options.pretty) {
            css << "  " << decl << ";\n";
        } else {
            css << decl << ";";
        }
    }

    return css.str();
}

std::string CssGenerator::wrapWithPrefix(
    const std::string& className,
    const std::string& prefix,
    const std::vector<std::string>& declarations
) {
    std::string selector = getPrefixSelector(className, prefix);
    return formatRule(selector, declarations);
}

} // namespace artic
