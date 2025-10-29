#include "frontend/lexer/Token.h"
#include <sstream>

namespace artic {

const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::USE: return "USE";
        case TokenType::FROM: return "FROM";
        case TokenType::THEME: return "THEME";
        case TokenType::PROPS: return "PROPS";
        case TokenType::EXPORT: return "EXPORT";
        case TokenType::AT_ROUTE: return "AT_ROUTE";
        case TokenType::AT_LAYOUT: return "AT_LAYOUT";
        case TokenType::AT_UTILITY: return "AT_UTILITY";
        case TokenType::AT_SERVER: return "AT_SERVER";
        case TokenType::AT_CLIENT: return "AT_CLIENT";
        case TokenType::AT_EFFECT: return "AT_EFFECT";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING: return "STRING";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::TRUE: return "TRUE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::NULL_LITERAL: return "NULL";
        case TokenType::UNDEFINED: return "UNDEFINED";
        case TokenType::EQUALS: return "EQUALS";
        case TokenType::COLON: return "COLON";
        case TokenType::QUESTION: return "QUESTION";
        case TokenType::PIPE: return "PIPE";
        case TokenType::DOT: return "DOT";
        case TokenType::COMMA: return "COMMA";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::ARROW: return "ARROW";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::BANG: return "BANG";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::LBRACKET: return "LBRACKET";
        case TokenType::RBRACKET: return "RBRACKET";
        case TokenType::LT: return "LT";
        case TokenType::GT: return "GT";
        case TokenType::LT_SLASH: return "LT_SLASH";
        case TokenType::SLASH_GT: return "SLASH_GT";
        case TokenType::COMMENT: return "COMMENT";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

bool Token::isKeyword() const {
    return type >= TokenType::USE && type <= TokenType::EXPORT;
}

bool Token::isDecorator() const {
    return type >= TokenType::AT_ROUTE && type <= TokenType::AT_EFFECT;
}

bool Token::isLiteral() const {
    return type >= TokenType::IDENTIFIER && type <= TokenType::UNDEFINED;
}

bool Token::isOperator() const {
    return type >= TokenType::EQUALS && type <= TokenType::BANG;
}

std::string Token::toString() const {
    std::stringstream ss;
    ss << tokenTypeToString(type);
    if (!lexeme.empty()) {
        ss << "('" << lexeme << "')";
    }
    ss << " [" << line << ":" << column << "]";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.toString();
    return os;
}

} // namespace artic
