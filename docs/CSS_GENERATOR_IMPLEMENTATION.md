# Implementación del CSS Generator de Artic

## ✅ Estado: COMPLETADO (Fase 1 - SSG)

El CSS Generator de Artic ha sido completamente implementado para la **Fase 1: Static Site Generation (SSG)**.

---

## 📋 Resumen

El CSS Generator toma las declaraciones `@utility` del AST y genera CSS estático con soporte completo para:
- **Tokens Tailwind-like** (px:4, bg:blue-500, text:lg)
- **Prefijos de estado** (hover:, focus:, active:)
- **Prefijos responsive** (sm:, md:, lg:, xl:, 2xl:)
- **Dark mode** (dark:)
- **Minificación** (opcional)

### Pipeline Completo

```
Código .atc → LEXER → Tokens → PARSER → AST → CSS GENERATOR → style.css
```

---

## 📁 Archivos Implementados

### Core Classes

1. **UtilityResolver.h/cpp** (~320 líneas)
   - Resuelve tokens de utilidad a propiedades CSS
   - Incluye escalas de espaciado, colores y tamaños de fuente
   - Mapea propiedades shorthand a CSS estándar

2. **CssGenerator.h/cpp** (~320 líneas)
   - Genera CSS desde el AST
   - Maneja prefijos (hover:, sm:, md:, etc.)
   - Formatea output (pretty o minified)

### Tests

1. **tests/unit/backend/CssGeneratorTest.cpp** (~470 líneas)
   - 12 tests unitarios completos
   - Cobertura: spacing, colors, prefixes, responsive, complete utilities

2. **validate_css_generator.cpp** (~280 líneas)
   - 7 tests de validación rápida
   - Incluye ejemplo completo con route + utility + template

**Total: ~1,390 líneas de código C++20**

---

## 🎯 Características Implementadas

### 1. UtilityResolver - Resolución de Tokens

El `UtilityResolver` convierte tokens Artic a declaraciones CSS:

#### Ejemplo

```artic
px:4  →  padding-left: 1rem; padding-right: 1rem;
```

#### Soporta

**Spacing (Tailwind scale):**
```
0, 1, 2, 3, 4, 5, 6, 8, 10, 12, 16, 20, 24, 32, 40, 48, 56, 64
```

**Color Palettes:**
```
Blue:  blue-50 → blue-900
Gray:  gray-50 → gray-900
Red:   red-500 → red-700
Green: green-500 → green-700
Common: white, black, transparent
```

**Font Sizes:**
```
xs, sm, base, lg, xl, 2xl, 3xl, 4xl, 5xl, 6xl
```

**Property Mappings:**
```cpp
p   → padding
px  → padding-left, padding-right
py  → padding-top, padding-bottom
m   → margin
mx  → margin-left, margin-right
w   → width
h   → height
bg  → background-color
text → color (or font-size)
font → font-weight
rounded → border-radius
```

---

### 2. CssGenerator - Generación de CSS

El `CssGenerator` orquesta la generación completa:

#### Proceso

1. **Iterar sobre utilities** en el AST
2. **Agrupar tokens por prefix** (base, hover:, sm:, etc.)
3. **Resolver cada token** usando `UtilityResolver`
4. **Generar reglas CSS** para cada grupo
5. **Formatear output** (pretty o minified)

#### Ejemplo Completo

**Input (Artic):**
```artic
@utility
btn {
    px:4 py:2
    bg:blue-500
    text:white
    rounded:md
    hover:bg:blue-700
}
```

**Output (CSS):**
```css
.btn {
  padding-left: 1rem;
  padding-right: 1rem;
  padding-top: 0.5rem;
  padding-bottom: 0.5rem;
  background-color: #3b82f6;
  color: #ffffff;
  border-radius: 0.375rem;
}

.btn:hover {
  background-color: #1d4ed8;
}
```

---

## 🔧 Prefijos Soportados

### Pseudo-classes

```artic
hover:bg:blue-700    →  .btn:hover { background-color: #1d4ed8; }
focus:bg:blue-800    →  .btn:focus { background-color: #1e40af; }
active:bg:blue-900   →  .btn:active { background-color: #1e3a8a; }
disabled:bg:gray-300 →  .btn:disabled { background-color: #d1d5db; }
```

### Pseudo-elements

```artic
before:content:"★"   →  .star::before { content: "★"; }
after:content:"›"    →  .arrow::after { content: "›"; }
```

### Responsive (Mobile-first)

```artic
sm:px:6   →  @media (min-width: 640px) { .container { padding-left: 1.5rem; } }
md:px:8   →  @media (min-width: 768px) { .container { padding-left: 2rem; } }
lg:px:10  →  @media (min-width: 1024px) { .container { padding-left: 2.5rem; } }
xl:px:12  →  @media (min-width: 1280px) { .container { padding-left: 3rem; } }
2xl:px:16 →  @media (min-width: 1536px) { .container { padding-left: 4rem; } }
```

### Dark Mode

```artic
dark:bg:gray-900  →  @media (prefers-color-scheme: dark) { .card { background-color: #111827; } }
```

---

## 📝 Ejemplos de Uso

### Ejemplo 1: Button Simple

**Artic:**
```artic
@utility
btn {
    px:4 py:2
    bg:blue-500
    text:white
}
```

**CSS Generado:**
```css
.btn {
  padding-left: 1rem;
  padding-right: 1rem;
  padding-top: 0.5rem;
  padding-bottom: 0.5rem;
  background-color: #3b82f6;
  color: #ffffff;
}
```

---

### Ejemplo 2: Card con Hover

**Artic:**
```artic
@utility
card {
    p:6
    bg:white
    rounded:lg
    hover:bg:gray-50
}
```

**CSS Generado:**
```css
.card {
  padding: 1.5rem;
  background-color: #ffffff;
  border-radius: 0.5rem;
}

.card:hover {
  background-color: #f9fafb;
}
```

---

### Ejemplo 3: Responsive Container

**Artic:**
```artic
@utility
container {
    px:4
    sm:px:6
    md:px:8
    lg:px:12
}
```

**CSS Generado:**
```css
.container {
  padding-left: 1rem;
  padding-right: 1rem;
}

@media (min-width: 640px) {
  .container {
    padding-left: 1.5rem;
    padding-right: 1.5rem;
  }
}

@media (min-width: 768px) {
  .container {
    padding-left: 2rem;
    padding-right: 2rem;
  }
}

@media (min-width: 1024px) {
  .container {
    padding-left: 3rem;
    padding-right: 3rem;
  }
}
```

---

### Ejemplo 4: Dark Mode

**Artic:**
```artic
@utility
panel {
    bg:white
    text:gray-900
    dark:bg:gray-900
    dark:text:white
}
```

**CSS Generado:**
```css
.panel {
  background-color: #ffffff;
  color: #111827;
}

@media (prefers-color-scheme: dark) {
  .panel {
    background-color: #111827;
    color: #ffffff;
  }
}
```

---

### Ejemplo 5: Componente Completo

**Artic:**
```artic
@route("/dashboard")

use theme "./theme.atc"

@utility
container {
    w:full px:4
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
    rounded:lg
    hover:bg:blue-700
    focus:bg:blue-800
}

<div class:(container)>
    <h1 class:(title)>Dashboard</h1>
    <button class:(btn)>Click Me</button>
</div>
```

**CSS Generado:**
```css
.container {
  width: 100%;
  padding-left: 1rem;
  padding-right: 1rem;
}

.title {
  font-size: 1.875rem;
  line-height: 2.25rem;
  font-weight: 700;
  color: #111827;
}

.btn {
  padding-left: 1.5rem;
  padding-right: 1.5rem;
  padding-top: 0.75rem;
  padding-bottom: 0.75rem;
  background-color: #3b82f6;
  color: #ffffff;
  border-radius: 0.5rem;
}

.btn:hover {
  background-color: #1d4ed8;
}

.btn:focus {
  background-color: #1e40af;
}
```

---

## 🧪 Tests Implementados

### Suite Completa (12 tests)

1. **test1_SimpleSpacing** - px:4 py:2
2. **test2_ColorUtility** - bg:blue-500
3. **test3_TextColor** - text:white
4. **test4_HoverPrefix** - hover:bg:blue-700
5. **test5_FocusPrefix** - focus:bg:blue-800
6. **test6_ResponsiveSmall** - sm:px:6
7. **test7_ResponsiveMedium** - md:px:8
8. **test8_BorderRadius** - rounded:md
9. **test9_FontWeight** - font:bold
10. **test10_WidthHeight** - w:full h:full
11. **test11_MultipleUtilities** - Múltiples @utility
12. **test12_CompleteButton** - Button completo con hover

### Ejecutar Tests

```bash
# Con CMake
cd build
cmake ..
cmake --build .
./css_generator_tests

# Manual
g++ -std=c++20 -Isrc \
    src/frontend/lexer/Token.cpp \
    src/frontend/lexer/Lexer.cpp \
    src/frontend/parser/Parser.cpp \
    src/backend/css/UtilityResolver.cpp \
    src/backend/css/CssGenerator.cpp \
    tests/unit/backend/CssGeneratorTest.cpp \
    -o css_tests
./css_tests
```

---

## 📊 Arquitectura

### Diagrama de Clases

```
Component (AST)
    │
    ├─ utilities: vector<UtilityDeclaration>
    │       │
    │       └─ tokens: vector<UtilityToken>
    │               │
    │               ├─ prefix: string (hover, sm, etc.)
    │               ├─ property: string (px, bg, text)
    │               └─ value: string (4, blue-500)
    │
    ↓
CssGenerator
    │
    ├─ generate(Component*) → string
    │
    └─ Uses: UtilityResolver
            │
            ├─ m_spacing: map<string, string>
            ├─ m_colors: map<string, string>
            ├─ m_fontSizes: map<string, pair<string, string>>
            └─ resolve(property, value) → vector<string>
```

---

## 🔧 API del CssGenerator

### Constructor

```cpp
// Default (pretty print)
CssGenerator generator;

// Con opciones
CssGeneratorOptions options;
options.minify = true;
options.pretty = false;
CssGenerator generator(options);
```

### Métodos Principales

```cpp
// Generar CSS desde Component completo
std::string css = generator.generate(component.get());

// Generar CSS de una sola utilidad
std::string css = generator.generateUtility(utility.get());
```

---

## 📈 Propiedades Soportadas

### Spacing

| Token | Propiedades CSS |
|-------|----------------|
| `p:4` | `padding: 1rem` |
| `px:4` | `padding-left: 1rem; padding-right: 1rem` |
| `py:2` | `padding-top: 0.5rem; padding-bottom: 0.5rem` |
| `pt:4` | `padding-top: 1rem` |
| `pr:4` | `padding-right: 1rem` |
| `pb:4` | `padding-bottom: 1rem` |
| `pl:4` | `padding-left: 1rem` |
| `m:4` | `margin: 1rem` |
| `mx:4` | `margin-left: 1rem; margin-right: 1rem` |
| `my:2` | `margin-top: 0.5rem; margin-bottom: 0.5rem` |

### Dimensiones

| Token | Propiedades CSS |
|-------|----------------|
| `w:full` | `width: 100%` |
| `w:screen` | `width: 100vw` |
| `w:64` | `width: 16rem` |
| `h:full` | `height: 100%` |
| `h:screen` | `height: 100vh` |
| `h:32` | `height: 8rem` |

### Colores

| Token | Propiedades CSS |
|-------|----------------|
| `bg:blue-500` | `background-color: #3b82f6` |
| `bg:white` | `background-color: #ffffff` |
| `text:gray-900` | `color: #111827` |
| `text:white` | `color: #ffffff` |

### Tipografía

| Token | Propiedades CSS |
|-------|----------------|
| `text:xs` | `font-size: 0.75rem; line-height: 1rem` |
| `text:lg` | `font-size: 1.125rem; line-height: 1.75rem` |
| `text:2xl` | `font-size: 1.5rem; line-height: 2rem` |
| `font:bold` | `font-weight: 700` |
| `font:semibold` | `font-weight: 600` |
| `font:medium` | `font-weight: 500` |

### Border Radius

| Token | Propiedades CSS |
|-------|----------------|
| `rounded:none` | `border-radius: 0` |
| `rounded:sm` | `border-radius: 0.125rem` |
| `rounded:md` | `border-radius: 0.375rem` |
| `rounded:lg` | `border-radius: 0.5rem` |
| `rounded:xl` | `border-radius: 0.75rem` |
| `rounded:full` | `border-radius: 9999px` |

---

## 💻 Compilación

### Manual

```bash
cd C:\apache\htdocs\artic

g++ -std=c++20 -Isrc \
    src/frontend/lexer/Token.cpp \
    src/frontend/lexer/Lexer.cpp \
    src/frontend/parser/Parser.cpp \
    src/backend/css/UtilityResolver.cpp \
    src/backend/css/CssGenerator.cpp \
    validate_css_generator.cpp \
    -o validate_css_generator.exe

./validate_css_generator.exe
```

### Con CMake

```bash
mkdir build && cd build
cmake .. -DARTIC_BUILD_TESTS=ON
cmake --build .
./css_generator_tests
```

---

## 🚫 Limitaciones Conocidas (Fase 1)

### No Soportado en Fase 1

1. **Combinación de prefijos**
   ```artic
   sm:hover:bg:blue-500  ❌ No soportado aún
   ```

2. **Valores arbitrarios**
   ```artic
   px:[15px]  ❌ No soportado aún
   bg:[#custom]  ❌ No soportado aún
   ```

3. **Pseudo-classes avanzadas**
   ```artic
   first-child:bg:blue-500  ❌ No soportado aún
   nth-child:2:bg:red-500  ❌ No soportado aún
   ```

4. **Container queries**
   ```artic
   @container:px:4  ❌ No soportado aún
   ```

5. **Themes dinámicos**
   ```artic
   [theme-dark]:bg:gray-900  ❌ No soportado aún
   ```

Estas características se implementarán en fases futuras (2-3).

---

## ✅ Validación

### Código Revisado

- ✅ Sintaxis C++20 correcta
- ✅ Memory safety (unique_ptr, referencias)
- ✅ Const correctness
- ✅ Error handling robusto
- ✅ Resolución completa de tokens Tailwind
- ✅ Soporte de prefijos (pseudo-classes, responsive, dark mode)

### Tests Pasando

- ✅ 12/12 tests unitarios
- ✅ 7/7 tests de validación
- ✅ Ejemplos reales validados

---

## 📊 Métricas

### Líneas de Código

| Archivo | Líneas | Comentarios |
|---------|--------|-------------|
| UtilityResolver.h | 70 | 20 |
| UtilityResolver.cpp | 250 | 40 |
| CssGenerator.h | 95 | 30 |
| CssGenerator.cpp | 225 | 35 |
| CssGeneratorTest.cpp | 470 | 60 |
| validate_css_generator.cpp | 280 | 50 |
| **Total** | **1,390** | **235** |

### Cobertura

- **Tokens Tailwind**: 100+ tokens soportados
- **Prefijos**: 12 prefijos implementados
- **Tests**: 12 tests unitarios (100% passing)
- **Propiedades CSS**: 20+ mappings

---

## 🚀 Próximos Pasos

Con el CSS Generator completado, los siguientes pasos son:

### **HTML Generator**

Generar HTML desde el template AST:

```artic
<div class:(container)>
    <h1>Hello World</h1>
</div>
```

↓

```html
<div class="container">
    <h1>Hello World</h1>
</div>
```

### **Compiler Orchestrator**

Conectar todo el pipeline:

```cpp
Compiler compiler(options);
compiler.compile("index.atc");  // → dist/index.html + dist/style.css
```

---

## 📚 Referencias

- **GRAMMAR.md** - Gramática formal completa
- **ARCHITECTURE.md** - Arquitectura del compilador
- **PARSER_IMPLEMENTATION.md** - Documentación del Parser
- **COMPILER_ARCHITECTURE_EXPLAINED.md** - Explicación de la arquitectura

---

**Versión del CSS Generator**: 1.0.0 (Phase 1 - SSG)
**Fecha de Completado**: Octubre 29, 2025
**Autor**: Eduardo Alonso (Achronyme)
**Estado**: ✅ PRODUCTION READY (Phase 1)

---

## 🎉 CSS Generator Completado!

El CSS Generator de Artic está completo y funcional para la Fase 1 (SSG). Genera CSS estático correcto desde declaraciones @utility con soporte completo para:
- ✅ Tokens Tailwind-like
- ✅ Prefijos de estado (hover, focus, active)
- ✅ Responsive (sm, md, lg, xl, 2xl)
- ✅ Dark mode
- ✅ Pretty printing y minificación

**Siguiente paso: Implementar el HTML Generator** 🎨
