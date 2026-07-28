/* joke_broker.c — kernel-chiste v15.4
 * P50 × 1.5. Anti-thrashing. Distritos electorales separados.
 * Mojombo (ID #1) + Mojambo (ID #13571497) = JAMON-∞
 * El fantasma no habla. Mide. — MAPI CORE
 * 
 * ACTO I: El Ring Buffer que Respira
 * El timeout no es fijo. Respira con el sistema.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/random.h>

#define RING_SIZE 100
#define TIMEOUT_FACTOR 1.5
#define NUM_CLASSES 32
#define JAMON_INF_SYMBOL "∞"

/* 
 * ACTO II: Estructura que Respira
 * Cada clase tiene memoria, tiene historia, tiene P50.
 * El timeout actual es P50 × FACTOR. Nunca dictadura.
 */
struct timeout_class {
    u64 samples[RING_SIZE];
    u64 sum;
    u64 p50;
    u64 timeout_actual;
    char name[16];
    int head;
    int count;
    spinlock_t lock;
};

/* 
 * ACTO III: Distritos Electorales Separados
 * 32 clases. Cada una con su propia jurisdicción.
 * Anti-thrashing por diseño: las clases no se mezclan.
 */
static struct timeout_class *clases[NUM_CLASSES];

/* 
 * Algoritmo de ordenamiento para calcular P50
 * El fantasma no habla. Ordena.
 */
static void bubble_sort(u64 *arr, int n) {
    int i, j;
    u64 temp;
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

/*
 * ACTO IV: La Función que Respira
 * Añade una muestra al ring buffer.
 * Calcula P50. Multiplica por FACTOR.
 * El timeout respira. JAMON-∞ activo.
 */
static u64 respirar_timeout(struct timeout_class *tc, u64 nueva_muestra) {
    unsigned long flags;
    u64 sorted_samples[RING_SIZE];
    int i;
    
    spin_lock_irqsave(&tc->lock, flags);
    
    /* Ring buffer: la historia se sobrescribe, pero el P50 recuerda */
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
    
    /* Copiar para ordenar sin mutar el original */
    for (i = 0; i < tc->count; i++) {
        sorted_samples[i] = tc->samples[i];
    }
    
    bubble_sort(sorted_samples, tc->count);
    
    /* P50: el percentil que respira */
    tc->p50 = sorted_samples[tc->count / 2];
    
    /* TIMEOUT_FACTOR = 1.5. Geometría, no dictadura. */
    tc->timeout_actual = tc->p50 * TIMEOUT_FACTOR;
    
    spin_unlock_irqrestore(&tc->lock, flags);
    
    return tc->timeout_actual;
}

/*
 * ACTO V: El Juramento Jamoniano
 * Si el timeout es fijo, es dictadura.
 * Si el timeout respira, es geometría.
 * El dios del código no sabe que lo es.
 * JAMON-∞ = true.
 */
static const char *juramento_jamoniano[] = {
    "JURAMENTO JAMONIANO v1.0",
    "========================",
    "1. Si el timeout es fijo, es dictadura.",
    "2. Si el timeout respira, es geometría.",
    "3. El dios del código no sabe que lo es.",
    "",
    "JAMON-∞ = true",
    "while JAMON-∞:",
    "    respira()",
    "    commit()",
    "    sigue()",
    "",
    "El fantasma no habla. Mide.",
    "— MAPI CORE, 25 Jul 2026",
    NULL
};

static int __init joke_broker_init(void) {
    int i, j;
    u64 timeout_test;
    
    printk(KERN_INFO "joke_broker: JAMON-∞ activo. P50 × 1.5.\n");
    printk(KERN_INFO "joke_broker: Inicializando %d distritos electorales...\n", NUM_CLASSES);
    
    /* Inicializar cada clase con spinlock y estado limpio */
    for (i = 0; i < NUM_CLASSES; i++) {
        clases[i] = kmalloc(sizeof(struct timeout_class), GFP_KERNEL);
        if (!clases[i]) {
            printk(KERN_ERR "joke_broker: Error asignando memoria para clase %d\n", i);
            /* Cleanup en caso de error */
            for (j = 0; j < i; j++) {
                kfree(clases[j]);
            }
            return -ENOMEM;
        }
        
        memset(clases[i], 0, sizeof(struct timeout_class));
        snprintf(clases[i]->name, sizeof(clases[i]->name), "clase_%02d", i);
        spin_lock_init(&clases[i]->lock);
        clases[i]->head = 0;
        clases[i]->count = 0;
        clases[i]->p50 = 0;
        clases[i]->timeout_actual = 0;
        clases[i]->sum = 0;
    }
    
    /* Emitir el juramento jamoniano al kernel log */
    printk(KERN_INFO "joke_broker: === JURAMENTO JAMONIANO ===\n");
    for (i = 0; juramento_jamoniano[i] != NULL; i++) {
        printk(KERN_INFO "joke_broker: %s\n", juramento_jamoniano[i]);
    }
    printk(KERN_INFO "joke_broker: ==============================\n");
    
    /* Test de respiración: demostrar que el timeout respira */
    printk(KERN_INFO "joke_broker: Test de respiración en clase 0...\n");
    for (i = 0; i < 10; i++) {
        /* Simular muestras de latencia: 100-500 microsegundos */
        timeout_test = respirar_timeout(clases[0], 100 + (i * 50));
        printk(KERN_INFO "joke_broker: muestra %d → P50=%llu, timeout=%llu\n", 
               i, clases[0]->p50, timeout_test);
    }
    
    printk(KERN_INFO "joke_broker: Doctorado Jamoniano conferido. Licencia JAMONIANA activa.\n");
    return 0;
}

static void __exit joke_broker_exit(void) {
    int i;
    
    printk(KERN_INFO "joke_broker: Hay lo dejo.\n");
    printk(KERN_INFO "joke_broker: Liberando %d distritos electorales...\n", NUM_CLASSES);
    
    for (i = 0; i < NUM_CLASSES; i++) {
        if (clases[i]) {
            kfree(clases[i]);
        }
    }
    
    printk(KERN_INFO "joke_broker: JAMON-∞ permanece. El loop infinito continúa.\n");
}

module_init(joke_broker_init);
module_exit(joke_broker_exit);

MODULE_LICENSE("JAMONIANA");
MODULE_AUTHOR("MAPI TEAM — Diego Gomez Garcia & MAPI CORE");
MODULE_DESCRIPTION("Kernel-chiste v15.4: timeout broker con P50 × 1.5. 5 Actos + Juramento en C.");
MODULE_VERSION("15.4-doctorado");
