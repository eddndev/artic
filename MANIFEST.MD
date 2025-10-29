# Artic Manifest v3
### Un Lenguaje para el Desarrollo Web del Futuro

---

## 🎯 Visión

**Artic** (`.atc`) es un lenguaje de dominio específico (DSL) diseñado para **eliminar la fragmentación** en el desarrollo web full-stack. Nuestra misión es unificar la experiencia del desarrollador bajo un solo lenguaje, una sola sintaxis y una sola cadena de herramientas, mientras alcanzamos **rendimiento nativo** en todos los targets de compilación.

### El Problema Actual

El desarrollo web moderno obliga a los desarrolladores a dominar múltiples lenguajes, paradigmas y herramientas:

- **Backend:** Node.js, Python, PHP, Go, Rust
- **Frontend:** JavaScript, TypeScript, JSX/TSX
- **Estilos:** CSS, SCSS, Tailwind, CSS-in-JS
- **Tooling:** webpack, Vite, esbuild, tsc, babel
- **Runtime:** Intérpretes pesados, node_modules gigantes, cold starts

Esta fragmentación tiene costos:
- **Cambio de contexto mental** constante
- **Duplicación de lógica** entre frontend y backend
- **Performance overhead** de intérpretes y bundlers
- **Experiencia de desarrollador (DX)** inconsistente

### Nuestra Solución

**Un lenguaje. Una sintaxis. Dos targets.**

Artic compila un solo archivo fuente (`.atc`) a:

1. **Binario nativo de servidor** (C++ compilado) → Zero cold start, máximo throughput
2. **JavaScript/WebAssembly optimizado** (para el cliente) → Hydration rápida, lógica en Wasm cuando sea necesario

---

## 💎 Principios Fundamentales

### 1. **Sintaxis Unificada**
Una sola gramática para todo: lógica de servidor, lógica de cliente, templates, estilos y tipos.

### 2. **Rendimiento Nativo**
El compilador genera código de máquina real, no interpretado.

- **Backend:** Binario C++ compilado con GCC/Clang
- **Frontend:** JavaScript minimalista + Wasm para computación intensiva
- **Estilos:** CSS estático generado en tiempo de compilación

### 3. **Zero Runtime Dependencies**
No más `node_modules`, `vendor/`, ni intérpretes. El deploy de un backend Artic es un solo archivo binario.

### 4. **Developer Experience First**
Tooling de primera clase escrito en C++:

- **Compilador (`artic`):** Compilation times < 100ms
- **Language Server (LSP):** Autocompletado, navegación, refactoring
- **Dev Server:** Hot reload con latencia < 50ms

### 5. **Explícito sobre Implícito**
Los decoradores `@server`, `@client`, `@utility` dejan claro dónde se ejecuta cada pieza de código.

### 6. **Declarativo y Centrado en el Compilador**
El archivo `.atc` es una declaración pura que el compilador transforma. No hay runtime mágico, solo transformaciones deterministas.

---

## 📐 Definición de Sintaxis Formal

### Regla de Oro: `:` vs `=`

La distinción más importante en Artic:

> **`:` = Directiva del Compilador** (procesamiento especial)  
> **`=` = Asignación de Valor** (pasa el valor tal cual)

```artic
// ✅ Directivas del compilador (usan :)
class:(px:4 bg:blue-500)      // Procesa utilidades y genera CSS
on:click={handler}             // Genera addEventListener
bind:value={text}              // Genera two-way binding
slot:header                    // Routing de contenido

// ✅ Atributos HTML y props (usan =)
src="logo.png"                 // Atributo HTML estático
href={profileUrl}              // Atributo HTML dinámico
title={props.label}            // Prop de componente
disabled={isLoading}           // Atributo booleano
```

---

## 📂 Estructura Canónica del Fichero

Un fichero `.atc` **ES** el componente. El parser procesa el archivo de forma determinista, esperando estos bloques **en orden estricto**:

```artic
// 1️⃣ METADATA (Opcional)
@route("/path")
@layout("layout-name")

// 2️⃣ IMPORTS (Opcional)
use theme "./theme.atc"
use { Component } from "./components"

// 3️⃣ PROPS (Opcional, solo en componentes)
props {
    title: string,
    count?: number = 0
}

// 4️⃣ LOGIC (Opcional)
let state = $state(0)

@server
let fetchData = async () => { /* ... */ }

@client
let handleClick = () => { /* ... */ }

// 5️⃣ STYLES (Opcional)
@utility
btn {
    px:4 py:2 rounded:md
}

// 6️⃣ TEMPLATE (Requerido - DEBE SER EL ÚLTIMO)
<div class:(container)>
    {/* Contenido */}
</div>
```

---

## 1️⃣ Metadata: Decoradores de Fichero

Define **qué es** este componente.

### `@route(path: string)`

Define el fichero como una **Página** (en lugar de un componente reutilizable).

```artic
@route("/dashboard")                    // Ruta estática
@route("/blog/:slug")                   // Parámetros de ruta
@route("/users/:id/posts/:postId")     // Múltiples parámetros
```

**Comportamiento:**
- Los parámetros de ruta se inyectan automáticamente en `props`
- Solo páginas (con `@route`) se registran en el router del servidor
- Componentes sin `@route` son reutilizables

### `@layout(name: string)`

Envuelve la página con un componente de layout.

```artic
@route("/admin/users")
@layout("admin")  // Usa _layouts/admin.atc
```

El layout debe contener un `<slot />` donde se inyectará el contenido de la página.

---

## 2️⃣ Imports: Sistema `use`

### Importar Tema Global

```artic
use theme "./theme.atc"
```

Importa todas las utilidades exportadas del tema.

### Importar Componentes y Utilidades

```artic
// Componentes
use { Card, Button } from "./components"

// Utilidades de estilo
use { container, btn_primary } from "./theme"

// Mixto
use { Card, container } from "./ui"
```

### Importar APIs/Módulos

```artic
use api from "./api"
use db from "./database"
use { validateEmail } from "./validators"
```

---

## 3️⃣ Props: API del Componente

Define la interfaz pública de un componente **reutilizable** (sin `@route`).

```artic
props {
    // Requerido con tipo
    title: string,
    
    // Con valor por defecto
    variant: "primary" | "secondary" = "primary",
    
    // Opcional
    onClick?: () => void,
    
    // Tipos complejos
    user: {
        id: string,
        name: string,
        email: string
    },
    
    // Arrays
    items: string[],
    tags: Array<string>,
    
    // Funciones callback
    onSave: (data: any) => void,
    onDelete: (id: string) => Promise<void>,
    
    // Union types
    status: "loading" | "success" | "error",
    
    // Any para casos especiales
    metadata?: any
}
```

**Notas Importantes:**
- Los componentes con `@route` reciben `props` automáticamente desde los parámetros de ruta
- La composición de UI se hace con `<slot />`, NO con props
- Las páginas (con `@route`) NO deben declarar un bloque `props` explícito

---

## 4️⃣ Logic: El Script del Componente

### Declaración de Variables

```artic
// let = mutable
let count = 0
let name = "Eduardo"

// const = inmutable
const MAX_ITEMS = 100
const API_URL = "https://api.example.com"
```

### Estado Reactivo

```artic
// Syntax sugar con $
let count = $state(0)
let user = $state({ name: "", email: "" })
let items = $state([])

// Valores derivados
let doubled = $derived(count * 2)
let hasItems = $derived(items.length > 0)

// El compilador reescribe asignaciones para disparar reactividad
count++              // Dispara actualizaciones
user.name = "Juan"   // Dispara actualizaciones
items.push(newItem)  // Dispara actualizaciones
```

### Contexto de Servidor

```artic
@server
let loadUsers = async () => {
    // Este código SOLO existe en el binario de servidor
    // Acceso completo a: db, filesystem, env vars
    let users = await db.users.findMany()
    return users
}

@server
let deleteUser = async (id: string) => {
    await db.users.delete(id)
    return { success: true }
}

@server
let getCurrentTime = () => {
    // Funciones síncronas también son válidas
    return new Date().toISOString()
}
```

### Contexto de Cliente

```artic
@client
let handleClick = () => {
    // Este código SOLO existe en el bundle JS del cliente
    // Acceso a: DOM, localStorage, window
    console.log("Clicked!")
}

@client
let trackEvent = (name: string, data?: any) => {
    // Analytics, tracking, etc.
    window.analytics?.track(name, data)
}

@client
let saveToLocalStorage = (key: string, value: any) => {
    localStorage.setItem(key, JSON.stringify(value))
}
```

### Llamadas Cross-Boundary (RPC Automático)

El compilador genera automáticamente endpoints HTTP y llamadas fetch.

```artic
@server
let fetchData = async (filter: string) => {
    return await db.items.findMany({ filter })
}

@client
let refreshUI = async () => {
    // ✨ Magia: El compilador genera:
    // 1. POST /api/__rpc/fetchData en el servidor
    // 2. fetch() call en el cliente
    // 3. Serialización/deserialización automática
    
    let result = await fetchData("active")
    items = result
}
```

### Funciones Compartidas

Sin decorador = disponible en ambos lados.

```artic
// Esta función existe en servidor Y cliente
let formatDate = (date: Date) => {
    return date.toLocaleDateString("es-MX")
}

let validateEmail = (email: string) => {
    return /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email)
}

// El compilador incluye estas funciones en ambos targets
```

### Efectos y Ciclo de Vida

```artic
// Effect de mounting (se ejecuta una vez)
@effect
onMount() {
    console.log("Component mounted")
    loadInitialData()
}

// Effect reactivo (se ejecuta cuando cambian dependencias)
@effect
logCount() {
    console.log(`Count changed: ${count}`)
    // El compilador detecta que depende de 'count'
}

// Effect con dependencias múltiples
@effect
syncData() {
    if (user && token) {
        api.sync(user.id, token)
    }
    // Depende de 'user' y 'token'
}

// Effect con cleanup
@effect
setupTimer() {
    let timerId = setInterval(() => {
        count++
    }, 1000)
    
    // Cleanup automático cuando el componente se desmonta
    return () => {
        clearInterval(timerId)
    }
}
```

---

## 5️⃣ Styles: Sistema de Utilidades Unificado

Un lenguaje de tokens **tipo Tailwind**, tanto para definir como para consumir.

### Utilidades Estáticas

```artic
@utility
btn {
    px:4 py:2
    rounded:md
    font:semibold
    cursor:pointer
    transition:all
}

@utility
card {
    bg:white
    rounded:lg
    shadow:md
    p:6
}
```

### Composición de Utilidades

```artic
@utility
btn_primary {
    btn                  // ✅ Compone otra utilidad
    bg:blue-600
    text:white
    hover:bg:blue-700
}

@utility
card_interactive {
    card                 // Compone 'card'
    hover:shadow:xl
    hover:scale:105
    transition:transform
}
```

### Estados y Pseudo-clases

```artic
@utility
link {
    text:blue-600
    hover:text:blue-800    // :hover
    hover:underline
    active:text:blue-900   // :active
    visited:text:purple-600 // :visited
}

@utility
input {
    border border:gray-300
    focus:outline:none     // :focus
    focus:ring:2
    focus:ring:blue-500
    disabled:bg:gray-100   // :disabled
    disabled:cursor:not-allowed
}
```

### Responsive Breakpoints

```artic
@utility
grid {
    grid gap:4
    grid-cols:1          // Mobile por defecto
    sm:grid-cols:2       // ≥640px
    md:grid-cols:3       // ≥768px
    lg:grid-cols:4       // ≥1024px
    xl:grid-cols:6       // ≥1280px
}

@utility
text_responsive {
    text:base            // Mobile
    md:text:lg           // Tablet
    lg:text:xl           // Desktop
}
```

### Dark Mode

```artic
@utility
themed {
    bg:white text:gray-900
    dark:bg:gray-900 dark:text:white
    border border:gray-200
    dark:border:gray-700
}

@utility
card_themed {
    bg:white
    dark:bg:gray-800
    text:gray-900
    dark:text:gray-100
}
```

### Macros en Compile-Time

```artic
// Función que retorna string de utilidades
@utility
spacing(size: number) {
    return `p:${size * 4} m:${size * 2}`
}

// Uso: class:(spacing(2))
// Genera: p:8 m:4

@utility
colorScheme(color: string, intensity: number) {
    return `bg:${color}-${intensity} text:${color}-${intensity + 400}`
}

// Uso: class:(colorScheme("blue", 500))
// Genera: bg:blue-500 text:blue-900
```

### Exportar Utilidades

```artic
// En theme.atc
@utility
container { 
    w:full max-w:7xl mx:auto px:4 
}

@utility
btn_primary { 
    px:4 py:2 bg:blue-600 text:white 
}

// Exportar para uso en otros archivos
export { container, btn_primary }
```

---

## 6️⃣ Template: Sintaxis de Plantilla

La estructura de UI. **Debe ser el último bloque del fichero.**

### Elementos y Expresiones

```artic
// HTML nativo
<div class:(container)>
    <h1>Hello World</h1>
</div>

// Componentes custom
<Card title="My Card">
    <p>Content</p>
</Card>

// Expresiones JavaScript
<p>{count}</p>
<p>{count * 2}</p>
<p>{user.name.toUpperCase()}</p>
<p>{formatDate(new Date())}</p>

// Expresiones complejas
<p>{items.filter(i => i.active).length} items activos</p>
```

### Atributos HTML (con `=`)

```artic
// Atributos estáticos
<img src="logo.png" alt="Logo" />
<a href="/about">About</a>

// Atributos dinámicos
<img src={logoUrl} alt={logoAlt} />
<a href={`/users/${user.id}`}>Profile</a>

// Atributos booleanos
<button disabled={isLoading}>Submit</button>
<input checked={isChecked} />
<details open={isExpanded} />

// Atributos condicionales
<div 
    id={elementId}
    title={hasTooltip ? tooltipText : undefined}
    data-active={isActive}
>
```

### Props de Componentes (con `=`)

```artic
// Props simples
<Card title="Hello" />
<Button variant="primary" />

// Props dinámicos
<Card title={pageTitle} />
<Button variant={buttonVariant} />

// Props de funciones
<Button onClick={handleClick} />
<Form onSubmit={handleSubmit} />

// Props de objetos
<UserCard user={currentUser} />
<DataGrid 
    columns={columnDefs}
    data={items}
    config={{ sortable: true, paginated: false }}
/>

// Spread props
<Card {...cardProps} />
<Button {...buttonProps} className="extra" />
```

### Directiva `class:` (con `:`)

```artic
// Utilidades inline
<div class:(px:4 py:2 bg:white rounded:lg)>

// Utilidades locales
<div class:(btn_primary)>

// Condicional con expresiones JS
<div class:(base {isActive ? "active" : "inactive"})>

// Dinámico con interpolación
<div class:(bg:{color}-500 text:{size})>

// Todo mezclado
<div class:(
    px:4 rounded:lg
    {isActive ? "bg:blue-500" : "bg:gray-300"}
    {isLarge ? "text:2xl" : "text:base"}
    hover:shadow:xl
    dark:bg:gray-800
)>
```

### Eventos (con `:`)

```artic
// Evento básico
<button on:click={handleClick}>Click</button>

// Función inline
<button on:click={() => count++}>Increment</button>

// Con parámetros
<button on:click={(e) => handleClick(e, itemId)}>
    Delete
</button>

// Múltiples eventos
<input 
    on:input={handleInput}
    on:focus={handleFocus}
    on:blur={handleBlur}
/>
```

### Modificadores de Eventos (con `|`)

```artic
// preventDefault
<form on:submit|preventDefault={handleSubmit}>

// stopPropagation
<button on:click|stopPropagation={handleClick}>

// Múltiples modificadores (se encadenan)
<input 
    on:keydown|enter|preventDefault={handleEnter}
    on:keydown|escape={handleEscape}
/>

// Modificadores disponibles:
// - preventDefault: llama event.preventDefault()
// - stopPropagation: llama event.stopPropagation()
// - self: solo dispara si event.target === currentTarget
// - once: solo se ejecuta una vez
// - capture: usa capture phase
// - passive: mejora scroll performance
```

### Modificadores de Tecla

```artic
<input 
    on:keydown|enter={submitForm}
    on:keydown|escape={closeModal}
    on:keydown|space={toggleSelection}
    on:keydown|ctrl|s={saveDocument}
/>

// Teclas disponibles:
// - enter, escape, space, tab
// - up, down, left, right
// - ctrl, shift, alt, meta
```

### Binding de Datos (con `:`)

```artic
// Input de texto
<input bind:value={searchQuery} />

// Textarea
<textarea bind:value={description} />

// Checkbox (individual)
<input type="checkbox" bind:checked={isAdmin} />

// Radio buttons (con bind:group)
<input type="radio" bind:group={selectedOption} value="a" />
<input type="radio" bind:group={selectedOption} value="b" />
<input type="radio" bind:group={selectedOption} value="c" />

// Select
<select bind:value={selectedCity}>
    <option value="cdmx">Ciudad de México</option>
    <option value="gdl">Guadalajara</option>
    <option value="mty">Monterrey</option>
</select>

// Select múltiple
<select multiple bind:value={selectedItems}>
    <option value="a">Opción A</option>
    <option value="b">Opción B</option>
</select>

// Checkboxes múltiples (bind:group)
let selected = $state([])

<input type="checkbox" bind:group={selected} value="red" />
<input type="checkbox" bind:group={selected} value="blue" />
<input type="checkbox" bind:group={selected} value="green" />
```

### Composición con Slots (con `:`)

Los slots permiten inyectar contenido en componentes.

**Componente que define slots:**

```artic
// Card.atc
props { title: string }

<article class:(card)>
    <header class:(card-header)>
        <h2>{props.title}</h2>
        <slot:actions />  {/* Slot nombrado */}
    </header>
    
    <div class:(card-body)>
        <slot />  {/* Slot por defecto */}
    </div>
    
    <footer class:(card-footer)>
        <slot:footer />  {/* Slot nombrado */}
    </footer>
</article>
```

**Uso del componente:**

```artic
<Card title="Mi Tarjeta">
    {/* Contenido del slot por defecto */}
    <p>Este es el contenido principal</p>
    <p>Puede tener múltiples elementos</p>
    
    {/* Slot nombrado con directiva slot: */}
    <button slot:actions class:(btn)>
        Editar
    </button>
    
    {/* Otro slot nombrado */}
    <div slot:footer>
        <span>Actualizado hace 2h</span>
    </div>
</Card>
```

**Slots con fallback:**

```artic
// El contenido dentro de <slot:nombre> es el fallback
<div class:(card)>
    <slot:header>
        {/* Este contenido se muestra si no se provee slot:header */}
        <h2>Título por defecto</h2>
    </slot:header>
    
    <slot>
        {/* Contenido por defecto del slot principal */}
        <p>Sin contenido</p>
    </slot>
</div>
```

### Control de Flujo (JavaScript Nativo)

Artic NO tiene sintaxis especial de template. Usa JavaScript puro.

**Condicionales:**

```artic
// Ternario básico
{loading ? <Spinner /> : <DataGrid data={items} />}

// Cortocircuito (&&)
{error && <ErrorAlert message={error} />}
{user && <Welcome name={user.name} />}

// Negación (!!)
{!isLoading && <Content />}

// Condicionales múltiples
{
    status === "loading" ? <Spinner /> :
    status === "error" ? <Error message={errorMsg} /> :
    status === "empty" ? <EmptyState /> :
    <DataGrid data={items} />
}

// If-else en variables
let content = loading ? <Spinner /> : <Content />

<div class:(container)>
    {content}
</div>
```

**Bucles:**

```artic
// map() básico
{items.map(item => (
    <li key={item.id}>
        {item.name}
    </li>
))}

// map() con componentes
{users.map(user => (
    <UserCard key={user.id} user={user} />
))}

// map() con destructuring
{posts.map(({ id, title, author }) => (
    <article key={id}>
        <h2>{title}</h2>
        <p>By {author}</p>
    </article>
))}

// filter() + map()
{posts
    .filter(p => p.published)
    .map(post => (
        <PostCard key={post.id} post={post} />
    ))
}

// map() con index
{items.map((item, index) => (
    <div key={item.id}>
        {index + 1}. {item.name}
    </div>
))}
```

**⚠️ IMPORTANTE:** Siempre usa `key={uniqueId}` en elementos repetidos.

---

## 🚀 Ejemplo Completo: Mini Blog

### `theme.atc` - Tema Global

```artic
// src/theme.atc

@utility
container {
    w:full
    max-w:4xl
    mx:auto
    px:4
}

@utility
btn {
    px:4 py:2
    rounded:md
    font:semibold
    cursor:pointer
    transition:all
}

@utility
btn_primary {
    btn
    bg:blue-600
    text:white
    hover:bg:blue-700
    active:scale:95
}

@utility
btn_secondary {
    btn
    bg:gray-200
    text:gray-900
    hover:bg:gray-300
}

@utility
input_base {
    w:full
    px:3 py:2
    border border:gray-300
    rounded:md
    focus:outline:none
    focus:ring:2
    focus:ring:blue-500
}

export { container, btn_primary, btn_secondary, input_base }
```

### `Card.atc` - Componente Reutilizable

```artic
// src/components/Card.atc

props {
    title: string
}

@utility
card {
    bg:white
    rounded:lg
    shadow:md
    overflow:hidden
    dark:bg:gray-800
}

@utility
header {
    p:4
    border-b border:gray-200
    dark:border:gray-700
}

@utility
body {
    p:4
}

@utility
footer {
    p:4
    bg:gray-50
    dark:bg:gray-900
}

<article class:(card)>
    <header class:(header)>
        <h2 class:(font:bold text:lg)>
            {props.title}
        </h2>
    </header>
    
    <div class:(body)>
        <slot />
    </div>
    
    <footer class:(footer)>
        <slot:footer />
    </footer>
</article>
```

### `Blog.atc` - Página Principal

```artic
// src/pages/Blog.atc

@route("/blog")

use theme "../theme.atc"
use { Card } from "../components/Card"

let posts = $state([])
let query = $state("")
let loading = $state(true)

@server
let loadPosts = async (search: string) => {
    let allPosts = await db.posts.findMany()
    
    if (!search) return allPosts
    
    return allPosts.filter(p => 
        p.title.toLowerCase().includes(search.toLowerCase())
    )
}

@client
let handleClear = () => {
    query = ""
}

@effect
searchPosts() {
    loading = true
    loadPosts(query).then(data => {
        posts = data
        loading = false
    })
}

<div class:(container py:8)>
    <h1 class:(text:3xl font:bold mb:6)>
        Blog de Artic
    </h1>
    
    <div class:(flex gap:2 mb:6)>
        <input 
            class:(input_base)
            placeholder="Buscar posts..."
            bind:value={query}
        />
        <button 
            class:(btn_secondary)
            on:click={handleClear}>
            Limpiar
        </button>
    </div>
    
    {loading ? (
        <div class:(text:center py:8)>
            <p>Cargando...</p>
        </div>
    ) : posts.length === 0 ? (
        <div class:(text:center py:8)>
            <p>No se encontraron posts</p>
        </div>
    ) : (
        <div class:(grid gap:4)>
            {posts.map(post => (
                <Card key={post.id} title={post.title}>
                    <p class:(text:gray-700)>
                        {post.excerpt}
                    </p>
                    
                    <a 
                        href={`/blog/${post.slug}`}
                        slot:footer
                        class:(text:blue-600 hover:underline)>
                        Leer más →
                    </a>
                </Card>
            ))}
        </div>
    )}
</div>
```

---

## 🗺️ Roadmap de Implementación

### Fase 1: Generador Estático (SSG)
**Objetivo:** Compilar `.atc` → HTML + CSS estáticos

**Tareas:**
- [ ] Definir gramática completa (EBNF)
- [ ] Implementar Lexer (Tokenización)
- [ ] Implementar Parser (AST)
- [ ] HtmlGenerator (JSX → HTML)
- [ ] CssGenerator (utilidades → CSS)
- [ ] CLI básico: `artic build`

**Entregable:** Landing pages estáticas ultrarrápidas

### Fase 2: Reactividad (Cliente Interactivo)
**Objetivo:** Añadir JavaScript e interactividad

**Tareas:**
- [ ] Extender gramática (eventos, estado, effects)
- [ ] ClientScriptGenerator (Artic → JS)
- [ ] Runtime de hydration (`runtime.js`)
- [ ] Sistema de reactividad (signals)
- [ ] Event handlers y modificadores
- [ ] Two-way binding

**Entregable:** Apps web interactivas

### Fase 3: Servidor Nativo
**Objetivo:** Backend en binario C++

**Tareas:**
- [ ] ServerBinaryGenerator (Artic → C++)
- [ ] Runtime de servidor (HTTP server en C++)
- [ ] Sistema de routing
- [ ] RPC automático (cliente ↔ servidor)
- [ ] Manejo de requests/responses
- [ ] Integración con librerías HTTP (Boost.Asio/oatpp)

**Entregable:** Full-stack app con servidor nativo

### Fase 4: Capa de Datos (ORM)
**Objetivo:** Abstraer acceso a bases de datos

**Tareas:**
- [ ] Definir sintaxis de ORM
- [ ] Query builder type-safe
- [ ] Generador de SQL
- [ ] Integración con PostgreSQL/MySQL
- [ ] Migraciones declarativas
- [ ] Relaciones y joins

**Entregable:** ORM integrado y type-safe

### Fase 5: Tooling Avanzado
**Objetivo:** DX de primera clase

**Tareas:**
- [ ] Language Server Protocol (LSP)
- [ ] Autocompletado inteligente
- [ ] Navegación de código
- [ ] Refactoring automático
- [ ] Linting y formatting
- [ ] Dev server con HMR
- [ ] Plugin de VS Code

**Entregable:** Tooling comparable a TypeScript

### Fase 6: WebAssembly
**Objetivo:** Performance extremo en cliente

**Tareas:**
- [ ] Compilador Artic → C++ → Wasm
- [ ] Bridge JS ↔ Wasm
- [ ] Optimizaciones de bundle size
- [ ] Threading (Web Workers)
- [ ] SIMD optimizations

**Entregable:** Computación pesada en Wasm

---

## 🛠️ Stack Tecnológico

### Compilador (`artic`)
- **Lenguaje:** C++20
- **Parser:** Recursive Descent (manual)
- **AST:** Estructura custom optimizada
- **Backends:** 
  - ServerBinaryGenerator (C++)
  - ClientScriptGenerator (JS)
  - CssGenerator (CSS)
- **Build System:** CMake
- **Testing:** Google Test

### Runtime de Servidor
- **Lenguaje:** C++20
- **HTTP:** Boost.Asio / oatpp / drogon
- **Event Loop:** Asíncrono
- **Output:** Binario estático
- **Tamaño:** ~2-5 MB (con todas las libs)

### Runtime de Cliente
- **Core:** `runtime.js` (~5-8KB gzipped)
- **Reactivity:** Fine-grained signals
- **Hydration:** Virtual DOM minimal
- **Wasm Bridge:** JS ↔ WebAssembly

### Tooling
- **LSP:** C++ (reutiliza parser)
- **Dev Server:** C++ con WebSocket
- **CLI:** `artic` (build, dev, create)
- **Package Manager:** Compatible con npm

---

## 🎯 Comparación con Alternativas

| Feature | **Artic** | Next.js | SvelteKit | Solid.js | Go+HTMX |
|---------|-----------|---------|-----------|----------|---------|
| **Lenguaje** | Uno (.atc) | JS+TS+JSX+CSS | JS+Svelte+CSS | JS+JSX+CSS | 2 lenguajes |
| **Backend** | Binario C++ | Node.js | Node.js | Node.js | Binario Go |
| **Cold Start** | ~1ms | ~100ms | ~100ms | ~100ms | ~2ms |
| **Sintaxis** | Una | Múltiples | Dos | Dos | Dos |
| **Reactividad** | Signals | React Hooks | Stores | Signals | Ninguna |
| **Estilos** | Integrados | Separados | Separados | Separados | Separados |
| **Deploy** | 1 binario | Node + deps | Node + deps | Node + deps | 1 binario |
| **Bundle** | ~10KB | ~80KB | ~30KB | ~20KB | ~5KB |
| **Type Safety** | Nativo | TypeScript | TypeScript | TypeScript | Opcional |
| **LSP** | Nativo | TS Server | Svelte LSP | TS Server | Go+TS |

---

## 🔍 Análisis Exhaustivo de Lagunas

### 🔴 Críticas (Fase 1-3) - Bloquean MVP

#### 1. Sistema de Tipos Completo
**Estado:** ❌ No definido

```artic
// Necesitamos definir:
type User = {
    id: string,
    name: string,
    role: "admin" | "user"
}

interface Pageable {
    page: number,
    pageSize: number
}

// Generics
type Result<T> = {
    data: T,
    error?: string
}

// Type guards
let isUser = (obj: any): obj is User => {
    return obj && typeof obj.id === "string"
}
```

**Prioridad:** 🔥 ALTA

#### 2. Manejo de Errores
**Estado:** ❌ No definido

```artic
// Try-catch en funciones
@server
let riskyOperation = async () => {
    try {
        return await db.query()
    } catch (error) {
        return { error: error.message }
    }
}

// Error boundaries en templates
<ErrorBoundary fallback={<ErrorPage />}>
    <RiskyComponent />
</ErrorBoundary>

// Páginas de error
// ¿Cómo se definen 404.atc, 500.atc?
```

**Prioridad:** 🔥 ALTA

#### 3. Routing Avanzado
**Estado:** ⚠️ Parcial (solo rutas simples)

```artic
// Nested routes
@route("/dashboard")
// ¿Cómo se anidan /dashboard/users y /dashboard/settings?

// Route guards
@route("/admin")
@guard("requireAuth")  // ¿Sintaxis?
@guard((req) => req.user?.isAdmin)

// Redirects
@route("/old-path")
@redirect("/new-path")

// Catch-all routes
@route("/docs/*")  // Captura /docs/anything/here

// Query parameters
// ¿Cómo se accede a ?search=foo&page=2?
```

**Prioridad:** 🔥 ALTA

#### 4. SEO y Meta Tags
**Estado:** ❌ No definido

```artic
// ¿Cómo se define el <head>?
@route("/blog/:slug")
@meta({
    title: "Blog Post",
    description: "...",
    ogImage: "/og.png"
})

// O dentro del template?
<Head>
    <title>{post.title}</title>
    <meta name="description" content={post.excerpt} />
</Head>
```

**Prioridad:** 🔥 ALTA

#### 5. Environment Variables
**Estado:** ❌ No definido

```artic
// ¿Cómo se accede a env vars?
const API_KEY = process.env.API_KEY
const DB_URL = env.DATABASE_URL

// Build-time vs Runtime
const STATIC_VAR = env.PUBLIC_URL  // Compile-time
const DYNAMIC_VAR = env.USER_TOKEN // Runtime (solo servidor)
```

**Prioridad:** 🔥 ALTA

#### 6. Comentarios en el Código
**Estado:** ❌ No especificado

```artic
// ¿Comentarios de línea?
/* Comentarios de bloque? */
{/* JSX comments? */}

// ¿JSDoc para documentación?
/**
 * Loads user data
 * @param id User ID
 * @returns User object
 */
@server
let loadUser = async (id: string) => { ... }
```

**Prioridad:** 🟡 MEDIA

### 🟡 Importantes (Fase 4-5) - Mejoran DX

#### 7. Formularios y Validación
**Estado:** ❌ No definido

```artic
// Validación declarativa
props {
    email: string @validate("email"),
    age: number @validate("min:18, max:100")
}

// Form state
let form = $form({
    email: "",
    password: ""
})

// Validaciones custom
let validatePassword = (pwd: string) => {
    return pwd.length >= 8
}
```

**Prioridad:** 🟡 MEDIA

#### 8. Referencias al DOM
**Estado:** ❌ No definido

```artic
// ¿Sintaxis de refs?
let inputRef = $ref()

@client
let focusInput = () => {
    inputRef.current?.focus()
}

<input ref={inputRef} />
```

**Prioridad:** 🟡 MEDIA

#### 9. Context/Dependency Injection
**Estado:** ❌ No definido

```artic
// Provider de contexto global
@provide("theme", { mode: "dark" })

// Consumer
@inject("theme")
let theme = useContext("theme")
```

**Prioridad:** 🟡 MEDIA

#### 10. Assets Estáticos
**Estado:** ❌ No definido

```artic
// ¿Cómo se importan imágenes?
use logo from "./assets/logo.png"
use styles from "./assets/main.css"

<img src={logo} />

// ¿Optimización automática?
// ¿CDN integration?
```

**Prioridad:** 🟡 MEDIA

#### 11. Scope de Variables
**Estado:** ⚠️ Ambiguo

```artic
// ¿Las variables en Logic son globales al archivo?
let count = 0  // ¿Scope de archivo?

@server
let data = []  // ¿Solo existe en servidor?

@client
let localVar = 0  // ¿Solo en cliente?

// ¿Qué pasa si uso 'count' en una función @server?
```

**Prioridad:** 🔥 ALTA (aclaración necesaria)

#### 12. Sintaxis JavaScript Moderna
**Estado:** ⚠️ No especificado qué soportamos

```artic
// ¿Soportamos destructuring?
let { name, email } = user
let [first, ...rest] = items

// ¿Spread operators?
let newUser = { ...user, age: 30 }

// ¿Optional chaining?
let value = user?.address?.city

// ¿Nullish coalescing?
let name = user.name ?? "Anonymous"

// ¿Template literals?
let msg = `Hello ${name}`

// ¿Arrow functions?
let double = (x) => x * 2

// ¿Async/await en @client?
@client
let fetchData = async () => {
    let res = await fetch("/api/data")
    return res.json()
}
```

**Prioridad:** 🔥 ALTA (especificar qué soportamos)

### 🟢 Deseables (Fase 6+) - Futuro

#### 13. Internacionalización (i18n)
```artic
use { t } from "./i18n"

<h1>{t("welcome")}</h1>
<p>{t("items_count", { count: items.length })}</p>
```

#### 14. Animaciones y Transiciones
```artic
<div transition:fade>...</div>
<div animate:slide>...</div>
```

#### 15. Testing
```artic
// Unit tests para funciones
test("suma correctamente", () => {
    expect(sum(2, 3)).toBe(5)
})

// Component tests
test("renderiza correctamente", () => {
    let { getByText } = render(<Button label="Click" />)
    expect(getByText("Click")).toBeInTheDocument()
})
```

#### 16. Portales
```artic
<Portal target="body">
    <Modal />
</Portal>
```

#### 17. WebSocket
```artic
@server
@websocket("/ws")
let handleConnection = (socket) => {
    socket.on("message", (data) => {
        socket.broadcast(data)
    })
}
```

#### 18. Middleware
```artic
@middleware
let authenticate = (req, res, next) => {
    if (!req.user) {
        return res.redirect("/login")
    }
    next()
}
```

---

## 🎓 Filosofía de Diseño

### 1. Principio de Menor Sorpresa
La sintaxis debe ser predecible. Si aprendiste una parte de Artic, el resto se siente familiar.

### 2. Explícito > Implícito
Los decoradores hacen obvio qué código va dónde. No hay "magia negra".

### 3. Zero-Cost Abstractions
Las abstracciones de alto nivel no deben tener costo en runtime. El compilador optimiza todo.

### 4. Compilador como Aliado
El compilador no es un obstáculo, es tu mejor herramienta. Errores claros, sugerencias útiles, optimizaciones automáticas.

### 5. JavaScript como Target, No como Dependencia
Artic compila A JavaScript, no depende DE JavaScript. No heredamos sus limitaciones.

### 6. Consistencia Sintáctica
`:` para directivas del compilador, `=` para valores. Sin excepciones.

---

## 🚀 Casos de Uso

### Landing Pages
```artic
@route("/")

<section class:(hero bg:gradient py:20 text:center)>
    <h1 class:(text:6xl font:extrabold)>
        Bienvenido a Artic
    </h1>
</section>
```

**Ventaja:** SSG puro, ~2KB de JS, perfecto Lighthouse score

### Dashboards
```artic
let data = $state([])

@effect
loadData() {
    api.getMetrics().then(d => data = d)
}

<Dashboard data={data} />
```

**Ventaja:** Reactividad fine-grained, actualizaciones quirúrgicas

### E-commerce
```artic
@route("/products/:id")

@server
let product = await db.products.findOne(props.id)

<ProductPage product={product} />
```

**Ventaja:** SSR nativo, SEO perfecto, < 10ms response

### APIs REST
```artic
@route("/api/users/:id")

@server
let handler = async (req) => {
    let user = await db.users.findOne(req.params.id)
    return { status: 200, body: user }
}
```

**Ventaja:** Binario autoejecutable, latencia mínima

---

## 🌟 Misión

Hacer que el desarrollo web sea:

1. **Más rápido** (para desarrolladores y usuarios)
2. **Más simple** (una herramienta, no un ecosistema)
3. **Más poderoso** (rendimiento nativo sin complejidad)
4. **Más consistente** (una sintaxis para todo)

---

## 📜 Licencia y Comunidad

- **Licencia:** MIT (open source desde el día 1)
- **Repositorio:** `github.com/achronyme/artic` (TBD)
- **Documentación:** `artic-lang.org` (TBD)
- **Discord:** Comunidad en construcción

---

**Creado por Eduardo y la comunidad Achronyme**  
*Versión 3.0.0 - Octubre 2025*