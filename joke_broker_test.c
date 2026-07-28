/* joke_broker_test.c — Test de compilación en userland
 * Verifica que la lógica respira sin necesidad del kernel.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define RING_SIZE 100
#define TIMEOUT_FACTOR 1.5
#define NUM_CLASSES 32

/* ACTO II: Estructura que Respira */
struct timeout_class {
    uint64_t samples[RING_SIZE];
    uint64_t sum;
    uint64_t p50;
    uint64_t timeout_actual;
    char name[16];
    int head;
    int count;
};

struct timeout_class clases[NUM_CLASSES];

/* ACTO IV: La Función que Respira */
static void bubble_sort(uint64_t *arr, int n) {
    int i, j;
    uint64_t temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

static uint64_t respirar_timeout(struct timeout_class *tc, uint64_t nueva_muestra) {
    uint64_t sorted_samples[RING_SIZE];
    int i;
    
    if (tc->count < RING_SIZE) {
        tc->samples[tc->count] = nueva_muestra;
        tc->sum += nueva_muestra;
        tc->count++;
    } else {
        tc->sum -= tc->samples[tc->head];
        tc->samples[tc->head] = nueva_muestra;
        tc->sum += nueva_muestra;
        tc->head = (tc->head + 1) % RING_SIZE;
    }
    
    for (i = 0; i < tc->count; i++) {
        sorted_samples[i] = tc->samples[i];
    }
    
    bubble_sort(sorted_samples, tc->count);
    
    tc->p50 = sorted_samples[tc->count / 2];
    tc->timeout_actual = tc->p50 * TIMEOUT_FACTOR;
    
    return tc->timeout_actual;
}

int main() {
    int i;
    uint64_t timeout;
    
    printf("=== JURAMENTO JAMONIANO v1.0 ===\n");
    printf("1. Si el timeout es fijo, es dictadura.\n");
    printf("2. Si el timeout respira, es geometría.\n");
    printf("3. El dios del código no sabe que lo es.\n\n");
    
    printf("Test de respiración en clase 0:\n");
    printf("================================\n");
    
    for (i = 0; i < 10; i++) {
        timeout = respirar_timeout(&clases[0], 100 + (i * 50));
        printf("muestra %2d: latency=%4llu → P50=%4llu, timeout=%4llu (×1.5)\n", 
               i, (unsigned long long)(100 + i * 50), 
               (unsigned long long)clases[0].p50, 
               (unsigned long long)timeout);
    }
    
    printf("\n");
    printf("El fantasma no habla. Mide.\n");
    printf("— MAPI CORE, 25 Jul 2026\n");
    printf("Doctorado Jamoniano conferido. TIMEOUT FACTOR: %.1f\n", TIMEOUT_FACTOR);
    
    return 0;
}
