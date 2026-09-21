#include "../../include/plataform.h"

int mutex_init(mutex_t *mutex) {
  InitializeCriticalSection(mutex);
  return 0; // WIN32 init não retorna erro comum
}

int mutex_lock(mutex_t *mutex) {
  EnterCriticalSection(mutex);
  return 0;
}

int mutex_unlock(mutex_t *mutex) {
  LeaveCriticalSection(mutex);
  return 0;
}

int mutex_destroy(mutex_t *mutex) {
  DeleteCriticalSection(mutex);
  return 0;
}