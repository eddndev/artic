# Contributing to Artic

Thank you for your interest in contributing to Artic! This document provides guidelines and instructions for contributing.

## 🌟 Ways to Contribute

- **Report bugs** - Found a bug? Open an issue!
- **Suggest features** - Have an idea? We'd love to hear it!
- **Write code** - Implement features or fix bugs
- **Improve documentation** - Help others understand Artic
- **Write tests** - Increase code coverage and reliability
- **Create examples** - Show what's possible with Artic

## 🚀 Getting Started

### Prerequisites

- C++20 compatible compiler (GCC 11+, Clang 14+, MSVC 2022+)
- CMake 3.20 or higher
- Git
- vcpkg (recommended) or manual dependency installation

### Setting Up Development Environment

1. **Fork and clone the repository**
   ```bash
   git clone https://github.com/YOUR_USERNAME/artic.git
   cd artic
   ```

2. **Install dependencies**
   ```bash
   ./scripts/install_deps.sh
   ```

3. **Build in debug mode**
   ```bash
   cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DARTIC_BUILD_TESTS=ON
   cmake --build build
   ```

4. **Run tests**
   ```bash
   cd build && ctest --verbose
   ```

## 📝 Code Style

We follow strict coding standards to maintain consistency:

### C++ Style Guide

- **Standard**: C++20
- **Formatting**: LLVM style with modifications (see `.clang-format`)
- **Naming Conventions**:
  - Classes: `PascalCase` (e.g., `LexerToken`)
  - Functions: `camelCase` (e.g., `parseExpression()`)
  - Variables: `camelCase` (e.g., `tokenCount`)
  - Constants: `UPPER_CASE` (e.g., `MAX_TOKENS`)
  - Private members: `m_` prefix (e.g., `m_position`)

### Before Committing

1. **Format your code**
   ```bash
   ./scripts/format.sh
   ```

2. **Run linter**
   ```bash
   clang-tidy src/**/*.cpp -- -Isrc
   ```

3. **Run all tests**
   ```bash
   ./scripts/test.sh
   ```

## 🔨 Development Workflow

### 1. Create a Branch

```bash
git checkout -b feature/your-feature-name
# or
git checkout -b fix/bug-description
```

Branch naming conventions:
- `feature/` - New features
- `fix/` - Bug fixes
- `docs/` - Documentation changes
- `refactor/` - Code refactoring
- `test/` - Test additions or changes

### 2. Make Changes

- Write clear, concise commit messages
- Keep commits atomic (one logical change per commit)
- Add tests for new functionality
- Update documentation as needed

### 3. Commit Guidelines

We follow [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

**Examples:**
```
feat(lexer): add support for template literals

Implement tokenization of template strings with interpolation.
Handles nested expressions and escape sequences.

Closes #123
```

```
fix(parser): correct precedence of ternary operator

The ternary operator was being parsed with incorrect precedence,
causing issues with nested expressions.
```

### 4. Push and Create Pull Request

```bash
git push origin feature/your-feature-name
```

Then create a Pull Request on GitHub with:
- Clear title and description
- Reference to related issues
- Screenshots/examples if applicable
- Checklist of changes

## ✅ Pull Request Checklist

Before submitting a PR, ensure:

- [ ] Code follows style guidelines
- [ ] All tests pass
- [ ] New tests added for new functionality
- [ ] Documentation updated
- [ ] Commit messages are clear and follow conventions
- [ ] No merge conflicts
- [ ] PR description is complete

## 🧪 Testing Guidelines

### Writing Tests

Tests are located in `tests/` directory:

```cpp
// tests/unit/lexer/LexerTest.cpp
#include <gtest/gtest.h>
#include "frontend/lexer/Lexer.h"

TEST(LexerTest, TokenizesSimpleExpression) {
    Lexer lexer("let x = 42");
    auto tokens = lexer.tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::LET);
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].type, TokenType::EQUALS);
    EXPECT_EQ(tokens[3].type, TokenType::NUMBER);
}
```

### Running Tests

```bash
# All tests
ctest --test-dir build

# Specific test suite
./build/artic_tests --gtest_filter=LexerTest*

# With verbose output
./build/artic_tests --gtest_filter=LexerTest* --verbose
```

## 📚 Documentation

### Code Documentation

Use Doxygen-style comments:

```cpp
/**
 * @brief Tokenizes the input source code
 *
 * @param source The source code string to tokenize
 * @return Vector of tokens
 * @throws LexerError if invalid syntax is encountered
 */
std::vector<Token> tokenize(const std::string& source);
```

### User Documentation

- Update `docs/` when adding features
- Add examples to `examples/` directory
- Update README.md if needed

## 🐛 Reporting Bugs

When reporting bugs, include:

1. **Description** - Clear summary of the issue
2. **Steps to reproduce** - Minimal reproducible example
3. **Expected behavior** - What should happen
4. **Actual behavior** - What actually happens
5. **Environment** - OS, compiler version, Artic version
6. **Code sample** - Minimal `.atc` file that reproduces the bug

**Bug Report Template:**

```markdown
## Description
Brief description of the bug

## Steps to Reproduce
1. Create file `test.atc` with...
2. Run `artic build test.atc`
3. Observe error...

## Expected Behavior
Should compile successfully

## Actual Behavior
Compiler crashes with...

## Environment
- OS: Ubuntu 22.04
- Compiler: GCC 11.4
- Artic version: 0.1.0

## Code Sample
\`\`\`artic
@route("/test")
let count = $state(0)
<div>{count}</div>
\`\`\`
```

## 💡 Feature Requests

When suggesting features:

1. **Use case** - Why is this feature needed?
2. **Proposed syntax** - How would it work?
3. **Alternatives** - What alternatives did you consider?
4. **Impact** - How does it affect existing code?

## 🔍 Code Review Process

All PRs go through code review:

1. **Automated checks** - CI runs tests and linting
2. **Maintainer review** - A maintainer reviews the code
3. **Feedback** - Address any requested changes
4. **Approval** - Once approved, PR is merged

### Review Criteria

- Code quality and style
- Test coverage
- Documentation completeness
- Performance impact
- API design
- Backward compatibility

## 📞 Getting Help

- **Discord** - Join our community (link in README)
- **GitHub Issues** - Ask questions
- **Documentation** - Check the docs first

## 🏆 Recognition

Contributors are recognized in:
- README.md contributors section
- Release notes
- Project website (coming soon)

## 📄 License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for contributing to Artic!** 🎉
