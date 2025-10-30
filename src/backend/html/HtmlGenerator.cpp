#include "backend/html/HtmlGenerator.h"
#include "compiler/ComponentRegistry.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <unordered_map>

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

HtmlGenerator::HtmlGenerator() : m_options{}, m_registry(nullptr) {}

HtmlGenerator::HtmlGenerator(const HtmlGeneratorOptions& options)
    : m_options(options), m_registry(nullptr) {}

HtmlGenerator::HtmlGenerator(const HtmlGeneratorOptions& options, const ComponentRegistry* registry)
    : m_options(options), m_registry(registry) {}

std::string HtmlGenerator::generate(Component* component) {
    if (!component || component->templateNodes.empty()) {
        return "";
    }

    std::ostringstream bodyContent;

    // Generate body content from template nodes
    for (const auto& node : component->templateNodes) {
        bodyContent << generateNode(node.get(), m_options.fullHtml ? 2 : 0);
    }

    // Generate full HTML document or just body content
    if (m_options.fullHtml) {
        return generateFullHtml(component, bodyContent.str());
    } else {
        // Legacy mode: just DOCTYPE + content
        std::ostringstream html;
        if (m_options.includeDoctype) {
            html << "<!DOCTYPE html>\n";
        }
        html << bodyContent.str();
        return html.str();
    }
}

std::string HtmlGenerator::generateNode(TemplateNode* node, int indentLevel, RenderContext* context) {
    if (!node) {
        return "";
    }

    // Dispatch based on node type
    switch (node->type) {
        case ASTNodeType::ELEMENT:
            return generateElement(static_cast<Element*>(node), indentLevel, context);

        case ASTNodeType::TEXT_NODE:
            return generateTextNode(static_cast<TextNode*>(node), indentLevel, context);

        case ASTNodeType::EXPRESSION_NODE:
            return generateExpressionNode(static_cast<ExpressionNode*>(node), indentLevel, context);

        case ASTNodeType::COMMENT_NODE:
            return generateCommentNode(static_cast<CommentNode*>(node), indentLevel, context);

        case ASTNodeType::SLOT_NODE:
            return generateSlotNode(static_cast<SlotNode*>(node), indentLevel, context);

        default:
            return "";
    }
}

std::string HtmlGenerator::generateElement(Element* element, int indentLevel, RenderContext* context) {
    if (!element) {
        return "";
    }

    // Check if this is a custom component (starts with uppercase)
    if (isCustomComponent(element->tagName)) {
        return expandCustomComponent(element, indentLevel);
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
                html << generateNode(child.get(), indentLevel + 1, context);
            } else {
                // Inline content (text nodes in inline elements)
                html << generateNode(child.get(), 0, context);
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

std::string HtmlGenerator::generateTextNode(TextNode* textNode, int indentLevel, RenderContext* context) {
    if (!textNode || textNode->text.empty()) {
        return "";
    }

    // Trim whitespace if minifying
    if (m_options.minify) {
        std::string content = textNode->text;
        // Remove leading/trailing whitespace
        content.erase(0, content.find_first_not_of(" \t\n\r"));
        content.erase(content.find_last_not_of(" \t\n\r") + 1);
        return content;
    }

    return textNode->text;
}

std::string HtmlGenerator::generateExpressionNode(ExpressionNode* exprNode, int indentLevel, RenderContext* context) {
    if (!exprNode) {
        return "";
    }

    // If we have a context with props, try to evaluate simple prop references
    if (context && !context->props.empty()) {
        std::string expr = exprNode->expression;

        // Simple case: {propName} or {props.propName}
        // Remove whitespace
        expr.erase(std::remove_if(expr.begin(), expr.end(), ::isspace), expr.end());

        // Check if it's a simple prop reference
        auto it = context->props.find(expr);
        if (it != context->props.end()) {
            return it->second;
        }
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

std::string HtmlGenerator::generateCommentNode(CommentNode* commentNode, int indentLevel, RenderContext* context) {
    if (!commentNode) {
        return "";
    }

    std::ostringstream html;

    if (m_options.pretty) {
        html << indent(indentLevel);
    }

    html << "<!-- " << commentNode->comment << " -->";

    if (m_options.pretty) {
        html << "\n";
    }

    return html.str();
}

std::string HtmlGenerator::generateSlotNode(SlotNode* slotNode, int indentLevel, RenderContext* context) {
    if (!slotNode) {
        return "";
    }

    std::ostringstream html;

    // If we have a context, try to replace slot with actual content
    if (context) {
        std::vector<TemplateNode*>* slotContent = nullptr;

        if (slotNode->hasName()) {
            // Named slot: look for content marked with slot:name
            auto it = context->slots.find(slotNode->name);
            if (it != context->slots.end()) {
                slotContent = &it->second;
            }
        } else {
            // Default slot: use unnamed children
            if (!context->defaultSlot.empty()) {
                slotContent = &context->defaultSlot;
            }
        }

        // If we found slot content, render it
        if (slotContent && !slotContent->empty()) {
            for (auto* node : *slotContent) {
                html << generateNode(node, indentLevel, nullptr);  // Don't pass context to slot content
            }
            return html.str();
        }
    }

    // No slot content found, use fallback if available
    if (slotNode->hasFallback()) {
        for (const auto& child : slotNode->fallback) {
            html << generateNode(child.get(), indentLevel, context);
        }
        return html.str();
    }

    // No content and no fallback: render comment for debugging
    if (m_options.pretty) {
        html << indent(indentLevel);
        if (slotNode->hasName()) {
            html << "<!-- <slot:" << slotNode->name << " /> -->";
        } else {
            html << "<!-- <slot /> -->";
        }
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

    // Class directives: class:(container btn)
    if (!element->classDirectives.empty()) {
        if (!first) {
            attrs << " ";
        }
        attrs << "class=\"";

        // Concatenate all class directives
        bool firstDir = true;
        for (const auto& directive : element->classDirectives) {
            if (!firstDir) {
                attrs << " ";
            }
            attrs << generateClassDirective(directive.get());
            firstDir = false;
        }

        attrs << "\"";
        first = false;
    }

    return attrs.str();
}

std::string HtmlGenerator::generateClassDirective(ClassDirective* classDir) {
    if (!classDir || classDir->tokens.empty()) {
        return "";
    }

    // tokens is already a space-separated string like "container flex center"
    return classDir->tokens;
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

std::string HtmlGenerator::generateFullHtml(Component* component, const std::string& bodyContent) {
    std::ostringstream html;

    // DOCTYPE
    if (m_options.includeDoctype) {
        html << "<!DOCTYPE html>\n";
    }

    // <html lang="...">
    html << "<html lang=\"" << m_options.lang << "\">\n";

    // <head>
    html << generateHead(component);

    // <body>
    html << "<body>\n";
    html << bodyContent;
    html << "</body>\n";

    // </html>
    html << "</html>\n";

    return html.str();
}

std::string HtmlGenerator::generateHead(Component* component) {
    std::ostringstream head;

    head << "<head>\n";

    // Meta charset
    head << "  <meta charset=\"" << m_options.charset << "\">\n";

    // Viewport meta tag
    if (m_options.includeViewportMeta) {
        head << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    }

    // Title
    std::string title = extractTitle(component);
    head << "  <title>" << title << "</title>\n";

    // CSS link
    if (!m_options.cssFile.empty()) {
        head << "  <link rel=\"stylesheet\" href=\"" << m_options.cssFile << "\">\n";
    }

    head << "</head>\n";

    return head.str();
}

std::string HtmlGenerator::extractTitle(Component* component) {
    // Try to extract title from @route path
    if (component && component->route) {
        std::string path = component->route->path;

        // Remove leading/trailing slashes
        if (!path.empty() && path[0] == '/') {
            path = path.substr(1);
        }
        if (!path.empty() && path.back() == '/') {
            path = path.substr(0, path.length() - 1);
        }

        // Convert path to title
        if (path.empty() || path == "/") {
            // Root path → "Home"
            return "Home - " + m_options.title;
        } else {
            // "/about" → "About - Artic"
            // Capitalize first letter
            if (!path.empty()) {
                path[0] = std::toupper(path[0]);
            }
            return path + " - " + m_options.title;
        }
    }

    // Default title
    return m_options.title;
}

bool HtmlGenerator::isCustomComponent(const std::string& tagName) const {
    // Component tags start with uppercase letter
    if (tagName.empty()) {
        return false;
    }
    return std::isupper(tagName[0]);
}

std::string HtmlGenerator::expandCustomComponent(Element* element, int indentLevel) {
    if (!m_registry) {
        std::ostringstream html;
        if (m_options.pretty) {
            html << indent(indentLevel);
        }
        html << "<!-- Custom component: <" << element->tagName << "> -->";
        if (m_options.pretty) {
            html << "\n";
        }
        return html.str();
    }

    // Try to find component in registry
    Component* componentDef = m_registry->getComponent(element->tagName);

    if (!componentDef) {
        std::ostringstream html;
        if (m_options.pretty) {
            html << indent(indentLevel);
        }
        html << "<!-- Component not found: <" << element->tagName << "> -->";
        if (m_options.pretty) {
            html << "\n";
        }
        return html.str();
    }

    // Create render context with props and slots
    RenderContext context;

    // 1. Extract props from attributes
    for (const auto& attr : element->attributes) {
        context.props[attr->name] = attr->value;
    }

    // 2. Extract slots from children
    for (const auto& child : element->children) {
        // Check if child is an element with slot:name directive
        if (child->type == ASTNodeType::ELEMENT) {
            Element* childElement = static_cast<Element*>(child.get());
            if (childElement->slotDirective) {
                // Named slot
                std::string slotName = childElement->slotDirective->slotName;
                context.slots[slotName].push_back(child.get());
                continue;
            }
        }

        // Default slot (no slot: directive)
        context.defaultSlot.push_back(child.get());
    }

    // 3. Render component's template with context
    std::ostringstream html;

    if (m_options.pretty) {
        html << indent(indentLevel);
        html << "<!-- Component: " << element->tagName << " -->\n";
    }

    for (const auto& node : componentDef->templateNodes) {
        html << generateNode(node.get(), indentLevel, &context);
    }

    if (m_options.pretty) {
        html << indent(indentLevel);
        html << "<!-- /" << element->tagName << " -->\n";
    }

    return html.str();
}

} // namespace artic
