# Artic Language Support for VS Code

Soporte oficial de sintaxis para el lenguaje Artic (`.atc`).

## Características

Esta extensión proporciona:

- ✅ **Syntax Highlighting completo** para archivos `.atc`
- ✅ **Decoradores**: `@route`, `@server`, `@client`, `@utility`, `@effect`, etc.
- ✅ **Directivas**: `class:`, `on:`, `bind:`, `slot:`
- ✅ **Templates JSX/HTML**
- ✅ **Utilidades de estilo** (tokens tipo Tailwind)
- ✅ **Auto-cierre de brackets** y paréntesis
- ✅ **Comentarios**: `//` y `/* */`
- ✅ **Indentación automática**

## Elementos resaltados

### Decoradores
```artic
@route("/dashboard")
@server
@client
@utility
@effect
```

### Imports
```artic
use theme "./theme.atc"
use { Card, Button } from "./components"
```

### Props
```artic
props {
    title: string,
    count?: number = 0
}
```

### Lógica
```artic
let state = $state(0)
let derived = $derived(count * 2)

@server
let fetchData = async () => { ... }

@client
let handleClick = () => { ... }
```

### Utilidades
```artic
@utility
btn {
    px:4 py:2
    rounded:md
    bg:blue-600
    hover:bg:blue-700
}
```

### Templates
```artic
<div class:(container px:4 bg:white)>
    <button on:click={handleClick}>
        {count}
    </button>
    <input bind:value={text} />
    <slot:header />
</div>
```

## Instalación

### Desde el código fuente

1. Copia la carpeta `tools/vscode` a tu directorio de extensiones de VS Code:
   - **Windows**: `%USERPROFILE%\.vscode\extensions\artic-language`
   - **macOS/Linux**: `~/.vscode/extensions/artic-language`

2. Recarga VS Code (`Ctrl+Shift+P` → "Reload Window")

3. Abre cualquier archivo `.atc` y verás el resaltado de sintaxis

### Desarrollo

Para trabajar en el desarrollo de esta extensión:

1. Abre esta carpeta en VS Code
2. Presiona `F5` para abrir una nueva ventana con la extensión cargada
3. Abre un archivo `.atc` para probar los cambios

## Estructura de archivos

```
tools/vscode/
├── package.json              # Configuración de la extensión
├── language-configuration.json  # Configuración de brackets, comentarios, etc.
├── syntaxes/
│   └── artic.tmLanguage.json   # Gramática TextMate
└── README.md                 # Este archivo
```

## Contribuir

Para mejorar el resaltado de sintaxis:

1. Edita `syntaxes/artic.tmLanguage.json`
2. Prueba los cambios presionando `F5`
3. Verifica que todos los elementos se resalten correctamente

## Scopes disponibles

La extensión define los siguientes scopes para personalización:

- `storage.type.decorator.artic` - Decoradores (@route, @server, etc.)
- `keyword.control.import.artic` - Palabra clave `use`
- `keyword.other.props.artic` - Bloque `props`
- `entity.name.function.utility.artic` - Nombres de utilidades
- `entity.other.utility-token.artic` - Tokens de utilidades (px, py, etc.)
- `entity.other.attribute-name.artic` - Atributos HTML/JSX
- `keyword.operator.directive.artic` - Operador `:` en directivas
- `support.type.event-name.artic` - Nombres de eventos (click, input, etc.)
- `support.function.artic` - Funciones especiales ($state, $derived, etc.)

## Licencia

MIT

---

**Creado por Eduardo y la comunidad Achronyme**