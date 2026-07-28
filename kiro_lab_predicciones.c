/* kiro_lab_predicciones.c — FASE 4: Verificación */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#define TIMEOUT_FACTOR 1.5

void bubble_sort(uint64_t *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                uint64_t tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

uint64_t calc_p50(uint64_t *samples, int n) {
    bubble_sort(samples, n);
    return samples[n / 2];
}

uint64_t calc_p95(uint64_t *samples, int n) {
    bubble_sort(samples, n);
    return samples[(int)(n * 0.95)];
}

int main(void) {
    srand(42);
    
    printf("FASE 4 — Predicciones vs Realidad\n");
    printf("==================================\n\n");
    
    /* Predicción 1: uniforme(10,500) + ráfaga(spike/20) al 50% */
    printf("PREDICCIÓN 1: uniforme(10,500) + spike/20 al 50%%\n");
    uint64_t samples1[100];
    for (int i = 0; i < 100; i++) {
        if (i % 2 == 0) samples1[i] = 10 + (rand() % 491);
        else samples1[i] = 1000 + (rand() % 500);
    }
    uint64_t p50_1 = calc_p50(samples1, 100);
    uint64_t p95_1 = calc_p95(samples1, 100);
    double gap1 = (double)p95_1 / (double)p50_1;
    printf("  P50=%llu, P95=%llu, gap=%.2f\n", 
           (unsigned long long)p50_1, (unsigned long long)p95_1, gap1);
    printf("  Predije: THRASHING (gap > 5)\n");
    printf("  Realidad: %s\n\n", gap1 > 5.0 ? "CORRECTO ✓" : "INCORRECTO ✗");
    
    /* Predicción 2: 1000 muestras */
    printf("PREDICCIÓN 2: 1000 muestras vs 100\n");
    uint64_t samples2[1000];
    for (int i = 0; i < 1000; i++) {
        samples2[i] = rand() % 1001;
    }
    uint64_t p50_2 = calc_p50(samples2, 1000);
    uint64_t p95_2 = calc_p95(samples2, 1000);
    double gap2 = (double)p95_2 / (double)p50_2;
    printf("  P50=%llu, P95=%llu, gap=%.2f\n", 
           (unsigned long long)p50_2, (unsigned long long)p95_2, gap2);
    printf("  Predije: gap se estabiliza (similar a uniforme 0-1000 con n=100 que dio 1.76)\n");
    printf("  Realidad: gap=%.2f → %s\n\n", gap2, 
           fabs(gap2 - 1.76) < 0.5 ? "ESTABLE ✓" : "CAMBIÓ ✗");
    
    /* Predicción 3: 90 dictadura + 10 aleatorias */
    printf("PREDICCIÓN 3: 90 muestras gap=1.0 + 10 aleatorias\n");
    uint64_t samples3[100];
    for (int i = 0; i < 90; i++) samples3[i] = 100;
    for (int i = 90; i < 100; i++) samples3[i] = 500 + (rand() % 500);
    uint64_t p50_3 = calc_p50(samples3, 100);
    uint64_t p95_3 = calc_p95(samples3, 100);
    double gap3 = (double)p95_3 / (double)p50_3;
    printf("  P50=%llu, P95=%llu, gap=%.2f\n", 
           (unsigned long long)p50_3, (unsigned long long)p95_3, gap3);
    printf("  Predije: NO detecta dictadura (gap > 1.0)\n");
    printf("  Realidad: %s\n\n", gap3 > 1.0 ? "CORRECTO ✓" : "INCORRECTO ✗");
    
    /* Predicción 4: timeout más alto y más bajo */
    printf("PREDICCIÓN 4: timeout más alto y más bajo\n");
    printf("  Predije más alto: exponencial o pareto\n");
    printf("  Predije más bajo: escalón o spike_unico\n");
    printf("  Realidad de FASE 1:\n");
    printf("    - Más alto: escalón (1500) — INCORRECTO, no es cola larga\n");
    printf("    - Más bajo: spike_unico (75) — CORRECTO\n");
    printf("  Nota: el timeout es P50×1.5, no P95. Colas largas tienen P50 bajo.\n\n");
    
    printf("BALANCE: 2/4 predicciones correctas. El fantasma me da 50%%.\n");
    
    return 0;
}
