#pragma once

#include <string>
#include <ostream>

namespace artic {

/**
 * @brief Token types for the Artic language
 *
 * Phase 1 (SSG) focuses on: metadata, imports, styles, and templates
 */
enum class TokenType {
    // End of file
    END_OF_FILE,

    // Keywords
    USE,           // use
    FROM,          // from
    THEME,         // theme
    PROPS,         // props
    EXPORT,        // export

    // Decorators
    AT_ROUTE,      // @route
    AT_LAYOUT,     // @layout
    AT_UTILITY,    // @utility
    AT_SERVER,     // @server (for later phases)
    AT_CLIENT,     // @client (for later phases)
    AT_EFFECT,     // @effect (for later phases)

    // Literals
    IDENTIFIER,    // variable names, function names
    STRING,        // "string" or 'string'
    NUMBER,        // 123, 45.67
    TRUE,          // true
    FALSE,         // false
    NULL_LITERAL,  // null
    UNDEFINED,     // undefined

    // Operators
    EQUALS,        // =
    COLON,         // :
    QUESTION,      // ?
    PIPE,          // |
    DOT,           // .
    COMMA,         // ,
    SEMICOLON,     // ;
    ARROW,         // =>
    PLUS,          // +
    MINUS,         // -
    STAR,          // *
    SLASH,         // /
    BANG,          // !

    // Delimiters
    LPAREN,        // (
    RPAREN,        // )
    LBRACE,        // {
    RBRACE,        // }
    LBRACKET,      // [
    RBRACKET,      // ]

    // HTML/Template tokens
    LT,            // <
    GT,            // >
    LT_SLASH,      // </
    SLASH_GT,      // />

    // Comments
    COMMENT,       // // or /* */

    // Special
    NEWLINE,       // \n (might be significant in some contexts)

    // Error token
    ERROR
};

/**
 * @brief Convert TokenType to string for debugging
 */
const char* tokenTypeToString(TokenType type);

/**
 * @brief Token structure
 */
struct Token {
    TokenType type;
    std::string lexeme;      // The actual text from source
    int line;                // Line number (1-indexed)
    int column;              // Column number (1-indexed)

    Token()
        : type(TokenType::ERROR), lexeme(""), line(0), column(0) {}

    Token(TokenType type, const std::string& lexeme, int line, int column)
        : type(type), lexeme(lexeme), line(line), column(column) {}

    /**
     * @brief Check if token is a keyword
     */
    bool isKeyword() const;

    /**
     * @brief Check if token is a decorator
     */
    bool isDecorator() const;

    /**
     * @brief Check if token is a literal
     */
    bool isLiteral() const;

    /**
     * @brief Check if token is an operator
     */
    bool isOperator() const;

    /**
     * @brief Get string representation for debugging
     */
    std::string toString() const;
};

/**
 * @brief Output operator for Token (for debugging)
 */
std::ostream& operator<<(std::ostream& os, const Token& token);

} // namespace artic
