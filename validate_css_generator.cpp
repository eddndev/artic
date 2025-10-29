// Validation test for Artic CSS Generator
#include <iostream>
#include <string>

#include "src/frontend/lexer/Lexer.h"
#include "src/frontend/parser/Parser.h"
#include "src/backend/css/CssGenerator.h"

using namespace artic;

void printSeparator() {
    std::cout << "\n" << std::string(60, '=') << "\n";
}

void test_SimpleSpacing() {
    std::cout << "\n[TEST 1] Simple spacing utility (px:4, py:2)... ";

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

        CssGenerator generator;
        std::string css = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated CSS:\n";
        std::cout << css << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_ColorUtility() {
    std::cout << "\n[TEST 2] Color utility (bg:blue-500, text:white)... ";

    std::string source = R"(
        @utility
        primary {
            bg:blue-500
            text:white
        }
    )";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        CssGenerator generator;
        std::string css = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated CSS:\n";
        std::cout << css << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_HoverPrefix() {
    std::cout << "\n[TEST 3] Hover prefix (hover:bg:blue-700)... ";

    std::string source = R"(
        @utility
        btn {
            bg:blue-500
            hover:bg:blue-700
        }
    )";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        CssGenerator generator;
        std::string css = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated CSS:\n";
        std::cout << css << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_ResponsivePrefix() {
    std::cout << "\n[TEST 4] Responsive prefix (sm:px:6, md:px:8)... ";

    std::string source = R"(
        @utility
        container {
            px:4
            sm:px:6
            md:px:8
        }
    )";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        CssGenerator generator;
        std::string css = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated CSS:\n";
        std::cout << css << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_CompleteButton() {
    std::cout << "\n[TEST 5] Complete button utility... ";

    std::string source = R"(
        @utility
        btn {
            px:4 py:2
            bg:blue-500
            text:white
            rounded:md
            hover:bg:blue-700
            focus:bg:blue-800
        }
    )";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        CssGenerator generator;
        std::string css = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated CSS:\n";
        std::cout << css << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_MultipleUtilities() {
    std::cout << "\n[TEST 6] Multiple utilities... ";

    std::string source = R"(
        @utility
        container {
            w:full
            px:4
        }

        @utility
        title {
            text:2xl
            font:bold
        }
    )";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        CssGenerator generator;
        std::string css = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated CSS:\n";
        std::cout << css << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_CompleteExample() {
    std::cout << "\n[TEST 7] Complete component with route + utility... ";

    std::string source = R"(
        @route("/dashboard")

        use theme "./theme.atc"

        @utility
        card {
            bg:white
            rounded:lg
            p:6
            hover:bg:gray-50
        }

        <div class:(card)>
            <h1>Dashboard</h1>
        </div>
    )";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        CssGenerator generator;
        std::string css = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated CSS:\n";
        std::cout << css << "\n";
        std::cout << "\nComponent Route: " << component->getRoutePath() << "\n";
        std::cout << "Template nodes: " << component->templateNodes.size() << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

int main() {
    printSeparator();
    std::cout << "  ARTIC CSS GENERATOR VALIDATION TESTS\n";
    printSeparator();

    test_SimpleSpacing();
    printSeparator();

    test_ColorUtility();
    printSeparator();

    test_HoverPrefix();
    printSeparator();

    test_ResponsivePrefix();
    printSeparator();

    test_CompleteButton();
    printSeparator();

    test_MultipleUtilities();
    printSeparator();

    test_CompleteExample();
    printSeparator();

    std::cout << "\n  VALIDATION COMPLETED!\n";
    printSeparator();

    return 0;
}
