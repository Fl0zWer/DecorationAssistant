# DecorationAssistant

DecorationAssistant es un mod para [Geometry Dash](https://www.robtopgames.com/) desarrollado con el [Geode SDK](https://geode-sdk.org/) que actúa como asistente de decoración inteligente dentro del Editor de niveles. Su objetivo es agilizar la creación de escenas complejas mediante sugerencias contextuales, automatización de capas y un sistema de deshacer/rehacer exclusivo del mod.

> **Estado actual:** MVP 0.2.0. Las características principales están implementadas de forma modular, pero algunas heurísticas avanzadas permanecen como _stubs_ listos para iteración.

## Características

- **Panel "💡 Asistente Deco"**: muestra sugerencias de decoración, paletas recomendadas y paletas alternativas según intensidad (`minimal`, `balanced`, `loaded`).
- **Autodeco**: aplica sugerencias seleccionadas utilizando grupos temporales para vista previa y reportes detallados de lo aplicado.
- **Aprendizaje por referencia**: importa niveles externos, genera perfiles de estilo, estima dificultad y los utiliza como pista para las sugerencias.
- **Auto Capa**: normaliza capas, z-order y grupos según presets declarativos definidos en `data/layer_presets.json`.
- **Undo/Redo global**: sistema de comandos propio para revertir cualquier cambio realizado por el mod, incluidos autodeco y auto layer, con etiqueta descriptiva en la UI.
- **Overlay HUD**: visualización en tiempo real de densidad decorativa, hotspots y tintes de preview.
- **Persistencia & Telemetría**: almacena preferencias, historial de referencias y estadísticas locales para generar reportes.

## Instalación y compilación

1. Instala el [Geode SDK](https://docs.geode-sdk.org/geode/getting-started/installation) y asegúrate de tener configurado el toolchain recomendado.
2. Clona este repositorio y navega al directorio raíz.
3. Genera la build con CMake y Ninja (o tu generador preferido):

   ```bash
   geode build -g ninja
   ninja -C build
   ```

   Consulta la [guía oficial para crear mods](https://docs.geode-sdk.org/geode/getting-started/create-mod) para más detalles sobre `geode init`, `geode build` y empaquetado.

4. Copia el paquete generado (`flozwer.decorationassistant.geode`) a tu carpeta de mods o utiliza `geode deploy`.

## Hotkeys y ajustes

Los ajustes se exponen mediante Settings v3 en `mod.json`:

| Clave | Tipo | Descripción |
| --- | --- | --- |
| `hotkey_toggle_panel` | Hotkey | Alterna el panel principal (predeterminado `Y`). |
| `style_intensity` | Enum | Controla la cantidad de sugerencias (minimal/balanced/loaded). |
| `theme_mode` | Enum | Fuerza el modo temático (`auto`, `space`, `tech`, `nature`, `neon`). |
| `apply_preview_only` | Bool | Limita Autodeco a modo vista previa. |
| `auto_pulse_on_drops` | Bool | Activa triggers de pulso en drops detectados. |
| `seed` | Int | Semilla para operaciones deterministas. |
| `autolayer_active_preset` | String | Preset actual de Auto Capa. |
| `autolayer_preview_tint` | Bool | Activa tintado por capa en la simulación. |
| `corpus_max_size` | Int | Máximo de niveles en el corpus de referencia. |
| `learn_from_selection_only` | Bool | Limita el aprendizaje a la selección actual. |

Consulta la [documentación de Settings v3](https://docs.geode-sdk.org/geode/features/settings) para extender los ajustes.

## Recursos y Node IDs

- Los íconos UHD del mod se ubican en `resources/` y se referencian en `mod.json`. Geode generará automáticamente las variantes M/L según la [guía de recursos](https://docs.geode-sdk.org/geode/features/resources).
- La integración con la UI del Editor utiliza Node IDs declarados para garantizar compatibilidad entre mods siguiendo la [documentación sobre Node IDs](https://docs.geode-sdk.org/geode/features/node-ids) y [acceso a nodos](https://docs.geode-sdk.org/geode/advanced/layers#node-tree).

## Flujo de trabajo recomendado

1. Abre el Editor y activa el panel con la hotkey (`Y`).
2. Escanea la escena o selección para obtener sugerencias y visualización del overlay.
3. Importa un nivel de referencia desde la pestaña "Corpus" y asigna un preset de Auto Capa.
4. Simula la asignación de capas con tintes, revisa conflictos y aplica cuando estés conforme.
5. Utiliza la barra de Undo/Redo para revertir operaciones del mod cuando sea necesario.
6. Exporta un reporte desde la sección de Telemetría si deseas compartir feedback o registrar cambios.

## Documentación adicional

- [Layers y Hooks en Geode](https://docs.geode-sdk.org/geode/advanced/layers)
- [Acceso a nodos por ID](https://docs.geode-sdk.org/geode/advanced/node-ids)
- [Guía de recursos UHD](https://docs.geode-sdk.org/geode/features/resources)

## Licencia

Este proyecto se distribuye bajo la licencia MIT. Consulta el archivo `LICENSE` para más detalles.
