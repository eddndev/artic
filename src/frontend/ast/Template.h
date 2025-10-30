#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

namespace artic {

/**
 * @brief Base class for template nodes
 */
class TemplateNode : public ASTNode {
public:
    explicit TemplateNode(ASTNodeType type, SourceLocation location = SourceLocation())
        : ASTNode(type, location) {}

    virtual ~TemplateNode() = default;
};

/**
 * @brief Attribute: name="value" or name={expression}
 */
class Attribute : public ASTNode {
public:
    std::string name;     // Attribute name
    std::string value;    // Value (can be string literal or expression)
    bool isDynamic;       // true if value is {expression}, false if "literal"

    Attribute(const std::string& name, const std::string& value, bool isDynamic = false, SourceLocation location = SourceLocation())
        : ASTNode(ASTNodeType::ATTRIBUTE, location), name(name), value(value), isDynamic(isDynamic) {}

    std::string toString() const override {
        if (isDynamic) {
            return name + "={" + value + "}";
        } else {
            return name + "=\"" + value + "\"";
        }
    }
};

/**
 * @brief Class directive: class:(tokens)
 *
 * Example: class:(px:4 bg:blue-500)
 */
class ClassDirective : public ASTNode {
public:
    std::string tokens; // Raw tokens inside class:()

    ClassDirective(const std::string& tokens, SourceLocation location = SourceLocation())
        : ASTNode(ASTNodeType::CLASS_DIRECTIVE, location), tokens(tokens) {}

    std::string toString() const override {
        return "class:(" + tokens + ")";
    }
};

/**
 * @brief Slot directive: slot:name attribute
 *
 * Used to assign content to a named slot:
 * <button slot:footer>Close</button>
 */
class SlotDirective : public ASTNode {
public:
    std::string slotName;  // Name of the slot to assign to

    SlotDirective(const std::string& name, SourceLocation location = SourceLocation())
        : ASTNode(ASTNodeType::SLOT_DIRECTIVE, location), slotName(name) {}

    std::string toString() const override {
        return "slot:" + slotName;
    }
};

/**
 * @brief Text node in template
 */
class TextNode : public TemplateNode {
public:
    std::string text;

    TextNode(const std::string& text, SourceLocation location = SourceLocation())
        : TemplateNode(ASTNodeType::TEXT_NODE, location), text(text) {}

    std::string toString() const override {
        return text;
    }
};

/**
 * @brief Expression node: {expression}
 */
class ExpressionNode : public TemplateNode {
public:
    std::string expression;

    ExpressionNode(const std::string& expression, SourceLocation location = SourceLocation())
        : TemplateNode(ASTNodeType::EXPRESSION_NODE, location), expression(expression) {}

    std::string toString() const override {
        return "{" + expression + "}";
    }
};

/**
 * @brief Element node: <tag attributes>children</tag>
 */
class Element : public TemplateNode {
public:
    std::string tagName;                                  // Tag name (div, h1, Card, etc.)
    std::vector<std::unique_ptr<Attribute>> attributes;   // HTML attributes (name="value")
    std::vector<std::unique_ptr<ClassDirective>> classDirectives; // class:() directives
    std::unique_ptr<SlotDirective> slotDirective;         // slot:name directive (optional)
    std::vector<std::unique_ptr<TemplateNode>> children; // Child nodes
    bool isSelfClosing;                                   // true for <img /> etc.

    Element(const std::string& tagName, bool isSelfClosing = false, SourceLocation location = SourceLocation())
        : TemplateNode(ASTNodeType::ELEMENT, location), tagName(tagName), isSelfClosing(isSelfClosing) {}

    void addAttribute(std::unique_ptr<Attribute> attr) {
        attributes.push_back(std::move(attr));
    }

    void addClassDirective(std::unique_ptr<ClassDirective> directive) {
        classDirectives.push_back(std::move(directive));
    }

    void setSlotDirective(std::unique_ptr<SlotDirective> directive) {
        slotDirective = std::move(directive);
    }

    void addChild(std::unique_ptr<TemplateNode> child) {
        children.push_back(std::move(child));
    }

    std::string toString() const override {
        std::string result = "<" + tagName;

        // Attributes
        for (const auto& attr : attributes) {
            result += " " + attr->toString();
        }

        // Class directives
        for (const auto& dir : classDirectives) {
            result += " " + dir->toString();
        }

        if (isSelfClosing) {
            result += " />";
        } else {
            result += ">";
            for (const auto& child : children) {
                result += child->toString();
            }
            result += "</" + tagName + ">";
        }

        return result;
    }
};

/**
 * @brief Comment node: {/* comment *\/}
 */
class CommentNode : public TemplateNode {
public:
    std::string comment;

    CommentNode(const std::string& comment, SourceLocation location = SourceLocation())
        : TemplateNode(ASTNodeType::COMMENT_NODE, location), comment(comment) {}

    std::string toString() const override {
        return "{/* " + comment + " */}";
    }
};

/**
 * @brief Slot node: <slot /> or <slot:name />
 *
 * Examples:
 * - <slot /> → default slot
 * - <slot:header /> → named slot "header"
 * - <slot:footer>Default content</slot:footer> → slot with fallback
 */
class SlotNode : public TemplateNode {
public:
    std::string name;  // Empty string = default slot, otherwise named slot
    std::vector<std::unique_ptr<TemplateNode>> fallback; // Fallback content (optional)

    SlotNode(const std::string& slotName = "", SourceLocation location = SourceLocation())
        : TemplateNode(ASTNodeType::SLOT_NODE, location), name(slotName) {}

    void addFallback(std::unique_ptr<TemplateNode> node) {
        fallback.push_back(std::move(node));
    }

    bool hasName() const {
        return !name.empty();
    }

    bool hasFallback() const {
        return !fallback.empty();
    }

    std::string toString() const override {
        if (name.empty()) {
            return hasFallback() ? "<slot>...</slot>" : "<slot />";
        } else {
            return hasFallback()
                ? "<slot:" + name + ">...</slot:" + name + ">"
                : "<slot:" + name + " />";
        }
    }
};

} // namespace artic
