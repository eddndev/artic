#include "frontend/parser/Parser.h"
#include <sstream>

namespace artic {

Parser::Parser(const std::vector<Token>& tokens)
    : m_tokens(tokens), m_position(0) {
    if (!m_tokens.empty()) {
        m_current = m_tokens[0];
    }
}

std::unique_ptr<Component> Parser::parse() {
    auto component = std::make_unique<Component>();

    // Skip leading newlines
    skipNewlines();

    // Parse in order: metadata → imports → utilities → template

    // 1. Metadata (@route, @layout)
    parseMetadata(component.get());

    // 2. Imports (use statements)
    parseImports(component.get());

    // 3. Utilities (@utility declarations)
    parseUtilities(component.get());

    // 4. Template (required)
    parseTemplate(component.get());

    return component;
}

// ===== Helper methods =====

Token& Parser::current() {
    return m_current;
}

Token Parser::peek(int offset) const {
    size_t pos = m_position + offset;
    if (pos >= m_tokens.size()) {
        return m_tokens.back(); // Return END_OF_FILE
    }
    return m_tokens[pos];
}

bool Parser::isAtEnd() const {
    return m_current.type == TokenType::END_OF_FILE;
}

Token Parser::advance() {
    Token previous = m_current;
    if (!isAtEnd()) {
        m_position++;
        if (m_position < m_tokens.size()) {
            m_current = m_tokens[m_position];
        }
    }
    return previous;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return m_current.type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::expect(TokenType type, const std::string& message) {
    if (check(type)) {
        return advance();
    }
    throw error(message);
}

void Parser::skipNewlines() {
    while (match(TokenType::NEWLINE)) {
        // Skip
    }
}

ParseError Parser::error(const std::string& message) {
    return ParseError(m_current, message);
}

// ===== Metadata parsing =====

void Parser::parseMetadata(Component* component) {
    // Parse @route
    if (check(TokenType::AT_ROUTE)) {
        component->route = parseRouteDecorator();
        skipNewlines();
    }

    // Parse @layout
    if (check(TokenType::AT_LAYOUT)) {
        component->layout = parseLayoutDecorator();
        skipNewlines();
    }
}

std::unique_ptr<RouteDecorator> Parser::parseRouteDecorator() {
    Token routeToken = expect(TokenType::AT_ROUTE, "Expected @route");
    expect(TokenType::LPAREN, "Expected '(' after @route");

    Token pathToken = expect(TokenType::STRING, "Expected string path in @route");

    expect(TokenType::RPAREN, "Expected ')' after route path");

    return std::make_unique<RouteDecorator>(
        pathToken.lexeme,
        SourceLocation(routeToken.line, routeToken.column, 0)
    );
}

std::unique_ptr<LayoutDecorator> Parser::parseLayoutDecorator() {
    Token layoutToken = expect(TokenType::AT_LAYOUT, "Expected @layout");
    expect(TokenType::LPAREN, "Expected '(' after @layout");

    Token nameToken = expect(TokenType::STRING, "Expected string name in @layout");

    expect(TokenType::RPAREN, "Expected ')' after layout name");

    return std::make_unique<LayoutDecorator>(
        nameToken.lexeme,
        SourceLocation(layoutToken.line, layoutToken.column, 0)
    );
}

// ===== Import parsing =====

void Parser::parseImports(Component* component) {
    while (check(TokenType::USE)) {
        skipNewlines();

        if (peek(1).type == TokenType::THEME) {
            component->addImport(parseThemeImport());
        } else if (peek(1).type == TokenType::LBRACE) {
            component->addImport(parseNamedImport());
        } else {
            throw error("Expected 'theme' or '{' after 'use'");
        }

        skipNewlines();
    }
}

std::unique_ptr<ThemeImport> Parser::parseThemeImport() {
    Token useToken = expect(TokenType::USE, "Expected 'use'");
    expect(TokenType::THEME, "Expected 'theme'");

    Token pathToken = expect(TokenType::STRING, "Expected theme path");

    return std::make_unique<ThemeImport>(
        pathToken.lexeme,
        SourceLocation(useToken.line, useToken.column, 0)
    );
}

std::unique_ptr<NamedImport> Parser::parseNamedImport() {
    Token useToken = expect(TokenType::USE, "Expected 'use'");
    expect(TokenType::LBRACE, "Expected '{'");

    // Parse names
    std::vector<std::string> names;
    do {
        Token nameToken = expect(TokenType::IDENTIFIER, "Expected identifier in import");
        names.push_back(nameToken.lexeme);

        if (!match(TokenType::COMMA)) {
            break;
        }
    } while (true);

    expect(TokenType::RBRACE, "Expected '}'");
    expect(TokenType::FROM, "Expected 'from'");

    Token pathToken = expect(TokenType::STRING, "Expected import path");

    return std::make_unique<NamedImport>(
        names,
        pathToken.lexeme,
        SourceLocation(useToken.line, useToken.column, 0)
    );
}

// ===== Utility parsing =====

void Parser::parseUtilities(Component* component) {
    while (check(TokenType::AT_UTILITY)) {
        component->addUtility(parseUtilityDeclaration());
        skipNewlines();
    }
}

std::unique_ptr<UtilityDeclaration> Parser::parseUtilityDeclaration() {
    Token utilityToken = expect(TokenType::AT_UTILITY, "Expected @utility");
    skipNewlines();

    Token nameToken = expect(TokenType::IDENTIFIER, "Expected utility name");
    skipNewlines();

    expect(TokenType::LBRACE, "Expected '{' after utility name");
    skipNewlines();

    auto utility = std::make_unique<UtilityDeclaration>(
        nameToken.lexeme,
        SourceLocation(utilityToken.line, utilityToken.column, 0)
    );

    // Parse utility tokens
    auto tokens = parseUtilityTokens();
    for (auto& token : tokens) {
        utility->addToken(std::move(token));
    }

    expect(TokenType::RBRACE, "Expected '}' after utility tokens");

    return utility;
}

std::vector<std::unique_ptr<UtilityToken>> Parser::parseUtilityTokens() {
    std::vector<std::unique_ptr<UtilityToken>> tokens;

    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        skipNewlines();

        if (check(TokenType::RBRACE)) {
            break;
        }

        auto token = parseUtilityToken();
        if (token) {
            tokens.push_back(std::move(token));
        }

        skipNewlines();
    }

    return tokens;
}

std::unique_ptr<UtilityToken> Parser::parseUtilityToken() {
    if (!check(TokenType::IDENTIFIER)) {
        return nullptr;
    }

    Token first = advance();
    std::string prefix;
    std::string property = first.lexeme;
    std::string value;

    // Check for prefix:property:value (e.g., hover:bg:blue)
    if (match(TokenType::COLON)) {
        if (check(TokenType::IDENTIFIER)) {
            prefix = property;
            property = advance().lexeme;

            if (match(TokenType::COLON)) {
                if (check(TokenType::IDENTIFIER) || check(TokenType::NUMBER)) {
                    Token valueToken = advance();
                    value = valueToken.lexeme;

                    // Handle compound values like "blue-500"
                    while (match(TokenType::MINUS)) {
                        if (check(TokenType::IDENTIFIER) || check(TokenType::NUMBER)) {
                            value += "-" + advance().lexeme;
                        }
                    }
                }
            }
        }
    }

    return std::make_unique<UtilityToken>(
        property,
        value,
        prefix,
        SourceLocation(first.line, first.column, 0)
    );
}

// ===== Template parsing =====

void Parser::parseTemplate(Component* component) {
    skipNewlines();

    while (!isAtEnd() && check(TokenType::LT)) {
        auto node = parseTemplateNode();
        if (node) {
            component->addTemplateNode(std::move(node));
        }
        skipNewlines();
    }
}

std::unique_ptr<TemplateNode> Parser::parseTemplateNode() {
    if (check(TokenType::LT)) {
        return parseElement();
    } else if (check(TokenType::LBRACE)) {
        return parseExpressionNode();
    } else {
        // Text node
        return parseTextNode();
    }
}

std::unique_ptr<Element> Parser::parseElement() {
    Token ltToken = expect(TokenType::LT, "Expected '<'");

    // Get tag name
    Token tagToken = expect(TokenType::IDENTIFIER, "Expected tag name");
    std::string tagName = tagToken.lexeme;

    auto element = std::make_unique<Element>(
        tagName,
        false,
        SourceLocation(ltToken.line, ltToken.column, 0)
    );

    // Parse attributes and directives
    parseAttributesAndDirectives(element.get());

    // Check for self-closing />
    if (match(TokenType::SLASH_GT)) {
        element->isSelfClosing = true;
        return element;
    }

    // Expect >
    expect(TokenType::GT, "Expected '>' or '/>'");

    // Parse children (until closing tag)
    while (!check(TokenType::LT_SLASH) && !isAtEnd()) {
        // Check for text content
        if (!check(TokenType::LT) && !check(TokenType::LBRACE)) {
            auto textNode = parseTextNode();
            if (textNode && !textNode->text.empty()) {
                element->addChild(std::move(textNode));
            }
        } else {
            auto child = parseTemplateNode();
            if (child) {
                element->addChild(std::move(child));
            }
        }

        // Break if we hit the closing tag
        if (check(TokenType::LT_SLASH)) {
            break;
        }
    }

    // Parse closing tag
    expect(TokenType::LT_SLASH, "Expected '</'");
    Token closingTag = expect(TokenType::IDENTIFIER, "Expected closing tag name");

    if (closingTag.lexeme != tagName) {
        throw error("Closing tag '" + closingTag.lexeme + "' doesn't match opening tag '" + tagName + "'");
    }

    expect(TokenType::GT, "Expected '>' after closing tag");

    return element;
}

void Parser::parseAttributesAndDirectives(Element* element) {
    while (!check(TokenType::GT) && !check(TokenType::SLASH_GT) && !isAtEnd()) {
        skipNewlines();

        if (check(TokenType::GT) || check(TokenType::SLASH_GT)) {
            break;
        }

        // Check for class: directive
        if (check(TokenType::IDENTIFIER)) {
            Token nameToken = current();

            if (nameToken.lexeme == "class" && peek(1).type == TokenType::COLON) {
                advance(); // consume "class"
                element->addClassDirective(parseClassDirective());
                continue;
            }
        }

        // Regular attribute
        if (check(TokenType::IDENTIFIER)) {
            Token attrName = advance();

            if (match(TokenType::EQUALS)) {
                // Attribute with value
                if (check(TokenType::STRING)) {
                    Token value = advance();
                    element->addAttribute(std::make_unique<Attribute>(
                        attrName.lexeme,
                        value.lexeme,
                        false
                    ));
                } else if (match(TokenType::LBRACE)) {
                    // Dynamic attribute {expression}
                    std::string expr = collectText(); // Collect until }
                    expect(TokenType::RBRACE, "Expected '}'");
                    element->addAttribute(std::make_unique<Attribute>(
                        attrName.lexeme,
                        expr,
                        true
                    ));
                }
            } else {
                // Boolean attribute (no value)
                element->addAttribute(std::make_unique<Attribute>(
                    attrName.lexeme,
                    "",
                    false
                ));
            }
        } else {
            break;
        }
    }
}

std::unique_ptr<ClassDirective> Parser::parseClassDirective() {
    expect(TokenType::COLON, "Expected ':' after 'class'");
    expect(TokenType::LPAREN, "Expected '(' after 'class:'");

    // Collect everything until matching )
    std::string tokens;
    int parenDepth = 1;

    while (parenDepth > 0 && !isAtEnd()) {
        if (check(TokenType::LPAREN)) {
            parenDepth++;
            tokens += "(";
            advance();
        } else if (check(TokenType::RPAREN)) {
            parenDepth--;
            if (parenDepth > 0) {
                tokens += ")";
            }
            advance();
        } else {
            Token t = advance();
            if (!tokens.empty() && tokens.back() != '(' && tokens.back() != ':') {
                tokens += " ";
            }
            tokens += t.lexeme;
        }
    }

    return std::make_unique<ClassDirective>(tokens);
}

std::unique_ptr<TextNode> Parser::parseTextNode() {
    std::string text = collectText();

    if (text.empty()) {
        return nullptr;
    }

    return std::make_unique<TextNode>(text);
}

std::unique_ptr<ExpressionNode> Parser::parseExpressionNode() {
    expect(TokenType::LBRACE, "Expected '{'");

    std::string expr = collectText();

    expect(TokenType::RBRACE, "Expected '}'");

    return std::make_unique<ExpressionNode>(expr);
}

std::string Parser::collectText() {
    std::string text;

    while (!check(TokenType::LT) &&
           !check(TokenType::LT_SLASH) &&
           !check(TokenType::LBRACE) &&
           !check(TokenType::RBRACE) &&
           !check(TokenType::GT) &&
           !check(TokenType::SLASH_GT) &&
           !isAtEnd()) {

        Token t = advance();
        if (!text.empty()) {
            text += " ";
        }
        text += t.lexeme;
    }

    return text;
}

} // namespace artic
