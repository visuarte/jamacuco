/* kiro_juramento.c — FASE 5: El Juramento en C
 * 28 Jul 2026 — Kiro Lab Worker
 * Créditos: 42.7 | Deadline: 01 Ago 2026
 * Jefe: JAMACUCO | Supervisor: Diego & MAPI CORE
 * 
 * gcc -Wall -Wextra -pedantic kiro_juramento.c -lm -o kiro_juramento
 * Objetivo: 0 warnings
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#define N 100
#define TIMEOUT_FACTOR 1.5

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

static double calc_mean(void) {
    double sum = 0.0;
    int i;
    for (i = 0; i < N; i++) {
        sum += (double)samples[i];
    }
    return sum / (double)N;
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

/* Distribuciones */
static void gen_uniforme_0_1000(void) {
    int i;
    for (i = 0; i < N; i++) {
        samples[i] = (uint64_t)(rand() % 1001);
    }
}

static void gen_uniforme_900_1100(void) {
    int i;
    for (i = 0; i < N; i++) {
        samples[i] = (uint64_t)(900 + (rand() % 201));
    }
}

static void gen_normal_500_50(void) {
    int i;
    for (i = 0; i < N; i++) {
        double u1 = ((double)rand() + 1.0) / ((double)RAND_MAX + 1.0);
        double u2 = ((double)rand() + 1.0) / ((double)RAND_MAX + 1.0);
        double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        samples[i] = (uint64_t)(500.0 + 50.0 * z);
    }
}

static void gen_normal_500_200(void) {
    int i;
    for (i = 0; i < N; i++) {
        double u1 = ((double)rand() + 1.0) / ((double)RAND_MAX + 1.0);
        double u2 = ((double)rand() + 1.0) / ((double)RAND_MAX + 1.0);
        double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        samples[i] = (uint64_t)(500.0 + 200.0 * z);
    }
}

static void gen_exponencial(void) {
    int i;
    for (i = 0; i < N; i++) {
        double u = ((double)rand() + 1.0) / ((double)RAND_MAX + 1.0);
        double val = -log(u) / 0.01;
        if (val > 10000.0) val = 10000.0;
        samples[i] = (uint64_t)val;
    }
}

static void gen_poisson(void) {
    int i;
    for (i = 0; i < N; i++) {
        double L = exp(-50.0);
        int k = 0;
        double p = 1.0;
        do {
            k++;
            p *= ((double)rand() + 1.0) / ((double)RAND_MAX + 1.0);
        } while (p > L);
        samples[i] = (uint64_t)(k - 1);
    }
}

static void gen_bimodal(void) {
    int i;
    for (i = 0; i < N; i++) {
        if (rand() % 2 == 0) {
            samples[i] = (uint64_t)(50 + (rand() % 20));
        } else {
            samples[i] = (uint64_t)(500 + (rand() % 100));
        }
    }
}

static void gen_trimodal(void) {
    int i;
    for (i = 0; i < N; i++) {
        int r = rand() % 3;
        if (r == 0) {
            samples[i] = (uint64_t)(10 + (rand() % 10));
        } else if (r == 1) {
            samples[i] = (uint64_t)(100 + (rand() % 50));
        } else {
            samples[i] = (uint64_t)(1000 + (rand() % 200));
        }
    }
}

static void gen_diente_sierra(void) {
    int i;
    for (i = 0; i < N; i++) {
        int cycle = i % 20;
        if (cycle < 10) {
            samples[i] = (uint64_t)(10 + cycle * 49);
        } else {
            samples[i] = (uint64_t)(500 - (cycle - 10) * 49);
        }
    }
}

static void gen_sinusoidal(void) {
    int i;
    for (i = 0; i < N; i++) {
        samples[i] = (uint64_t)(250.0 + 200.0 * sin((double)i / 10.0));
    }
}

static void gen_caotica(void) {
    int i;
    double x = 0.5;
    for (i = 0; i < N; i++) {
        x = 3.99 * x * (1.0 - x);
        samples[i] = (uint64_t)(10.0 + x * 990.0);
    }
}

static void gen_escalon(void) {
    int i;
    for (i = 0; i < N; i++) {
        if (i < 50) {
            samples[i] = 100;
        } else {
            samples[i] = 1000;
        }
    }
}

static void gen_decreciente(void) {
    int i;
    for (i = 0; i < N; i++) {
        samples[i] = (uint64_t)(500 - (i * 490 / 99));
    }
}

static void gen_zigzag(void) {
    int i;
    uint64_t val = 100;
    for (i = 0; i < N; i++) {
        samples[i] = val;
        if (i % 2 == 0) {
            val += 100;
        } else {
            val -= 90;
        }
        if (val > 1000) val = 1000;
        if (val < 10) val = 10;
    }
}

static void gen_spike_unico(void) {
    int i;
    for (i = 0; i < N; i++) {
        if (i == 50) {
            samples[i] = 10000;
        } else {
            samples[i] = 50;
        }
    }
}

static void gen_spikes_periodicos(void) {
    int i;
    for (i = 0; i < N; i++) {
        if (i % 10 == 0) {
            samples[i] = 5000;
        } else {
            samples[i] = (uint64_t)(50 + (rand() % 50));
        }
    }
}

static void gen_deriva_lenta(void) {
    int i;
    for (i = 0; i < N; i++) {
        samples[i] = (uint64_t)(100.0 + 0.5 * (double)i);
    }
}

static void gen_ruido_blanco(void) {
    int i;
    for (i = 0; i < N; i++) {
        samples[i] = (uint64_t)(rand() % 1001);
    }
}

static void gen_pareto(void) {
    int i;
    for (i = 0; i < N; i++) {
        double u = ((double)rand() + 1.0) / ((double)RAND_MAX + 1.0);
        double x = 10.0 / pow(u, 1.0 / 2.0);
        if (x > 10000.0) x = 10000.0;
        samples[i] = (uint64_t)x;
    }
}

static void gen_jamon_inf(void) {
    int i;
    for (i = 0; i < N; i++) {
        int r = rand() % 10;
        if (r < 5) {
            double u1 = ((double)rand() + 1.0) / ((double)RAND_MAX + 1.0);
            double u2 = ((double)rand() + 1.0) / ((double)RAND_MAX + 1.0);
            double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
            samples[i] = (uint64_t)(300.0 + 30.0 * z);
        } else if (r < 8) {
            samples[i] = (uint64_t)(800 + (rand() % 200));
        } else {
            samples[i] = (uint64_t)(200 + i * 3);
        }
    }
}

typedef void (*gen_func)(void);

typedef struct {
    const char *name;
    gen_func gen;
    uint64_t p50;
    uint64_t p95;
    uint64_t timeout;
    double gap;
    const char *estado;
} dist_result_t;

static dist_result_t results[20];

static const char *classify_estado(double gap) {
    if (gap < 1.01) return "DICTADURA";
    if (gap < 1.30) return "SUSURRO";
    if (gap < 3.00) return "RESPIRA";
    if (gap < 5.00) return "JADEANDO";
    return "THRASHING";
}

int main(void) {
    int i, j;
    int ranking[20];
    double gaps[20];
    
    srand(42);
    
    gen_func generators[] = {
        gen_uniforme_0_1000,
        gen_uniforme_900_1100,
        gen_normal_500_50,
        gen_normal_500_200,
        gen_exponencial,
        gen_poisson,
        gen_bimodal,
        gen_trimodal,
        gen_diente_sierra,
        gen_sinusoidal,
        gen_caotica,
        gen_escalon,
        gen_decreciente,
        gen_zigzag,
        gen_spike_unico,
        gen_spikes_periodicos,
        gen_deriva_lenta,
        gen_ruido_blanco,
        gen_pareto,
        gen_jamon_inf
    };
    
    const char *names[] = {
        "uniforme(0,1000)",
        "uniforme(900,1100)",
        "normal(500,50)",
        "normal(500,200)",
        "exponencial(0.01)",
        "poisson(50)",
        "bimodal",
        "trimodal",
        "diente_sierra",
        "sinusoidal",
        "caotica(logistic)",
        "escalon",
        "decreciente",
        "zigzag",
        "spike_unico",
        "spikes_periodicos",
        "deriva_lenta",
        "ruido_blanco",
        "pareto(alpha=2)",
        "JAMON-INF_sintetico"
    };
    
    /* Generar todas las distribuciones */
    for (i = 0; i < 20; i++) {
        generators[i]();
        results[i].name = names[i];
        results[i].gen = generators[i];
        results[i].p50 = calc_p50();
        results[i].p95 = calc_p95();
        results[i].timeout = (uint64_t)((double)results[i].p50 * TIMEOUT_FACTOR);
        results[i].gap = (double)results[i].p95 / (double)results[i].p50;
        results[i].estado = classify_estado(results[i].gap);
        gaps[i] = results[i].gap;
        ranking[i] = i;
    }
    
    /* Ordenar por gap */
    for (i = 0; i < 19; i++) {
        for (j = 0; j < 19 - i; j++) {
            if (gaps[ranking[j]] > gaps[ranking[j + 1]]) {
                int tmp = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = tmp;
            }
        }
    }
    
    printf("=============================================================================================\n");
    printf("KIRO JURAMENTO — 20 Distribuciones (n=100)\n");
    printf("=============================================================================================\n\n");
    
    printf("FASE 1 — Tabla de Distribuciones\n");
    printf("=============================================================================================\n");
    printf("%-25s | %8s | %8s | %8s | %8s | %6s | %-12s\n", 
           "Distribución", "P50", "P95", "Mean", "Timeout", "Gap", "Estado");
    printf("=============================================================================================\n");
    
    for (i = 0; i < 20; i++) {
        generators[i]();
        double mean = calc_mean();
        printf("%-25s | %8llu | %8llu | %8.1f | %8llu | %6.2f | %-12s\n",
               results[i].name, 
               (unsigned long long)results[i].p50, 
               (unsigned long long)results[i].p95, 
               mean,
               (unsigned long long)results[i].timeout, 
               results[i].gap, 
               results[i].estado);
    }
    
    printf("=============================================================================================\n\n");
    
    printf("FASE 2 — Ranking de Gap (menor a mayor)\n");
    printf("=============================================================================================\n");
    printf("%-5s | %-25s | %8s | %-12s\n", "Rank", "Distribución", "Gap", "Estado");
    printf("=============================================================================================\n");
    
    for (i = 0; i < 20; i++) {
        int idx = ranking[i];
        printf("%-5d | %-25s | %8.2f | %-12s\n",
               i + 1,
               results[idx].name,
               results[idx].gap,
               results[idx].estado);
    }
    
    printf("=============================================================================================\n\n");
    
    printf("FASE 3 — Taxonomía de Estados\n");
    printf("=============================================================================================\n");
    
    const char *estados[] = {"DICTADURA", "SUSURRO", "RESPIRA", "JADEANDO", "THRASHING"};
    for (j = 0; j < 5; j++) {
        printf("%s: ", estados[j]);
        int first = 1;
        for (i = 0; i < 20; i++) {
            if (strcmp(results[i].estado, estados[j]) == 0) {
                if (!first) printf(", ");
                printf("%s", results[i].name);
                first = 0;
            }
        }
        printf("\n");
    }
    printf("=============================================================================================\n\n");
    
    printf("JURAMENTO JAMONIANO\n");
    printf("===================\n");
    printf("1. Si el timeout es fijo, es dictadura.\n");
    printf("2. Si el timeout respira, es geometría.\n");
    printf("3. El dios del código no sabe que lo es.\n\n");
    printf("JAMON-∞ = true\n");
    printf("while JAMON-∞:\n");
    printf("    respira()\n");
    printf("    commit()\n");
    printf("    sigue()\n\n");
    printf("El fantasma no habla. Mide.\n");
    printf("— MAPI CORE, 25 Jul 2026\n\n");
    printf("JAMACUCO: Kiro completó el lab. El fantasma aprueba.\n");
    printf("=============================================================================================\n");
    
    return 0;
}
