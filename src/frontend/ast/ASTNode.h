#pragma once

#include "frontend/lexer/SourceLocation.h"
#include <memory>
#include <string>

namespace artic {

/**
 * @brief Node types for the AST
 */
enum class ASTNodeType {
    // Root
    COMPONENT,

    // Metadata
    ROUTE_DECORATOR,
    LAYOUT_DECORATOR,
    UTILITY_DECORATOR,

    // Imports
    THEME_IMPORT,
    NAMED_IMPORT,

    // Props
    PROP_TYPE,
    PROP_DECLARATION,
    PROPS_BLOCK,

    // Styles
    UTILITY_DECLARATION,
    UTILITY_TOKEN,

    // Template
    ELEMENT,
    TEXT_NODE,
    EXPRESSION_NODE,
    COMMENT_NODE,
    SLOT_NODE,

    // Attributes and Directives
    ATTRIBUTE,
    CLASS_DIRECTIVE,
    EVENT_DIRECTIVE,
    BIND_DIRECTIVE,
    SLOT_DIRECTIVE
};

/**
 * @brief Base class for all AST nodes
 */
class ASTNode {
public:
    ASTNodeType type;
    SourceLocation location;

    explicit ASTNode(ASTNodeType type, SourceLocation location = SourceLocation())
        : type(type), location(location) {}

    virtual ~ASTNode() = default;

    /**
     * @brief Get string representation for debugging
     */
    virtual std::string toString() const = 0;

    /**
     * @brief Check node type
     */
    bool isType(ASTNodeType t) const {
        return type == t;
    }
};

} // namespace artic
