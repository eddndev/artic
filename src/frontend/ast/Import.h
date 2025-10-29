#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>

namespace artic {

/**
 * @brief Base class for import nodes
 */
class Import : public ASTNode {
public:
    std::string source; // Path to import from

    explicit Import(ASTNodeType type, const std::string& source, SourceLocation location = SourceLocation())
        : ASTNode(type, location), source(source) {}

    virtual ~Import() = default;
};

/**
 * @brief Theme import: use theme "./theme.atc"
 */
class ThemeImport : public Import {
public:
    explicit ThemeImport(const std::string& source, SourceLocation location = SourceLocation())
        : Import(ASTNodeType::THEME_IMPORT, source, location) {}

    std::string toString() const override {
        return "use theme \"" + source + "\"";
    }
};

/**
 * @brief Named import: use { Card, Button } from "./components"
 */
class NamedImport : public Import {
public:
    std::vector<std::string> names; // Names to import

    NamedImport(const std::vector<std::string>& names, const std::string& source, SourceLocation location = SourceLocation())
        : Import(ASTNodeType::NAMED_IMPORT, source, location), names(names) {}

    std::string toString() const override {
        std::string result = "use { ";
        for (size_t i = 0; i < names.size(); ++i) {
            if (i > 0) result += ", ";
            result += names[i];
        }
        result += " } from \"" + source + "\"";
        return result;
    }
};

} // namespace artic
