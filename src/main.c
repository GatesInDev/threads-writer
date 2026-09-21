#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/plataform.h"

int *lista_ids = NULL;
int total_ids = 0;
int indice_atual = 0;
mutex_t mutex_leitura;
mutex_t mutex_escrita;
FILE *arquivo_log = NULL;

void consultar_api_mock(int id, char *buffer) {
    sprintf(buffer, "{\"id\": %d, \"status\": \"ok\", \"valor\": %.2f}", id, (float)id * 1.5);
}

void* rotina_thread(void* arg) {
    int id_da_thread = (int)(long)arg;
    char resposta_json[256];
    char string_tempo[64];
    time_t tempo_atual;
    struct tm *info_tempo;

    while (1) {
        mutex_lock(&mutex_leitura);
        if (indice_atual >= total_ids) {
            mutex_unlock(&mutex_leitura);
            break;
        }
        int id_processado = lista_ids[indice_atual];
        indice_atual++;
        mutex_unlock(&mutex_leitura);

        consultar_api_mock(id_processado, resposta_json);

        time(&tempo_atual);
        info_tempo = localtime(&tempo_atual);
        strftime(string_tempo, sizeof(string_tempo), "%Y-%m-%d %H:%M:%S", info_tempo);

        mutex_lock(&mutex_escrita);
        fprintf(arquivo_log, "%s, Thread-%d, %d, %s\n", string_tempo, id_da_thread, id_processado, resposta_json);
        fflush(arquivo_log);
        mutex_unlock(&mutex_escrita);
    }
    return NULL;
}

int executar_p1(int n_threads, const char* arquivo_entrada, const char* arquivo_saida) {
    mutex_init(&mutex_leitura);
    mutex_init(&mutex_escrita);

    FILE *f_entrada = fopen(arquivo_entrada, "r");
    if (!f_entrada) {
        printf("Erro ao abrir lista de IDs.\n");
        return 1;
    }

    int capacidade = 100;
    lista_ids = malloc(capacidade * sizeof(int));
    while (fscanf(f_entrada, "%d", &lista_ids[total_ids]) == 1) {
        total_ids++;
        if (total_ids >= capacidade) {
            capacidade *= 2;
            lista_ids = realloc(lista_ids, capacidade * sizeof(int));
        }
    }
    fclose(f_entrada);

    arquivo_log = fopen(arquivo_saida, "w");

    thread_t *threads = malloc(n_threads * sizeof(thread_t));
    for (int i = 0; i < n_threads; i++) {
        thread_create(&threads[i], rotina_thread, (void*)(long)(i + 1));
    }

    for (int i = 0; i < n_threads; i++) {
        thread_join(threads[i]);
    }

    fclose(arquivo_log);
    free(lista_ids);
    free(threads);
    mutex_destroy(&mutex_leitura);
    mutex_destroy(&mutex_escrita);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc >= 4 && strcmp(argv[1], "p1") == 0) {
        int num_threads = atoi(argv[2]);
        return executar_p1(num_threads, argv[3], argv[4]);
    } else {
        printf("[P0] Iniciando orquestração...\n");

        char comando[512];

        printf("[P0] Disparando P1 com 1 Thread...\n");
        sprintf(comando, "%s p1 1 lista_ids.txt log_1_thread.txt", argv[0]);
        system(comando);

        int N_THREADS = 16;
        printf("[P0] Disparando P1 com %d Threads...\n", N_THREADS);
        sprintf(comando, "%s p1 %d lista_ids.txt log_N_threads.txt", argv[0], N_THREADS);
        system(comando);

        printf("[P0] Execuções finalizadas. Verifique os logs gerados.\n");
        return 0;
    }
}