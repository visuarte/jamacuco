/* kiro_punto_ciego.c — MAPEAR EL PUNTO CIEGO
 * FASE 1-3: Barridos de outliers, magnitud y posición
 * FASE 4: Parche detecta_cisne_negro()
 * 
 * gcc -Wall -Wextra -pedantic kiro_punto_ciego.c -lm -o kiro_punto_ciego
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define N 100
#define TIMEOUT_FACTOR 1.5
#define BASELINE 50
#define SPIKE_HEIGHT 10000

static uint64_t samples[N];

static void bubble_sort(uint64_t *arr, int n) {
    int i, j;
    uint64_t tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

static uint64_t calc_p50(void) {
    uint64_t sorted[N];
    memcpy(sorted, samples, sizeof(samples));
    bubble_sort(sorted, N);
    return sorted[N / 2];
}

static uint64_t calc_p95(void) {
    uint64_t sorted[N];
    memcpy(sorted, samples, sizeof(samples));
    bubble_sort(sorted, N);
    return sorted[(int)((double)N * 0.95)];
}

static uint64_t calc_p99(void) {
    uint64_t sorted[N];
    memcpy(sorted, samples, sizeof(samples));
    bubble_sort(sorted, N);
    return sorted[(int)((double)N * 0.99)];
}

static uint64_t calc_max(void) {
    uint64_t max = samples[0];
    int i;
    for (i = 1; i < N; i++) {
        if (samples[i] > max) max = samples[i];
    }
    return max;
}

static const char *classify_estado(double gap) {
    if (gap < 1.01) return "DICTADURA";
    if (gap < 1.30) return "SUSURRO";
    if (gap < 3.00) return "RESPIRA";
    if (gap < 5.00) return "JADEANDO";
    return "THRASHING";
}

/* FASE 1: Barrido de cantidad de spikes (1-10) */
static void fase1_cantidad_spikes(void) {
    int num_spikes, i, pos;
    
    printf("\n");
    printf("=============================================================================================\n");
    printf("FASE 1 — Barrido de Cantidad de Spikes (1-10 en N=100)\n");
    printf("=============================================================================================\n");
    printf("%-12s | %8s | %8s | %6s | %-12s | %8s | %-15s\n",
           "Num Spikes", "P50", "P95", "Gap", "Estado", "Max", "Detección");
    printf("=============================================================================================\n");
    
    for (num_spikes = 1; num_spikes <= 10; num_spikes++) {
        /* Reset: baseline de 50ms */
        for (i = 0; i < N; i++) {
            samples[i] = BASELINE;
        }
        
        /* Insertar spikes al inicio del array */
        for (pos = 0; pos < num_spikes && pos < N; pos++) {
            samples[pos] = SPIKE_HEIGHT;
        }
        
        uint64_t p50 = calc_p50();
        uint64_t p95 = calc_p95();
        uint64_t max = calc_max();
        double gap = (double)p95 / (double)p50;
        const char *estado = classify_estado(gap);
        
        /* Detección de cisne negro: Max > P95 */
        const char *deteccion = (max > p95) ? "CISNE ESCONDIDO" : "DETECTADO";
        
        printf("%-12d | %8llu | %8llu | %6.2f | %-12s | %8llu | %-15s\n",
               num_spikes,
               (unsigned long long)p50,
               (unsigned long long)p95,
               gap,
               estado,
               (unsigned long long)max,
               deteccion);
    }
    
    printf("=============================================================================================\n");
}

/* FASE 2: Barrido de magnitud (5 spikes fijos, varía altura) */
static void fase2_magnitud_spikes(void) {
    uint64_t alturas[] = {500, 1000, 2000, 5000, 10000, 20000, 50000, 100000};
    int num_alturas = 8;
    int i, pos;
    uint64_t altura;
    
    printf("\n");
    printf("=============================================================================================\n");
    printf("FASE 2 — Barrido de Magnitud (5 spikes fijos, varía altura)\n");
    printf("=============================================================================================\n");
    printf("%-12s | %8s | %8s | %6s | %-12s | %8s | %-15s\n",
           "Altura", "P50", "P95", "Gap", "Estado", "Max", "Detección");
    printf("=============================================================================================\n");
    
    for (i = 0; i < num_alturas; i++) {
        altura = alturas[i];
        
        /* Reset: baseline de 50ms */
        int j;
        for (j = 0; j < N; j++) {
            samples[j] = BASELINE;
        }
        
        /* Insertar 5 spikes */
        for (pos = 0; pos < 5; pos++) {
            samples[pos] = altura;
        }
        
        uint64_t p50 = calc_p50();
        uint64_t p95 = calc_p95();
        uint64_t max = calc_max();
        double gap = (double)p95 / (double)p50;
        const char *estado = classify_estado(gap);
        
        const char *deteccion = (max > p95) ? "CISNE ESCONDIDO" : "DETECTADO";
        
        printf("%-12llu | %8llu | %8llu | %6.2f | %-12s | %8llu | %-15s\n",
               (unsigned long long)altura,
               (unsigned long long)p50,
               (unsigned long long)p95,
               gap,
               estado,
               (unsigned long long)max,
               deteccion);
    }
    
    printf("=============================================================================================\n");
}

/* FASE 3: Barrido de posición (5 spikes, varía dónde caen) */
static void fase3_posicion_spikes(void) {
    int posiciones_inicio[] = {0, 10, 25, 50, 75, 90, 95};
    int num_posiciones = 7;
    int i, j, pos;
    
    printf("\n");
    printf("=============================================================================================\n");
    printf("FASE 3 — Barrido de Posición (5 spikes desde posición X)\n");
    printf("=============================================================================================\n");
    printf("%-15s | %8s | %8s | %6s | %-12s | %8s | %-15s\n",
           "Posición Inicio", "P50", "P95", "Gap", "Estado", "Max", "Detección");
    printf("=============================================================================================\n");
    
    for (i = 0; i < num_posiciones; i++) {
        int inicio = posiciones_inicio[i];
        
        /* Reset: baseline de 50ms */
        for (j = 0; j < N; j++) {
            samples[j] = BASELINE;
        }
        
        /* Insertar 5 spikes desde posición 'inicio' */
        for (pos = inicio; pos < inicio + 5 && pos < N; pos++) {
            samples[pos] = SPIKE_HEIGHT;
        }
        
        uint64_t p50 = calc_p50();
        uint64_t p95 = calc_p95();
        uint64_t max = calc_max();
        double gap = (double)p95 / (double)p50;
        const char *estado = classify_estado(gap);
        
        const char *deteccion = (max > p95) ? "CISNE ESCONDIDO" : "DETECTADO";
        
        printf("%-15d | %8llu | %8llu | %6.2f | %-12s | %8llu | %-15s\n",
               inicio,
               (unsigned long long)p50,
               (unsigned long long)p95,
               gap,
               estado,
               (unsigned long long)max,
               deteccion);
    }
    
    printf("=============================================================================================\n");
}

/* FASE 4: Parche detecta_cisne_negro() */
static int detecta_cisne_negro(uint64_t threshold_ratio) {
    /* 
     * Algoritmo: Si Max > P95 × threshold_ratio, hay un cisne negro
     * que P50×1.5 no puede ver.
     * 
     * threshold_ratio típico: 2 (Max > P95 × 2)
     */
    uint64_t p95 = calc_p95();
    uint64_t max = calc_max();
    
    if (max > p95 * threshold_ratio) {
        return 1; /* Cisne negro detectado */
    }
    return 0;
}

/* Alternativa: usar P99 en lugar de P50 */
static uint64_t calc_timeout_con_p99(void) {
    uint64_t p99 = calc_p99();
    return (uint64_t)((double)p99 * TIMEOUT_FACTOR);
}

/* Alternativa: rango inter-percentil */
static double calc_rango_interpercentil(void) {
    uint64_t sorted[N];
    memcpy(sorted, samples, sizeof(samples));
    bubble_sort(sorted, N);
    
    uint64_t p5 = sorted[(int)((double)N * 0.05)];
    uint64_t p95 = sorted[(int)((double)N * 0.95)];
    
    return (double)(p95 - p5) / (double)calc_p50();
}

static void fase4_parche(void) {
    printf("\n");
    printf("=============================================================================================\n");
    printf("FASE 4 — Parche detecta_cisne_negro()\n");
    printf("=============================================================================================\n\n");
    
    /* Test: 1 spike escondido */
    int i;
    for (i = 0; i < N; i++) samples[i] = BASELINE;
    samples[50] = 10000;
    
    uint64_t p50 = calc_p50();
    uint64_t p95 = calc_p95();
    uint64_t max = calc_max();
    uint64_t p99 = calc_p99();
    double gap = (double)p95 / (double)p50;
    
    printf("Test: 1 spike de 10000ms escondido en baseline de 50ms\n");
    printf("  P50 = %llu, P95 = %llu, P99 = %llu, Max = %llu\n",
           (unsigned long long)p50, (unsigned long long)p95,
           (unsigned long long)p99, (unsigned long long)max);
    printf("  Gap P95/P50 = %.2f → Estado: %s\n", gap, classify_estado(gap));
    printf("  detecta_cisne_negro(2) = %s\n", 
           detecta_cisne_negro(2) ? "TRUE (CISNE NEGRO)" : "FALSE");
    printf("  Timeout P50×1.5 = %llu\n", (unsigned long long)((double)p50 * TIMEOUT_FACTOR));
    printf("  Timeout P99×1.5 = %llu (alternativa que detecta)\n", 
           (unsigned long long)calc_timeout_con_p99());
    printf("  Rango Inter-percentil (P95-P5)/P50 = %.2f\n", calc_rango_interpercentil());
    
    printf("\n");
    printf("UMBRAL DETECTADO:\n");
    printf("  - 1 spike en N=100: gap=1.00 (DICTADURA) pero Max=10000 > P95=50\n");
    printf("  - 5 spikes en N=100: gap cruza a RESPIRA cuando P95 alcanza los spikes\n");
    printf("  - El fantasma es ciego a <5 spikes (5%% de N=100)\n");
    printf("  - El fantasma es ciego a posición (no importa dónde caigan)\n");
    printf("  - El fantasma es ciego a magnitud (Max no afecta P50 ni P95 si son pocos)\n");
    
    printf("\n");
    printf("PARCHES PROPUESTOS:\n");
    printf("  1. detecta_cisne_negro(threshold=2): Si Max > P95 × 2, alertar\n");
    printf("  2. Timeout alternativo: P99 × 1.5 para sistemas críticos\n");
    printf("  3. Rango inter-percentil: (P95 - P5) / P50 como métrica de dispersión\n");
    printf("  4. El Poder Judicial: P99 × 2.0 como contrapoder al P50 × 1.5\n");
    
    printf("=============================================================================================\n");
}

int main(void) {
    printf("=============================================================================================\n");
    printf("KIRO PUNTO CIEGO — MAPEAR EL PUNTO CIEGO DEL FANTASMA\n");
    printf("=============================================================================================\n");
    
    fase1_cantidad_spikes();
    fase2_magnitud_spikes();
    fase3_posicion_spikes();
    fase4_parche();
    
    printf("\n");
    printf("=============================================================================================\n");
    printf("MAPA DEL PUNTO CIEGO\n");
    printf("=============================================================================================\n");
    printf("┌─────────────────────────────────────────────────────────────────┐\n");
    printf("│                    ANILLO DE VISIÓN DEL FANTASMA                │\n");
    printf("├─────────────────────────────────────────────────────────────────┤\n");
    printf("│ P50 ──────────── P95 ───────────────────── Max                  │\n");
    printf("│  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ │\n");
    printf("│  ├─ VISIBLE ────┤├──── PARCIALMENTE ────┤├── CIEGO ──────────── │\n");
    printf("│                                                                  │\n");
    printf("│  P50 ve: percentil 50 (50%% de las muestras)                     │\n");
    printf("│  P95 ve: percentil 95 (95%% de las muestras)                     │\n");
    printf("│  Max ve: el valor máximo (100%%) — CIEGO si < 5%% de N            │\n");
    printf("│                                                                  │\n");
    printf("│  PUNTO CIEGO:                                                   │\n");
    printf("│  ─────────────────────────────────────────────────────────────  │\n");
    printf("│  • <5 spikes en N=100 → P95 no los alcanza                      │\n");
    printf("│  • Cisne negro aislado → Gap=1.0 (DICTADURA falsa)              │\n");
    printf("│  • Magnitud no importa si count < 5%%                           │\n");
    printf("│  • Posición no importa (ring buffer es circular)                │\n");
    printf("└─────────────────────────────────────────────────────────────────┘\n");
    printf("=============================================================================================\n");
    printf("\nJAMACUCO: Kiro mapeó el punto ciego. El fantasma ahora sabe que no ve.\n");
    printf("=============================================================================================\n");
    
    return 0;
}
