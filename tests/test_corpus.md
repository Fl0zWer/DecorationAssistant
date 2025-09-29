# Test Manual – LevelCorpus

1. **Importación manual (.json)**
   - Archivo: `samples/tech_sample.json`
   - Esperado: `ImportBridge` produce `ImportedLevel` con BPM detectado y 1243 objetos.
2. **Extracción de estilo**
   - Entrada: nivel anterior
   - Esperado: `StyleProfile.density≈0.55`, `contrast≈0.78` (variación ±0.05).
3. **Estimación de dificultad**
   - Entrada: nivel anterior
   - Esperado: `difficultyScore` entre 0.55 y 0.65.
4. **Inserción en corpus**
   - Acción: `LevelCorpus::add(level)`
   - Esperado: `corpus_index.json` actualizado con timestamp `lastUsed`.
5. **Consulta top-K**
   - Acción: `SimilarityEngine::topK(level, 3)` incluyendo el propio nivel
   - Esperado: el nivel importado aparece en la posición 1.
6. **Límite corpus**
   - Ajuste: `corpus_max_size=4`
   - Esperado: al añadir 5 niveles, se elimina el más antiguo.
7. **Filtrado por tags**
   - Acción: `LevelCorpus::query({"tech"})`
   - Esperado: solo niveles etiquetados como `tech`.
8. **Referencia activa**
   - Acción: marcar nivel como referencia actual
   - Esperado: `Persist` almacena vínculo en `profile.json`.
9. **Reentrenamiento parcial**
   - Acción: actualizar estilo tras modificar presets
   - Esperado: se recalculan métricas en < 150 ms.
10. **Fallback sin corpus**
    - Acción: vaciar corpus y consultar sugerencias
    - Esperado: motor funciona con defaults sin errores.
