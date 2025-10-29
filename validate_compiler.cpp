// End-to-end validation test for Artic Compiler
#include <iostream>
#include <string>

#include "src/compiler/Compiler.h"
#include "src/compiler/CompilerOptions.h"
#include "src/shared/utils/FileSystem.h"

using namespace artic;

void printSeparator() {
    std::cout << "\n" << std::string(60, '=') << "\n";
}

void test_CompileToString() {
    std::cout << "\n[TEST 1] Compile to string (in-memory)... ";

    std::string source = R"(
        @route("/test")

        @utility
        btn {
            px:4 py:2
            bg:blue-500
            text:white
            rounded:md
        }

        <div>
            <button class:(btn)>Click Me</button>
        </div>
    )";

    try {
        // Create temporary file
        std::string tempFile = "temp_test.atc";
        if (!FileSystem::writeFile(tempFile, source)) {
            std::cout << "FAILED ✗ (could not write temp file)\n";
            return;
        }

        // Compile
        CompilerOptions options;
        options.inputFile = tempFile;
        options.silent = true;

        Compiler compiler(options);
        auto result = compiler.compileToString();

        // Cleanup
        std::filesystem::remove(tempFile);

        if (result.success) {
            std::cout << "PASSED ✓\n";
            std::cout << "\n=== Generated CSS ===\n";
            std::cout << result.generatedCss << "\n";
            std::cout << "\n=== Generated HTML ===\n";
            std::cout << result.generatedHtml << "\n";
        } else {
            std::cout << "FAILED ✗\n";
            std::cout << "  Error: " << result.errorMessage << "\n";
        }

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Exception: " << e.what() << "\n";
    }
}

void test_CompileToDisk() {
    std::cout << "\n[TEST 2] Compile to disk (with file I/O)... ";

    std::string source = R"(
        @route("/home")

        @utility
        container {
            w:full px:4
        }

        @utility
        title {
            text:2xl font:bold
        }

        <div class:(container)>
            <h1 class:(title)>Home Page</h1>
            <p>This is a test page.</p>
        </div>
    )";

    try {
        // Create temporary file
        std::string tempFile = "temp_test2.atc";
        if (!FileSystem::writeFile(tempFile, source)) {
            std::cout << "FAILED ✗ (could not write temp file)\n";
            return;
        }

        // Compile
        CompilerOptions options;
        options.inputFile = tempFile;
        options.outputDir = "test_output";
        options.silent = true;

        Compiler compiler(options);
        bool success = compiler.compile();

        // Cleanup temp file
        std::filesystem::remove(tempFile);

        if (success) {
            // Check output files exist
            auto cssPath = options.getCssOutputPath();
            auto htmlPath = options.getHtmlOutputPath();

            bool cssExists = FileSystem::fileExists(cssPath);
            bool htmlExists = FileSystem::fileExists(htmlPath);

            if (cssExists && htmlExists) {
                std::cout << "PASSED ✓\n";
                std::cout << "\nOutput files created:\n";
                std::cout << "  CSS:  " << cssPath << "\n";
                std::cout << "  HTML: " << htmlPath << "\n";

                // Read and display contents
                auto css = FileSystem::readFile(cssPath);
                auto html = FileSystem::readFile(htmlPath);

                if (css && html) {
                    std::cout << "\n=== Generated CSS ===\n";
                    std::cout << css.value() << "\n";
                    std::cout << "\n=== Generated HTML ===\n";
                    std::cout << html.value() << "\n";
                }

                // Cleanup output directory
                std::filesystem::remove_all("test_output");

            } else {
                std::cout << "FAILED ✗ (output files not created)\n";
            }
        } else {
            std::cout << "FAILED ✗\n";
            std::cout << "  Error: " << compiler.getLastError() << "\n";
        }

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Exception: " << e.what() << "\n";
    }
}

void test_CompleteExample() {
    std::cout << "\n[TEST 3] Compile complete example... ";

    std::string source = R"(
        @route("/dashboard")

        use theme "./theme.atc"

        @utility
        container {
            w:full px:4 py:8
        }

        @utility
        card {
            bg:white
            rounded:lg
            p:6
            hover:bg:gray-50
        }

        @utility
        title {
            text:3xl
            font:bold
            text:gray-900
        }

        @utility
        btn {
            px:6 py:3
            bg:blue-500
            text:white
            rounded:md
            hover:bg:blue-700
            focus:bg:blue-800
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
        // Create temporary file
        std::string tempFile = "temp_test3.atc";
        if (!FileSystem::writeFile(tempFile, source)) {
            std::cout << "FAILED ✗ (could not write temp file)\n";
            return;
        }

        // Compile with verbose output
        CompilerOptions options;
        options.inputFile = tempFile;
        options.outputDir = "test_output_complete";
        options.verbose = true;

        std::cout << "\n";
        Compiler compiler(options);
        bool success = compiler.compile();

        // Cleanup temp file
        std::filesystem::remove(tempFile);

        if (success) {
            std::cout << "\nPASSED ✓\n";

            // Read and display contents
            auto cssPath = options.getCssOutputPath();
            auto htmlPath = options.getHtmlOutputPath();

            auto css = FileSystem::readFile(cssPath);
            auto html = FileSystem::readFile(htmlPath);

            if (css && html) {
                std::cout << "\n=== Final Output ===\n";
                std::cout << "\nCSS (" << css.value().size() << " bytes):\n";
                std::cout << css.value() << "\n";
                std::cout << "\nHTML (" << html.value().size() << " bytes):\n";
                std::cout << html.value() << "\n";
            }

            // Cleanup
            std::filesystem::remove_all("test_output_complete");

        } else {
            std::cout << "\nFAILED ✗\n";
            std::cout << "  Error: " << compiler.getLastError() << "\n";
        }

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Exception: " << e.what() << "\n";
    }
}

void test_ErrorHandling() {
    std::cout << "\n[TEST 4] Error handling (invalid syntax)... ";

    std::string source = R"(
        @route("/test"

        <div>Invalid syntax
    )";

    try {
        // Create temporary file
        std::string tempFile = "temp_test4.atc";
        if (!FileSystem::writeFile(tempFile, source)) {
            std::cout << "FAILED ✗ (could not write temp file)\n";
            return;
        }

        // Compile (should fail)
        CompilerOptions options;
        options.inputFile = tempFile;
        options.silent = true;

        Compiler compiler(options);
        auto result = compiler.compileToString();

        // Cleanup
        std::filesystem::remove(tempFile);

        if (!result.success) {
            std::cout << "PASSED ✓ (correctly detected error)\n";
            std::cout << "  Error message: " << result.errorMessage << "\n";
        } else {
            std::cout << "FAILED ✗ (should have failed but succeeded)\n";
        }

    } catch (const std::exception& e) {
        std::cout << "FAILED ✗\n";
        std::cout << "  Exception: " << e.what() << "\n";
    }
}

int main() {
    printSeparator();
    std::cout << "  ARTIC COMPILER END-TO-END VALIDATION\n";
    printSeparator();

    test_CompileToString();
    printSeparator();

    test_CompileToDisk();
    printSeparator();

    test_CompleteExample();
    printSeparator();

    test_ErrorHandling();
    printSeparator();

    std::cout << "\n  ALL VALIDATION TESTS COMPLETED!\n";
    printSeparator();

    return 0;
}
