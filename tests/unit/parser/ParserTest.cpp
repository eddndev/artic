// Parser Unit Tests
// Phase 1: SSG - Testing metadata, imports, utilities, and templates

#include "../../../src/frontend/lexer/Lexer.h"
#include "../../../src/frontend/parser/Parser.h"
#include <iostream>
#include <cassert>
#include <memory>

using namespace artic;

void test1_RouteDecorator() {
    std::cout << "[TEST 1] @route decorator... ";

    std::string source = "@route(\"/hello\")";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    assert(component->hasRoute());
    assert(component->getRoutePath() == "/hello");

    std::cout << "PASSED ✓\n";
}

void test2_LayoutDecorator() {
    std::cout << "[TEST 2] @layout decorator... ";

    std::string source = "@route(\"/page\")\n@layout(\"admin\")";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    assert(component->hasRoute());
    assert(component->hasLayout());
    assert(component->getRoutePath() == "/page");
    assert(component->getLayoutName() == "admin");

    std::cout << "PASSED ✓\n";
}

void test3_ThemeImport() {
    std::cout << "[TEST 3] Theme import... ";

    std::string source = "use theme \"./theme.atc\"";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    assert(component->imports.size() == 1);
    assert(component->imports[0]->isType(ASTNodeType::THEME_IMPORT));

    auto* themeImport = dynamic_cast<ThemeImport*>(component->imports[0].get());
    assert(themeImport != nullptr);
    assert(themeImport->source == "./theme.atc");

    std::cout << "PASSED ✓\n";
}

void test4_NamedImport() {
    std::cout << "[TEST 4] Named import... ";

    std::string source = "use { Card, Button } from \"./components\"";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    assert(component->imports.size() == 1);
    assert(component->imports[0]->isType(ASTNodeType::NAMED_IMPORT));

    auto* namedImport = dynamic_cast<NamedImport*>(component->imports[0].get());
    assert(namedImport != nullptr);
    assert(namedImport->names.size() == 2);
    assert(namedImport->names[0] == "Card");
    assert(namedImport->names[1] == "Button");
    assert(namedImport->source == "./components");

    std::cout << "PASSED ✓\n";
}

void test5_UtilityDeclaration() {
    std::cout << "[TEST 5] Utility declaration... ";

    std::string source = R"(
        @utility
        btn {
            px:4 py:2
            bg:blue-500
        }
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    assert(component->utilities.size() == 1);
    assert(component->utilities[0]->name == "btn");
    assert(component->utilities[0]->tokens.size() >= 2);

    std::cout << "PASSED ✓\n";
}

void test6_SimpleElement() {
    std::cout << "[TEST 6] Simple element... ";

    std::string source = "<div>Hello</div>";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    assert(component->templateNodes.size() == 1);
    assert(component->templateNodes[0]->isType(ASTNodeType::ELEMENT));

    auto* element = dynamic_cast<Element*>(component->templateNodes[0].get());
    assert(element != nullptr);
    assert(element->tagName == "div");
    assert(!element->isSelfClosing);
    assert(element->children.size() == 1);

    std::cout << "PASSED ✓\n";
}

void test7_ElementWithAttributes() {
    std::cout << "[TEST 7] Element with attributes... ";

    std::string source = "<div id=\"container\" class=\"box\">Content</div>";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    assert(component->templateNodes.size() == 1);

    auto* element = dynamic_cast<Element*>(component->templateNodes[0].get());
    assert(element != nullptr);
    assert(element->attributes.size() == 2);
    assert(element->attributes[0]->name == "id");
    assert(element->attributes[0]->value == "container");
    assert(element->attributes[1]->name == "class");
    assert(element->attributes[1]->value == "box");

    std::cout << "PASSED ✓\n";
}

void test8_ClassDirective() {
    std::cout << "[TEST 8] Class directive... ";

    std::string source = "<div class:(container px:4)>Text</div>";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    auto* element = dynamic_cast<Element*>(component->templateNodes[0].get());
    assert(element != nullptr);
    assert(element->classDirectives.size() == 1);

    std::cout << "PASSED ✓\n";
}

void test9_SelfClosingElement() {
    std::cout << "[TEST 9] Self-closing element... ";

    std::string source = "<img src=\"logo.png\" />";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    auto* element = dynamic_cast<Element*>(component->templateNodes[0].get());
    assert(element != nullptr);
    assert(element->tagName == "img");
    assert(element->isSelfClosing);
    assert(element->attributes.size() == 1);

    std::cout << "PASSED ✓\n";
}

void test10_NestedElements() {
    std::cout << "[TEST 10] Nested elements... ";

    std::string source = "<div><h1>Title</h1><p>Paragraph</p></div>";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    auto* element = dynamic_cast<Element*>(component->templateNodes[0].get());
    assert(element != nullptr);
    assert(element->tagName == "div");
    assert(element->children.size() == 2);

    auto* h1 = dynamic_cast<Element*>(element->children[0].get());
    assert(h1 != nullptr);
    assert(h1->tagName == "h1");

    auto* p = dynamic_cast<Element*>(element->children[1].get());
    assert(p != nullptr);
    assert(p->tagName == "p");

    std::cout << "PASSED ✓\n";
}

void test11_CompleteComponent() {
    std::cout << "[TEST 11] Complete component... ";

    std::string source = R"(
        @route("/dashboard")

        use theme "./theme.atc"

        @utility
        container {
            w:full px:4
        }

        <div class:(container)>
            <h1>Dashboard</h1>
        </div>
    )";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto component = parser.parse();

    // Check metadata
    assert(component->hasRoute());
    assert(component->getRoutePath() == "/dashboard");

    // Check imports
    assert(component->imports.size() == 1);

    // Check utilities
    assert(component->utilities.size() == 1);
    assert(component->utilities[0]->name == "container");

    // Check template
    assert(component->templateNodes.size() == 1);

    std::cout << "PASSED ✓\n";
}

int main() {
    std::cout << "=================================\n";
    std::cout << "  ARTIC PARSER TESTS (Phase 1)  \n";
    std::cout << "=================================\n\n";

    try {
        test1_RouteDecorator();
        test2_LayoutDecorator();
        test3_ThemeImport();
        test4_NamedImport();
        test5_UtilityDeclaration();
        test6_SimpleElement();
        test7_ElementWithAttributes();
        test8_ClassDirective();
        test9_SelfClosingElement();
        test10_NestedElements();
        test11_CompleteComponent();

        std::cout << "\n=================================\n";
        std::cout << "  ALL TESTS PASSED!  \n";
        std::cout << "=================================\n";

    } catch (const std::exception& e) {
        std::cerr << "\nERROR: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
