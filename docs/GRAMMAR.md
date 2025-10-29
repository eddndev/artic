# Gramática Formal de Artic (EBNF)

## Notación EBNF

- `|` = Alternativa (OR)
- `[]` = Opcional (0 o 1)
- `{}` = Repetición (0 o más)
- `()` = Agrupación
- `*` = 0 o más ocurrencias
- `+` = 1 o más ocurrencias
- `?` = 0 o 1 ocurrencia
- `""` = Terminal (literal)
- `<>` = No-terminal (regla)

---

## Estructura Principal

```ebnf
(* Un archivo .atc es un componente completo *)
<component> ::= [<metadata>] [<imports>] [<props>] [<logic>] [<styles>] <template>

(* El orden es ESTRICTO: metadata → imports → props → logic → styles → template *)
```

---

## 1. Metadata (Decoradores de Fichero)

```ebnf
<metadata> ::= <decorator>+

<decorator> ::= "@route" "(" <string_literal> ")"
              | "@layout" "(" <string_literal> ")"

(* Ejemplos:
   @route("/dashboard")
   @route("/blog/:slug")
   @layout("admin")
*)
```

---

## 2. Imports

```ebnf
<imports> ::= <import_statement>+

<import_statement> ::= <theme_import>
                     | <named_import>
                     | <default_import>

<theme_import> ::= "use" "theme" <string_literal>

<named_import> ::= "use" "{" <identifier_list> "}" "from" <string_literal>

<default_import> ::= "use" <identifier> "from" <string_literal>

<identifier_list> ::= <identifier> ("," <identifier>)*

(* Ejemplos:
   use theme "./theme.atc"
   use { Card, Button } from "./components"
   use api from "./api"
*)
```

---

## 3. Props

```ebnf
<props> ::= "props" "{" <prop_definition>* "}"

<prop_definition> ::= <identifier> ["?"] ":" <type> ["=" <expression>] ","?

<type> ::= <primitive_type>
         | <array_type>
         | <object_type>
         | <function_type>
         | <union_type>
         | <literal_type>
         | "any"

<primitive_type> ::= "string" | "number" | "boolean" | "void"

<array_type> ::= <type> "[" "]"
               | "Array" "<" <type> ">"

<object_type> ::= "{" <object_property>* "}"

<object_property> ::= <identifier> ["?"] ":" <type> ","?

<function_type> ::= "(" [<parameter_list>] ")" "=>" <type>

<parameter_list> ::= <parameter> ("," <parameter>)*

<parameter> ::= <identifier> ["?"] ":" <type>

<union_type> ::= <type> ("|" <type>)+

<literal_type> ::= <string_literal> | <number_literal> | <boolean_literal>

(* Ejemplos:
   props {
       title: string,
       count?: number = 0,
       variant: "primary" | "secondary",
       onClick: (id: string) => void
   }
*)
```

---

## 4. Logic (Script)

```ebnf
<logic> ::= <statement>*

<statement> ::= <variable_declaration>
              | <function_declaration>
              | <effect_declaration>
              | <expression_statement>

(* Variable Declaration *)
<variable_declaration> ::= <variable_kind> <identifier> "=" <expression>

<variable_kind> ::= "let" | "const"

(* Function Declaration *)
<function_declaration> ::= [<context_decorator>]
                          <variable_kind> <identifier> "=" <function_expression>

<context_decorator> ::= "@server" | "@client"

<function_expression> ::= ["async"] "(" [<parameter_list>] ")" "=>" <function_body>

<function_body> ::= <expression>
                  | "{" <statement>* "}"

(* Effect Declaration *)
<effect_declaration> ::= "@effect" <identifier> "(" ")" "{" <statement>* "}"

(* Expression Statement *)
<expression_statement> ::= <expression>

(* Expressions *)
<expression> ::= <assignment_expression>

<assignment_expression> ::= <ternary_expression>
                          | <identifier> <assignment_operator> <expression>

<assignment_operator> ::= "=" | "+=" | "-=" | "*=" | "/=" | "++" | "--"

<ternary_expression> ::= <logical_or_expression>
                       | <logical_or_expression> "?" <expression> ":" <expression>

<logical_or_expression> ::= <logical_and_expression>
                          | <logical_or_expression> "||" <logical_and_expression>

<logical_and_expression> ::= <equality_expression>
                           | <logical_and_expression> "&&" <equality_expression>

<equality_expression> ::= <relational_expression>
                        | <equality_expression> ("==" | "!=" | "===" | "!==") <relational_expression>

<relational_expression> ::= <additive_expression>
                          | <relational_expression> ("<" | ">" | "<=" | ">=") <additive_expression>

<additive_expression> ::= <multiplicative_expression>
                        | <additive_expression> ("+" | "-") <multiplicative_expression>

<multiplicative_expression> ::= <unary_expression>
                              | <multiplicative_expression> ("*" | "/" | "%") <unary_expression>

<unary_expression> ::= <postfix_expression>
                     | ("!" | "-" | "++" | "--" | "typeof" | "await") <unary_expression>

<postfix_expression> ::= <primary_expression>
                       | <postfix_expression> "++"
                       | <postfix_expression> "--"
                       | <postfix_expression> "(" [<argument_list>] ")"
                       | <postfix_expression> "." <identifier>
                       | <postfix_expression> "[" <expression> "]"
                       | <postfix_expression> "?." <identifier>

<argument_list> ::= <expression> ("," <expression>)*

<primary_expression> ::= <identifier>
                       | <literal>
                       | "(" <expression> ")"
                       | <array_expression>
                       | <object_expression>
                       | <arrow_function>
                       | <reactive_call>

<reactive_call> ::= "$state" "(" <expression> ")"
                  | "$derived" "(" <expression> ")"

<array_expression> ::= "[" [<expression_list>] "]"

<expression_list> ::= <expression> ("," <expression>)*

<object_expression> ::= "{" [<property_list>] "}"

<property_list> ::= <property> ("," <property>)*

<property> ::= <identifier> ":" <expression>
             | <identifier>
             | "..." <expression>

<arrow_function> ::= "(" [<parameter_list>] ")" "=>" <function_body>

(* Literals *)
<literal> ::= <string_literal>
            | <number_literal>
            | <boolean_literal>
            | <null_literal>
            | <undefined_literal>

<string_literal> ::= '"' <char>* '"' | "'" <char>* "'" | "`" <template_char>* "`"

<number_literal> ::= <digit>+ ["." <digit>+]

<boolean_literal> ::= "true" | "false"

<null_literal> ::= "null"

<undefined_literal> ::= "undefined"

(* Ejemplos:
   let count = $state(0)

   @server
   let fetchData = async () => {
       return await db.query()
   }

   @client
   let handleClick = () => count++

   @effect
   loadData() {
       console.log("Mounted")
   }
*)
```

---

## 5. Styles (Utilidades)

```ebnf
<styles> ::= <utility_declaration>*

<utility_declaration> ::= "@utility" <identifier> "{" <utility_body> "}"

<utility_body> ::= <utility_token>*

<utility_token> ::= <identifier>
                  | <utility_property>
                  | <interpolated_utility>

<utility_property> ::= <property_name> ":" <property_value>

<property_name> ::= <identifier>
                  | <state_prefix> ":" <identifier>
                  | <breakpoint_prefix> ":" <identifier>
                  | <breakpoint_prefix> ":" <state_prefix> ":" <identifier>

<state_prefix> ::= "hover" | "focus" | "active" | "disabled" | "visited" | "dark"

<breakpoint_prefix> ::= "sm" | "md" | "lg" | "xl" | "2xl"

<property_value> ::= <identifier>
                   | <number>
                   | <identifier> "-" <number>

<interpolated_utility> ::= <property_name> ":" "{" <identifier> "}"

(* Ejemplos:
   @utility
   btn {
       px:4 py:2
       bg:blue-500
       hover:bg:blue-700
       rounded:md
   }

   @utility
   responsive {
       grid-cols:1
       md:grid-cols:3
       lg:grid-cols:4
   }

   @utility
   themed {
       bg:white
       dark:bg:gray-900
   }
*)
```

---

## 6. Template (HTML-like)

```ebnf
<template> ::= <template_node>+

<template_node> ::= <element>
                  | <text_node>
                  | <expression_node>
                  | <comment_node>

(* Elements *)
<element> ::= "<" <tag_name> <attributes> <directives> ["/"] ">"
            | "<" <tag_name> <attributes> <directives> ">" <template_node>* "</" <tag_name> ">"

<tag_name> ::= <identifier>
             | <identifier> "-" <identifier>

(* Attributes (usan =) *)
<attributes> ::= <attribute>*

<attribute> ::= <identifier> "=" <attribute_value>
              | <identifier> "=" "{" <expression> "}"
              | <identifier>

<attribute_value> ::= <string_literal>

(* Directives (usan :) *)
<directives> ::= <directive>*

<directive> ::= <class_directive>
              | <event_directive>
              | <bind_directive>
              | <slot_directive>
              | <ref_directive>

<class_directive> ::= "class" ":" "(" <class_content> ")"

<class_content> ::= <utility_tokens>
                  | <utility_tokens> "{" <expression> "}"

<utility_tokens> ::= <utility_token>+

<event_directive> ::= "on" ":" <event_name> [<event_modifiers>] "=" "{" <expression> "}"

<event_name> ::= <identifier>

<event_modifiers> ::= ("|" <event_modifier>)+

<event_modifier> ::= "preventDefault"
                   | "stopPropagation"
                   | "self"
                   | "once"
                   | "capture"
                   | "passive"
                   | <key_modifier>

<key_modifier> ::= "enter" | "escape" | "space" | "tab"
                 | "up" | "down" | "left" | "right"
                 | "ctrl" | "shift" | "alt" | "meta"

<bind_directive> ::= "bind" ":" <bind_target> "=" "{" <identifier> "}"

<bind_target> ::= "value" | "checked" | "group"

<slot_directive> ::= "slot" ":" <identifier>

<ref_directive> ::= "ref" "=" "{" <identifier> "}"

(* Slots *)
<element> ::= "<slot" [<slot_name>] ["/"] ">"
            | "<slot" [<slot_name>] ">" <template_node>* "</slot>"

<slot_name> ::= ":" <identifier>

(* Text Node *)
<text_node> ::= <char>+

(* Expression Node *)
<expression_node> ::= "{" <expression> "}"

(* Comment Node *)
<comment_node> ::= "{" "/*" <char>* "*/" "}"

(* Ejemplos:
   <div class:(container px:4)>
       <h1>{title}</h1>
   </div>

   <button on:click={handleClick} class:(btn_primary)>
       Click me
   </button>

   <input bind:value={searchQuery} />

   <Card title="Hello">
       <p slot:footer>Footer content</p>
   </Card>

   {items.map(item => (
       <li key={item.id}>{item.name}</li>
   ))}
*)
```

---

## Tokens Léxicos

```ebnf
(* Keywords *)
<keyword> ::= "let" | "const" | "use" | "from" | "props" | "export"
            | "if" | "else" | "return" | "async" | "await"
            | "true" | "false" | "null" | "undefined"
            | "typeof" | "theme"

(* Decorators *)
<decorator_keyword> ::= "@route" | "@layout" | "@server" | "@client"
                      | "@utility" | "@effect"

(* Identifiers *)
<identifier> ::= <letter> (<letter> | <digit> | "_")*

<letter> ::= "a".."z" | "A".."Z"

<digit> ::= "0".."9"

(* Operators *)
<operator> ::= "+" | "-" | "*" | "/" | "%"
             | "=" | "==" | "!=" | "===" | "!=="
             | "<" | ">" | "<=" | ">="
             | "&&" | "||" | "!"
             | "++" | "--" | "+=" | "-=" | "*=" | "/="
             | "=>" | "?." | "..."

(* Delimiters *)
<delimiter> ::= "(" | ")" | "{" | "}" | "[" | "]"
              | "," | ";" | ":" | "."
              | "<" | ">" | "</" | "/>"

(* Whitespace *)
<whitespace> ::= " " | "\t" | "\n" | "\r"

(* Comments *)
<line_comment> ::= "//" <char>* "\n"

<block_comment> ::= "/*" <char>* "*/"
```

---

## Ejemplos Completos

### Ejemplo 1: Componente Simple

```artic
@route("/hello")

let name = $state("World")

<div class:(p:4)>
    <h1>Hello, {name}!</h1>
</div>
```

**Árbol de Parsing:**
```
Component
├── metadata: RouteDecorator("/hello")
├── logic:
│   └── VariableDeclaration("name", ReactiveCall("$state", ["World"]))
└── template:
    └── Element("div")
        ├── directives: [ClassDirective("p:4")]
        └── children:
            └── Element("h1")
                └── children:
                    ├── TextNode("Hello, ")
                    ├── ExpressionNode(Identifier("name"))
                    └── TextNode("!")
```

### Ejemplo 2: Componente Interactivo

```artic
use { Card } from "./components"

props {
    initialCount: number = 0
}

let count = $state(props.initialCount)

@client
let increment = () => count++

@utility
btn {
    px:4 py:2
    bg:blue-600
    hover:bg:blue-700
    rounded:md
}

<Card title="Counter">
    <p>Count: {count}</p>
    <button on:click={increment} class:(btn)>
        Increment
    </button>
</Card>
```

**Árbol de Parsing:**
```
Component
├── imports:
│   └── NamedImport(["Card"], "./components")
├── props:
│   └── PropDefinition("initialCount", NumberType, Literal(0))
├── logic:
│   ├── VariableDeclaration("count", ReactiveCall("$state", [MemberExpression("props.initialCount")]))
│   └── FunctionDeclaration("increment", ClientDecorator, ArrowFunction([], UnaryExpression("++")))
├── styles:
│   └── UtilityDeclaration("btn", [
│       UtilityProperty("px", "4"),
│       UtilityProperty("py", "2"),
│       ...
│   ])
└── template:
    └── Element("Card")
        ├── attributes: [Attribute("title", "Counter")]
        └── children:
            ├── Element("p")
            │   └── children: [TextNode("Count: "), ExpressionNode(...)]
            └── Element("button")
                ├── directives: [
                │   EventDirective("click", Identifier("increment")),
                │   ClassDirective("btn")
                │ ]
                └── children: [TextNode("Increment")]
```

### Ejemplo 3: Servidor + Cliente (RPC)

```artic
@route("/users")

let users = $state([])
let loading = $state(true)

@server
let fetchUsers = async () => {
    return await db.users.findMany()
}

@client
let loadUsers = async () => {
    loading = true
    users = await fetchUsers()
    loading = false
}

@effect
onMount() {
    loadUsers()
}

<div class:(container)>
    {loading ? (
        <p>Loading...</p>
    ) : (
        <ul>
            {users.map(user => (
                <li key={user.id}>{user.name}</li>
            ))}
        </ul>
    )}
</div>
```

---

## Precedencia de Operadores

De mayor a menor precedencia:

1. **Postfix**: `()`, `.`, `[]`, `++`, `--`
2. **Unary**: `!`, `-`, `+`, `++`, `--`, `typeof`, `await`
3. **Multiplicative**: `*`, `/`, `%`
4. **Additive**: `+`, `-`
5. **Relational**: `<`, `>`, `<=`, `>=`
6. **Equality**: `==`, `!=`, `===`, `!==`
7. **Logical AND**: `&&`
8. **Logical OR**: `||`
9. **Ternary**: `? :`
10. **Assignment**: `=`, `+=`, `-=`, `*=`, `/=`

---

## Asociatividad

- **Left-to-right**: Operadores binarios (`+`, `-`, `*`, etc.)
- **Right-to-left**: Operadores unarios (`!`, `-`), asignaciones (`=`)

---

## Reglas de Whitespace

- Whitespace es **insignificante** excepto para separar tokens
- Saltos de línea son opcionales en la mayoría de contextos
- Indentación es libre (no afecta el parsing)

---

## Reglas de Punto y Coma

- Los puntos y coma son **opcionales**
- Se insertan automáticamente en final de línea cuando:
  - El siguiente token es `}`
  - El siguiente token no puede continuar la expresión
  - La línea contiene `return`, `break`, `continue`

---

## Ambigüedades Resueltas

### `:` vs `=`

```artic
class:(px:4)     // Directiva del compilador (procesa utilidades)
src="logo.png"   // Atributo HTML (valor literal)
```

**Regla:** Dentro de elementos HTML:
- `:` después del nombre del atributo → Directiva
- `=` después del nombre del atributo → Atributo/Prop

### `{}` en Templates

```artic
{count}           // Expression node
{/* comment */}   // Comment node
{{key: value}}    // Object inside expression
```

**Regla:**
- `{` seguido de identificador o expresión → Expression node
- `{/*` → Comment node
- `{{` → Object literal dentro de expression

### Componentes vs HTML Tags

```artic
<div>             // HTML tag (lowercase)
<Card>            // Componente (PascalCase)
<custom-element>  // Web Component (kebab-case)
```

**Regla:**
- PascalCase → Componente Artic
- lowercase → HTML nativo
- kebab-case → Web Component

---

## Palabras Reservadas

```
let, const, use, from, props, export
if, else, return, async, await
true, false, null, undefined, typeof
theme, key
```

**Nota:** Los decoradores (`@route`, `@server`, etc.) son palabras clave contextuales que solo tienen significado en posiciones específicas.

---

## Extensiones Futuras

Esta gramática es para **Fase 1-2**. Extensiones planeadas:

### Fase 3: ORM
```ebnf
<orm_schema> ::= "@schema" <identifier> "{" <field_definition>* "}"
```

### Fase 4: Type Definitions
```ebnf
<type_alias> ::= "type" <identifier> "=" <type>
<interface> ::= "interface" <identifier> "{" <interface_body> "}"
```

### Fase 5: Error Boundaries
```ebnf
<error_boundary> ::= "<ErrorBoundary" <attributes> ">" <template_node>* "</ErrorBoundary>"
```

---

## Validación Semántica (No en Gramática)

Estas restricciones NO están en la gramática pero se validan en el Semantic Analyzer:

1. **Orden de bloques**: metadata → imports → props → logic → styles → template
2. **Variables declaradas antes de uso**
3. **Tipos compatibles** en asignaciones
4. **`@server` no usa APIs del navegador** (localStorage, window, etc.)
5. **`@client` no accede a base de datos**
6. **Props solo en componentes** sin `@route`
7. **Utilidades importadas** antes de uso
8. **Slots nombrados** deben existir en el componente

---

## Referencias

- **ECMA-262**: JavaScript Language Specification
- **HTML5**: HTML Living Standard
- **CSS**: CSS Syntax Module Level 3
- **TypeScript**: TypeScript Language Specification

---

**Versión:** 1.0.0
**Estado:** Completo para Fase 1-2
**Última actualización:** Octubre 2025