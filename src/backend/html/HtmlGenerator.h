#pragma once

#include <string>
#include <memory>
#include <vector>
#include <set>
#include "frontend/ast/Component.h"
#include "frontend/ast/Template.h"

namespace artic {

/**
 * @brief Options for HTML generation
 */
struct HtmlGeneratorOptions {
    bool pretty = true;         // Pretty print with indentation
    bool includeDoctype = false; // Include <!DOCTYPE html> (for standalone pages)
    int indentSize = 2;         // Number of spaces per indent level
    bool minify = false;        // Minify output HTML
};

/**
 * @brief HTML Generator - Converts AST templates to HTML
 *
 * Takes template nodes from the AST and generates HTML:
 *
 * <div class:(container)>
 *     <h1>Hello World</h1>
 * </div>
 *
 * →
 *
 * <div class="container">
 *     <h1>Hello World</h1>
 * </div>
 */
class HtmlGenerator {
public:
    HtmlGenerator();
    explicit HtmlGenerator(const HtmlGeneratorOptions& options);

    /**
     * @brief Generate HTML from Component AST
     * @param component Component with template nodes
     * @return Generated HTML string
     */
    std::string generate(Component* component);

    /**
     * @brief Generate HTML for a single template node
     * @param node Template node (Element, TextNode, etc.)
     * @param indentLevel Current indentation level
     * @return Generated HTML for this node
     */
    std::string generateNode(TemplateNode* node, int indentLevel = 0);

private:
    HtmlGeneratorOptions m_options;

    // Generate specific node types
    std::string generateElement(Element* element, int indentLevel);
    std::string generateTextNode(TextNode* textNode, int indentLevel);
    std::string generateExpressionNode(ExpressionNode* exprNode, int indentLevel);
    std::string generateCommentNode(CommentNode* commentNode, int indentLevel);

    // Helper methods
    std::string generateAttributes(Element* element);
    std::string generateClassDirective(ClassDirective* classDir);
    std::string indent(int level) const;
    bool isSelfClosing(const std::string& tagName) const;
    bool isInline(const std::string& tagName) const;

    // Self-closing tags
    static const std::set<std::string> SELF_CLOSING_TAGS;

    // Inline tags (don't add newlines)
    static const std::set<std::string> INLINE_TAGS;
};

} // namespace artic
