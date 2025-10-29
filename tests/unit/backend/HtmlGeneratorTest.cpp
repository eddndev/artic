// Unit tests for Artic HTML Generator
#include <iostream>
#include <string>
#include <cassert>

#include "frontend/lexer/Lexer.h"
#include "frontend/parser/Parser.h"
#include "backend/html/HtmlGenerator.h"

using namespace artic;

void assert_contains(const std::string& haystack, const std::string& needle, const std::string& testName) {
    if (haystack.find(needle) == std::string::npos) {
        std::cerr << "FAILED: " << testName << "\n";
        std::cerr << "  Expected to find: \"" << needle << "\"\n";
        std::cerr << "  In: \"" << haystack << "\"\n";
        assert(false);
    }
}

void test1_SimpleDiv() {
    std::cout << "Test 1: Simple div element... ";

    std::string source = "<div>Hello World</div>";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    assert_contains(html, "<div>", "Test 1");
    assert_contains(html, "Hello World", "Test 1");
    assert_contains(html, "</div>", "Test 1");

    std::cout << "PASSED ✓\n";
}

void test2_ElementWithAttributes() {
    std::cout << "Test 2: Element with attributes... ";

    std::string source = R"(<div id="container" role="main">Content</div>)";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    assert_contains(html, "<div", "Test 2");
    assert_contains(html, "id=\"container\"", "Test 2");
    assert_contains(html, "role=\"main\"", "Test 2");
    assert_contains(html, "Content", "Test 2");

    std::cout << "PASSED ✓\n";
}

void test3_ClassDirective() {
    std::cout << "Test 3: Class directive class:()... ";

    std::string source = "<div class:(container)>Content</div>";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    assert_contains(html, "<div", "Test 3");
    assert_contains(html, "class=\"container\"", "Test 3");
    assert_contains(html, "Content", "Test 3");

    std::cout << "PASSED ✓\n";
}

void test4_MultipleClasses() {
    std::cout << "Test 4: Multiple classes in class:()... ";

    std::string source = "<div class:(container flex center)>Content</div>";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    assert_contains(html, "class=\"container flex center\"", "Test 4");

    std::cout << "PASSED ✓\n";
}

void test5_SelfClosingTag() {
    std::cout << "Test 5: Self-closing tag (img)... ";

    std::string source = R"(<img src="logo.png" alt="Logo" />)";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    assert_contains(html, "<img", "Test 5");
    assert_contains(html, "src=\"logo.png\"", "Test 5");
    assert_contains(html, "alt=\"Logo\"", "Test 5");
    assert_contains(html, "/>", "Test 5");

    std::cout << "PASSED ✓\n";
}

void test6_NestedElements() {
    std::cout << "Test 6: Nested elements... ";

    std::string source = R"(
        <div>
            <h1>Title</h1>
            <p>Paragraph</p>
        </div>
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    assert_contains(html, "<div>", "Test 6");
    assert_contains(html, "<h1>", "Test 6");
    assert_contains(html, "Title", "Test 6");
    assert_contains(html, "</h1>", "Test 6");
    assert_contains(html, "<p>", "Test 6");
    assert_contains(html, "Paragraph", "Test 6");
    assert_contains(html, "</p>", "Test 6");
    assert_contains(html, "</div>", "Test 6");

    std::cout << "PASSED ✓\n";
}

void test7_DeeplyNested() {
    std::cout << "Test 7: Deeply nested elements... ";

    std::string source = R"(
        <div>
            <section>
                <article>
                    <h1>Title</h1>
                </article>
            </section>
        </div>
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    assert_contains(html, "<div>", "Test 7");
    assert_contains(html, "<section>", "Test 7");
    assert_contains(html, "<article>", "Test 7");
    assert_contains(html, "<h1>", "Test 7");
    assert_contains(html, "Title", "Test 7");

    std::cout << "PASSED ✓\n";
}

void test8_InlineElements() {
    std::cout << "Test 8: Inline elements (span, strong)... ";

    std::string source = "<p>This is <strong>bold</strong> and <span>styled</span>.</p>";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    assert_contains(html, "<p>", "Test 8");
    assert_contains(html, "<strong>", "Test 8");
    assert_contains(html, "bold", "Test 8");
    assert_contains(html, "<span>", "Test 8");
    assert_contains(html, "styled", "Test 8");

    std::cout << "PASSED ✓\n";
}

void test9_EmptyElement() {
    std::cout << "Test 9: Empty element... ";

    std::string source = "<div></div>";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    assert_contains(html, "<div>", "Test 9");
    assert_contains(html, "</div>", "Test 9");

    std::cout << "PASSED ✓\n";
}

void test10_MultipleRootElements() {
    std::cout << "Test 10: Multiple root elements... ";

    std::string source = R"(
        <header>Header</header>
        <main>Content</main>
        <footer>Footer</footer>
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    assert_contains(html, "<header>", "Test 10");
    assert_contains(html, "Header", "Test 10");
    assert_contains(html, "<main>", "Test 10");
    assert_contains(html, "Content", "Test 10");
    assert_contains(html, "<footer>", "Test 10");
    assert_contains(html, "Footer", "Test 10");

    std::cout << "PASSED ✓\n";
}

void test11_CompleteComponent() {
    std::cout << "Test 11: Complete component with utilities... ";

    std::string source = R"(
        @route("/dashboard")

        @utility
        container {
            w:full px:4
        }

        @utility
        title {
            text:2xl font:bold
        }

        <div class:(container)>
            <h1 class:(title)>Dashboard</h1>
            <p>Welcome to the dashboard.</p>
        </div>
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    // Check structure
    assert_contains(html, "<div", "Test 11");
    assert_contains(html, "class=\"container\"", "Test 11");
    assert_contains(html, "<h1", "Test 11");
    assert_contains(html, "class=\"title\"", "Test 11");
    assert_contains(html, "Dashboard", "Test 11");
    assert_contains(html, "<p>", "Test 11");
    assert_contains(html, "Welcome to the dashboard.", "Test 11");

    std::cout << "PASSED ✓\n";
}

void test12_Button() {
    std::cout << "Test 12: Button with class directive... ";

    std::string source = R"(<button class:(btn primary)>Click Me</button>)";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    assert_contains(html, "<button", "Test 12");
    assert_contains(html, "class=\"btn primary\"", "Test 12");
    assert_contains(html, "Click Me", "Test 12");
    assert_contains(html, "</button>", "Test 12");

    std::cout << "PASSED ✓\n";
}

void test13_Form() {
    std::cout << "Test 13: Form with input elements... ";

    std::string source = R"(
        <form>
            <input type="text" name="username" />
            <input type="password" name="password" />
            <button type="submit">Login</button>
        </form>
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    HtmlGenerator generator;
    std::string html = generator.generate(component.get());

    assert_contains(html, "<form>", "Test 13");
    assert_contains(html, "<input", "Test 13");
    assert_contains(html, "type=\"text\"", "Test 13");
    assert_contains(html, "name=\"username\"", "Test 13");
    assert_contains(html, "type=\"password\"", "Test 13");
    assert_contains(html, "<button", "Test 13");
    assert_contains(html, "Login", "Test 13");

    std::cout << "PASSED ✓\n";
}

int main() {
    std::cout << "=================================\n";
    std::cout << "  HTML GENERATOR UNIT TESTS\n";
    std::cout << "=================================\n\n";

    try {
        test1_SimpleDiv();
        test2_ElementWithAttributes();
        test3_ClassDirective();
        test4_MultipleClasses();
        test5_SelfClosingTag();
        test6_NestedElements();
        test7_DeeplyNested();
        test8_InlineElements();
        test9_EmptyElement();
        test10_MultipleRootElements();
        test11_CompleteComponent();
        test12_Button();
        test13_Form();

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
