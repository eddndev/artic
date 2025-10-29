#pragma once

#include "Token.h"
#include "SourceLocation.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace artic {

/**
 * @brief Lexer for the Artic language
 *
 * Converts source code into a stream of tokens.
 * Phase 1 focus: metadata, imports, styles, and templates
 */
class Lexer {
public:
    /**
     * @brief Construct a lexer with source code
     * @param source The source code to tokenize
     */
    explicit Lexer(const std::string& source);

    /**
     * @brief Tokenize the entire source code
     * @return Vector of tokens
     */
    std::vector<Token> tokenize();

    /**
     * @brief Get the next token
     * @return The next token, or END_OF_FILE if at end
     */
    Token nextToken();

    /**
     * @brief Check if at end of file
     */
    bool isAtEnd() const;

private:
    std::string m_source;
    size_t m_position;
    SourceLocation m_location;
    std::unordered_map<std::string, TokenType> m_keywords;

    /**
     * @brief Initialize keyword map
     */
    void initKeywords();

    /**
     * @brief Peek at current character without consuming
     */
    char peek() const;

    /**
     * @brief Peek at next character
     */
    char peekNext() const;

    /**
     * @brief Consume and return current character
     */
    char advance();

    /**
     * @brief Check if current character matches expected
     */
    bool match(char expected);

    /**
     * @brief Skip whitespace (except newlines in some contexts)
     */
    void skipWhitespace();

    /**
     * @brief Skip single-line comment
     */
    void skipLineComment();

    /**
     * @brief Skip block comment
     */
    void skipBlockComment();

    /**
     * @brief Scan identifier or keyword
     */
    Token scanIdentifier();

    /**
     * @brief Scan string literal
     */
    Token scanString(char quote);

    /**
     * @brief Scan number literal
     */
    Token scanNumber();

    /**
     * @brief Scan decorator (@route, @utility, etc)
     */
    Token scanDecorator();

    /**
     * @brief Check if character is alphabetic
     */
    bool isAlpha(char c) const;

    /**
     * @brief Check if character is digit
     */
    bool isDigit(char c) const;

    /**
     * @brief Check if character is alphanumeric
     */
    bool isAlphaNumeric(char c) const;

    /**
     * @brief Create token at current location
     */
    Token makeToken(TokenType type, const std::string& lexeme);

    /**
     * @brief Create error token
     */
    Token errorToken(const std::string& message);
};

} // namespace artic
