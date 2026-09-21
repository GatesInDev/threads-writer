#include "../../include/plataform.h"

int mutex_init(mutex_t *mutex) {
  return pthread_mutex_init(mutex, NULL);
}

int mutex_lock(mutex_t *mutex) {
  return pthread_mutex_lock(mutex);
}

int mutex_unlock(mutex_t *mutex) {
  return pthread_mutex_unlock(mutex);
}

int mutex_destroy(mutex_t *mutex) {
  return pthread_mutex_destroy(mutex);
}