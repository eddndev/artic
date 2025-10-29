// Simple validation test for Artic Parser
#include <iostream>
#include <string>

#include "src/frontend/lexer/Lexer.h"
#include "src/frontend/parser/Parser.h"

using namespace artic;

void test_SimpleRoute() {
    std::cout << "\n[TEST] Parsing @route decorator... ";

    std::string source = "@route(\"/hello\")";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        if (component->hasRoute() && component->getRoutePath() == "/hello") {
            std::cout << "PASSED ✓\n";
            std::cout << "  AST: " << component->toString() << "\n";
        } else {
            std::cout << "FAILED ✗\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_ImportStatement() {
    std::cout << "\n[TEST] Parsing import statement... ";

    std::string source = "use theme \"./theme.atc\"";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        if (component->imports.size() == 1) {
            std::cout << "PASSED ✓\n";
            std::cout << "  Import: " << component->imports[0]->toString() << "\n";
        } else {
            std::cout << "FAILED ✗\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_UtilityDeclaration() {
    std::cout << "\n[TEST] Parsing utility declaration... ";

    std::string source = R"(
        @utility
        btn {
            px:4 py:2
        }
    )";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        if (component->utilities.size() == 1) {
            std::cout << "PASSED ✓\n";
            std::cout << "  Utility name: " << component->utilities[0]->name << "\n";
            std::cout << "  Tokens: " << component->utilities[0]->tokens.size() << "\n";
        } else {
            std::cout << "FAILED ✗\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_SimpleElement() {
    std::cout << "\n[TEST] Parsing simple element... ";

    std::string source = "<div>Hello World</div>";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        if (component->templateNodes.size() == 1) {
            std::cout << "PASSED ✓\n";
            std::cout << "  Template: " << component->templateNodes[0]->toString() << "\n";
        } else {
            std::cout << "FAILED ✗\n";
        }
    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_CompleteExample() {
    std::cout << "\n[TEST] Parsing complete example... ";

    std::string source = R"(
        @route("/hello")

        use theme "./theme.atc"

        @utility
        container {
            w:full px:4
        }

        <div class:(container)>
            <h1>Hello World</h1>
        </div>
    )";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        std::cout << "PASSED ✓\n";
        std::cout << "\n=== COMPLETE AST ===\n";
        std::cout << component->toString() << "\n";
        std::cout << "====================\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

int main() {
    std::cout << "=================================\n";
    std::cout << "  ARTIC PARSER VALIDATION TESTS\n";
    std::cout << "=================================\n";

    test_SimpleRoute();
    test_ImportStatement();
    test_UtilityDeclaration();
    test_SimpleElement();
    test_CompleteExample();

    std::cout << "\n=================================\n";
    std::cout << "  VALIDATION COMPLETED!\n";
    std::cout << "=================================\n";

    return 0;
}
