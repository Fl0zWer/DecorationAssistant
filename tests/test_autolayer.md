# Test Manual – AutoLayer & Undo

1. **Preset Clean-Tech**
   - Entrada: 300 objetos variados.
   - Acción: Simular → Aplicar.
   - Esperado: todos los objetos reciben capa y z-order según preset.
2. **Resolución de conflictos**
   - Caso: objetos duplicados en mismas coordenadas.
   - Esperado: `conflictPolicy=last-wins` prevalece y log de conflictos listados.
3. **Throttle de partículas**
   - Caso: 100 partículas seleccionadas.
   - Esperado: solo 60% recibe asignación (`throttle=0.6`).
4. **Preview idempotente**
   - Acción: ejecutar vista previa dos veces sin aplicar.
   - Esperado: no se duplican grupos `DA_PREVIEW_LAYER_*`.
5. **Revertir preview**
   - Acción: Botón "Revertir".
   - Esperado: limpia grupos y capas temporales.
6. **Undo Autolayer**
   - Acción: Aplicar preset → Undo.
   - Esperado: restaura capas, z-order y grupos previos.
7. **Redo Autolayer**
   - Acción: Undo → Redo.
   - Esperado: reaplica exactamente el mismo plan.
8. **Undo creación masiva**
   - Acción: Crear 50 objetos con Autodeco → Undo.
   - Esperado: objetos eliminados.
9. **Redo creación masiva**
   - Acción: Undo anterior → Redo.
   - Esperado: objetos restaurados en mismo orden determinista.
10. **Persistencia de seguridad**
    - Acción: Forzar cierre tras Autolayer.
    - Esperado: al reabrir, se ofrece revertir mediante `UndoManager`.
