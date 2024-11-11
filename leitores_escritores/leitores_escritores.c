#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <locale.h>

sem_t rw_mutex; // Sem�foro para garantir acesso exclusivo � se��o cr�tica para escritores
sem_t mutex;    // Sem�foro para garantir exclus�o m�tua ao contador de leitores
int read_count = 0; // Contador de leitores que est�o lendo no momento
int shared_data = 0; // Dados compartilhados que ser�o lidos e escritos

void* reader(void* arg) {
    int reader_id = *((int*)arg);
    while (1) {
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1) {
            sem_wait(&rw_mutex); // O primeiro leitor bloqueia os escritores
        }
        sem_post(&mutex);

        // Se��o cr�tica de leitura
        printf("Leitor %d est� lendo o dado: %d\n", reader_id, shared_data);
        sleep(1); // Simula o tempo de leitura

        sem_wait(&mutex);
        read_count--;
        if (read_count == 0) {
            sem_post(&rw_mutex); // O �ltimo leitor libera os escritores
        }
        sem_post(&mutex);

        sleep(1); // Simula intervalo antes de tentar ler novamente
    }
    return NULL;
}

void* writer(void* arg) {
    int writer_id = *((int*)arg);
    while (1) {
        sem_wait(&rw_mutex); // Garante que nenhum leitor ou escritor acesse a se��o cr�tica
        shared_data++; // Se��o cr�tica de escrita
        printf("Escritor %d est� escrevendo o dado: %d\n", writer_id, shared_data);
        sleep(2); // Simula o tempo de escrita
        sem_post(&rw_mutex); // Libera a se��o cr�tica para outros

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

    // Cria��o das threads de leitores
    for (int i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Cria��o das threads de escritores
    for (int i = 0; i < 2; i++) {
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Aguarda as threads (nunca acontecer� neste exemplo, pois as threads s�o infinitas)
    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroi os sem�foros
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);

    return 0;
}
