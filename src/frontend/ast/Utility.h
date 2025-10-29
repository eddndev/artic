#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

namespace artic {

/**
 * @brief Utility token: property:value or standalone identifier
 *
 * Examples:
 *   px:4       -> property="px", value="4"
 *   bg:blue-500 -> property="bg", value="blue-500"
 *   rounded:md -> property="rounded", value="md"
 *   hover:bg:blue-700 -> prefix="hover", property="bg", value="blue-700"
 */
class UtilityToken : public ASTNode {
public:
    std::string prefix;    // Optional: hover, focus, sm, md, etc.
    std::string property;  // Property name: px, bg, text, etc.
    std::string value;     // Value: 4, blue-500, md, etc.

    UtilityToken(const std::string& property, const std::string& value,
                 const std::string& prefix = "", SourceLocation location = SourceLocation())
        : ASTNode(ASTNodeType::UTILITY_TOKEN, location),
          prefix(prefix), property(property), value(value) {}

    std::string toString() const override {
        std::string result;
        if (!prefix.empty()) {
            result += prefix + ":";
        }
        result += property;
        if (!value.empty()) {
            result += ":" + value;
        }
        return result;
    }
};

/**
 * @brief Utility declaration: @utility name { tokens }
 *
 * Example:
 *   @utility
 *   btn {
 *       px:4 py:2
 *       bg:blue-500
 *   }
 */
class UtilityDeclaration : public ASTNode {
public:
    std::string name;                             // Utility name (e.g., "btn")
    std::vector<std::unique_ptr<UtilityToken>> tokens; // Utility tokens

    UtilityDeclaration(const std::string& name, SourceLocation location = SourceLocation())
        : ASTNode(ASTNodeType::UTILITY_DECLARATION, location), name(name) {}

    void addToken(std::unique_ptr<UtilityToken> token) {
        tokens.push_back(std::move(token));
    }

    std::string toString() const override {
        std::string result = "@utility\n" + name + " {\n";
        for (const auto& token : tokens) {
            result += "    " + token->toString() + "\n";
        }
        result += "}";
        return result;
    }
};

} // namespace artic
