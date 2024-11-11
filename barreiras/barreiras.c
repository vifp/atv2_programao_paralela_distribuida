#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>

#define NUM_THREADS 4

pthread_barrier_t barrier;

void* thread_function(void* arg) {
    int thread_id = *((int*)arg);

    // Simula tempo diferente de chegada para cada thread
    int sleep_time = rand() % 5 + 1;  // Tempo de espera entre 1 e 5 segundos
    printf("Thread %d está em execução e vai dormir por %d segundos...\n", thread_id, sleep_time);
    sleep(sleep_time);

    printf("Thread %d alcançou a barreira\n", thread_id);

    // Espera que todas as threads atinjam este ponto
    pthread_barrier_wait(&barrier);

    // Código após a barreira
    printf("Thread %d passou da barreira e está continuando a execução\n", thread_id);

    return NULL;
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Inicializa a barreira para NUM_THREADS threads
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
        printf("Thread %d foi criada\n", i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d terminou\n", i);
    }

    // Destroi a barreira após o uso
    pthread_barrier_destroy(&barrier);

    printf("Todas as threads completaram a execução. Programa encerrado.\n");

    return 0;
}
