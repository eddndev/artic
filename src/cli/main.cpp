#include <iostream>
#include <fstream>
#include <sstream>

#include "frontend/lexer/Lexer.h"
#include "compiler/Compiler.h"
#include "compiler/CompilerOptions.h"

void printUsage() {
    std::cout << "Artic Compiler v0.1.0 (Phase 1 - SSG)\n";
    std::cout << "Usage: artic <command> [options]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  lex <file>           Tokenize file and display tokens\n";
    std::cout << "  build <file>         Build Artic file to HTML + CSS\n";
    std::cout << "  help                 Show this help message\n\n";
    std::cout << "Build options:\n";
    std::cout << "  -o, --output <dir>   Output directory (default: dist)\n";
    std::cout << "  -v, --verbose        Verbose output\n";
    std::cout << "  --minify-css         Minify CSS output\n";
    std::cout << "  --minify-html        Minify HTML output\n";
    std::cout << "  --no-doctype         Don't include <!DOCTYPE html>\n";
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

void buildCommand(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Error: Missing filename\n";
        std::cerr << "Usage: artic build <file> [options]\n";
        return;
    }

    // Parse options
    artic::CompilerOptions options;
    options.inputFile = argv[2];

    // Parse additional arguments
    for (int i = 3; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                options.outputDir = argv[i + 1];
                i++;
            } else {
                std::cerr << "Error: Missing output directory\n";
                return;
            }
        } else if (arg == "-v" || arg == "--verbose") {
            options.verbose = true;
        } else if (arg == "--minify-css") {
            options.minifyCss = true;
        } else if (arg == "--minify-html") {
            options.minifyHtml = true;
        } else if (arg == "--no-doctype") {
            options.includeDoctype = false;
        } else {
            std::cerr << "Warning: Unknown option '" << arg << "'\n";
        }
    }

    // Compile
    std::cout << "Artic Compiler - Phase 1 (SSG)\n";
    std::cout << "================================\n\n";

    artic::Compiler compiler(options);
    bool success = compiler.compile();

    if (success) {
        std::cout << "\n✓ Compilation successful!\n";
    } else {
        std::cerr << "\n✗ Compilation failed\n";
        std::cerr << "Error: " << compiler.getLastError() << "\n";
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
        buildCommand(argc, argv);
        return 0;
    }

    std::cerr << "Error: Unknown command '" << command << "'\n";
    printUsage();
    return 1;
}
