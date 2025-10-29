#include "backend/html/HtmlGenerator.h"
#include <sstream>
#include <algorithm>

namespace artic {

// Self-closing tags (void elements)
const std::set<std::string> HtmlGenerator::SELF_CLOSING_TAGS = {
    "area", "base", "br", "col", "embed", "hr", "img", "input",
    "link", "meta", "param", "source", "track", "wbr"
};

// Inline tags (no newlines)
const std::set<std::string> HtmlGenerator::INLINE_TAGS = {
    "a", "abbr", "b", "bdi", "bdo", "br", "cite", "code", "data",
    "dfn", "em", "i", "kbd", "mark", "q", "rp", "rt", "ruby", "s",
    "samp", "small", "span", "strong", "sub", "sup", "time", "u",
    "var", "wbr"
};

HtmlGenerator::HtmlGenerator() : m_options{} {}

HtmlGenerator::HtmlGenerator(const HtmlGeneratorOptions& options)
    : m_options(options) {}

std::string HtmlGenerator::generate(Component* component) {
    if (!component || component->templateNodes.empty()) {
        return "";
    }

    std::ostringstream html;

    // Optional DOCTYPE
    if (m_options.includeDoctype) {
        html << "<!DOCTYPE html>\n";
    }

    // Generate each template node
    for (const auto& node : component->templateNodes) {
        html << generateNode(node.get(), 0);
    }

    return html.str();
}

std::string HtmlGenerator::generateNode(TemplateNode* node, int indentLevel) {
    if (!node) {
        return "";
    }

    // Dispatch based on node type
    switch (node->type) {
        case ASTNodeType::ELEMENT:
            return generateElement(static_cast<Element*>(node), indentLevel);

        case ASTNodeType::TEXT_NODE:
            return generateTextNode(static_cast<TextNode*>(node), indentLevel);

        case ASTNodeType::EXPRESSION_NODE:
            return generateExpressionNode(static_cast<ExpressionNode*>(node), indentLevel);

        case ASTNodeType::COMMENT_NODE:
            return generateCommentNode(static_cast<CommentNode*>(node), indentLevel);

        default:
            return "";
    }
}

std::string HtmlGenerator::generateElement(Element* element, int indentLevel) {
    if (!element) {
        return "";
    }

    std::ostringstream html;
    bool selfClosing = isSelfClosing(element->tagName);
    bool isInlineTag = isInline(element->tagName);

    // Opening tag
    if (m_options.pretty && !isInlineTag) {
        html << indent(indentLevel);
    }

    html << "<" << element->tagName;

    // Attributes
    std::string attrs = generateAttributes(element);
    if (!attrs.empty()) {
        html << " " << attrs;
    }

    if (selfClosing) {
        // Self-closing tag: <img src="..." />
        html << " />";
        if (m_options.pretty && !isInlineTag) {
            html << "\n";
        }
        return html.str();
    }

    html << ">";

    // Children
    if (!element->children.empty()) {
        bool hasBlockChildren = false;

        // Check if we have block-level children
        for (const auto& child : element->children) {
            if (child->type == ASTNodeType::ELEMENT) {
                auto* childElem = static_cast<Element*>(child.get());
                if (!isInline(childElem->tagName)) {
                    hasBlockChildren = true;
                    break;
                }
            }
        }

        // Add newline after opening tag if we have block children
        if (m_options.pretty && hasBlockChildren && !isInlineTag) {
            html << "\n";
        }

        // Generate children
        for (const auto& child : element->children) {
            if (hasBlockChildren || child->type == ASTNodeType::ELEMENT) {
                html << generateNode(child.get(), indentLevel + 1);
            } else {
                // Inline content (text nodes in inline elements)
                html << generateNode(child.get(), 0);
            }
        }

        // Add indent before closing tag if we have block children
        if (m_options.pretty && hasBlockChildren && !isInlineTag) {
            html << indent(indentLevel);
        }
    }

    // Closing tag
    html << "</" << element->tagName << ">";

    if (m_options.pretty && !isInlineTag) {
        html << "\n";
    }

    return html.str();
}

std::string HtmlGenerator::generateTextNode(TextNode* textNode, int indentLevel) {
    if (!textNode || textNode->content.empty()) {
        return "";
    }

    // Trim whitespace if minifying
    if (m_options.minify) {
        std::string content = textNode->content;
        // Remove leading/trailing whitespace
        content.erase(0, content.find_first_not_of(" \t\n\r"));
        content.erase(content.find_last_not_of(" \t\n\r") + 1);
        return content;
    }

    return textNode->content;
}

std::string HtmlGenerator::generateExpressionNode(ExpressionNode* exprNode, int indentLevel) {
    if (!exprNode) {
        return "";
    }

    // Phase 1 (SSG): Expressions are not evaluated, render as comment
    std::ostringstream html;

    if (m_options.pretty) {
        html << "<!-- Expression: {" << exprNode->expression << "} -->";
    } else {
        html << "<!--{" << exprNode->expression << "}-->";
    }

    return html.str();
}

std::string HtmlGenerator::generateCommentNode(CommentNode* commentNode, int indentLevel) {
    if (!commentNode) {
        return "";
    }

    std::ostringstream html;

    if (m_options.pretty) {
        html << indent(indentLevel);
    }

    html << "<!-- " << commentNode->content << " -->";

    if (m_options.pretty) {
        html << "\n";
    }

    return html.str();
}

std::string HtmlGenerator::generateAttributes(Element* element) {
    if (!element) {
        return "";
    }

    std::ostringstream attrs;
    bool first = true;

    // Regular attributes
    for (const auto& attr : element->attributes) {
        if (!first) {
            attrs << " ";
        }
        attrs << attr->name;
        if (!attr->value.empty()) {
            attrs << "=\"" << attr->value << "\"";
        }
        first = false;
    }

    // Class directive: class:(container btn)
    if (element->classDirective) {
        if (!first) {
            attrs << " ";
        }
        attrs << "class=\"" << generateClassDirective(element->classDirective.get()) << "\"";
        first = false;
    }

    return attrs.str();
}

std::string HtmlGenerator::generateClassDirective(ClassDirective* classDir) {
    if (!classDir || classDir->tokens.empty()) {
        return "";
    }

    std::ostringstream classes;
    bool first = true;

    for (const auto& token : classDir->tokens) {
        if (!first) {
            classes << " ";
        }
        classes << token;
        first = false;
    }

    return classes.str();
}

std::string HtmlGenerator::indent(int level) const {
    if (!m_options.pretty || level <= 0) {
        return "";
    }

    return std::string(level * m_options.indentSize, ' ');
}

bool HtmlGenerator::isSelfClosing(const std::string& tagName) const {
    return SELF_CLOSING_TAGS.count(tagName) > 0;
}

bool HtmlGenerator::isInline(const std::string& tagName) const {
    return INLINE_TAGS.count(tagName) > 0;
}

} // namespace artic
