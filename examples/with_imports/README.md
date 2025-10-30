# Component Import Example

Este ejemplo demuestra el sistema de importación de componentes de Artic (Fase 2).

## Archivos

- `Card.atc` - Componente reutilizable con props y slots
- `App.atc` - Aplicación que importa y usa el componente Card

## Características demostradas

1. **Props**: El componente Card acepta `title` (requerido) y `subtitle` (opcional)
2. **Slots**:
   - Slot por defecto para el contenido principal
   - Slot nombrado `slot:footer` con contenido de respaldo
3. **Imports**: App.atc importa Card usando `use { Card } from "./Card"`
4. **Component Registry**: El compilador carga automáticamente todos los componentes importados
5. **Convención de nombres**: Sigue el estándar del MANIFEST
   - Componentes: `PascalCase` (Card, Button)
   - Props: `camelCase` (title, subtitle)
   - Utilidades: `snake_case` (card_title, page_title)
   - Propiedades CSS: `kebab-case` (border-color, max-w)

## Cómo compilar

```bash
# Desde el directorio raíz de artic
artic.exe build:html examples/with_imports/App.atc
```

Esto generará:
- `build/index.html` - HTML con los componentes Card expandidos
- `build/style.css` - CSS con todas las utilidades combinadas

## Comportamiento esperado

El HtmlGenerator debería:
1. Detectar `<Card>` como componente custom (empieza con mayúscula)
2. Buscar el componente en el ComponentRegistry
3. Expandir el template del componente Card
4. (Fase 2 completa) Reemplazar los slots con el contenido proporcionado
5. (Fase 2 completa) Pasar las props a las expresiones dentro del componente

**Nota**: En la implementación actual (Fase 2 parcial), los componentes se expanden pero los slots y props aún no se reemplazan completamente. Se renderizan como comentarios HTML para indicar su posición.
