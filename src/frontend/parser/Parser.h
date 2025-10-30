#pragma once

#include "frontend/lexer/Token.h"
#include "frontend/lexer/Lexer.h"
#include "frontend/ast/Component.h"
#include "ParseError.h"
#include <vector>
#include <memory>
#include <string>

namespace artic {

/**
 * @brief Recursive descent parser for Artic language (Phase 1: SSG)
 *
 * Parses tokens into an Abstract Syntax Tree (AST).
 * Phase 1 focus: metadata, imports, styles, and templates
 */
class Parser {
public:
    /**
     * @brief Construct parser with tokens
     * @param tokens Vector of tokens from Lexer
     */
    explicit Parser(const std::vector<Token>& tokens);

    /**
     * @brief Parse tokens into Component AST
     * @return Unique pointer to Component node
     * @throws ParseError on syntax error
     */
    std::unique_ptr<Component> parse();

private:
    std::vector<Token> m_tokens;
    size_t m_position;
    Token m_current;

    // ===== Helper methods =====

    /**
     * @brief Get current token
     */
    Token& current();

    /**
     * @brief Peek at next token without consuming
     */
    Token peek(int offset = 1) const;

    /**
     * @brief Check if at end of tokens
     */
    bool isAtEnd() const;

    /**
     * @brief Consume and return current token
     */
    Token advance();

    /**
     * @brief Check if current token matches type
     */
    bool check(TokenType type) const;

    /**
     * @brief Consume token if it matches type
     * @return true if consumed, false otherwise
     */
    bool match(TokenType type);

    /**
     * @brief Consume token and verify it matches expected type
     * @throws ParseError if type doesn't match
     */
    Token expect(TokenType type, const std::string& message);

    /**
     * @brief Skip newlines and whitespace
     */
    void skipNewlines();

    /**
     * @brief Create parse error
     */
    ParseError error(const std::string& message);

    // ===== Parsing methods =====

    /**
     * @brief Parse metadata (@route, @layout)
     */
    void parseMetadata(Component* component);

    /**
     * @brief Parse @route decorator
     */
    std::unique_ptr<RouteDecorator> parseRouteDecorator();

    /**
     * @brief Parse @layout decorator
     */
    std::unique_ptr<LayoutDecorator> parseLayoutDecorator();

    /**
     * @brief Parse imports (use statements)
     */
    void parseImports(Component* component);

    /**
     * @brief Parse theme import: use theme "path"
     */
    std::unique_ptr<ThemeImport> parseThemeImport();

    /**
     * @brief Parse named import: use { names } from "path"
     */
    std::unique_ptr<NamedImport> parseNamedImport();

    /**
     * @brief Parse props block (optional)
     */
    void parseProps(Component* component);

    /**
     * @brief Parse props { ... } block
     */
    std::unique_ptr<PropsBlock> parsePropsBlock();

    /**
     * @brief Parse single prop declaration: name: type
     */
    std::unique_ptr<PropDeclaration> parsePropDeclaration();

    /**
     * @brief Parse prop type (string, number, etc.)
     */
    std::unique_ptr<PropType> parsePropType();

    /**
     * @brief Collect tokens until one of the stop tokens is found
     */
    std::string collectUntil(const std::vector<TokenType>& stopTokens);

    /**
     * @brief Parse utility declarations
     */
    void parseUtilities(Component* component);

    /**
     * @brief Parse single utility: @utility name { tokens }
     */
    std::unique_ptr<UtilityDeclaration> parseUtilityDeclaration();

    /**
     * @brief Parse utility tokens inside { }
     */
    std::vector<std::unique_ptr<UtilityToken>> parseUtilityTokens();

    /**
     * @brief Parse single utility token (px:4, hover:bg:blue)
     */
    std::unique_ptr<UtilityToken> parseUtilityToken();

    /**
     * @brief Parse template nodes
     */
    void parseTemplate(Component* component);

    /**
     * @brief Parse single template node (element, text, expression)
     */
    std::unique_ptr<TemplateNode> parseTemplateNode();

    /**
     * @brief Parse element: <tag>...</tag>
     */
    std::unique_ptr<Element> parseElement();

    /**
     * @brief Parse attributes and directives for an element
     */
    void parseAttributesAndDirectives(Element* element);

    /**
     * @brief Parse class directive: class:(tokens)
     */
    std::unique_ptr<ClassDirective> parseClassDirective();

    /**
     * @brief Parse slot directive: slot:name
     */
    std::unique_ptr<SlotDirective> parseSlotDirective();

    /**
     * @brief Parse text node
     */
    std::unique_ptr<TextNode> parseTextNode();

    /**
     * @brief Parse expression node: {expression}
     */
    std::unique_ptr<ExpressionNode> parseExpressionNode();

    /**
     * @brief Parse slot node: <slot /> or <slot:name />
     */
    std::unique_ptr<SlotNode> parseSlotNode();

    /**
     * @brief Collect text until < or { is found
     */
    std::string collectText();
};

} // namespace artic
