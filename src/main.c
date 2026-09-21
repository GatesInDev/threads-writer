#include "../include/plataform.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  Process *proc;
  int status;

  /* Argumentos do comando no Windows:
     Equivalente a rodar um ping no terminal para aguardar 3 segundos */
  char *const args[] = {"cmd.exe", "/c", "ping", "-n", "3", "127.0.0.1", NULL};

  printf("=== Teste do Módulo de Processos (Windows) ===\n");

  /* 1. Aloca o processo opaco com process_new() */
  proc = process_new();
  if (proc == NULL)
  {
    printf("Erro ao alocar memoria para o processo.\n");
    return 1;
  }

  /* 2. Inicializa a estrutura */
  process_create(proc);

  /* 3. Executa o comando no Windows */
  printf("[Main] Disparando o comando no Windows...\n");
  process_exec(proc, "cmd.exe", args);

  /* 4. Aguarda a finalização do processo */
  printf("[Main] Processo criado. Aguardando conclusao...\n");
  status = process_wait(proc);

  printf("[Main] O processo finalizou com o codigo de saida: %d\n", status);

  /* 5. Destrói e libera a memória alocada */
  process_destroy(proc);
  printf("[Main] Recursos liberados com sucesso.\n");

  return 0;
}
