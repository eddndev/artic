// Lexer Unit Tests
// NOTE: Google Test integration will be added later
// For now, this is a manual test file

#include "../../../src/frontend/lexer/Lexer.h"
#include <iostream>
#include <cassert>

using namespace artic;

void testBasicTokens() {
    std::cout << "Test: Basic Tokens... ";

    Lexer lexer("( ) { } [ ] , ; : = => < > </ />");
    auto tokens = lexer.tokenize();

    assert(tokens.size() == 17); // 16 tokens + EOF
    assert(tokens[0].type == TokenType::LPAREN);
    assert(tokens[1].type == TokenType::RPAREN);
    assert(tokens[2].type == TokenType::LBRACE);
    assert(tokens[3].type == TokenType::RBRACE);
    assert(tokens[4].type == TokenType::LBRACKET);
    assert(tokens[5].type == TokenType::RBRACKET);
    assert(tokens[6].type == TokenType::COMMA);
    assert(tokens[7].type == TokenType::SEMICOLON);
    assert(tokens[8].type == TokenType::COLON);
    assert(tokens[9].type == TokenType::EQUALS);
    assert(tokens[10].type == TokenType::ARROW);
    assert(tokens[11].type == TokenType::LT);
    assert(tokens[12].type == TokenType::GT);
    assert(tokens[13].type == TokenType::LT_SLASH);
    assert(tokens[14].type == TokenType::SLASH_GT);
    assert(tokens[15].type == TokenType::END_OF_FILE);

    std::cout << "PASSED\n";
}

void testKeywords() {
    std::cout << "Test: Keywords... ";

    Lexer lexer("use from theme props export true false null undefined");
    auto tokens = lexer.tokenize();

    assert(tokens[0].type == TokenType::USE);
    assert(tokens[1].type == TokenType::FROM);
    assert(tokens[2].type == TokenType::THEME);
    assert(tokens[3].type == TokenType::PROPS);
    assert(tokens[4].type == TokenType::EXPORT);
    assert(tokens[5].type == TokenType::TRUE);
    assert(tokens[6].type == TokenType::FALSE);
    assert(tokens[7].type == TokenType::NULL_LITERAL);
    assert(tokens[8].type == TokenType::UNDEFINED);

    std::cout << "PASSED\n";
}

void testDecorators() {
    std::cout << "Test: Decorators... ";

    Lexer lexer("@route @layout @utility @server @client @effect");
    auto tokens = lexer.tokenize();

    assert(tokens[0].type == TokenType::AT_ROUTE);
    assert(tokens[0].lexeme == "@route");
    assert(tokens[1].type == TokenType::AT_LAYOUT);
    assert(tokens[2].type == TokenType::AT_UTILITY);
    assert(tokens[3].type == TokenType::AT_SERVER);
    assert(tokens[4].type == TokenType::AT_CLIENT);
    assert(tokens[5].type == TokenType::AT_EFFECT);

    std::cout << "PASSED\n";
}

void testIdentifiers() {
    std::cout << "Test: Identifiers... ";

    Lexer lexer("count userName _private $state my_var");
    auto tokens = lexer.tokenize();

    assert(tokens[0].type == TokenType::IDENTIFIER);
    assert(tokens[0].lexeme == "count");
    assert(tokens[1].type == TokenType::IDENTIFIER);
    assert(tokens[1].lexeme == "userName");
    assert(tokens[2].type == TokenType::IDENTIFIER);
    assert(tokens[2].lexeme == "_private");
    assert(tokens[3].type == TokenType::IDENTIFIER);
    assert(tokens[3].lexeme == "$state");
    assert(tokens[4].type == TokenType::IDENTIFIER);
    assert(tokens[4].lexeme == "my_var");

    std::cout << "PASSED\n";
}

void testStrings() {
    std::cout << "Test: String Literals... ";

    Lexer lexer(R"("hello" 'world' "escaped \"quote\"")");
    auto tokens = lexer.tokenize();

    assert(tokens[0].type == TokenType::STRING);
    assert(tokens[0].lexeme == "hello");
    assert(tokens[1].type == TokenType::STRING);
    assert(tokens[1].lexeme == "world");
    assert(tokens[2].type == TokenType::STRING);
    assert(tokens[2].lexeme == "escaped \"quote\"");

    std::cout << "PASSED\n";
}

void testNumbers() {
    std::cout << "Test: Number Literals... ";

    Lexer lexer("42 3.14 0 123.456");
    auto tokens = lexer.tokenize();

    assert(tokens[0].type == TokenType::NUMBER);
    assert(tokens[0].lexeme == "42");
    assert(tokens[1].type == TokenType::NUMBER);
    assert(tokens[1].lexeme == "3.14");
    assert(tokens[2].type == TokenType::NUMBER);
    assert(tokens[2].lexeme == "0");
    assert(tokens[3].type == TokenType::NUMBER);
    assert(tokens[3].lexeme == "123.456");

    std::cout << "PASSED\n";
}

void testComments() {
    std::cout << "Test: Comments... ";

    Lexer lexer("count // line comment\n/* block comment */ name");
    auto tokens = lexer.tokenize();

    // Comments are filtered out
    assert(tokens[0].type == TokenType::IDENTIFIER);
    assert(tokens[0].lexeme == "count");
    assert(tokens[1].type == TokenType::NEWLINE);
    assert(tokens[2].type == TokenType::IDENTIFIER);
    assert(tokens[2].lexeme == "name");

    std::cout << "PASSED\n";
}

void testCompleteExample() {
    std::cout << "Test: Complete Example... ";

    std::string source = R"(
        @route("/hello")

        use theme "./theme.atc"

        @utility
        btn {
            px:4 py:2
        }

        <div class:(container)>
            <h1>Hello</h1>
        </div>
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    // Verify first few tokens
    assert(tokens[0].type == TokenType::NEWLINE);
    assert(tokens[1].type == TokenType::AT_ROUTE);
    assert(tokens[2].type == TokenType::LPAREN);
    assert(tokens[3].type == TokenType::STRING);
    assert(tokens[3].lexeme == "/hello");
    assert(tokens[4].type == TokenType::RPAREN);

    // Find "use" keyword
    bool foundUse = false;
    for (const auto& token : tokens) {
        if (token.type == TokenType::USE) {
            foundUse = true;
            break;
        }
    }
    assert(foundUse);

    // Find @utility decorator
    bool foundUtility = false;
    for (const auto& token : tokens) {
        if (token.type == TokenType::AT_UTILITY) {
            foundUtility = true;
            break;
        }
    }
    assert(foundUtility);

    std::cout << "PASSED\n";
}

void testLineAndColumn() {
    std::cout << "Test: Line and Column Tracking... ";

    std::string source = "count\nname\n  value";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    assert(tokens[0].line == 1);
    assert(tokens[0].column == 1);
    assert(tokens[0].lexeme == "count");

    assert(tokens[1].line == 1); // newline
    assert(tokens[2].line == 2);
    assert(tokens[2].column == 1);
    assert(tokens[2].lexeme == "name");

    assert(tokens[3].line == 2); // newline
    assert(tokens[4].line == 3);
    assert(tokens[4].column == 3); // after 2 spaces
    assert(tokens[4].lexeme == "value");

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "=== Artic Lexer Tests ===\n\n";

    testBasicTokens();
    testKeywords();
    testDecorators();
    testIdentifiers();
    testStrings();
    testNumbers();
    testComments();
    testCompleteExample();
    testLineAndColumn();

    std::cout << "\n=== All Tests Passed! ===\n";

    return 0;
}
