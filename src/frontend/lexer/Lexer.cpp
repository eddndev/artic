#include "frontend/lexer/Lexer.h"
#include <cctype>

namespace artic {

Lexer::Lexer(const std::string& source)
    : m_source(source), m_position(0), m_location() {
    initKeywords();
}

void Lexer::initKeywords() {
    // Keywords
    m_keywords["use"] = TokenType::USE;
    m_keywords["from"] = TokenType::FROM;
    m_keywords["theme"] = TokenType::THEME;
    m_keywords["props"] = TokenType::PROPS;
    m_keywords["export"] = TokenType::EXPORT;

    // Boolean literals
    m_keywords["true"] = TokenType::TRUE;
    m_keywords["false"] = TokenType::FALSE;
    m_keywords["null"] = TokenType::NULL_LITERAL;
    m_keywords["undefined"] = TokenType::UNDEFINED;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!isAtEnd()) {
        Token token = nextToken();
        if (token.type != TokenType::COMMENT) { // Skip comments
            tokens.push_back(token);
        }
    }

    // Add EOF token
    tokens.push_back(makeToken(TokenType::END_OF_FILE, ""));

    return tokens;
}

Token Lexer::nextToken() {
    skipWhitespace();

    if (isAtEnd()) {
        return makeToken(TokenType::END_OF_FILE, "");
    }

    char c = peek();

    // Identifiers and keywords
    if (isAlpha(c) || c == '_' || c == '$') {
        return scanIdentifier();
    }

    // Numbers
    if (isDigit(c)) {
        return scanNumber();
    }

    // Decorators
    if (c == '@') {
        return scanDecorator();
    }

    // Single character tokens
    switch (c) {
        case '(': advance(); return makeToken(TokenType::LPAREN, "(");
        case ')': advance(); return makeToken(TokenType::RPAREN, ")");
        case '{': advance(); return makeToken(TokenType::LBRACE, "{");
        case '}': advance(); return makeToken(TokenType::RBRACE, "}");
        case '[': advance(); return makeToken(TokenType::LBRACKET, "[");
        case ']': advance(); return makeToken(TokenType::RBRACKET, "]");
        case ',': advance(); return makeToken(TokenType::COMMA, ",");
        case ';': advance(); return makeToken(TokenType::SEMICOLON, ";");
        case '?': advance(); return makeToken(TokenType::QUESTION, "?");
        case '|': advance(); return makeToken(TokenType::PIPE, "|");
        case '+': advance(); return makeToken(TokenType::PLUS, "+");
        case '-': advance(); return makeToken(TokenType::MINUS, "-");
        case '*': advance(); return makeToken(TokenType::STAR, "*");
        case '!': advance(); return makeToken(TokenType::BANG, "!");

        case '.':
            advance();
            return makeToken(TokenType::DOT, ".");

        case ':':
            advance();
            return makeToken(TokenType::COLON, ":");

        case '=':
            advance();
            if (match('>')) {
                return makeToken(TokenType::ARROW, "=>");
            }
            return makeToken(TokenType::EQUALS, "=");

        case '<':
            advance();
            if (match('/')) {
                return makeToken(TokenType::LT_SLASH, "</");
            }
            return makeToken(TokenType::LT, "<");

        case '>':
            advance();
            return makeToken(TokenType::GT, ">");

        case '/':
            advance();
            if (match('/')) {
                // Line comment
                skipLineComment();
                return makeToken(TokenType::COMMENT, "");
            } else if (match('*')) {
                // Block comment
                skipBlockComment();
                return makeToken(TokenType::COMMENT, "");
            } else if (match('>')) {
                // Self-closing tag
                return makeToken(TokenType::SLASH_GT, "/>");
            }
            return makeToken(TokenType::SLASH, "/");

        case '"':
        case '\'':
            return scanString(c);

        case '\n':
            advance();
            return makeToken(TokenType::NEWLINE, "\n");
    }

    // Unknown character
    char unknown = advance();
    return errorToken("Unexpected character: " + std::string(1, unknown));
}

bool Lexer::isAtEnd() const {
    return m_position >= m_source.length();
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return m_source[m_position];
}

char Lexer::peekNext() const {
    if (m_position + 1 >= m_source.length()) return '\0';
    return m_source[m_position + 1];
}

char Lexer::advance() {
    if (isAtEnd()) return '\0';
    char c = m_source[m_position++];
    m_location.advance(c);
    return c;
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (peek() != expected) return false;
    advance();
    return true;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
        } else {
            break;
        }
    }
}

void Lexer::skipLineComment() {
    // Skip until end of line
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
}

void Lexer::skipBlockComment() {
    // Skip until */
    while (!isAtEnd()) {
        if (peek() == '*' && peekNext() == '/') {
            advance(); // consume *
            advance(); // consume /
            break;
        }
        advance();
    }
}

Token Lexer::scanIdentifier() {
    int startLine = m_location.line;
    int startColumn = m_location.column;
    std::string lexeme;

    // First character (already validated as alpha, _, or $)
    lexeme += advance();

    // Continue with alphanumeric or underscore
    while (!isAtEnd() && (isAlphaNumeric(peek()) || peek() == '_' || peek() == '$')) {
        lexeme += advance();
    }

    // Check if it's a keyword
    auto it = m_keywords.find(lexeme);
    if (it != m_keywords.end()) {
        return Token(it->second, lexeme, startLine, startColumn);
    }

    return Token(TokenType::IDENTIFIER, lexeme, startLine, startColumn);
}

Token Lexer::scanString(char quote) {
    int startLine = m_location.line;
    int startColumn = m_location.column;

    advance(); // consume opening quote

    std::string value;

    while (!isAtEnd() && peek() != quote) {
        if (peek() == '\n') {
            return errorToken("Unterminated string");
        }

        // Handle escape sequences
        if (peek() == '\\' && peekNext() == quote) {
            advance(); // consume backslash
            value += advance(); // consume escaped quote
        } else if (peek() == '\\') {
            advance(); // consume backslash
            char escaped = advance();
            // Handle common escape sequences
            switch (escaped) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '\\': value += '\\'; break;
                default: value += escaped; break;
            }
        } else {
            value += advance();
        }
    }

    if (isAtEnd()) {
        return errorToken("Unterminated string");
    }

    advance(); // consume closing quote

    return Token(TokenType::STRING, value, startLine, startColumn);
}

Token Lexer::scanNumber() {
    int startLine = m_location.line;
    int startColumn = m_location.column;
    std::string lexeme;

    // Integer part
    while (!isAtEnd() && isDigit(peek())) {
        lexeme += advance();
    }

    // Decimal part
    if (!isAtEnd() && peek() == '.' && isDigit(peekNext())) {
        lexeme += advance(); // consume .

        while (!isAtEnd() && isDigit(peek())) {
            lexeme += advance();
        }
    }

    return Token(TokenType::NUMBER, lexeme, startLine, startColumn);
}

Token Lexer::scanDecorator() {
    int startLine = m_location.line;
    int startColumn = m_location.column;

    advance(); // consume @

    std::string name;
    while (!isAtEnd() && (isAlphaNumeric(peek()) || peek() == '_')) {
        name += advance();
    }

    // Map decorator names to token types
    if (name == "route") {
        return Token(TokenType::AT_ROUTE, "@route", startLine, startColumn);
    } else if (name == "layout") {
        return Token(TokenType::AT_LAYOUT, "@layout", startLine, startColumn);
    } else if (name == "utility") {
        return Token(TokenType::AT_UTILITY, "@utility", startLine, startColumn);
    } else if (name == "server") {
        return Token(TokenType::AT_SERVER, "@server", startLine, startColumn);
    } else if (name == "client") {
        return Token(TokenType::AT_CLIENT, "@client", startLine, startColumn);
    } else if (name == "effect") {
        return Token(TokenType::AT_EFFECT, "@effect", startLine, startColumn);
    }

    return errorToken("Unknown decorator: @" + name);
}

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

Token Lexer::makeToken(TokenType type, const std::string& lexeme) {
    return Token(type, lexeme, m_location.line, m_location.column);
}

Token Lexer::errorToken(const std::string& message) {
    return Token(TokenType::ERROR, message, m_location.line, m_location.column);
}

} // namespace artic
