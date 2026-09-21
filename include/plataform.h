#ifndef PLATAFORM_H
#define PLATAFORM_H

#if defined(_WIN32) || defined(_WIN64)
  #include <windows.h>
  typedef HANDLE thread_t;
  typedef CRITICAL_SECTION mutex_t;
#else
  #include <pthread.h>
  typedef pthread_t thread_t;
  typedef pthread_mutex_t mutex_t;
#endif

typedef struct Process Process;

int process_create(Process *process);
int process_wait(Process *process);
int process_kill(Process *process);
void process_destroy(Process *process);

typedef struct Thread Thread;

typedef void* (*thread_func_t)(void*);
int thread_create(thread_t *thread, thread_func_t func, void *arg);
int thread_join(thread_t thread);
void thread_destroy(Thread *thread);

int mutex_init(mutex_t *mutex);
int mutex_lock(mutex_t *mutex);
int mutex_unlock(mutex_t *mutex);
int mutex_destroy(mutex_t *mutex);

Process* process_new(void);
void process_exec(Process *process, const char *command, char *const argv[]);

#endif
