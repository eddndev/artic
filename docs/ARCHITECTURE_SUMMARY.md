# Resumen Ejecutivo: Arquitectura del Compilador Artic

## 🎯 Visión General

El compilador Artic es un **compilador multi-target** que transforma archivos `.atc` en tres outputs independientes:

1. **CSS estático** (estilos compilados)
2. **JavaScript optimizado** (cliente interactivo)
3. **Binario C++ nativo** (servidor de alto rendimiento)

---

## 📊 Pipeline en 4 Fases

```
.atc → [LEXER] → Tokens → [PARSER] → AST → [SEMANTIC] → Validated AST → [GENERATORS] → Outputs
```

### Fase 1: LEXER
- **Input:** Código fuente `.atc` (string)
- **Output:** Array de tokens
- **Tiempo:** ~5-10ms para archivos típicos

### Fase 2: PARSER
- **Input:** Tokens
- **Output:** Abstract Syntax Tree (AST)
- **Tipo:** Recursive Descent Parser
- **Tiempo:** ~20-30ms

### Fase 3: SEMANTIC ANALYZER
- **Input:** AST
- **Output:** AST validado + Symbol Table
- **Tareas:** Type checking, scope resolution, validación de decoradores
- **Tiempo:** ~10-20ms

### Fase 4: GENERATORS (Paralelo)
- **CssGenerator:** AST → CSS (~5ms)
- **ClientGenerator:** AST → JavaScript (~15ms)
- **ServerGenerator:** AST → C++ (~20ms)

**Tiempo total de compilación objetivo:** < 100ms

---

## 🏗️ Arquitectura de Módulos

```
artic/
├── Frontend (Análisis)
│   ├── Lexer          → Tokenización
│   ├── Parser         → Construcción del AST
│   ├── AST            → Representación del programa
│   └── Semantic       → Validación semántica
│
├── Backend (Generación)
│   ├── CssGenerator      → Utilidades → CSS
│   ├── ClientGenerator   → Logic/Template → JS
│   └── ServerGenerator   → Logic/Routes → C++
│
├── Shared (Utilidades)
│   ├── SymbolTable    → Tracking de variables
│   ├── TypeSystem     → Sistema de tipos
│   ├── Diagnostics    → Manejo de errores
│   └── Utils          → Helpers generales
│
└── Runtime
    ├── Client (TypeScript)   → runtime.js (~8KB)
    └── Server (C++ headers)  → http_server.hpp
```

---

## 🧩 Componentes Clave

### 1. Lexer
**Archivo:** `src/frontend/lexer/Lexer.cpp`

```cpp
class Lexer {
public:
    std::vector<Token> tokenize(const std::string& source);
private:
    char peek();
    char advance();
    Token scanToken();
};
```

**Tokens principales:**
- Keywords: `let`, `const`, `use`, `props`
- Decorators: `@route`, `@server`, `@client`, `@utility`, `@effect`
- Operators: `=`, `:`, `=>`, `{`, `}`
- Identifiers: nombres de variables
- Literals: strings, numbers, booleans

### 2. Parser
**Archivo:** `src/frontend/parser/Parser.cpp`

```cpp
class Parser {
public:
    std::unique_ptr<Component> parse(const std::vector<Token>& tokens);
private:
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<TemplateNode> parseTemplate();
};
```

**Produce un AST con estructura:**
```
Component
├── metadata (decoradores)
├── imports (use statements)
├── props (definición de props)
├── logic (variables, funciones)
├── styles (utilidades)
└── template (estructura HTML)
```

### 3. Semantic Analyzer
**Archivo:** `src/frontend/semantic/SemanticAnalyzer.cpp`

```cpp
class SemanticAnalyzer {
public:
    bool analyze(Component* ast, DiagnosticEngine& diag);
    SymbolTable* getSymbolTable();
private:
    void validateDecorators();
    void checkTypes();
    void resolveScopes();
};
```

**Validaciones:**
- Variables declaradas antes de uso
- Tipos compatibles en asignaciones
- Funciones `@server` no usan APIs del navegador
- Funciones `@client` no acceden a base de datos

### 4. CSS Generator
**Archivo:** `src/backend/css/CssGenerator.cpp`

```cpp
class CssGenerator {
public:
    std::string generate(const Component* ast);
private:
    void processUtility(const UtilityDeclaration* util);
    std::string resolveToken(const std::string& token);
};
```

**Transformación:**
```artic
@utility
btn { px:4 py:2 bg:blue-500 rounded:md }

↓

.btn {
    padding-left: 1rem;
    padding-right: 1rem;
    padding-top: 0.5rem;
    padding-bottom: 0.5rem;
    background-color: #3b82f6;
    border-radius: 0.375rem;
}
```

### 5. Client Generator
**Archivo:** `src/backend/client/ClientGenerator.cpp`

```cpp
class ClientGenerator {
public:
    std::string generate(const Component* ast, const SymbolTable* symbols);
private:
    void emitReactiveVariables();
    void emitEventHandlers();
    void compileTemplate();
};
```

**Transformación:**
```artic
let count = $state(0)

<button on:click={() => count++}>
    {count}
</button>

↓

import { signal, h } from './runtime';

let count = signal(0);

return h('button', {
    onClick: () => count.set(count.get() + 1)
}, count.get());
```

### 6. Server Generator
**Archivo:** `src/backend/server/ServerGenerator.cpp`

```cpp
class ServerGenerator {
public:
    std::string generate(const Component* ast, const SymbolTable* symbols);
private:
    void emitRoutes();
    void emitRpcHandlers();
    void emitServerFunctions();
};
```

**Transformación:**
```artic
@route("/api/users")

@server
let getUsers = async () => {
    return await db.users.findMany()
}

↓

#include <artic/runtime.hpp>

int main() {
    artic::HttpServer server(8080);

    server.get("/api/users", [](auto req, auto res) {
        auto users = db.users.findMany();
        res.json(users);
    });

    server.listen();
}
```

---

## 🎨 Estructura del AST

El AST es una representación en árbol del programa Artic:

```cpp
// Nodo base
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor* visitor) = 0;
    SourceLocation location;
};

// Nodo raíz
class Component : public ASTNode {
public:
    std::vector<Decorator*> metadata;
    std::vector<ImportDeclaration*> imports;
    PropsDeclaration* props;
    std::vector<Statement*> logic;
    std::vector<UtilityDeclaration*> styles;
    TemplateNode* template;
};

// Ejemplo de Statement
class VariableDeclaration : public Statement {
public:
    std::string kind;  // "let" | "const"
    std::string identifier;
    Expression* initializer;
};

// Ejemplo de Expression
class CallExpression : public Expression {
public:
    std::string callee;
    std::vector<Expression*> arguments;
};
```

---

## 🔄 Flujo de Datos Completo

### Ejemplo: Counter Component

**Input:** `counter.atc`
```artic
@route("/counter")

let count = $state(0)

@client
let increment = () => count++

<div class:(p:4)>
    <p>Count: {count}</p>
    <button on:click={increment} class:(btn)>+</button>
</div>
```

**Paso 1: Tokenización**
```
[@ROUTE, LPAREN, STRING("/counter"), RPAREN, LET, IDENTIFIER("count"), ...]
```

**Paso 2: Parsing**
```
Component {
    metadata: [RouteDecorator("/counter")],
    logic: [
        VariableDeclaration("count", CallExpression("$state", [0])),
        FunctionDeclaration("increment", ClientDecorator, ...)
    ],
    template: Element("div", ...)
}
```

**Paso 3: Semantic Analysis**
- Valida que `count` esté declarado
- Infiere tipo de `count` como `number`
- Valida que `increment` sea función
- Marca `count` como reactivo

**Paso 4: Generación**

**CSS Output:**
```css
.p-4 { padding: 1rem; }
.btn { /* estilos del btn */ }
```

**JS Output:**
```javascript
import { signal } from './runtime';

let count = signal(0);
let increment = () => count.set(count.get() + 1);

// Template rendering...
```

**C++ Output:**
```cpp
server.get("/counter", [](auto req, auto res) {
    res.html(renderCounterPage());
});
```

---

## 🛠️ Herramientas y Dependencias

### Compilador (C++)
- **Boost.Asio** - Servidor HTTP asíncrono
- **nlohmann/json** - Serialización JSON
- **spdlog** - Logging rápido
- **CLI11** - Parsing de argumentos CLI
- **Google Test** - Testing framework

### Runtime Client (TypeScript)
- **Zero dependencies** en producción
- **esbuild** para build
- Output: ~8KB gzipped

### Runtime Server (C++ header-only)
- **cpp-httplib** o **Boost.Asio**
- Header-only para fácil integración

---

## 📦 Build System

### CMake (C++)
```bash
# Configure
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Test
cd build && ctest

# Install
sudo cmake --install build
```

### Output del Build
```
build/
├── artic                    # Ejecutable del compilador
├── libartic_compiler.a      # Librería estática
└── tests/
    └── artic_tests          # Suite de tests
```

---

## 🎯 Métricas de Performance

### Objetivos de Fase 1 (SSG)

| Métrica | Objetivo | Medición |
|---------|----------|----------|
| **Compile time** | < 100ms | Archivo típico (~300 líneas) |
| **Memory usage** | < 50MB | Durante compilación |
| **CSS output** | < 10KB | Minificado |
| **Startup time** | < 1ms | Compilador CLI |

### Objetivos de Fase 2 (Reactivity)

| Métrica | Objetivo | Medición |
|---------|----------|----------|
| **JS bundle** | < 15KB | Con runtime incluido |
| **Hydration** | < 50ms | Tiempo de interactividad |
| **Update latency** | < 5ms | Cambio de estado → DOM |

### Objetivos de Fase 3 (Server)

| Métrica | Objetivo | Medición |
|---------|----------|----------|
| **Binary size** | < 5MB | Servidor compilado |
| **Cold start** | < 5ms | Primera request |
| **Throughput** | > 50k req/s | Hello World endpoint |
| **Memory** | < 10MB | Servidor idle |

---

## 🚀 Próximos Pasos Inmediatos

### 1. Setup del Proyecto
- [ ] Crear estructura de directorios
- [ ] Configurar CMakeLists.txt
- [ ] Setup de Git y .gitignore
- [ ] Instalar dependencias (vcpkg)

### 2. Implementación Básica
- [ ] Definir gramática formal (EBNF)
- [ ] Implementar Token.h y tipos de tokens
- [ ] Implementar Lexer básico
- [ ] Tests unitarios del Lexer
- [ ] Implementar Parser básico (metadata + imports)

### 3. Validación del Pipeline
- [ ] Crear ejemplo hello_world.atc
- [ ] Compilar con pipeline básico
- [ ] Generar HTML estático simple
- [ ] Validar output

### 4. Iteración
- [ ] Extender Parser (logic + template)
- [ ] Implementar CssGenerator
- [ ] Validar generación de CSS
- [ ] Añadir más tests

---

## 📚 Documentación Disponible

1. **MANIFEST.MD** - Especificación completa del lenguaje Artic
2. **ARCHITECTURE.md** - Arquitectura detallada del compilador
3. **DIRECTORY_STRUCTURE.md** - Estructura de carpetas completa
4. **ARCHITECTURE_SUMMARY.md** - Este documento (resumen ejecutivo)

---

## ❓ Preguntas Clave a Resolver

### Alta Prioridad
1. **Sistema de tipos:** ¿Inferencia completa o anotaciones requeridas?
2. **Manejo de errores:** ¿Try-catch nativo o Result<T, E>?
3. **Scope de variables:** ¿Qué pasa con variables sin decorador?
4. **Environment variables:** ¿Cómo se accede y se valida en compile-time?

### Media Prioridad
5. **Validación de formularios:** ¿Sintaxis declarativa?
6. **Referencias al DOM:** ¿Sistema de refs?
7. **Context/DI:** ¿Cómo se inyectan dependencias?
8. **Assets estáticos:** ¿Import de imágenes, fonts?

### Baja Prioridad (Fase 6+)
9. **i18n:** Sistema de internacionalización
10. **Animaciones:** Directivas de transición
11. **Testing:** Framework integrado
12. **WebSockets:** Sintaxis para conexiones persistentes

---

## 🎨 Filosofía de Diseño

1. **Explícito > Implícito**: Los decoradores dejan claro qué código va dónde
2. **Performance Nativa**: El compilador genera código de máquina real
3. **Zero Runtime Deps**: No más node_modules gigantes
4. **Developer Experience**: Errores claros, compilación rápida, tooling de primera
5. **Consistencia**: `:` para directivas, `=` para valores. Sin excepciones.

---

**Versión:** 0.1.0
**Última actualización:** Octubre 2025
**Autor:** Eduardo & Achronyme Team
