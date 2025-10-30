#pragma once

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <unordered_map>
#include "frontend/ast/Component.h"
#include "frontend/ast/Template.h"

namespace artic {

// Forward declaration
class ComponentRegistry;

/**
 * @brief Context for rendering components with props and slots
 */
struct RenderContext {
    std::unordered_map<std::string, std::string> props;  // Props passed to component
    std::unordered_map<std::string, std::vector<TemplateNode*>> slots;  // Slot content (name -> nodes)
    std::vector<TemplateNode*> defaultSlot;  // Default slot content (unnamed children)
};


/**
 * @brief Options for HTML generation
 */
struct HtmlGeneratorOptions {
    bool pretty = true;          // Pretty print with indentation
    bool includeDoctype = true;  // Include <!DOCTYPE html>
    int indentSize = 2;          // Number of spaces per indent level
    bool minify = false;         // Minify output HTML

    // Full HTML document options
    bool fullHtml = true;        // Generate full HTML5 document (html, head, body)
    std::string title = "Artic"; // Default page title
    std::string lang = "en";     // Language attribute
    std::string charset = "UTF-8"; // Character encoding
    std::string cssFile = "style.css"; // CSS file to link
    bool includeViewportMeta = true; // Include viewport meta tag
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
    HtmlGenerator(const HtmlGeneratorOptions& options, const ComponentRegistry* registry);

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
     * @param context Optional render context for props and slots
     * @return Generated HTML for this node
     */
    std::string generateNode(TemplateNode* node, int indentLevel = 0, RenderContext* context = nullptr);

private:
    HtmlGeneratorOptions m_options;
    const ComponentRegistry* m_registry;

    // Generate specific node types
    std::string generateElement(Element* element, int indentLevel, RenderContext* context);
    std::string generateTextNode(TextNode* textNode, int indentLevel, RenderContext* context);
    std::string generateExpressionNode(ExpressionNode* exprNode, int indentLevel, RenderContext* context);
    std::string generateCommentNode(CommentNode* commentNode, int indentLevel, RenderContext* context);
    std::string generateSlotNode(SlotNode* slotNode, int indentLevel, RenderContext* context);

    // Component expansion
    std::string expandCustomComponent(Element* element, int indentLevel);
    bool isCustomComponent(const std::string& tagName) const;

    // Full HTML document generation
    std::string generateFullHtml(Component* component, const std::string& bodyContent);
    std::string generateHead(Component* component);
    std::string extractTitle(Component* component);

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
