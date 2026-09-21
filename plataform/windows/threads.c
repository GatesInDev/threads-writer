#include "../../include/plataform.h"

int thread_create(thread_t *thread, thread_func_t func, void *arg) {
  *thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, arg, 0, NULL);
  if (*thread == NULL) {
    return -1;
  }
  return 0;
}

int thread_join(thread_t thread) {
  WaitForSingleObject(thread, INFINITE);
  CloseHandle(thread);
  return 0;
}