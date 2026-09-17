# PPG Wave Clone — Fase 6: Presets

Implementación original de un instrumento virtual VST3 de síntesis wavetable
inspirado conceptualmente en el PPG Wave 3.3. No contiene ROMs, muestras,
gráficos ni código propietario de terceros.

## Estado de esta entrega

**Fases 1-5**: listo y confirmado sonando en hardware real (arquitectura,
osciladores wavetable, filtro + envolventes, LFOs + matriz de modulación,
efectos).

**Fase 6** (esta entrega): gestión completa de presets.

- `PresetManager`: guardar / cargar / eliminar / navegar (anterior,
  siguiente, aleatorio) / favoritos / búsqueda por nombre y categoría a
  nivel de datos. Un preset es una foto completa del estado del APVTS — el
  mismo mecanismo que usa Ableton Live para guardar el estado del plugin en
  el proyecto, así que cargar un preset es indistinguible de mover todos
  los parámetros a mano.
- **Factory presets**: 12 presets reales y distintos, uno por cada
  categoría del documento de diseño (Bass, Lead, Pad, Keys, Bell, Pluck,
  Sequence, FX, Atmospheric, Digital, Experimental, Percussive), horneados
  en el binario (`FactoryPresets.cpp`) y protegidos — no se pueden
  sobrescribir ni borrar desde el plugin.
- **User presets**: guardar con nombre, quedan en disco como XML en el
  directorio de datos de la aplicación (`~/Library/Application
  Support/PPGWaveClone/Presets/` en macOS), se recargan solos la próxima
  vez que abras el plugin. Se pueden eliminar.
- **Favoritos**: persisten en un archivo aparte (`favorites.xml`), aplican
  tanto a presets de fábrica como de usuario.
- Barra de presets en la GUI: selector (fábrica y usuario agrupados),
  anterior/siguiente, aleatorio, favorito, guardar, eliminar.

**Lo que NO incluye todavía**: carácter "vintage", interfaz gráfica
definitiva.

## Problemas conocidos

- **12 presets de fábrica, no 128**: el documento original pide una
  biblioteca de al menos 128. Lo que se entrega aquí es la arquitectura
  completa y funcional del sistema de presets, con un ejemplo real por
  categoría. Ampliar a 128 es trabajo de contenido — agregar más funciones
  como las de `FactoryPresets.cpp` — no un cambio de arquitectura. Puedo
  generar más en cualquier momento si quieres, dentro o fuera del orden de
  fases.
- **Sin buscador de texto ni filtro de categoría en la GUI todavía**:
  `PresetManager::findMatching()` ya soporta ambos a nivel de datos; falta
  conectarlos a controles de texto/combo reales en la interfaz, lo cual es
  trabajo de UI que encaja natural en la Fase 7 (interfaz definitiva) junto
  con el browser completo descrito en el documento original (sección 17).
- El diálogo para nombrar un preset nuevo usa `juce::AlertWindow` (un
  cuadro de sistema simple) en vez de un campo integrado en la GUI — mismo
  criterio de "funcional primero, pulido en la Fase 7".
- Sigue el mismo aliasing en notas agudas de fases anteriores
  (comportamiento esperado, no un bug).

## Compilación sin instalar nada (recomendado)

Este proyecto incluye `.github/workflows/build.yml`: compila el VST3 en un
Mac con Xcode ya instalado, en la nube, gratis, vía GitHub Actions. Sube la
carpeta a un repositorio (con GitHub Desktop, para no perder `.github` por
ser una carpeta oculta), espera la palomita verde en la pestaña "Actions", y
descarga el `.vst3` desde "Artifacts".

## Compilación local (requiere CMake + compilador C++20)

1. Clona JUCE dentro de esta carpeta:
   ```
   git clone --depth 1 --branch 7.0.12 https://github.com/juce-framework/JUCE.git
   ```
   Debe quedar como `PPGWaveClone/JUCE/`.

2. Configura y compila:

   **macOS** (Xcode instalado):
   ```
   cmake -B build -G Xcode
   cmake --build build --config Release
   ```
   El `.vst3` queda en `build/PPGWaveClone_artefacts/Release/VST3/`.
   Cópialo a `~/Library/Audio/Plug-Ins/VST3/` (tu usuario) o
   `/Library/Audio/Plug-Ins/VST3/` (todo el sistema — recomendado si Ableton
   corre en otra sesión de usuario). Si lo bajaste de un navegador:
   ```
   xattr -dr com.apple.quarantine "/ruta/al/PPG Wave Clone.vst3"
   ```

   **Windows** (Visual Studio 2022):
   ```
   cmake -B build -G "Visual Studio 17 2022"
   cmake --build build --config Release
   ```
   El `.vst3` queda en `build/PPGWaveClone_artefacts/Release/VST3/`.
   Cópialo a `C:\Program Files\Common Files\VST3\`.

3. Rescanea plugins en Ableton Live (Preferences → Plug-ins → Rescan).

## Estructura

```
PPGWaveClone/
  CMakeLists.txt
  .github/workflows/build.yml   — compila el VST3 en la nube (macOS, x86_64)
  Source/
    PluginProcessor.h/.cpp      — AudioProcessor, APVTS, Synthesiser, efectos, presetManager
    PluginEditor.h/.cpp         — GUI (Fase 6): + barra de presets
    Params/  ...                 (sin cambios de fondo esta fase)
    DSP/     ...                 (sin cambios de fondo esta fase)
    Synth/   ...                 (sin cambios de fondo esta fase)
    Presets/
      PresetManager.h/.cpp        — guardar/cargar/eliminar/navegar/favoritos
      FactoryPresets.h/.cpp       — 12 presets horneados en el binario
```

## Próxima fase (Fase 8 — Vintage Character)

(La Fase 7, interfaz profesional definitiva, la dejamos para el final a
propósito — es más eficiente diseñarla una sola vez cuando ya no queden
paneles nuevos por agregar, en vez de rehacerla en cada fase.)

- `Source/DSP/VintageCharacter.h/.cpp`: bit depth simulation, sample rate
  reduction, ruido digital, inestabilidad de osciladores/pitch drift,
  variación de filtro/voz, cantidad de aliasing.
- Control global Vintage/Modern que los combina.
- Integración en `SynthVoice` (por voz, para que la variación entre voces
  sea real) y en el motor de audio a nivel de plugin donde corresponda.

Dime si seguimos con la Fase 8, o si prefieres la Fase 7 (interfaz) antes.
