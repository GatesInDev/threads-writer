#ifndef PLATAFORM_H
#define PLATAFORM_H

typedef struct Process Process;

int process_create(Process *process);
int process_wait(Process *process);
int process_kill(Process *process);
void process_destroy(Process *process);

typedef struct Thread Thread;

int thread_create(Thread *thread, void (*function)(void *), void *arg);
int thread_join(Thread *thread);
void thread_destroy(Thread *thread);

Process* process_new(void);
void process_exec(Process *process, const char *command, char *const argv[]);

#endif
