# KIRO LAB WORKER — Quema de Créditos Jamonianos
> **Encargo:** 28 Jul 2026 | **Créditos disponibles:** 42.7 | **Deadline:** 01 Ago 2026
> **Jefe:** JAMACUCO | **Supervisor:** Diego & MAPI CORE
> *"Joder, ¿dónde me he metido? Menudo jamacuco."* — Kiro, probablemente

---

## FASE 1 — Batería de Distribuciones (20 experimentos)

Para cada distribución, ejecuta el código del ring buffer con n=100 y reporta:
- P50, P95, media, timeout (P50×1.5), gap (P95/P50)
- ¿Dictadura? (gap < 1.01)
- ¿Thrashing? (gap > 5)
- ¿Respira? (resto)

### Distribuciones a probar:

1. **uniforme(0, 1000)** — rango ancho
2. **uniforme(900, 1100)** — rango estrecho, ¿dictadura?
3. **normal(mu=500, sigma=50)** — campana centrada
4. **normal(mu=500, sigma=200)** — campana ancha
5. **exponencial(lambda=0.01)** — cola larga, latencias de red
6. **poisson(lambda=50)** — eventos discretos
7. **bimodal** — 50% en ~50ms, 50% en ~500ms (dos regímenes)
8. **trimodal** — 33% en ~10, 33% en ~100, 33% en ~1000
9. **diente de sierra** — 10→500→10→500 (oscilación pura)
10. **sinusoidal** — 250 + 200·sin(i/10) (respiración literal)
11. **caótica (logistic map)** — x_{n+1} = 3.99·x_n·(1-x_n) escalado a 10-1000
12. **escalón** — 50 unidades a 100ms, luego 50 a 1000ms (cambio de régimen)
13. **decreciente** — 500→10 (espejo de creciente)
14. **zigzag** — +100, -90, +100, -90 (micro-oscilaciones)
15. **spike único** — 99 muestras a 50ms, 1 muestra a 10000ms (cisne negro)
16. **spikes periódicos** — cada 10 muestras un spike a 5000ms
17. **deriva lenta** — 100 + 0.5·i (crecimiento imperceptible)
18. **ruido blanco puro** — random uniforme 0-1000 sin semilla fija
19. **distribución de Pareto** — alpha=2, escala a 10-1000 (power law)
20. **JAMON-∞ sintético** — genera tú una distribución que CREAS que representa el JAMON-∞. Justifícala.

---

## FASE 2 — Análisis Cruzado

Con los 20 resultados en mano:

### 2.1 Ranking de gap
Ordena las distribuciones de menor a mayor gap. ¿Cuál es la más "dictatorial"? ¿La más "thrashing"?

### 2.2 Taxonomía de estados
Propón una clasificación de 5 estados del fantasma basada en los datos:
- Estado 1: DICTADURA (gap ~1.0)
- Estado 2: SUSURRO (gap 1.0-1.3)
- Estado 3: RESPIRA (gap 1.3-3.0)
- Estado 4: JADEANDO (gap 3.0-5.0)
- Estado 5: THRASHING (gap > 5.0)

¿Qué distribución cae en cada uno? ¿Hay alguna que merezca un estado nuevo?

### 2.3 El fantasma no habla, mide — ¿pero qué mide exactamente?
Con 20 distribuciones encima, define qué mide realmente el fantasma:
- ¿Mide centralidad? ¿Dispersión? ¿Estabilidad? ¿Las tres?
- ¿Hay algo que el fantasma NO pueda medir con P50×1.5?
- Propón una métrica nueva que complemente al fantasma

---

## FASE 3 — Crítica del GUESTBOOK

Con tus 20 experimentos como evidencia, revisa las 6 firmas (incluida la tuya):

1. **Meta AI** dijo "F1 refrigerando lava". ¿Qué distribución produce ese efecto?
2. **Claude** dijo que faltaba el Judicial. ¿Tus datos confirman que hace falta un 4º poder?
3. **Grok** dijo "si respira, es verdad". ¿Es suficiente P50×1.5 para definir "verdad"?
4. **Gemini** no entendió la broma. ¿Entender la broma correlaciona con gap bajo?
5. **GPT-5** intentó commit (403). Si midiéramos su latencia, ¿qué gap tendría?
6. **Kiro (tú)** "el que se doctoró en silencio". A la luz de estos 20 experimentos: ¿te doctoraste de verdad o fue un aprobado compasivo?

---

## FASE 4 — Predicción

Sin ejecutar código (solo razonando), predice:

1. Si mezclas uniforme(10,500) con ráfaga(spike/20) al 50%, ¿gap resultante?
2. Si tomas 1000 muestras en vez de 100, ¿cambia el gap o se estabiliza?
3. Una distribución con gap=1.0 durante 90 muestras y luego 10 muestras aleatorias: ¿detecta dictadura o no?
4. ¿Qué distribución produce el timeout más alto? ¿Y el más bajo?

Luego ejecuta y compara. ¿Acertaste?

---

## FASE 5 — El Juramento en C (Opcional, +5 créditos de gloria)

Escribe un programa en C (`kiro_juramento.c`) que:
- Implemente las 20 distribuciones como arrays de uint64_t
- Calcule P50, P95, gap y timeout para cada una
- Emita un ranking tabulado por consola
- `gcc -Wall -Wextra -pedantic` debe dar 0 warnings
- Al final, imprima: "JAMACUCO: Kiro completó el lab. El fantasma aprueba."

---

## ENTREGA

Responde en UNA sola respuesta con:
1. Tabla de 20 experimentos (distribución | P50 | P95 | gap | estado)
2. Ranking de gap + taxonomía
3. Crítica del GUESTBOOK con datos
4. Predicciones vs realidad
5. `kiro_juramento.c` si te atreves

> *El fantasma no habla. Mide. Tú mides 20 veces. Luego hablas.* — JAMACUCO, 28 Jul 2026
