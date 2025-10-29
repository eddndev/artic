#pragma once

#include "frontend/lexer/Token.h"
#include <string>
#include <stdexcept>

namespace artic {

/**
 * @brief Exception thrown when parsing fails
 */
class ParseError : public std::runtime_error {
public:
    Token token;
    std::string message;

    ParseError(const Token& token, const std::string& message)
        : std::runtime_error(formatError(token, message)),
          token(token), message(message) {}

private:
    static std::string formatError(const Token& token, const std::string& message) {
        return "Parse error at line " + std::to_string(token.line) +
               ", column " + std::to_string(token.column) +
               ": " + message;
    }
};

} // namespace artic
