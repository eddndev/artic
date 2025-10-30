#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

namespace artic {

/**
 * @brief Prop type definition
 *
 * Examples:
 * - string
 * - number
 * - "primary" | "secondary"
 * - string[]
 * - (data: any) => void
 */
class PropType : public ASTNode {
public:
    std::string typeString;  // Raw type as string for now (Phase 1)

    PropType(const std::string& type, SourceLocation location = SourceLocation())
        : ASTNode(ASTNodeType::PROP_TYPE, location), typeString(type) {}

    std::string toString() const override {
        return typeString;
    }
};

/**
 * @brief Single prop declaration
 *
 * Examples:
 * - title: string
 * - count?: number = 0
 * - onClick?: () => void
 */
class PropDeclaration : public ASTNode {
public:
    std::string name;                        // Prop name
    std::unique_ptr<PropType> type;          // Prop type
    bool optional;                           // Has ? (optional)
    std::string defaultValue;                // Default value (if any)

    PropDeclaration(
        const std::string& propName,
        std::unique_ptr<PropType> propType,
        bool isOptional = false,
        const std::string& defValue = "",
        SourceLocation location = SourceLocation()
    ) : ASTNode(ASTNodeType::PROP_DECLARATION, location),
        name(propName),
        type(std::move(propType)),
        optional(isOptional),
        defaultValue(defValue) {}

    std::string toString() const override {
        std::string result = name;
        if (optional) result += "?";
        result += ": " + type->toString();
        if (!defaultValue.empty()) {
            result += " = " + defaultValue;
        }
        return result;
    }
};

/**
 * @brief Props block (contains multiple prop declarations)
 *
 * Example:
 * props {
 *     title: string,
 *     count?: number = 0
 * }
 */
class PropsBlock : public ASTNode {
public:
    std::vector<std::unique_ptr<PropDeclaration>> props;

    PropsBlock(SourceLocation location = SourceLocation())
        : ASTNode(ASTNodeType::PROPS_BLOCK, location) {}

    void addProp(std::unique_ptr<PropDeclaration> prop) {
        props.push_back(std::move(prop));
    }

    std::string toString() const override {
        std::string result = "props {\n";
        for (const auto& prop : props) {
            result += "  " + prop->toString() + "\n";
        }
        result += "}";
        return result;
    }
};

} // namespace artic
