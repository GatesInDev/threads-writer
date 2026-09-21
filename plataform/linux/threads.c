#include "../../include/plataform.h"

int thread_create(thread_t *thread, thread_func_t func, void *arg) {
  return pthread_create(thread, NULL, func, arg);
}

int thread_join(thread_t thread) {
  return pthread_join(thread, NULL);
}