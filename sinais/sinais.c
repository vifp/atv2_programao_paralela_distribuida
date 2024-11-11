#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int ready = 0;

void* thread_wait(void* arg) {
    pthread_mutex_lock(&mutex);
    while (!ready) {
        printf("Thread esperando o sinal...\n");
        pthread_cond_wait(&condition, &mutex);
    }
    printf("Thread recebeu o sinal e continuou a execução.\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* thread_signal(void* arg) {
    sleep(2);
    pthread_mutex_lock(&mutex);
    ready = 1;
    printf("Enviando sinal para a outra thread.\n");
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread_wait, NULL);
    pthread_create(&t2, NULL, thread_signal, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);
    return 0;
}
