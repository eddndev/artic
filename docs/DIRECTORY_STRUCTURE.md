# Estructura de Directorios del Proyecto Artic

## Vista Completa del Árbol

```
artic/
├── .github/
│   └── workflows/
│       ├── ci.yml                    # CI/CD pipeline
│       └── release.yml               # Automated releases
│
├── docs/                             # Documentación
│   ├── MANIFEST.MD                   # Especificación del lenguaje
│   ├── ARCHITECTURE.md               # Arquitectura del compilador
│   ├── DIRECTORY_STRUCTURE.md        # Este archivo
│   ├── GRAMMAR.md                    # Gramática formal (EBNF)
│   ├── API_REFERENCE.md              # API del compilador
│   └── CONTRIBUTING.md               # Guía de contribución
│
├── src/                              # Código fuente del compilador
│   ├── frontend/                     # Frontend del compilador (análisis)
│   │   ├── lexer/
│   │   │   ├── Lexer.h
│   │   │   ├── Lexer.cpp
│   │   │   ├── Token.h
│   │   │   ├── Token.cpp
│   │   │   └── SourceLocation.h
│   │   │
│   │   ├── parser/
│   │   │   ├── Parser.h
│   │   │   ├── Parser.cpp
│   │   │   ├── ParseError.h
│   │   │   └── ParseError.cpp
│   │   │
│   │   ├── ast/                      # Definiciones del AST
│   │   │   ├── ASTNode.h             # Clase base
│   │   │   ├── Component.h           # Nodo raíz
│   │   │   ├── Component.cpp
│   │   │   ├── Decorator.h           # @route, @server, etc.
│   │   │   ├── Decorator.cpp
│   │   │   ├── Import.h              # use statements
│   │   │   ├── Import.cpp
│   │   │   ├── Props.h               # props { ... }
│   │   │   ├── Props.cpp
│   │   │   ├── Statement.h           # let, const, funciones
│   │   │   ├── Statement.cpp
│   │   │   ├── Expression.h          # Expresiones JS
│   │   │   ├── Expression.cpp
│   │   │   ├── Template.h            # Nodos del template
│   │   │   ├── Template.cpp
│   │   │   ├── Attribute.h           # Atributos HTML
│   │   │   ├── Attribute.cpp
│   │   │   ├── Directive.h           # class:, on:, bind:
│   │   │   ├── Directive.cpp
│   │   │   ├── Utility.h             # @utility declarations
│   │   │   ├── Utility.cpp
│   │   │   └── ASTVisitor.h          # Visitor pattern
│   │   │
│   │   └── semantic/                 # Análisis semántico
│   │       ├── SemanticAnalyzer.h
│   │       ├── SemanticAnalyzer.cpp
│   │       ├── ScopeBuilder.h        # Construcción de scopes
│   │       ├── ScopeBuilder.cpp
│   │       ├── TypeInference.h       # Inferencia de tipos
│   │       └── TypeInference.cpp
│   │
│   ├── backend/                      # Backend del compilador (generación)
│   │   ├── css/
│   │   │   ├── CssGenerator.h
│   │   │   ├── CssGenerator.cpp
│   │   │   ├── UtilityResolver.h     # Resuelve tokens tipo Tailwind
│   │   │   ├── UtilityResolver.cpp
│   │   │   ├── CssOptimizer.h        # Minificación
│   │   │   └── CssOptimizer.cpp
│   │   │
│   │   ├── client/                   # Generador de JavaScript
│   │   │   ├── ClientGenerator.h
│   │   │   ├── ClientGenerator.cpp
│   │   │   ├── JsEmitter.h           # Emite código JS
│   │   │   ├── JsEmitter.cpp
│   │   │   ├── ReactivityTransformer.h  # $state → signals
│   │   │   ├── ReactivityTransformer.cpp
│   │   │   ├── TemplateCompiler.h    # Template → JS
│   │   │   ├── TemplateCompiler.cpp
│   │   │   ├── RpcStubGenerator.h    # Genera fetch() para @server
│   │   │   └── RpcStubGenerator.cpp
│   │   │
│   │   └── server/                   # Generador de C++
│   │       ├── ServerGenerator.h
│   │       ├── ServerGenerator.cpp
│   │       ├── CppEmitter.h          # Emite código C++
│   │       ├── CppEmitter.cpp
│   │       ├── RouteGenerator.h      # Genera routing table
│   │       ├── RouteGenerator.cpp
│   │       ├── RpcHandlerGenerator.h # Genera endpoints HTTP
│   │       ├── RpcHandlerGenerator.cpp
│   │       ├── SsrGenerator.h        # Server-Side Rendering
│   │       └── SsrGenerator.cpp
│   │
│   ├── shared/                       # Código compartido
│   │   ├── symbols/
│   │   │   ├── SymbolTable.h
│   │   │   ├── SymbolTable.cpp
│   │   │   ├── Scope.h
│   │   │   ├── Scope.cpp
│   │   │   ├── Symbol.h
│   │   │   └── Symbol.cpp
│   │   │
│   │   ├── types/                    # Sistema de tipos
│   │   │   ├── Type.h                # Clase base
│   │   │   ├── PrimitiveType.h       # string, number, boolean
│   │   │   ├── PrimitiveType.cpp
│   │   │   ├── ArrayType.h
│   │   │   ├── ArrayType.cpp
│   │   │   ├── ObjectType.h
│   │   │   ├── ObjectType.cpp
│   │   │   ├── FunctionType.h
│   │   │   ├── FunctionType.cpp
│   │   │   ├── UnionType.h
│   │   │   ├── UnionType.cpp
│   │   │   ├── TypeChecker.h
│   │   │   └── TypeChecker.cpp
│   │   │
│   │   ├── diagnostics/
│   │   │   ├── DiagnosticEngine.h
│   │   │   ├── DiagnosticEngine.cpp
│   │   │   ├── Diagnostic.h
│   │   │   ├── Diagnostic.cpp
│   │   │   ├── DiagnosticRenderer.h  # Pretty printing
│   │   │   └── DiagnosticRenderer.cpp
│   │   │
│   │   └── utils/
│   │       ├── FileSystem.h
│   │       ├── FileSystem.cpp
│   │       ├── StringUtils.h
│   │       ├── StringUtils.cpp
│   │       ├── Logger.h
│   │       └── Logger.cpp
│   │
│   ├── cli/                          # Command-line interface
│   │   ├── main.cpp                  # Entry point
│   │   ├── CommandParser.h
│   │   ├── CommandParser.cpp
│   │   ├── BuildCommand.h
│   │   ├── BuildCommand.cpp
│   │   ├── DevCommand.h
│   │   ├── DevCommand.cpp
│   │   ├── CreateCommand.h
│   │   ├── CreateCommand.cpp
│   │   └── FormatCommand.h
│   │
│   └── compiler/                     # Orquestador principal
│       ├── Compiler.h                # Pipeline completo
│       ├── Compiler.cpp
│       ├── CompilerOptions.h         # Configuración
│       └── CompilerOptions.cpp
│
├── runtime/                          # Runtime libraries
│   ├── client/                       # Runtime del cliente (TypeScript)
│   │   ├── src/
│   │   │   ├── runtime.ts            # Entry point
│   │   │   ├── reactivity.ts         # Sistema de signals
│   │   │   ├── dom.ts                # Manipulación del DOM
│   │   │   ├── hydration.ts          # SSR hydration
│   │   │   ├── rpc.ts                # Cliente RPC
│   │   │   └── types.ts              # Type definitions
│   │   │
│   │   ├── dist/                     # Build output (generado)
│   │   │   └── runtime.min.js
│   │   │
│   │   ├── package.json
│   │   ├── tsconfig.json
│   │   └── esbuild.config.js
│   │
│   └── server/                       # Runtime del servidor (C++ header-only)
│       ├── include/
│       │   ├── artic/
│       │   │   ├── http_server.hpp   # Servidor HTTP
│       │   │   ├── router.hpp        # Sistema de routing
│       │   │   ├── request.hpp       # HTTP Request
│       │   │   ├── response.hpp      # HTTP Response
│       │   │   ├── json.hpp          # JSON utilities (wrapper)
│       │   │   └── runtime.hpp       # Entry point
│       │   │
│       │   └── third_party/          # Dependencias vendorizadas
│       │       ├── json.hpp          # nlohmann/json
│       │       └── httplib.h         # cpp-httplib (alternativa a Boost)
│       │
│       └── examples/
│           └── hello_server.cpp
│
├── tests/                            # Test suites
│   ├── unit/                         # Unit tests
│   │   ├── lexer/
│   │   │   └── LexerTest.cpp
│   │   ├── parser/
│   │   │   └── ParserTest.cpp
│   │   ├── semantic/
│   │   │   └── SemanticTest.cpp
│   │   └── generators/
│   │       ├── CssGeneratorTest.cpp
│   │       ├── ClientGeneratorTest.cpp
│   │       └── ServerGeneratorTest.cpp
│   │
│   ├── integration/                  # Integration tests
│   │   ├── fixtures/                 # Archivos .atc de prueba
│   │   │   ├── hello_world.atc
│   │   │   ├── counter.atc
│   │   │   ├── blog_post.atc
│   │   │   └── full_app.atc
│   │   │
│   │   └── CompilerTest.cpp          # Tests end-to-end
│   │
│   ├── snapshots/                    # Snapshot tests
│   │   ├── hello_world.css.snap
│   │   ├── hello_world.js.snap
│   │   └── hello_world.cpp.snap
│   │
│   └── CMakeLists.txt                # Test build config
│
├── examples/                         # Ejemplos de código Artic
│   ├── hello_world/
│   │   └── index.atc
│   ├── counter/
│   │   └── Counter.atc
│   ├── todo_app/
│   │   ├── theme.atc
│   │   ├── TodoItem.atc
│   │   └── TodoList.atc
│   └── blog/
│       ├── theme.atc
│       ├── _layouts/
│       │   └── default.atc
│       ├── components/
│       │   ├── Card.atc
│       │   └── Header.atc
│       └── pages/
│           ├── index.atc
│           └── post.atc
│
├── tools/                            # Herramientas de desarrollo
│   ├── vscode/                       # VS Code extension
│   │   ├── package.json
│   │   ├── syntaxes/
│   │   │   └── artic.tmLanguage.json
│   │   └── language-configuration.json
│   │
│   ├── lsp/                          # Language Server Protocol (Fase 5)
│   │   ├── src/
│   │   │   └── lsp_server.cpp
│   │   └── CMakeLists.txt
│   │
│   └── dev_server/                   # Dev server con HMR (Fase 5)
│       ├── src/
│       │   └── dev_server.cpp
│       └── CMakeLists.txt
│
├── benchmarks/                       # Performance benchmarks
│   ├── compile_time/
│   │   └── benchmark_compile.cpp
│   └── runtime/
│       └── benchmark_reactivity.cpp
│
├── scripts/                          # Scripts de utilidad
│   ├── build.sh                      # Build completo
│   ├── test.sh                       # Run all tests
│   ├── format.sh                     # Format código
│   └── install_deps.sh               # Instalar dependencias
│
├── third_party/                      # Dependencias externas
│   ├── boost/                        # Boost (submodule o vcpkg)
│   ├── googletest/                   # Google Test (submodule)
│   └── spdlog/                       # spdlog (submodule)
│
├── .clang-format                     # Configuración de formato
├── .clang-tidy                       # Configuración de linting
├── .gitignore
├── .gitmodules                       # Git submodules
│
├── CMakeLists.txt                    # Build system raíz
├── vcpkg.json                        # Dependencias (alternativa a submodules)
├── LICENSE                           # MIT License
├── README.md                         # Introducción al proyecto
└── CHANGELOG.md                      # Historial de cambios
```

## Descripción de Carpetas Principales

### `/src`
Código fuente del compilador en C++. Organizado en frontend (análisis), backend (generación) y shared (utilidades).

### `/runtime`
Librerías de runtime separadas:
- **client**: Runtime JavaScript minimalista (~8KB)
- **server**: Headers C++ para el servidor HTTP

### `/tests`
Suite de tests completa:
- **unit**: Tests unitarios por módulo
- **integration**: Tests end-to-end del compilador
- **snapshots**: Tests de regresión con outputs esperados

### `/examples`
Ejemplos de código `.atc` para documentación y testing.

### `/tools`
Herramientas de desarrollo:
- Extensión de VS Code
- Language Server (LSP)
- Dev server con HMR

### `/docs`
Documentación técnica completa del proyecto.

### `/benchmarks`
Performance benchmarks para validar métricas de velocidad.

## Archivos de Configuración

### `CMakeLists.txt` (Raíz)
```cmake
cmake_minimum_required(VERSION 3.20)
project(Artic VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(ARTIC_BUILD_TESTS "Build tests" ON)
option(ARTIC_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(ARTIC_BUILD_TOOLS "Build LSP and dev server" OFF)

# Dependencies
find_package(Boost REQUIRED COMPONENTS system)
find_package(spdlog REQUIRED)

# Subdirectories
add_subdirectory(src)
add_subdirectory(runtime/server)

if(ARTIC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(third_party/googletest)
    add_subdirectory(tests)
endif()

if(ARTIC_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(ARTIC_BUILD_TOOLS)
    add_subdirectory(tools)
endif()

# Main executable
add_executable(artic src/cli/main.cpp)
target_link_libraries(artic PRIVATE artic_compiler)
```

### `vcpkg.json`
```json
{
  "name": "artic",
  "version": "0.1.0",
  "dependencies": [
    "boost-asio",
    "boost-system",
    "nlohmann-json",
    "spdlog",
    "cli11"
  ],
  "builtin-baseline": "latest"
}
```

### `.clang-format`
```yaml
BasedOnStyle: LLVM
IndentWidth: 4
ColumnLimit: 100
PointerAlignment: Left
```

## Gestión de Dependencias

### Opción 1: vcpkg (Recomendado)
```bash
# Instalar vcpkg
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh

# Instalar dependencias
./vcpkg/vcpkg install

# Build con CMake
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

### Opción 2: Git Submodules
```bash
# Añadir dependencias como submodules
git submodule add https://github.com/google/googletest third_party/googletest
git submodule add https://github.com/gabime/spdlog third_party/spdlog

# Inicializar submodules
git submodule update --init --recursive
```

## Build Commands

### Desarrollo
```bash
# Build debug
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Run tests
cd build && ctest

# Run compiler
./build/artic build examples/hello_world/index.atc
```

### Producción
```bash
# Build release
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Install
sudo cmake --install build
```

## Estructura de Output

Cuando se compila un archivo `.atc`, el output es:

```
project/
├── src/
│   └── index.atc                     # Input
│
└── dist/                             # Output (generado)
    ├── index.html                    # HTML generado
    ├── index.css                     # CSS compilado
    ├── index.js                      # Client bundle
    ├── server                        # Binario ejecutable (Linux/Mac)
    └── server.exe                    # Binario ejecutable (Windows)
```

## Próximos Pasos

1. **Crear estructura base de directorios**
2. **Configurar CMake y build system**
3. **Implementar esqueletos de clases principales**
4. **Setup de testing con Google Test**
5. **Implementar Lexer básico**
6. **Validar pipeline con ejemplo "Hello World"**

## Notas de Implementación

- **Header-only vs Compiled:** Runtime de servidor es header-only para facilitar integración
- **Modularidad:** Cada módulo puede compilarse independientemente
- **Testing:** Cada módulo tiene su suite de tests en `/tests/unit/`
- **Documentación:** Cada header principal tiene documentación inline (Doxygen)