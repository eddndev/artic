#include <iostream>
#include "src/frontend/lexer/Lexer.h"

int main() {
    std::cout << "Testing Artic Lexer...\n\n";

    // Simple test
    std::string source = "@route(\"/hello\")\nlet count = 42";

    artic::Lexer lexer(source);
    auto tokens = lexer.tokenize();

    std::cout << "Found " << tokens.size() << " tokens:\n";
    for (const auto& token : tokens) {
        std::cout << "  " << token << "\n";
    }

    return 0;
}
