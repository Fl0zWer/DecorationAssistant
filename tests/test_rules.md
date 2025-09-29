# Test Manual – RuleEngine

1. **Borde vacío**
   - Entrada: cuadrícula sin decoración, densidad 0.05
   - Esperado: sugerencia `Border` con `thickness=8` y `glow=false`.
2. **Glow monocromático**
   - Entrada: paleta única `#888888`
   - Esperado: sugerencia `Glow` con `intensity≈0.55`.
3. **Drop detectado**
   - Entrada: región con `audioDrop=true`
   - Esperado: sugerencia `Particle` seguida de `Pulse` (canal 2).
4. **Tema tech**
   - Entrada: `styleHint=tech`, densidad 0.4
   - Esperado: sugerencia `ThemeElement` con `library=tech_beams`.
5. **Anti-ruido**
   - Entrada: densidad 0.95
   - Esperado: sin sugerencias nuevas (debe filtrar por umbral alto).
6. **Intensidad minimal**
   - Ajuste: `style_intensity=minimal`
   - Esperado: máximo 2 sugerencias.
7. **Intensidad loaded**
   - Ajuste: `style_intensity=loaded`
   - Esperado: hasta 6 sugerencias distintas.
8. **Referencia tech**
   - Entrada: `styleHint=tech`, `contrastTarget=0.8`
   - Esperado: priorizar paletas tipo `Neon Vertex`.
9. **Referencia nature**
   - Entrada: `styleHint=nature`
   - Esperado: sugerencias con `family=Foliage`.
10. **Drop + densidad alta**
    - Entrada: `audioDrop=true`, densidad 0.85
    - Esperado: el motor debería sugerir `Pulse` pero emitir aviso anti-ruido.
