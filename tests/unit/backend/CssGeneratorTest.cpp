// Unit tests for Artic CSS Generator
#include <iostream>
#include <string>
#include <cassert>

#include "frontend/lexer/Lexer.h"
#include "frontend/parser/Parser.h"
#include "backend/css/CssGenerator.h"

using namespace artic;

void assert_contains(const std::string& haystack, const std::string& needle, const std::string& testName) {
    if (haystack.find(needle) == std::string::npos) {
        std::cerr << "FAILED: " << testName << "\n";
        std::cerr << "  Expected to find: \"" << needle << "\"\n";
        std::cerr << "  In: \"" << haystack << "\"\n";
        assert(false);
    }
}

void test1_SimpleSpacing() {
    std::cout << "Test 1: Simple spacing (px:4 py:2)... ";

    std::string source = R"(
        @utility
        btn {
            px:4 py:2
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    CssGenerator generator;
    std::string css = generator.generate(component.get());

    // Verify CSS contains expected declarations
    assert_contains(css, ".btn", "Test 1");
    assert_contains(css, "padding-left: 1rem", "Test 1");
    assert_contains(css, "padding-right: 1rem", "Test 1");
    assert_contains(css, "padding-top: 0.5rem", "Test 1");
    assert_contains(css, "padding-bottom: 0.5rem", "Test 1");

    std::cout << "PASSED ✓\n";
}

void test2_ColorUtility() {
    std::cout << "Test 2: Color utility (bg:blue-500)... ";

    std::string source = R"(
        @utility
        primary {
            bg:blue-500
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    CssGenerator generator;
    std::string css = generator.generate(component.get());

    assert_contains(css, ".primary", "Test 2");
    assert_contains(css, "background-color: #3b82f6", "Test 2");

    std::cout << "PASSED ✓\n";
}

void test3_TextColor() {
    std::cout << "Test 3: Text color (text:white)... ";

    std::string source = R"(
        @utility
        light {
            text:white
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    CssGenerator generator;
    std::string css = generator.generate(component.get());

    assert_contains(css, ".light", "Test 3");
    assert_contains(css, "color: #ffffff", "Test 3");

    std::cout << "PASSED ✓\n";
}

void test4_HoverPrefix() {
    std::cout << "Test 4: Hover prefix (hover:bg:blue-700)... ";

    std::string source = R"(
        @utility
        btn {
            bg:blue-500
            hover:bg:blue-700
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    CssGenerator generator;
    std::string css = generator.generate(component.get());

    assert_contains(css, ".btn", "Test 4");
    assert_contains(css, "background-color: #3b82f6", "Test 4");
    assert_contains(css, ".btn:hover", "Test 4");
    assert_contains(css, "background-color: #1d4ed8", "Test 4 hover");

    std::cout << "PASSED ✓\n";
}

void test5_FocusPrefix() {
    std::cout << "Test 5: Focus prefix (focus:bg:blue-800)... ";

    std::string source = R"(
        @utility
        input {
            focus:bg:blue-800
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    CssGenerator generator;
    std::string css = generator.generate(component.get());

    assert_contains(css, ".input:focus", "Test 5");
    assert_contains(css, "background-color: #1e40af", "Test 5");

    std::cout << "PASSED ✓\n";
}

void test6_ResponsiveSmall() {
    std::cout << "Test 6: Responsive sm: prefix (sm:px:6)... ";

    std::string source = R"(
        @utility
        container {
            px:4
            sm:px:6
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    CssGenerator generator;
    std::string css = generator.generate(component.get());

    assert_contains(css, ".container", "Test 6");
    assert_contains(css, "padding-left: 1rem", "Test 6");
    assert_contains(css, "@media (min-width: 640px)", "Test 6");
    assert_contains(css, "padding-left: 1.5rem", "Test 6 sm");

    std::cout << "PASSED ✓\n";
}

void test7_ResponsiveMedium() {
    std::cout << "Test 7: Responsive md: prefix (md:px:8)... ";

    std::string source = R"(
        @utility
        section {
            md:px:8
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    CssGenerator generator;
    std::string css = generator.generate(component.get());

    assert_contains(css, "@media (min-width: 768px)", "Test 7");
    assert_contains(css, ".section", "Test 7");
    assert_contains(css, "padding-left: 2rem", "Test 7");

    std::cout << "PASSED ✓\n";
}

void test8_BorderRadius() {
    std::cout << "Test 8: Border radius (rounded:md)... ";

    std::string source = R"(
        @utility
        card {
            rounded:md
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    CssGenerator generator;
    std::string css = generator.generate(component.get());

    assert_contains(css, ".card", "Test 8");
    assert_contains(css, "border-radius: 0.375rem", "Test 8");

    std::cout << "PASSED ✓\n";
}

void test9_FontWeight() {
    std::cout << "Test 9: Font weight (font:bold)... ";

    std::string source = R"(
        @utility
        title {
            font:bold
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    CssGenerator generator;
    std::string css = generator.generate(component.get());

    assert_contains(css, ".title", "Test 9");
    assert_contains(css, "font-weight: 700", "Test 9");

    std::cout << "PASSED ✓\n";
}

void test10_WidthHeight() {
    std::cout << "Test 10: Width and height (w:full h:full)... ";

    std::string source = R"(
        @utility
        fullscreen {
            w:full h:full
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    CssGenerator generator;
    std::string css = generator.generate(component.get());

    assert_contains(css, ".fullscreen", "Test 10");
    assert_contains(css, "width: 100%", "Test 10");
    assert_contains(css, "height: 100%", "Test 10");

    std::cout << "PASSED ✓\n";
}

void test11_MultipleUtilities() {
    std::cout << "Test 11: Multiple utilities... ";

    std::string source = R"(
        @utility
        btn {
            px:4 py:2
        }

        @utility
        card {
            p:6
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    CssGenerator generator;
    std::string css = generator.generate(component.get());

    assert_contains(css, ".btn", "Test 11");
    assert_contains(css, ".card", "Test 11");
    assert_contains(css, "padding: 1.5rem", "Test 11");

    std::cout << "PASSED ✓\n";
}

void test12_CompleteButton() {
    std::cout << "Test 12: Complete button with multiple properties... ";

    std::string source = R"(
        @utility
        btn {
            px:4 py:2
            bg:blue-500
            text:white
            rounded:md
            hover:bg:blue-700
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    CssGenerator generator;
    std::string css = generator.generate(component.get());

    // Base styles
    assert_contains(css, ".btn", "Test 12");
    assert_contains(css, "padding-left: 1rem", "Test 12");
    assert_contains(css, "background-color: #3b82f6", "Test 12");
    assert_contains(css, "color: #ffffff", "Test 12");
    assert_contains(css, "border-radius: 0.375rem", "Test 12");

    // Hover styles
    assert_contains(css, ".btn:hover", "Test 12");
    assert_contains(css, "background-color: #1d4ed8", "Test 12 hover");

    std::cout << "PASSED ✓\n";
}

int main() {
    std::cout << "=================================\n";
    std::cout << "  CSS GENERATOR UNIT TESTS\n";
    std::cout << "=================================\n\n";

    try {
        test1_SimpleSpacing();
        test2_ColorUtility();
        test3_TextColor();
        test4_HoverPrefix();
        test5_FocusPrefix();
        test6_ResponsiveSmall();
        test7_ResponsiveMedium();
        test8_BorderRadius();
        test9_FontWeight();
        test10_WidthHeight();
        test11_MultipleUtilities();
        test12_CompleteButton();

        std::cout << "\n=================================\n";
        std::cout << "  ALL TESTS PASSED! ✓\n";
        std::cout << "=================================\n";

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\nTEST SUITE FAILED!\n";
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
