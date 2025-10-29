// Validation test for Artic HTML Generator
#include <iostream>
#include <string>

#include "src/frontend/lexer/Lexer.h"
#include "src/frontend/parser/Parser.h"
#include "src/backend/html/HtmlGenerator.h"

using namespace artic;

void printSeparator() {
    std::cout << "\n" << std::string(60, '=') << "\n";
}

void test_SimpleElement() {
    std::cout << "\n[TEST 1] Simple element... ";

    std::string source = "<div>Hello World</div>";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        HtmlGenerator generator;
        std::string html = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated HTML:\n";
        std::cout << html << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_ElementWithAttributes() {
    std::cout << "\n[TEST 2] Element with attributes... ";

    std::string source = R"(<div id="container" role="main">Content</div>)";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        HtmlGenerator generator;
        std::string html = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated HTML:\n";
        std::cout << html << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_ClassDirective() {
    std::cout << "\n[TEST 3] Class directive class:()... ";

    std::string source = "<div class:(container flex center)>Content</div>";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        HtmlGenerator generator;
        std::string html = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated HTML:\n";
        std::cout << html << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_SelfClosingTag() {
    std::cout << "\n[TEST 4] Self-closing tag (img)... ";

    std::string source = R"(<img src="logo.png" alt="Logo" />)";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        HtmlGenerator generator;
        std::string html = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated HTML:\n";
        std::cout << html << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_NestedElements() {
    std::cout << "\n[TEST 5] Nested elements... ";

    std::string source = R"(
        <div class:(container)>
            <header>
                <h1>Title</h1>
            </header>
            <main>
                <p>Content here</p>
            </main>
        </div>
    )";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        HtmlGenerator generator;
        std::string html = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated HTML:\n";
        std::cout << html << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_CompleteComponent() {
    std::cout << "\n[TEST 6] Complete component with CSS + HTML... ";

    std::string source = R"(
        @route("/dashboard")

        use theme "./theme.atc"

        @utility
        container {
            w:full px:4
        }

        @utility
        card {
            bg:white
            rounded:lg
            p:6
        }

        @utility
        title {
            text:3xl
            font:bold
        }

        @utility
        btn {
            px:6 py:3
            bg:blue-500
            text:white
            rounded:md
            hover:bg:blue-700
        }

        <div class:(container)>
            <div class:(card)>
                <h1 class:(title)>Dashboard</h1>
                <p>Welcome to your dashboard.</p>
                <button class:(btn)>Get Started</button>
            </div>
        </div>
    )";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        HtmlGenerator htmlGen;
        std::string html = htmlGen.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\n=== Generated HTML ===\n";
        std::cout << html << "\n";

        std::cout << "\nComponent Info:\n";
        std::cout << "  Route: " << component->getRoutePath() << "\n";
        std::cout << "  Utilities: " << component->utilities.size() << "\n";
        std::cout << "  Template nodes: " << component->templateNodes.size() << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

void test_Form() {
    std::cout << "\n[TEST 7] Form with inputs... ";

    std::string source = R"(
        <form class:(login-form)>
            <div class:(form-group)>
                <label>Username</label>
                <input type="text" name="username" class:(input) />
            </div>
            <div class:(form-group)>
                <label>Password</label>
                <input type="password" name="password" class:(input) />
            </div>
            <button type="submit" class:(btn primary)>Login</button>
        </form>
    )";

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto component = parser.parse();

        HtmlGenerator generator;
        std::string html = generator.generate(component.get());

        std::cout << "PASSED ✓\n";
        std::cout << "\nGenerated HTML:\n";
        std::cout << html << "\n";

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Error: " << e.what() << "\n";
    }
}

int main() {
    printSeparator();
    std::cout << "  ARTIC HTML GENERATOR VALIDATION TESTS\n";
    printSeparator();

    test_SimpleElement();
    printSeparator();

    test_ElementWithAttributes();
    printSeparator();

    test_ClassDirective();
    printSeparator();

    test_SelfClosingTag();
    printSeparator();

    test_NestedElements();
    printSeparator();

    test_CompleteComponent();
    printSeparator();

    test_Form();
    printSeparator();

    std::cout << "\n  VALIDATION COMPLETED!\n";
    printSeparator();

    return 0;
}
