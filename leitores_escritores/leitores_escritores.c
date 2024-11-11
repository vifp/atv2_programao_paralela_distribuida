#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <locale.h>

sem_t rw_mutex; // Semáforo para garantir acesso exclusivo à seção crítica para escritores
sem_t mutex;    // Semáforo para garantir exclusão mútua ao contador de leitores
int read_count = 0; // Contador de leitores que estão lendo no momento
int shared_data = 0; // Dados compartilhados que serão lidos e escritos

void* reader(void* arg) {
    int reader_id = *((int*)arg);
    while (1) {
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1) {
            sem_wait(&rw_mutex); // O primeiro leitor bloqueia os escritores
        }
        sem_post(&mutex);

        // Seção crítica de leitura
        printf("Leitor %d está lendo o dado: %d\n", reader_id, shared_data);
        sleep(1); // Simula o tempo de leitura

        sem_wait(&mutex);
        read_count--;
        if (read_count == 0) {
            sem_post(&rw_mutex); // O último leitor libera os escritores
        }
        sem_post(&mutex);

        sleep(1); // Simula intervalo antes de tentar ler novamente
    }
    return NULL;
}

void* writer(void* arg) {
    int writer_id = *((int*)arg);
    while (1) {
        sem_wait(&rw_mutex); // Garante que nenhum leitor ou escritor acesse a seção crítica
        shared_data++; // Seção crítica de escrita
        printf("Escritor %d está escrevendo o dado: %d\n", writer_id, shared_data);
        sleep(2); // Simula o tempo de escrita
        sem_post(&rw_mutex); // Libera a seção crítica para outros

        sleep(2); // Simula intervalo antes de tentar escrever novamente
    }
    return NULL;
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    pthread_t readers[5], writers[2];
    int reader_ids[5] = {1, 2, 3, 4, 5};
    int writer_ids[2] = {1, 2};

    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);

    // Criação das threads de leitores
    for (int i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Criação das threads de escritores
    for (int i = 0; i < 2; i++) {
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Aguarda as threads (nunca acontecerá neste exemplo, pois as threads são infinitas)
    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroi os semáforos
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);

    return 0;
}
