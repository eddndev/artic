# Artic

> A unified language for modern web development

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Version](https://img.shields.io/badge/version-0.1.0-orange.svg)]()

## 🎯 Overview

**Artic** (`.atc`) is a domain-specific language designed to eliminate fragmentation in full-stack web development. Write your entire application in one language, one syntax, and compile to native performance on both server and client.

### The Problem

Modern web development forces you to master multiple languages and paradigms:
- Backend: Node.js, Python, PHP, Go, Rust
- Frontend: JavaScript, TypeScript, JSX/TSX
- Styles: CSS, SCSS, Tailwind, CSS-in-JS
- Tooling: webpack, Vite, esbuild, babel

### Our Solution

**One language. One syntax. Multiple targets.**

```artic
@route("/hello")

let name = $state("World")

@utility
btn { px:4 py:2 bg:blue-500 rounded:md }

<div class:(container)>
    <h1>Hello, {name}!</h1>
    <button class:(btn) on:click={() => name = "Artic"}>
        Click me
    </button>
</div>
```

Artic compiles to:
1. **Native server binary** (C++) - Zero cold start, maximum throughput
2. **Optimized JavaScript** (client) - Fast hydration, minimal bundle size
3. **Static CSS** - Compiled at build time, zero runtime overhead

## ✨ Key Features

- **🎨 Unified Syntax** - One grammar for server, client, templates, and styles
- **⚡ Native Performance** - Server compiles to C++ binary, client to optimized JS
- **🔄 Fine-Grained Reactivity** - Signal-based updates, surgical DOM changes
- **📦 Zero Runtime Dependencies** - No node_modules, no vendor folders
- **🛠️ Developer Experience** - Fast compilation (<100ms), clear errors, built-in tooling
- **🎯 Explicit Semantics** - `@server`, `@client`, `@utility` decorators make code intent clear

## 🚀 Quick Start

### Installation

```bash
# Clone the repository
git clone https://github.com/achronyme/artic.git
cd artic

# Install dependencies with vcpkg
./scripts/install_deps.sh

# Build the compiler
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Install globally
sudo cmake --install build
```

### Create Your First App

```bash
# Create a new project
artic create my-app
cd my-app

# Start dev server
artic dev index.atc

# Build for production
artic build index.atc
```

### Example: Interactive Counter

```artic
@route("/counter")

let count = $state(0)

@client
let increment = () => count++

@client
let decrement = () => count--

@utility
btn {
    px:6 py:3
    bg:blue-600 text:white
    rounded:lg
    hover:bg:blue-700
    active:scale:95
    transition:all
}

<div class:(flex flex-col items:center gap:4 p:8)>
    <h1 class:(text:4xl font:bold)>Counter: {count}</h1>

    <div class:(flex gap:2)>
        <button class:(btn) on:click={decrement}>-</button>
        <button class:(btn) on:click={increment}>+</button>
    </div>
</div>
```

## 📚 Documentation

- **[Manifest](docs/MANIFEST.MD)** - Complete language specification
- **[Architecture](docs/ARCHITECTURE.md)** - Compiler architecture and design
- **[Grammar](docs/GRAMMAR.md)** - Formal EBNF grammar
- **[Directory Structure](docs/DIRECTORY_STRUCTURE.md)** - Project organization
- **[Architecture Summary](docs/ARCHITECTURE_SUMMARY.md)** - Quick overview

## 🗺️ Roadmap

### Phase 1: Static Site Generation ✅ (In Progress)
- [x] Define language specification
- [x] Design compiler architecture
- [x] Create formal grammar
- [ ] Implement Lexer
- [ ] Implement Parser
- [ ] CSS Generator (utilities → CSS)
- [ ] Basic HTML output

### Phase 2: Client Interactivity
- [ ] JavaScript code generation
- [ ] Reactivity system (signals)
- [ ] Event handlers and bindings
- [ ] Client runtime (~8KB)

### Phase 3: Native Server
- [ ] C++ code generation
- [ ] HTTP server runtime
- [ ] Routing system
- [ ] RPC (client ↔ server communication)

### Phase 4: Data Layer
- [ ] Type-safe ORM
- [ ] Database migrations
- [ ] Query builder

### Phase 5: Advanced Tooling
- [ ] Language Server (LSP)
- [ ] VS Code extension
- [ ] Dev server with HMR
- [ ] Debugging support

### Phase 6: WebAssembly
- [ ] Compile compute-heavy functions to Wasm
- [ ] JS ↔ Wasm bridge
- [ ] Performance optimizations

## 🏗️ Project Structure

```
artic/
├── src/              # Compiler source code (C++)
│   ├── frontend/     # Lexer, Parser, AST, Semantic analysis
│   ├── backend/      # Code generators (CSS, JS, C++)
│   ├── shared/       # Type system, diagnostics, utilities
│   └── cli/          # Command-line interface
├── runtime/          # Runtime libraries
│   ├── client/       # Client runtime (TypeScript)
│   └── server/       # Server runtime (C++ headers)
├── tests/            # Test suites
├── examples/         # Example Artic applications
├── tools/            # LSP, VS Code extension, dev server
└── docs/             # Documentation
```

## 🛠️ Development

### Build from Source

```bash
# Debug build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DARTIC_BUILD_TESTS=ON
cmake --build build

# Run tests
cd build && ctest --verbose

# Run compiler
./build/artic build examples/hello_world/index.atc
```

### Code Style

We use `clang-format` and `clang-tidy`:

```bash
# Format code
find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i

# Lint code
clang-tidy src/**/*.cpp -- -Isrc
```

### Testing

```bash
# Unit tests
./build/artic_tests

# Integration tests
./build/artic_tests --gtest_filter=Integration*

# Benchmark
./build/benchmarks/compile_time
```

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](docs/CONTRIBUTING.md) for guidelines.

### Areas We Need Help

- **Compiler Development** - Lexer, Parser, Code generators
- **Runtime Libraries** - Client reactivity, Server HTTP
- **Tooling** - LSP, VS Code extension
- **Documentation** - Tutorials, API reference
- **Testing** - Unit tests, integration tests
- **Examples** - Real-world applications

## 📊 Performance Goals

| Metric | Target | Status |
|--------|--------|--------|
| Compilation time | < 100ms | 🚧 In Progress |
| Server cold start | < 5ms | 📅 Planned |
| Client bundle | < 15KB | 📅 Planned |
| Binary size | < 5MB | 📅 Planned |
| Throughput | > 50k req/s | 📅 Planned |

## 📜 License

MIT License - see [LICENSE](LICENSE) file for details.

## 🌟 Credits

Created by **Eduardo Alonso** and the **Achronyme** team.

Special thanks to all contributors who have helped shape Artic.

## 🔗 Links

- **Website**: [artic-lang.org](https://artic-lang.org) (Coming soon)
- **GitHub**: [github.com/achronyme/artic](https://github.com/achronyme/artic)
- **Discord**: [Join our community](https://discord.gg/artic) (Coming soon)
- **Twitter**: [@artic_lang](https://twitter.com/artic_lang) (Coming soon)

---

**Status**: 🚧 Early Development (v0.1.0)

Artic is currently in active development. APIs and language features are subject to change. Not recommended for production use yet.

**Questions?** Open an issue or join our Discord!
