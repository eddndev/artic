# Arquitectura del Compilador Artic - Explicación Detallada

## 🤔 Confusión Terminológica Importante

Cuando hablamos de un **compilador**, los términos "frontend" y "backend" tienen un significado **diferente** a cuando hablamos de desarrollo web.

---

## 📚 Terminología de Compiladores vs Web

### En Desarrollo Web 🌐
- **Frontend** = Cliente (navegador, React, Vue, etc.)
- **Backend** = Servidor (Node.js, Django, etc.)

### En Compiladores 🔧
- **Frontend** = Análisis del código fuente (entiende el código)
- **Backend** = Generación de código de salida (produce el resultado)

---

## 🏗️ Estructura del Compilador Artic

```
src/
├── frontend/          ← FRONTEND DEL COMPILADOR (Análisis)
│   ├── lexer/         → Tokenización
│   ├── parser/        → Construcción del AST
│   ├── ast/           → Representación del programa
│   └── semantic/      → Análisis semántico (futuro)
│
├── backend/           ← BACKEND DEL COMPILADOR (Generación)
│   ├── css/           → Genera CSS
│   ├── client/        → Genera JavaScript (para el navegador)
│   └── server/        → Genera C++ (para el servidor HTTP)
│
├── compiler/          ← ORQUESTADOR PRINCIPAL
│   ├── Compiler.cpp   → Pipeline completo
│   └── CompilerOptions.cpp → Configuración
│
├── shared/            ← Utilidades compartidas
│   ├── symbols/
│   ├── types/
│   ├── diagnostics/
│   └── utils/
│
└── cli/               ← Interfaz de línea de comandos
    └── main.cpp
```

---

## 🎯 ¿Qué va en cada directorio?

### 1️⃣ `src/frontend/` - FRONTEND DEL COMPILADOR

**Propósito**: Analizar y entender el código fuente `.atc`

**Responsabilidades**:
- ✅ **Lexer** - Convierte texto en tokens
- ✅ **Parser** - Convierte tokens en AST
- 🔜 **Semantic Analyzer** - Valida tipos, scopes, semántica
- ✅ **AST** - Representación estructurada del programa

**No hace**: No genera ningún código de salida

**Analogía**: Es como un lector que **entiende** lo que está escrito, pero no lo traduce.

---

### 2️⃣ `src/backend/` - BACKEND DEL COMPILADOR

**Propósito**: Generar código de salida desde el AST

**Responsabilidades**:
- 🔜 **CSS Generator** - AST → CSS estático
- 🔜 **Client Generator** - AST → JavaScript (para navegador web)
- 🔜 **Server Generator** - AST → C++ (para servidor HTTP)

**No hace**: No analiza código fuente, solo lo genera

**Analogía**: Es como un traductor que **convierte** lo entendido a otros idiomas.

---

### 3️⃣ `src/compiler/` - ORQUESTADOR PRINCIPAL

**Propósito**: Conectar todo el pipeline y orquestar la compilación

**Lo que DEBERÍA contener**:

#### `Compiler.h/cpp` - Clase principal
```cpp
class Compiler {
public:
    Compiler(const CompilerOptions& options);

    // Método principal que ejecuta todo el pipeline
    bool compile(const std::string& inputFile);

private:
    CompilerOptions m_options;

    // Pipeline completo
    std::vector<Token> lex(const std::string& source);
    std::unique_ptr<Component> parse(const std::vector<Token>& tokens);
    bool analyze(Component* ast);
    bool generateCSS(Component* ast);
    bool generateHTML(Component* ast);
    bool generateServer(Component* ast);  // Fase 3
};
```

#### `CompilerOptions.h/cpp` - Opciones de compilación
```cpp
struct CompilerOptions {
    std::string inputFile;
    std::string outputDir;
    bool minify;
    bool sourceMaps;
    CompilationMode mode;  // SSG, SSR, SPA
    OptimizationLevel optimization;
};
```

**Analogía**: Es el **director de orquesta** que coordina a todos los músicos (módulos).

---

### 4️⃣ `src/shared/` - Código Compartido

**Propósito**: Código usado tanto por frontend como backend

**Contenido**:
- **SymbolTable** - Tabla de símbolos (variables, funciones)
- **TypeSystem** - Sistema de tipos
- **Diagnostics** - Manejo de errores y warnings
- **Utils** - FileSystem, StringUtils, Logger

---

### 5️⃣ `src/cli/` - Interfaz de Usuario

**Propósito**: Comandos de línea de comandos

**Contenido**:
- `main.cpp` - Entry point
- `BuildCommand.cpp` - Comando `artic build`
- `DevCommand.cpp` - Comando `artic dev`

---

## 🔄 Pipeline Completo del Compilador

### Flujo Actual (Fase 1)

```
1. CLI (main.cpp)
     ↓
2. Compiler::compile() [EN src/compiler/]
     ↓
3. FRONTEND
     ├── Lexer → Tokens
     └── Parser → AST
     ↓
4. BACKEND
     ├── CssGenerator → style.css
     └── HtmlGenerator → index.html
     ↓
5. OUTPUT
     └── dist/
         ├── index.html
         └── style.css
```

### Ejemplo de Uso (lo que falta implementar)

```cpp
// src/cli/main.cpp
int main(int argc, char* argv[]) {
    if (comando == "build") {
        // Crear opciones
        CompilerOptions options;
        options.inputFile = "index.atc";
        options.outputDir = "dist/";
        options.minify = true;

        // Crear compilador
        Compiler compiler(options);

        // Ejecutar pipeline completo
        bool success = compiler.compile(options.inputFile);

        return success ? 0 : 1;
    }
}
```

```cpp
// src/compiler/Compiler.cpp
bool Compiler::compile(const std::string& inputFile) {
    // 1. Leer archivo
    std::string source = FileSystem::readFile(inputFile);

    // 2. FRONTEND - Análisis
    auto tokens = lex(source);
    auto ast = parse(tokens);
    bool valid = analyze(ast.get());

    if (!valid) {
        return false;
    }

    // 3. BACKEND - Generación
    generateCSS(ast.get());
    generateHTML(ast.get());

    return true;
}
```

---

## 📋 Estado Actual vs Estado Deseado

### ✅ Implementado

```
src/
├── frontend/
│   ├── lexer/        ✅ Token.h, Lexer.h, Lexer.cpp
│   ├── parser/       ✅ Parser.h, Parser.cpp
│   └── ast/          ✅ Component.h, Decorator.h, etc.
├── cli/
│   └── main.cpp      ✅ Comando básico "lex"
```

### ❌ Falta Implementar

```
src/
├── compiler/
│   ├── Compiler.h         ❌ Orquestador principal
│   ├── Compiler.cpp       ❌ Pipeline completo
│   ├── CompilerOptions.h  ❌ Opciones
│   └── CompilerOptions.cpp ❌
├── backend/
│   ├── css/
│   │   ├── CssGenerator.h     ❌
│   │   └── CssGenerator.cpp   ❌
│   └── html/
│       ├── HtmlGenerator.h    ❌
│       └── HtmlGenerator.cpp  ❌
└── cli/
    ├── BuildCommand.h     ❌ Comando "build"
    └── BuildCommand.cpp   ❌
```

---

## 🎯 ¿Por qué esta estructura?

### Ventajas de Separar Frontend/Backend

1. **Modularidad**: Cada parte tiene una responsabilidad clara
2. **Reutilización**: El frontend puede usarse con diferentes backends
3. **Testing**: Se puede testear análisis y generación por separado
4. **Estándar de la industria**: Así funcionan LLVM, GCC, Clang, etc.

### Ventajas de tener `compiler/`

1. **Orquestación centralizada**: Un solo lugar controla el pipeline
2. **Configuración unificada**: CompilerOptions en un solo lugar
3. **Fácil de entender**: `Compiler::compile()` es el entry point
4. **Extensibilidad**: Fácil agregar nuevas fases (optimización, etc.)

---

## 🔍 Comparación con Otros Compiladores

### LLVM (Compilador de C++)

```
llvm/
├── lib/
│   ├── Frontend/        ← Clang: análisis de C++
│   ├── IR/              ← Representación intermedia
│   ├── Transforms/      ← Optimizaciones
│   └── CodeGen/         ← Backend: genera x86, ARM, etc.
```

### TypeScript

```
typescript/
├── src/
│   ├── compiler/        ← Orquestador
│   │   ├── scanner.ts   ← Lexer
│   │   ├── parser.ts    ← Parser
│   │   └── checker.ts   ← Semantic
│   └── services/        ← Language Server, etc.
```

### Rust

```
rust/
├── compiler/
│   ├── rustc_lexer/     ← Frontend: Lexer
│   ├── rustc_parse/     ← Frontend: Parser
│   ├── rustc_ast/       ← AST
│   ├── rustc_codegen/   ← Backend: generación
│   └── rustc_driver/    ← Orquestador
```

**Artic sigue el mismo patrón estándar.**

---

## 📝 Resumen

| Directorio | Propósito | Analogía |
|------------|-----------|----------|
| **frontend/** | Analizar código .atc | Lector que entiende |
| **backend/** | Generar CSS, JS, C++ | Traductor que convierte |
| **compiler/** | Orquestar el pipeline | Director de orquesta |
| **shared/** | Código común | Biblioteca compartida |
| **cli/** | Interfaz de usuario | Punto de entrada |

---

## 🚀 Próximos Pasos

### Implementar `compiler/` (Orquestador)

1. ✅ **Compiler.h** - Interfaz del compilador
2. ✅ **Compiler.cpp** - Pipeline completo
3. ✅ **CompilerOptions.h** - Opciones de compilación

### Implementar `backend/` (Generadores)

4. ✅ **CssGenerator** - Generar CSS desde @utility
5. ✅ **HtmlGenerator** - Generar HTML desde template

### Actualizar `cli/`

6. ✅ **BuildCommand** - Comando `artic build`

---

## 💡 Conclusión

**`src/frontend/`** y **`src/backend/`** son términos estándar de compiladores:

- **Frontend** = Entiende el código fuente
- **Backend** = Genera código de salida
- **Compiler** = Conecta ambos

Es diferente de "frontend web" y "backend web". Son conceptos separados.

**No hay confusión**: La estructura está correcta y sigue estándares de la industria.

---

**¿Te gustaría que implemente el módulo `compiler/` (orquestador) ahora para conectar todo el pipeline?**
