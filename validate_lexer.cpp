// Simple validation test for Artic Lexer
#include <iostream>
#include <string>
#include <vector>

// Include lexer files
#include "src/frontend/lexer/Token.h"
#include "src/frontend/lexer/Lexer.h"

using namespace artic;

void test1_BasicTokens() {
    std::cout << "\n[TEST 1] Basic Tokens... ";

    Lexer lexer("( ) { }");
    auto tokens = lexer.tokenize();

    if (tokens.size() == 5 &&
        tokens[0].type == TokenType::LPAREN &&
        tokens[1].type == TokenType::RPAREN &&
        tokens[2].type == TokenType::LBRACE &&
        tokens[3].type == TokenType::RBRACE &&
        tokens[4].type == TokenType::END_OF_FILE) {
        std::cout << "PASSED ✓\n";
    } else {
        std::cout << "FAILED ✗\n";
        std::cout << "  Found " << tokens.size() << " tokens\n";
    }
}

void test2_Keywords() {
    std::cout << "[TEST 2] Keywords... ";

    Lexer lexer("use from theme");
    auto tokens = lexer.tokenize();

    if (tokens[0].type == TokenType::USE &&
        tokens[1].type == TokenType::FROM &&
        tokens[2].type == TokenType::THEME) {
        std::cout << "PASSED ✓\n";
    } else {
        std::cout << "FAILED ✗\n";
    }
}

void test3_Decorator() {
    std::cout << "[TEST 3] Decorator... ";

    Lexer lexer("@route");
    auto tokens = lexer.tokenize();

    if (tokens[0].type == TokenType::AT_ROUTE &&
        tokens[0].lexeme == "@route") {
        std::cout << "PASSED ✓\n";
    } else {
        std::cout << "FAILED ✗\n";
        std::cout << "  Got: " << tokens[0].toString() << "\n";
    }
}

void test4_String() {
    std::cout << "[TEST 4] String Literal... ";

    Lexer lexer("\"hello world\"");
    auto tokens = lexer.tokenize();

    if (tokens[0].type == TokenType::STRING &&
        tokens[0].lexeme == "hello world") {
        std::cout << "PASSED ✓\n";
    } else {
        std::cout << "FAILED ✗\n";
        std::cout << "  Got: " << tokens[0].toString() << "\n";
    }
}

void test5_Number() {
    std::cout << "[TEST 5] Number Literal... ";

    Lexer lexer("42 3.14");
    auto tokens = lexer.tokenize();

    if (tokens[0].type == TokenType::NUMBER &&
        tokens[0].lexeme == "42" &&
        tokens[1].type == TokenType::NUMBER &&
        tokens[1].lexeme == "3.14") {
        std::cout << "PASSED ✓\n";
    } else {
        std::cout << "FAILED ✗\n";
    }
}

void test6_CompleteExample() {
    std::cout << "[TEST 6] Complete Example... ";

    std::string source = "@route(\"/hello\")\n<div>Hello</div>";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    bool foundRoute = false;
    bool foundString = false;
    bool foundDiv = false;

    for (const auto& token : tokens) {
        if (token.type == TokenType::AT_ROUTE) foundRoute = true;
        if (token.type == TokenType::STRING && token.lexeme == "/hello") foundString = true;
        if (token.type == TokenType::IDENTIFIER && token.lexeme == "div") foundDiv = true;
    }

    if (foundRoute && foundString && foundDiv) {
        std::cout << "PASSED ✓\n";
    } else {
        std::cout << "FAILED ✗\n";
        std::cout << "  Found route: " << foundRoute << "\n";
        std::cout << "  Found string: " << foundString << "\n";
        std::cout << "  Found div: " << foundDiv << "\n";
    }
}

int main() {
    std::cout << "=================================\n";
    std::cout << "  ARTIC LEXER VALIDATION TESTS  \n";
    std::cout << "=================================\n";

    try {
        test1_BasicTokens();
        test2_Keywords();
        test3_Decorator();
        test4_String();
        test5_Number();
        test6_CompleteExample();

        std::cout << "\n=================================\n";
        std::cout << "  ALL TESTS COMPLETED!  \n";
        std::cout << "=================================\n";

    } catch (const std::exception& e) {
        std::cerr << "\nERROR: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
