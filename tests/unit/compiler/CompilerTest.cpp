// Unit tests for Artic Compiler
#include <iostream>
#include <string>
#include <cassert>

#include "compiler/Compiler.h"
#include "compiler/CompilerOptions.h"
#include "shared/utils/FileSystem.h"

using namespace artic;

void test1_CompileSimpleComponent() {
    std::cout << "Test 1: Compile simple component... ";

    std::string source = R"(
        <div>Hello World</div>
    )";

    // Write temp file
    std::string tempFile = "test1.atc";
    FileSystem::writeFile(tempFile, source);

    // Compile
    CompilerOptions options;
    options.inputFile = tempFile;
    options.silent = true;

    Compiler compiler(options);
    auto result = compiler.compileToString();

    // Cleanup
    std::filesystem::remove(tempFile);

    assert(result.success);
    assert(result.generatedHtml.find("<div>") != std::string::npos);
    assert(result.generatedHtml.find("Hello World") != std::string::npos);

    std::cout << "PASSED ✓\n";
}

void test2_CompileWithUtility() {
    std::cout << "Test 2: Compile with @utility... ";

    std::string source = R"(
        @utility
        btn {
            px:4 py:2
        }

        <button class:(btn)>Click</button>
    )";

    std::string tempFile = "test2.atc";
    FileSystem::writeFile(tempFile, source);

    CompilerOptions options;
    options.inputFile = tempFile;
    options.silent = true;

    Compiler compiler(options);
    auto result = compiler.compileToString();

    std::filesystem::remove(tempFile);

    assert(result.success);
    assert(result.generatedCss.find(".btn") != std::string::npos);
    assert(result.generatedCss.find("padding-left") != std::string::npos);
    assert(result.generatedHtml.find("class=\"btn\"") != std::string::npos);

    std::cout << "PASSED ✓\n";
}

void test3_CompileWithRoute() {
    std::cout << "Test 3: Compile with @route... ";

    std::string source = R"(
        @route("/home")

        <div>Home Page</div>
    )";

    std::string tempFile = "test3.atc";
    FileSystem::writeFile(tempFile, source);

    CompilerOptions options;
    options.inputFile = tempFile;
    options.silent = true;

    Compiler compiler(options);
    auto result = compiler.compileToString();

    std::filesystem::remove(tempFile);

    assert(result.success);
    assert(result.generatedHtml.find("Home Page") != std::string::npos);

    std::cout << "PASSED ✓\n";
}

void test4_ErrorInvalidFile() {
    std::cout << "Test 4: Error handling - invalid file... ";

    CompilerOptions options;
    options.inputFile = "nonexistent.atc";
    options.silent = true;

    Compiler compiler(options);
    bool success = compiler.compile();

    assert(!success);
    assert(!compiler.getLastError().empty());

    std::cout << "PASSED ✓\n";
}

void test5_ErrorInvalidSyntax() {
    std::cout << "Test 5: Error handling - invalid syntax... ";

    std::string source = R"(
        <div>Unclosed tag
    )";

    std::string tempFile = "test5.atc";
    FileSystem::writeFile(tempFile, source);

    CompilerOptions options;
    options.inputFile = tempFile;
    options.silent = true;

    Compiler compiler(options);
    auto result = compiler.compileToString();

    std::filesystem::remove(tempFile);

    assert(!result.success);
    assert(!result.errorMessage.empty());

    std::cout << "PASSED ✓\n";
}

void test6_CompileToDisk() {
    std::cout << "Test 6: Compile to disk... ";

    std::string source = R"(
        @utility
        container {
            w:full
        }

        <div class:(container)>Content</div>
    )";

    std::string tempFile = "test6.atc";
    FileSystem::writeFile(tempFile, source);

    CompilerOptions options;
    options.inputFile = tempFile;
    options.outputDir = "test6_output";
    options.silent = true;

    Compiler compiler(options);
    bool success = compiler.compile();

    std::filesystem::remove(tempFile);

    assert(success);
    assert(FileSystem::fileExists(options.getCssOutputPath()));
    assert(FileSystem::fileExists(options.getHtmlOutputPath()));

    // Cleanup
    std::filesystem::remove_all("test6_output");

    std::cout << "PASSED ✓\n";
}

void test7_MinifyCss() {
    std::cout << "Test 7: Minify CSS option... ";

    std::string source = R"(
        @utility
        btn {
            px:4 py:2
            bg:blue-500
        }

        <button class:(btn)>Click</button>
    )";

    std::string tempFile = "test7.atc";
    FileSystem::writeFile(tempFile, source);

    CompilerOptions options;
    options.inputFile = tempFile;
    options.minifyCss = true;
    options.silent = true;

    Compiler compiler(options);
    auto result = compiler.compileToString();

    std::filesystem::remove(tempFile);

    assert(result.success);
    // Minified CSS should have no newlines in declarations
    assert(result.generatedCss.find("\n  ") == std::string::npos);

    std::cout << "PASSED ✓\n";
}

void test8_IncludeDoctype() {
    std::cout << "Test 8: Include DOCTYPE option... ";

    std::string source = "<div>Test</div>";

    std::string tempFile = "test8.atc";
    FileSystem::writeFile(tempFile, source);

    CompilerOptions options;
    options.inputFile = tempFile;
    options.includeDoctype = true;
    options.silent = true;

    Compiler compiler(options);
    auto result = compiler.compileToString();

    std::filesystem::remove(tempFile);

    assert(result.success);
    assert(result.generatedHtml.find("<!DOCTYPE html>") != std::string::npos);

    std::cout << "PASSED ✓\n";
}

int main() {
    std::cout << "=================================\n";
    std::cout << "  COMPILER UNIT TESTS\n";
    std::cout << "=================================\n\n";

    try {
        test1_CompileSimpleComponent();
        test2_CompileWithUtility();
        test3_CompileWithRoute();
        test4_ErrorInvalidFile();
        test5_ErrorInvalidSyntax();
        test6_CompileToDisk();
        test7_MinifyCss();
        test8_IncludeDoctype();

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
