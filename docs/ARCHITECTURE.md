# Arquitectura del Compilador Artic

## 1. Pipeline de Compilación

El compilador Artic sigue un pipeline clásico de múltiples fases con backends especializados:

```
┌──────────────┐
│  Archivo.atc │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│    LEXER     │  → Tokenización (caracteres → tokens)
└──────┬───────┘
       │
       ▼
┌──────────────┐
│    PARSER    │  → Construcción del AST (tokens → árbol)
└──────┬───────┘
       │
       ▼
┌──────────────┐
│   SEMANTIC   │  → Validación semántica (tipos, scopes)
│   ANALYZER   │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│ AST Validado │
└──────┬───────┘
       │
       ├─────────────────────────┬─────────────────────┐
       ▼                         ▼                     ▼
┌────────────────┐   ┌─────────────────┐   ┌──────────────┐
│  CSS GENERATOR │   │ CLIENT GENERATOR│   │SERVER GENERATOR│
│                │   │                 │   │               │
│  .atc → .css   │   │  .atc → .js     │   │  .atc → .cpp  │
└────────┬───────┘   └────────┬────────┘   └──────┬───────┘
       │                      │                    │
       ▼                      ▼                    ▼
   [style.css]          [bundle.js]          [server.cpp]
                             │                    │
                             │                    ▼
                             │              ┌──────────┐
                             │              │ C++ BUILD│
                             │              │ (GCC)    │
                             │              └────┬─────┘
                             │                   │
                             ▼                   ▼
                        [Web Client]        [Native Binary]
```

## 2. Módulos Principales

### 2.1 Frontend (Análisis)

#### **Lexer** (`src/frontend/lexer/`)
- **Responsabilidad:** Convertir código fuente en stream de tokens
- **Input:** String (código `.atc`)
- **Output:** Vector de `Token`
- **Componentes:**
  - `Lexer.h/cpp` - Clase principal
  - `Token.h` - Definición de tipos de token
  - `SourceLocation.h` - Tracking de posición en el archivo

#### **Parser** (`src/frontend/parser/`)
- **Responsabilidad:** Construir el AST desde los tokens
- **Input:** Vector de `Token`
- **Output:** `ASTRoot` (árbol completo)
- **Componentes:**
  - `Parser.h/cpp` - Parser recursivo descendente
  - `ParseError.h` - Manejo de errores de sintaxis

#### **AST** (`src/frontend/ast/`)
- **Responsabilidad:** Representar la estructura del programa
- **Componentes principales:**
  - `ASTNode.h` - Clase base abstracta
  - `Component.h` - Nodo raíz del componente
  - `Metadata.h` - Decoradores (@route, @layout)
  - `Import.h` - Statements de `use`
  - `Props.h` - Definición de props
  - `Logic.h` - Variables, funciones, efectos
  - `Styles.h` - Utilidades y CSS
  - `Template.h` - Estructura del template
  - `Expression.h` - Expresiones JavaScript
  - `Statement.h` - Statements (if, loop, etc.)

#### **Semantic Analyzer** (`src/frontend/semantic/`)
- **Responsabilidad:** Validar correctitud semántica
- **Input:** AST
- **Output:** AST validado + Symbol Table
- **Tareas:**
  - Type checking
  - Scope resolution
  - Validación de decoradores
  - Detección de variables no declaradas
  - Validación de RPC calls (server → client)

### 2.2 Backend (Generación)

#### **CSS Generator** (`src/backend/css/`)
- **Responsabilidad:** Generar CSS estático desde utilidades
- **Input:** AST (sección `@utility` + `class:`)
- **Output:** Archivo `.css`
- **Componentes:**
  - `CssGenerator.h/cpp`
  - `UtilityResolver.h` - Resuelve tokens de utilidades
  - `CssOptimizer.h` - Minificación y deduplicación

#### **Client Generator** (`src/backend/client/`)
- **Responsabilidad:** Generar JavaScript del cliente
- **Input:** AST (lógica `@client`, template, props)
- **Output:** Archivo `.js`
- **Componentes:**
  - `ClientGenerator.h/cpp`
  - `JsEmitter.h` - Emite código JavaScript
  - `ReactivityTransformer.h` - Transforma `$state` → signals
  - `TemplateCompiler.h` - JSX-like → JS (createElement)
  - `RpcStubGenerator.h` - Genera fetch() calls para `@server`

#### **Server Generator** (`src/backend/server/`)
- **Responsabilidad:** Generar código C++ del servidor
- **Input:** AST (lógica `@server`, rutas, handlers)
- **Output:** Archivo `.cpp`
- **Componentes:**
  - `ServerGenerator.h/cpp`
  - `CppEmitter.h` - Emite código C++
  - `RouteGenerator.h` - Genera routing table
  - `RpcHandlerGenerator.h` - Genera endpoints HTTP para `@server`
  - `SsrGenerator.h` - Server-Side Rendering

### 2.3 Utilidades Compartidas

#### **Symbol Table** (`src/shared/symbols/`)
- **Responsabilidad:** Tracking de variables, funciones, tipos
- **Componentes:**
  - `SymbolTable.h/cpp`
  - `Scope.h` - Gestión de scopes anidados
  - `Symbol.h` - Representación de símbolos

#### **Type System** (`src/shared/types/`)
- **Responsabilidad:** Sistema de tipos de Artic
- **Componentes:**
  - `Type.h` - Clase base abstracta
  - `PrimitiveType.h` - string, number, boolean
  - `ArrayType.h`
  - `ObjectType.h`
  - `FunctionType.h`
  - `UnionType.h`
  - `TypeChecker.h` - Validación de tipos

#### **Diagnostics** (`src/shared/diagnostics/`)
- **Responsabilidad:** Manejo de errores y warnings
- **Componentes:**
  - `DiagnosticEngine.h/cpp`
  - `Diagnostic.h` - Representación de error/warning
  - `DiagnosticRenderer.h` - Pretty printing de errores

#### **Utilities** (`src/shared/utils/`)
- **Responsabilidad:** Helpers generales
- **Componentes:**
  - `FileSystem.h` - I/O de archivos
  - `StringUtils.h` - Manipulación de strings
  - `Logger.h` - Sistema de logging

### 2.4 Runtime

#### **Client Runtime** (`runtime/client/`)
- **Lenguaje:** TypeScript → JavaScript minificado
- **Responsabilidad:** Runtime de reactividad en el cliente
- **Componentes:**
  - `runtime.ts` - Core del runtime
  - `reactivity.ts` - Sistema de signals
  - `dom.ts` - Manipulación del DOM
  - `hydration.ts` - Hydration de SSR
  - `rpc.ts` - Cliente RPC (fetch wrapper)

#### **Server Runtime** (`runtime/server/`)
- **Lenguaje:** C++ (header-only library)
- **Responsabilidad:** Runtime del servidor HTTP
- **Componentes:**
  - `http_server.hpp` - Servidor HTTP (wrapper de Boost.Asio)
  - `router.hpp` - Sistema de routing
  - `request.hpp` - Representación de HTTP Request
  - `response.hpp` - Representación de HTTP Response
  - `json.hpp` - Serialización JSON (nlohmann/json)

### 2.5 CLI

#### **CLI** (`src/cli/`)
- **Responsabilidad:** Interfaz de línea de comandos
- **Comandos:**
  - `artic build <file.atc>` - Compilar a producción
  - `artic dev <file.atc>` - Modo desarrollo con HMR
  - `artic create <name>` - Crear nuevo proyecto
  - `artic format <file.atc>` - Formatear código
- **Componentes:**
  - `main.cpp` - Entry point
  - `CommandParser.h` - Parsing de argumentos
  - `BuildCommand.h`
  - `DevCommand.h`
  - `CreateCommand.h`

## 3. Estructura del AST

### Jerarquía de Nodos

```cpp
ASTNode (base abstracta)
├── Component (raíz del archivo .atc)
│   ├── metadata: vector<Decorator>
│   ├── imports: vector<ImportDeclaration>
│   ├── props: PropsDeclaration?
│   ├── logic: vector<Statement>
│   ├── styles: vector<UtilityDeclaration>
│   └── template: TemplateNode
│
├── Decorator
│   ├── RouteDecorator (@route)
│   ├── LayoutDecorator (@layout)
│   ├── ServerDecorator (@server)
│   ├── ClientDecorator (@client)
│   └── UtilityDecorator (@utility)
│
├── ImportDeclaration
│   ├── type: "theme" | "component" | "module"
│   ├── imports: vector<string>
│   └── source: string
│
├── PropsDeclaration
│   └── properties: vector<PropDefinition>
│
├── Statement
│   ├── VariableDeclaration
│   ├── FunctionDeclaration
│   ├── EffectDeclaration (@effect)
│   ├── ExpressionStatement
│   └── ReturnStatement
│
├── Expression
│   ├── Identifier
│   ├── Literal (string, number, boolean)
│   ├── BinaryExpression (+, -, *, etc.)
│   ├── CallExpression
│   ├── MemberExpression (obj.prop)
│   ├── ArrayExpression ([1,2,3])
│   ├── ObjectExpression ({key: value})
│   ├── ArrowFunction
│   └── TernaryExpression (cond ? a : b)
│
├── TemplateNode
│   ├── Element
│   │   ├── tag: string
│   │   ├── attributes: vector<Attribute>
│   │   ├── directives: vector<Directive>
│   │   └── children: vector<TemplateNode>
│   ├── TextNode
│   ├── ExpressionNode ({...})
│   └── SlotNode (<slot />)
│
├── Attribute
│   ├── name: string
│   └── value: Expression | string
│
├── Directive
│   ├── ClassDirective (class:)
│   ├── EventDirective (on:)
│   ├── BindDirective (bind:)
│   └── SlotDirective (slot:)
│
└── UtilityDeclaration
    ├── name: string
    └── tokens: vector<UtilityToken>
```

## 4. Flujo de Datos Detallado

### 4.1 Fase 1: Tokenización

```
Input: "let count = $state(0)"

Lexer:
  → [
      Token(KEYWORD, "let"),
      Token(IDENTIFIER, "count"),
      Token(EQUALS, "="),
      Token(IDENTIFIER, "$state"),
      Token(LPAREN, "("),
      Token(NUMBER, "0"),
      Token(RPAREN, ")"),
    ]
```

### 4.2 Fase 2: Parsing

```
Tokens → Parser → AST:

VariableDeclaration {
  kind: "let",
  identifier: "count",
  initializer: CallExpression {
    callee: "$state",
    arguments: [Literal(0)]
  }
}
```

### 4.3 Fase 3: Análisis Semántico

```
AST → Semantic Analyzer:

1. Registrar "count" en Symbol Table
2. Detectar que usa $state() → marcar como reactivo
3. Type inference: count es number
4. Validar que $state está disponible
```

### 4.4 Fase 4: Generación

#### CSS Generator
```
@utility
btn { px:4 py:2 bg:blue-500 }

↓

.btn {
  padding-left: 1rem;
  padding-right: 1rem;
  padding-top: 0.5rem;
  padding-bottom: 0.5rem;
  background-color: #3b82f6;
}
```

#### Client Generator
```
let count = $state(0)

↓

import { signal } from './runtime';
let count = signal(0);
```

#### Server Generator
```
@server
let fetchData = async () => {
  return await db.query()
}

↓

#include "database.hpp"

json fetchData() {
  return db.query();
}
```

## 5. Interfaces entre Módulos

### 5.1 Lexer → Parser

```cpp
class Lexer {
public:
  std::vector<Token> tokenize(const std::string& source);
};

class Parser {
public:
  std::unique_ptr<Component> parse(const std::vector<Token>& tokens);
};
```

### 5.2 Parser → Semantic Analyzer

```cpp
class SemanticAnalyzer {
public:
  bool analyze(Component* ast, DiagnosticEngine& diag);
  SymbolTable* getSymbolTable();
};
```

### 5.3 Semantic Analyzer → Generators

```cpp
class CssGenerator {
public:
  std::string generate(const Component* ast);
};

class ClientGenerator {
public:
  std::string generate(const Component* ast, const SymbolTable* symbols);
};

class ServerGenerator {
public:
  std::string generate(const Component* ast, const SymbolTable* symbols);
};
```

## 6. Manejo de Errores

### 6.1 Diagnósticos

```cpp
enum class DiagnosticLevel {
  Error,
  Warning,
  Info,
  Hint
};

struct Diagnostic {
  DiagnosticLevel level;
  SourceLocation location;
  std::string message;
  std::string code; // Ej: "E001"
  std::vector<std::string> suggestions;
};
```

### 6.2 Pretty Printing

```
Error: Undefined variable 'user'
  ┌─ src/pages/Profile.atc:12:8
  │
12│   <h1>{user.name}</h1>
  │        ^^^^ Variable 'user' is not declared
  │
  = help: Did you mean 'currentUser'?
```

## 7. Optimizaciones

### 7.1 Compilador

- **Dead code elimination:** Eliminar código no usado
- **Tree shaking:** Eliminar imports no usados
- **Constant folding:** Evaluar expresiones constantes en compile-time
- **Minificación:** Reducir tamaño de JS generado

### 7.2 CSS

- **Deduplicación:** Eliminar reglas CSS duplicadas
- **Critical CSS:** Extraer CSS crítico para above-the-fold
- **PurgeCSS:** Eliminar utilidades no usadas

### 7.3 Runtime

- **Fine-grained reactivity:** Solo actualizar lo que cambió
- **Batch updates:** Agrupar cambios al DOM
- **Lazy loading:** Cargar componentes bajo demanda

## 8. Testing Strategy

### 8.1 Unit Tests

- Cada módulo tiene su suite de tests
- Framework: Google Test (C++)
- Coverage target: >80%

### 8.2 Integration Tests

- Tests end-to-end del pipeline completo
- Input: `.atc` files
- Output: Validar archivos generados

### 8.3 Snapshot Tests

- Guardar outputs esperados
- Detectar regresiones automáticamente

## 9. Build System

### CMake Structure

```cmake
# CMakeLists.txt (raíz)
cmake_minimum_required(VERSION 3.20)
project(Artic VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
find_package(Boost REQUIRED)

# Subdirectories
add_subdirectory(src)
add_subdirectory(runtime)
add_subdirectory(tests)

# Executable
add_executable(artic src/cli/main.cpp)
target_link_libraries(artic artic_compiler)
```

## 10. Dependencias Externas

### Compilador (C++)

- **Boost.Asio** - HTTP server asíncrono
- **nlohmann/json** - Serialización JSON
- **spdlog** - Logging rápido
- **CLI11** - Parsing de argumentos
- **Google Test** - Testing framework

### Runtime (TypeScript)

- **Ninguna** - Zero dependencies
- Build con **esbuild** para minificación

## 11. Métricas de Performance

### Objetivos:

- **Compilation time:** < 100ms para archivos individuales
- **Binary size:** < 5MB (servidor compilado)
- **Runtime JS:** < 8KB gzipped
- **Cold start:** < 5ms (servidor)
- **Memory usage:** < 50MB durante compilación

## 12. Próximos Pasos

1. Implementar estructura de directorios
2. Definir gramática formal (EBNF)
3. Implementar Lexer básico
4. Implementar Parser básico
5. Diseñar AST completo
6. Crear pipeline de prueba (hello world)
7. Iterar y extender