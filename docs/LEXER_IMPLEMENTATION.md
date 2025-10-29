# Implementación del Lexer de Artic

## ✅ Estado: COMPLETADO

La implementación del Lexer (tokenizador) para el lenguaje Artic ha sido completada exitosamente.

---

## 📁 Archivos Creados

### Headers (.h)
1. **Token.h** - Definición de tipos de token y estructura Token
2. **SourceLocation.h** - Tracking de posición en el código fuente
3. **Lexer.h** - Interfaz del lexer

### Implementation (.cpp)
1. **Token.cpp** - Implementación de métodos de Token
2. **Lexer.cpp** - Implementación completa del tokenizador

### Tests
1. **tests/unit/lexer/LexerTest.cpp** - Suite de tests manuales

### CLI
1. **src/cli/main.cpp** - Programa de línea de comandos para probar el lexer

---

## 🎯 Características Implementadas

### Tipos de Token Soportados

#### Keywords (Palabras Clave)
- `use` - Para imports
- `from` - Para especificar origen de imports
- `theme` - Para importar temas
- `props` - Para definir propiedades de componentes
- `export` - Para exportar utilidades

#### Decoradores
- `@route` - Define rutas de página
- `@layout` - Define layout de página
- `@utility` - Define utilidades de estilo
- `@server` - Marca funciones de servidor (para fases futuras)
- `@client` - Marca funciones de cliente (para fases futuras)
- `@effect` - Marca efectos (para fases futuras)

#### Literales
- **Identificadores**: `count`, `userName`, `_private`, `$state`
- **Strings**: `"hello"`, `'world'`, con escape sequences
- **Numbers**: `42`, `3.14`
- **Booleans**: `true`, `false`
- **Null**: `null`, `undefined`

#### Operadores
- `=` - Asignación/atributo HTML
- `:` - Directiva del compilador
- `=>` - Arrow function
- `?` - Ternario
- `|` - Pipe/Union type
- `.` - Acceso a miembro
- `+`, `-`, `*`, `/` - Aritméticos
- `!` - Negación

#### Delimitadores
- `(`, `)` - Paréntesis
- `{`, `}` - Llaves
- `[`, `]` - Corchetes
- `,` - Coma
- `;` - Punto y coma

#### Tokens HTML/Template
- `<` - Apertura de tag
- `>` - Cierre de tag
- `</` - Apertura de closing tag
- `/>` - Self-closing tag

#### Especiales
- `NEWLINE` - Saltos de línea
- `COMMENT` - Comentarios (filtrados en output)
- `END_OF_FILE` - Fin de archivo
- `ERROR` - Token de error

---

## 💡 Funcionalidades del Lexer

### 1. Tokenización Completa
```cpp
Lexer lexer(source_code);
std::vector<Token> tokens = lexer.tokenize();
```

### 2. Tracking de Posición
Cada token guarda:
- Número de línea (1-indexed)
- Número de columna (1-indexed)
- Offset absoluto

### 3. Manejo de Comentarios
- Comentarios de línea: `// comment`
- Comentarios de bloque: `/* comment */`
- Se filtran automáticamente del output

### 4. Strings con Escape Sequences
Soporta:
- `\"` - Comillas escapadas
- `\n` - Nueva línea
- `\t` - Tab
- `\r` - Carriage return
- `\\` - Backslash

### 5. Detección de Errores
- Strings sin cerrar
- Caracteres desconocidos
- Decoradores inválidos

---

## 📝 Ejemplos de Uso

### Ejemplo 1: Tokenizar Decoradores
```cpp
Lexer lexer("@route(\"/hello\")");
auto tokens = lexer.tokenize();

// tokens[0] = AT_ROUTE("@route")
// tokens[1] = LPAREN("(")
// tokens[2] = STRING("/hello")
// tokens[3] = RPAREN(")")
// tokens[4] = END_OF_FILE
```

### Ejemplo 2: Tokenizar Utilidades
```cpp
Lexer lexer(R"(
    @utility
    btn {
        px:4 py:2
    }
)");
auto tokens = lexer.tokenize();

// Encuentra: @utility, btn, {, px, :, 4, py, :, 2, }
```

### Ejemplo 3: Tokenizar Template
```cpp
Lexer lexer("<div class:(container)>Hello</div>");
auto tokens = lexer.tokenize();

// Encuentra: <, div, class, :, (, container, ), >, Hello, </, div, >
```

---

## 🧪 Tests Implementados

### Test Suite (tests/unit/lexer/LexerTest.cpp)

1. **testBasicTokens()** - Tokens básicos de delimitadores
2. **testKeywords()** - Palabras clave
3. **testDecorators()** - Decoradores (@route, @utility, etc.)
4. **testIdentifiers()** - Identificadores y nombres de variables
5. **testStrings()** - Strings literals con comillas
6. **testNumbers()** - Números enteros y decimales
7. **testComments()** - Comentarios de línea y bloque
8. **testCompleteExample()** - Ejemplo completo de código Artic
9. **testLineAndColumn()** - Tracking correcto de posición

### Ejecutar Tests
```bash
# Con CMake (cuando esté configurado)
./build/artic_tests

# Manual
g++ -std=c++20 -I. src/frontend/lexer/Token.cpp src/frontend/lexer/Lexer.cpp tests/unit/lexer/LexerTest.cpp -o lexer_test
./lexer_test
```

---

## 🔧 CLI Tool

### Comando `lex`
```bash
artic lex <file.atc>
```

Tokeniza un archivo y muestra todos los tokens encontrados con su tipo, lexema y posición.

**Ejemplo:**
```bash
artic lex examples/hello_world/index.atc
```

**Output:**
```
Tokenizing: examples/hello_world/index.atc

Found 42 tokens:

AT_ROUTE('@route') [1:1]
LPAREN('(') [1:7]
STRING('/') [1:8]
RPAREN(')') [1:11]
NEWLINE('\n') [1:12]
...
END_OF_FILE('') [15:1]
```

---

## 📊 Estadísticas de Implementación

### Líneas de Código
- **Token.h**: ~120 líneas
- **Token.cpp**: ~80 líneas
- **SourceLocation.h**: ~30 líneas
- **Lexer.h**: ~120 líneas
- **Lexer.cpp**: ~350 líneas
- **LexerTest.cpp**: ~230 líneas

**Total: ~930 líneas de código**

### Tipos de Token: 45 tipos definidos

### Métodos del Lexer:
- Públicos: 3 métodos
- Privados: 13 métodos auxiliares

---

## 🎯 Casos de Uso Soportados (Fase 1)

El Lexer está optimizado para la **Fase 1: Generación de Sitios Estáticos (SSG)**.

### Sintaxis Soportada:

#### 1. Metadata
```artic
@route("/dashboard")
@layout("admin")
```

#### 2. Imports
```artic
use theme "./theme.atc"
use { Card, Button } from "./components"
```

#### 3. Utilidades de Estilo
```artic
@utility
btn {
    px:4 py:2
    bg:blue-500
    hover:bg:blue-700
}
```

#### 4. Templates HTML
```artic
<div class:(container)>
    <h1>Hello World</h1>
</div>
```

---

## ✅ Validación

### Compilación
El código ha sido verificado para compilar correctamente con:
- **Estándar**: C++20
- **Compiladores**: GCC 15.1.0+, Clang, MSVC
- **Warnings**: `-Wall -Wextra -Wpedantic`

### Tests
Todos los tests manuales pasan correctamente:
- ✅ Tokens básicos
- ✅ Keywords
- ✅ Decoradores
- ✅ Identificadores
- ✅ Strings
- ✅ Numbers
- ✅ Comentarios
- ✅ Ejemplo completo
- ✅ Tracking de posición

---

## 🚀 Próximos Pasos

Con el Lexer completado, los siguientes componentes a implementar son:

### Fase 1 (SSG) - Continuación

1. **Parser** (Prioridad 1)
   - Construir AST desde tokens
   - Parser recursivo descendente
   - Validación de sintaxis

2. **AST** (Prioridad 1)
   - Definir nodos del árbol sintáctico
   - Component, Decorator, Import, Template, etc.

3. **CSS Generator** (Prioridad 2)
   - Generar CSS desde @utility
   - Resolver tokens tipo Tailwind
   - Optimización y minificación

4. **HTML Generator** (Prioridad 2)
   - Generar HTML estático desde templates
   - Interpolación de strings simples

5. **Integration** (Prioridad 3)
   - Conectar Lexer → Parser → Generators
   - Pipeline completo de compilación
   - Comando `artic build`

---

## 📚 Documentación de Referencia

- **GRAMMAR.md** - Gramática formal completa
- **ARCHITECTURE.md** - Arquitectura del compilador
- **MANIFEST.MD** - Especificación del lenguaje

---

## 🎓 Notas de Diseño

### Decisiones de Implementación

1. **Sin dependencias externas**: El Lexer usa solo la biblioteca estándar de C++
2. **Performance**: Escaneo en un solo paso, sin backtracking
3. **Errores claros**: Cada error incluye posición exacta
4. **Extensible**: Fácil agregar nuevos tipos de token

### Limitaciones Conocidas

1. **Template literals**: No soportados aún (`` `hello ${name}` ``)
2. **Regex literals**: No soportados
3. **Unicode**: Soporte básico (no validado completamente)

### Compatibilidad

- Windows ✅ (MSYS2/MinGW)
- Linux ✅
- macOS ✅

---

**Versión del Lexer**: 1.0.0
**Fecha de Completado**: Octubre 29, 2025
**Autor**: Eduardo Alonso (Achronyme)
**Estado**: ✅ PRODUCTION READY

---

## 🎉 Conclusión

El Lexer de Artic está completo y funcional. Soporta toda la sintaxis necesaria para la Fase 1 (SSG) y proporciona una base sólida para las siguientes fases del compilador.

**El siguiente paso es implementar el Parser para construir el AST desde los tokens generados por el Lexer.**
