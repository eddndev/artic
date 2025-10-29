#include <iostream>
#include <fstream>
#include <sstream>
#include "../frontend/lexer/Lexer.h"

void printUsage() {
    std::cout << "Artic Compiler v0.1.0\n";
    std::cout << "Usage: artic <command> [options]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  lex <file>      Tokenize file and display tokens\n";
    std::cout << "  build <file>    Build Artic file (not implemented yet)\n";
    std::cout << "  help            Show this help message\n";
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void lexCommand(const std::string& filename) {
    std::cout << "Tokenizing: " << filename << "\n\n";

    try {
        std::string source = readFile(filename);
        artic::Lexer lexer(source);
        auto tokens = lexer.tokenize();

        std::cout << "Found " << tokens.size() << " tokens:\n\n";

        for (const auto& token : tokens) {
            std::cout << token << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string command = argv[1];

    if (command == "help" || command == "--help" || command == "-h") {
        printUsage();
        return 0;
    }

    if (command == "lex") {
        if (argc < 3) {
            std::cerr << "Error: Missing filename\n";
            std::cerr << "Usage: artic lex <file>\n";
            return 1;
        }

        lexCommand(argv[2]);
        return 0;
    }

    if (command == "build") {
        std::cerr << "Error: Build command not implemented yet\n";
        std::cerr << "Currently in Phase 1: Lexer development\n";
        return 1;
    }

    std::cerr << "Error: Unknown command '" << command << "'\n";
    printUsage();
    return 1;
}
