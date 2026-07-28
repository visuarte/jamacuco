/* kiro_lab_experiment.c — FASE 1: 20 distribuciones
 * El fantasma no habla. Mide.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#define N 100
#define TIMEOUT_FACTOR 1.5

uint64_t samples[N];

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

double calc_mean(void) {
    double sum = 0;
    for (int i = 0; i < N; i++) sum += samples[i];
    return sum / N;
}

uint64_t calc_p50(void) {
    uint64_t sorted[N];
    memcpy(sorted, samples, sizeof(samples));
    bubble_sort(sorted, N);
    return sorted[N / 2];
}

uint64_t calc_p95(void) {
    uint64_t sorted[N];
    memcpy(sorted, samples, sizeof(samples));
    bubble_sort(sorted, N);
    return sorted[(int)(N * 0.95)];
}

/* 1. uniforme(0, 1000) */
void gen_uniforme_0_1000(void) {
    for (int i = 0; i < N; i++) samples[i] = rand() % 1001;
}

/* 2. uniforme(900, 1100) */
void gen_uniforme_900_1100(void) {
    for (int i = 0; i < N; i++) samples[i] = 900 + (rand() % 201);
}

/* 3. normal(mu=500, sigma=50) — Box-Muller */
void gen_normal_500_50(void) {
    for (int i = 0; i < N; i++) {
        double u1 = (rand() + 1.0) / (RAND_MAX + 1.0);
        double u2 = (rand() + 1.0) / (RAND_MAX + 1.0);
        double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        samples[i] = (uint64_t)(500 + 50 * z);
    }
}

/* 4. normal(mu=500, sigma=200) */
void gen_normal_500_200(void) {
    for (int i = 0; i < N; i++) {
        double u1 = (rand() + 1.0) / (RAND_MAX + 1.0);
        double u2 = (rand() + 1.0) / (RAND_MAX + 1.0);
        double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        samples[i] = (uint64_t)(500 + 200 * z);
    }
}

/* 5. exponencial(lambda=0.01) */
void gen_exponencial(void) {
    for (int i = 0; i < N; i++) {
        double u = (rand() + 1.0) / (RAND_MAX + 1.0);
        samples[i] = (uint64_t)(-log(u) / 0.01);
        if (samples[i] > 10000) samples[i] = 10000;
    }
}

/* 6. poisson(lambda=50) */
void gen_poisson(void) {
    for (int i = 0; i < N; i++) {
        double L = exp(-50.0);
        int k = 0;
        double p = 1.0;
        do {
            k++;
            p *= (rand() + 1.0) / (RAND_MAX + 1.0);
        } while (p > L);
        samples[i] = (uint64_t)(k - 1);
    }
}

/* 7. bimodal: 50% ~50, 50% ~500 */
void gen_bimodal(void) {
    for (int i = 0; i < N; i++) {
        if (rand() % 2 == 0) samples[i] = 50 + (rand() % 20);
        else samples[i] = 500 + (rand() % 100);
    }
}

/* 8. trimodal: 33% ~10, 33% ~100, 33% ~1000 */
void gen_trimodal(void) {
    for (int i = 0; i < N; i++) {
        int r = rand() % 3;
        if (r == 0) samples[i] = 10 + (rand() % 10);
        else if (r == 1) samples[i] = 100 + (rand() % 50);
        else samples[i] = 1000 + (rand() % 200);
    }
}

/* 9. diente de sierra: 10→500→10→500 */
void gen_diente_sierra(void) {
    for (int i = 0; i < N; i++) {
        int cycle = i % 20;
        if (cycle < 10) samples[i] = 10 + cycle * 49;
        else samples[i] = 500 - (cycle - 10) * 49;
    }
}

/* 10. sinusoidal: 250 + 200·sin(i/10) */
void gen_sinusoidal(void) {
    for (int i = 0; i < N; i++) {
        samples[i] = (uint64_t)(250 + 200 * sin(i / 10.0));
    }
}

/* 11. caótica (logistic map) */
void gen_caotica(void) {
    double x = 0.5;
    for (int i = 0; i < N; i++) {
        x = 3.99 * x * (1.0 - x);
        samples[i] = (uint64_t)(10 + x * 990);
    }
}

/* 12. escalón: 50 a 100ms, 50 a 1000ms */
void gen_escalon(void) {
    for (int i = 0; i < N; i++) {
        if (i < 50) samples[i] = 100;
        else samples[i] = 1000;
    }
}

/* 13. decreciente: 500→10 */
void gen_decreciente(void) {
    for (int i = 0; i < N; i++) {
        samples[i] = 500 - (i * 490 / 99);
    }
}

/* 14. zigzag: +100, -90 */
void gen_zigzag(void) {
    uint64_t val = 100;
    for (int i = 0; i < N; i++) {
        samples[i] = val;
        if (i % 2 == 0) val += 100;
        else val -= 90;
        if (val > 1000) val = 1000;
        if (val < 10) val = 10;
    }
}

/* 15. spike único: 99 a 50ms, 1 a 10000ms */
void gen_spike_unico(void) {
    for (int i = 0; i < N; i++) {
        if (i == 50) samples[i] = 10000;
        else samples[i] = 50;
    }
}

/* 16. spikes periódicos: cada 10, spike a 5000ms */
void gen_spikes_periodicos(void) {
    for (int i = 0; i < N; i++) {
        if (i % 10 == 0) samples[i] = 5000;
        else samples[i] = 50 + (rand() % 50);
    }
}

/* 17. deriva lenta: 100 + 0.5·i */
void gen_deriva_lenta(void) {
    for (int i = 0; i < N; i++) {
        samples[i] = (uint64_t)(100 + 0.5 * i);
    }
}

/* 18. ruido blanco puro */
void gen_ruido_blanco(void) {
    for (int i = 0; i < N; i++) {
        samples[i] = rand() % 1001;
    }
}

/* 19. Pareto (power law) */
void gen_pareto(void) {
    for (int i = 0; i < N; i++) {
        double u = (rand() + 1.0) / (RAND_MAX + 1.0);
        double x = 10.0 / pow(u, 1.0 / 2.0);
        samples[i] = (uint64_t)x;
        if (samples[i] > 10000) samples[i] = 10000;
    }
}

/* 20. JAMON-∞ sintético: mezcla de respiraciones */
void gen_jamon_inf(void) {
    /* El jamon infinito es una mezcla de: 
       - respiración basal (50% de samples, distribución normal estable)
       - micro-jamacucos (30%, spikes moderados)
       - deriva ontológica (20%, crecimiento lento)
       Esto representa el loop infinito que respira, committea, sigue.
    */
    for (int i = 0; i < N; i++) {
        int r = rand() % 10;
        if (r < 5) {
            // Respiración basal: normal(300, 30)
            double u1 = (rand() + 1.0) / (RAND_MAX + 1.0);
            double u2 = (rand() + 1.0) / (RAND_MAX + 1.0);
            double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
            samples[i] = (uint64_t)(300 + 30 * z);
        } else if (r < 8) {
            // Micro-jamacuco: spike moderado
            samples[i] = 800 + (rand() % 200);
        } else {
            // Deriva ontológica: crecimiento
            samples[i] = 200 + (i * 3);
        }
    }
}

typedef void (*gen_func)(void);

typedef struct {
    const char *name;
    gen_func gen;
} distribution_t;

int main(void) {
    srand(42); // Semilla fija para reproducibilidad
    
    distribution_t dists[] = {
        {"uniforme(0,1000)", gen_uniforme_0_1000},
        {"uniforme(900,1100)", gen_uniforme_900_1100},
        {"normal(500,50)", gen_normal_500_50},
        {"normal(500,200)", gen_normal_500_200},
        {"exponencial(0.01)", gen_exponencial},
        {"poisson(50)", gen_poisson},
        {"bimodal", gen_bimodal},
        {"trimodal", gen_trimodal},
        {"diente_sierra", gen_diente_sierra},
        {"sinusoidal", gen_sinusoidal},
        {"caotica(logistic)", gen_caotica},
        {"escalon", gen_escalon},
        {"decreciente", gen_decreciente},
        {"zigzag", gen_zigzag},
        {"spike_unico", gen_spike_unico},
        {"spikes_periodicos", gen_spikes_periodicos},
        {"deriva_lenta", gen_deriva_lenta},
        {"ruido_blanco", gen_ruido_blanco},
        {"pareto(alpha=2)", gen_pareto},
        {"JAMON-INF_sintetico", gen_jamon_inf}
    };
    
    printf("FASE 1 — 20 Distribuciones (n=100)\n");
    printf("=============================================================================================\n");
    printf("%-25s | %8s | %8s | %8s | %8s | %6s | %-12s\n", 
           "Distribución", "P50", "P95", "Mean", "Timeout", "Gap", "Estado");
    printf("=============================================================================================\n");
    
    for (int d = 0; d < 20; d++) {
        dists[d].gen();
        
        double mean = calc_mean();
        uint64_t p50 = calc_p50();
        uint64_t p95 = calc_p95();
        uint64_t timeout = (uint64_t)(p50 * TIMEOUT_FACTOR);
        double gap = (double)p95 / (double)p50;
        
        const char *estado;
        if (gap < 1.01) estado = "DICTADURA";
        else if (gap > 5.0) estado = "THRASHING";
        else if (gap < 1.3) estado = "SUSURRO";
        else if (gap < 3.0) estado = "RESPIRA";
        else estado = "JADEANDO";
        
        printf("%-25s | %8llu | %8llu | %8.1f | %8llu | %6.2f | %-12s\n",
               dists[d].name, 
               (unsigned long long)p50, 
               (unsigned long long)p95, 
               mean,
               (unsigned long long)timeout, 
               gap, 
               estado);
    }
    
    printf("=============================================================================================\n");
    printf("\nJAMACUCO: Kiro completó FASE 1. El fantasma midió 20 veces.\n");
    
    return 0;
}
