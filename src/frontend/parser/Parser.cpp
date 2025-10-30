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

    // Parse in order: metadata → imports → props → utilities → template

    // 1. Metadata (@route, @layout)
    parseMetadata(component.get());

    // 2. Imports (use statements)
    parseImports(component.get());

    // 3. Props (optional, only for reusable components)
    parseProps(component.get());

    // 4. Utilities (@utility declarations)
    parseUtilities(component.get());

    // 5. Template (required)
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

// ===== Props parsing =====

void Parser::parseProps(Component* component) {
    skipNewlines();

    if (check(TokenType::PROPS)) {
        component->props = parsePropsBlock();
        skipNewlines();
    }
}

std::unique_ptr<PropsBlock> Parser::parsePropsBlock() {
    Token propsToken = expect(TokenType::PROPS, "Expected 'props'");
    skipNewlines();

    expect(TokenType::LBRACE, "Expected '{' after 'props'");
    skipNewlines();

    auto propsBlock = std::make_unique<PropsBlock>(
        SourceLocation(propsToken.line, propsToken.column, 0)
    );

    // Parse prop declarations until }
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        skipNewlines();

        if (check(TokenType::RBRACE)) {
            break;
        }

        auto prop = parsePropDeclaration();
        if (prop) {
            propsBlock->addProp(std::move(prop));
        }

        // Expect comma or newline between props
        if (match(TokenType::COMMA)) {
            skipNewlines();
        } else {
            skipNewlines();
        }
    }

    expect(TokenType::RBRACE, "Expected '}' after props block");

    return propsBlock;
}

std::unique_ptr<PropDeclaration> Parser::parsePropDeclaration() {
    // Parse prop name
    Token nameToken = expect(TokenType::IDENTIFIER, "Expected prop name");
    std::string propName = nameToken.lexeme;

    // Check for optional ?
    bool optional = match(TokenType::QUESTION);

    // Expect :
    expect(TokenType::COLON, "Expected ':' after prop name");
    skipNewlines();

    // Parse type (simple version - just collect tokens until comma/equals/rbrace)
    auto propType = parsePropType();

    // Check for default value (= defaultValue)
    std::string defaultValue;
    if (match(TokenType::EQUALS)) {
        skipNewlines();
        // Collect default value until comma or newline or rbrace
        defaultValue = collectUntil({TokenType::COMMA, TokenType::NEWLINE, TokenType::RBRACE});
    }

    return std::make_unique<PropDeclaration>(
        propName,
        std::move(propType),
        optional,
        defaultValue,
        SourceLocation(nameToken.line, nameToken.column, 0)
    );
}

std::unique_ptr<PropType> Parser::parsePropType() {
    // Phase 1: Simple type parsing - just collect tokens until , = or }
    // Examples: string, number, "primary" | "secondary", string[], etc.

    std::string typeString;
    int parenDepth = 0;
    int bracketDepth = 0;

    while (!isAtEnd()) {
        // Stop conditions
        if (parenDepth == 0 && bracketDepth == 0) {
            if (check(TokenType::COMMA) || check(TokenType::EQUALS) ||
                check(TokenType::RBRACE) || check(TokenType::NEWLINE)) {
                break;
            }
        }

        Token t = advance();

        // Track nesting
        if (t.type == TokenType::LPAREN) parenDepth++;
        if (t.type == TokenType::RPAREN) parenDepth--;
        if (t.type == TokenType::LBRACKET) bracketDepth++;
        if (t.type == TokenType::RBRACKET) bracketDepth--;

        // Add to type string
        if (!typeString.empty() && typeString.back() != '(' && typeString.back() != '[') {
            typeString += " ";
        }
        typeString += t.lexeme;
    }

    return std::make_unique<PropType>(typeString);
}

std::string Parser::collectUntil(const std::vector<TokenType>& stopTokens) {
    std::string result;

    while (!isAtEnd()) {
        // Check if we hit a stop token
        bool shouldStop = false;
        for (auto stopType : stopTokens) {
            if (check(stopType)) {
                shouldStop = true;
                break;
            }
        }

        if (shouldStop) {
            break;
        }

        Token t = advance();
        if (!result.empty()) {
            result += " ";
        }
        result += t.lexeme;
    }

    return result;
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
        size_t positionBefore = m_position;
        skipNewlines();

        if (check(TokenType::RBRACE)) {
            break;
        }

        auto token = parseUtilityToken();
        if (token) {
            tokens.push_back(std::move(token));
        }

        // Safety check: if we didn't advance, force advancement to prevent infinite loop
        if (m_position == positionBefore && !check(TokenType::RBRACE) && !isAtEnd()) {
            advance(); // Force advance to prevent infinite loop
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

    // Check for colon (property:value or prefix:property:value)
    if (match(TokenType::COLON)) {
        // Next could be a value or another property
        if (check(TokenType::IDENTIFIER) || check(TokenType::NUMBER)) {
            Token second = advance();

            // Check if there's another colon (prefix:property:value case)
            if (match(TokenType::COLON)) {
                // This was prefix:property:value
                prefix = property;
                property = second.lexeme;

                // Now parse the value
                if (check(TokenType::IDENTIFIER) || check(TokenType::NUMBER)) {
                    Token valueToken = advance();
                    value = valueToken.lexeme;

                    // Handle adjacent identifier after number (3xl, 2xl, etc.)
                    if (valueToken.type == TokenType::NUMBER && check(TokenType::IDENTIFIER)) {
                        value += advance().lexeme;
                    }

                    // Handle compound values like "blue-500"
                    while (match(TokenType::MINUS)) {
                        if (check(TokenType::IDENTIFIER) || check(TokenType::NUMBER)) {
                            value += "-" + advance().lexeme;
                        }
                    }
                }
            } else {
                // This was property:value (2 parts)
                value = second.lexeme;

                // Handle adjacent identifier after number (3xl, 2xl, etc.)
                if (second.type == TokenType::NUMBER && check(TokenType::IDENTIFIER)) {
                    value += advance().lexeme;
                }

                // Handle compound values like "blue-500"
                while (match(TokenType::MINUS)) {
                    if (check(TokenType::IDENTIFIER) || check(TokenType::NUMBER)) {
                        value += "-" + advance().lexeme;
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
        // Peek ahead to check if it's a slot node
        if (peek(1).type == TokenType::SLOT) {
            return parseSlotNode();
        } else {
            return parseElement();
        }
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
        size_t positionBefore = m_position;

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

        // Safety check: if we didn't advance, force advancement to prevent infinite loop
        if (m_position == positionBefore && !check(TokenType::LT_SLASH) && !isAtEnd()) {
            advance(); // Force advance to prevent infinite loop
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
        size_t positionBefore = m_position;
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

        // Check for slot:name directive
        if (check(TokenType::SLOT) && peek(1).type == TokenType::COLON) {
            advance(); // consume "slot"
            element->setSlotDirective(parseSlotDirective());
            continue;
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
            // Safety: if we couldn't parse anything, break to avoid infinite loop
            if (m_position == positionBefore) {
                break;
            }
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

std::unique_ptr<SlotDirective> Parser::parseSlotDirective() {
    expect(TokenType::COLON, "Expected ':' after 'slot'");
    Token nameToken = expect(TokenType::IDENTIFIER, "Expected slot name after 'slot:'");

    return std::make_unique<SlotDirective>(nameToken.lexeme);
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

std::unique_ptr<SlotNode> Parser::parseSlotNode() {
    Token ltToken = expect(TokenType::LT, "Expected '<'");
    expect(TokenType::SLOT, "Expected 'slot'");

    std::string slotName;

    // Check for :name (named slot)
    if (match(TokenType::COLON)) {
        Token nameToken = expect(TokenType::IDENTIFIER, "Expected slot name after ':'");
        slotName = nameToken.lexeme;
    }

    auto slotNode = std::make_unique<SlotNode>(
        slotName,
        SourceLocation(ltToken.line, ltToken.column, 0)
    );

    // Check for self-closing />
    if (match(TokenType::SLASH_GT)) {
        return slotNode;
    }

    // Expect >
    expect(TokenType::GT, "Expected '>' or '/>'");

    // Parse fallback content until closing tag
    while (!check(TokenType::LT_SLASH) && !isAtEnd()) {
        size_t positionBefore = m_position;

        // Check for text content
        if (!check(TokenType::LT) && !check(TokenType::LBRACE)) {
            auto textNode = parseTextNode();
            if (textNode && !textNode->text.empty()) {
                slotNode->addFallback(std::move(textNode));
            }
        } else {
            auto child = parseTemplateNode();
            if (child) {
                slotNode->addFallback(std::move(child));
            }
        }

        // Safety check
        if (m_position == positionBefore && !check(TokenType::LT_SLASH) && !isAtEnd()) {
            advance();
        }

        if (check(TokenType::LT_SLASH)) {
            break;
        }
    }

    // Parse closing tag: </slot> or </slot:name>
    expect(TokenType::LT_SLASH, "Expected '</'");
    expect(TokenType::SLOT, "Expected 'slot' in closing tag");

    // If named slot, expect :name in closing tag too
    if (!slotName.empty()) {
        expect(TokenType::COLON, "Expected ':' in closing tag");
        Token closingNameToken = expect(TokenType::IDENTIFIER, "Expected slot name in closing tag");

        if (closingNameToken.lexeme != slotName) {
            throw error("Closing slot name '" + closingNameToken.lexeme +
                       "' doesn't match opening slot name '" + slotName + "'");
        }
    }

    expect(TokenType::GT, "Expected '>' after closing slot tag");

    return slotNode;
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

        // Add space before token only if:
        // 1. Text is not empty
        // 2. Token is not punctuation
        // 3. Previous character is not punctuation
        if (!text.empty()) {
            char lastChar = text.back();
            std::string punctuation = ".,!?;:";

            // Don't add space before punctuation
            if (punctuation.find(t.lexeme[0]) == std::string::npos) {
                // Don't add space if last char was opening punctuation
                if (lastChar != '(' && lastChar != '{' && lastChar != '[') {
                    text += " ";
                }
            }
        }

        text += t.lexeme;
    }

    return text;
}

} // namespace artic
