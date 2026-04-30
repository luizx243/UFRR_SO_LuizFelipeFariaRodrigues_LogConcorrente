#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define TAMANHO_FILA 5
#define NUM_PRODUTORES 3
#define LOGS_POR_PRODUTOR 4

char fila[TAMANHO_FILA][256];
int contagem = 0;
int entrada = 0;
int saida = 0;

pthread_mutex_t mutex;
pthread_cond_t pode_produzir;
pthread_cond_t pode_consumir;

FILE *arquivo_log;

void* produtor(void* arg) {
    int id = *(int*)arg;

    for (int i = 0; i < LOGS_POR_PRODUTOR; i++) {
        pthread_mutex_lock(&mutex);

        while (contagem == TAMANHO_FILA) {
            pthread_cond_wait(&pode_produzir, &mutex);
        }

        sprintf(fila[entrada], "Log da thread %d: evento %d\n", id, i + 1);
        entrada = (entrada + 1) % TAMANHO_FILA;
        contagem++;

        pthread_cond_signal(&pode_consumir);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
    return NULL;
}

void* consumidor(void* arg) {
    while (1) {
        char log_local[256];

        pthread_mutex_lock(&mutex);

        while (contagem == 0) {
            pthread_cond_wait(&pode_consumir, &mutex);
        }
        
        strcpy(log_local, fila[saida]);

        saida = (saida + 1) % TAMANHO_FILA;
        contagem--;

        pthread_cond_signal(&pode_produzir);
        pthread_mutex_unlock(&mutex);

        fprintf(arquivo_log, "%s", log_local);
        fflush(arquivo_log);
    }
    return NULL;
}

int main() {
    pthread_t produtores[NUM_PRODUTORES];
    pthread_t thread_consumidor;
    int ids[NUM_PRODUTORES];

    arquivo_log = fopen("log.txt", "w");
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&pode_produzir, NULL);
    pthread_cond_init(&pode_consumir, NULL);

    pthread_create(&thread_consumidor, NULL, consumidor, NULL);

    for (int i = 0; i < NUM_PRODUTORES; i++) {
        ids[i] = i + 1;
        pthread_create(&produtores[i], NULL, produtor, &ids[i]);
    }

    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(produtores[i], NULL);
    }

    sleep(1); 

    fclose(arquivo_log);
    printf("log.txt foi gerado.\n");

    return 0;
}

//Compilar gcc -o logger log.c -pthread
//Executar ./logger
//Ler cat log.txt