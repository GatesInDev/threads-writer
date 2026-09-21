#include "../../include/plataform.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

struct Process
{
  pid_t pid;
  int is_running;
};

int process_create(Process *process)
{
  pid_t pid;

  if (process == NULL)
  {
    return -1;
  }

  pid = fork();

  if (pid < 0)
  {
    process->is_running = 0;
    return -1;
  }
  else if (pid == 0)
  {
    return 0;
  }
  else
  {
    process->pid = pid;
    process->is_running = 1;
    return (int)pid;
  }
}

Process* process_new(void)
{
  Process *p = (Process *)malloc(sizeof(struct Process));
  if (p != NULL)
  {
    p->pid = 0;
    p->is_running = 0;
  }
  return p;
}

void process_exec(Process *process, const char *command, char *const argv[])
{
  (void)process;
  execvp(command, argv);
  exit(EXIT_FAILURE);
}

int process_wait(Process *process)
{
  int status;
  pid_t result;

  if (process == NULL || !process->is_running)
  {
    return -1;
  }

  result = waitpid(process->pid, &status, 0);

  if (result > 0)
  {
    process->is_running = 0;
    if (WIFEXITED(status))
    {
      return WEXITSTATUS(status);
    }
  }

  return -1;
}

int process_kill(Process *process)
{
  if (process == NULL || !process->is_running)
  {
    return -1;
  }

  if (kill(process->pid, SIGKILL) == 0)
  {
    process->is_running = 0;
    return 0;
  }

  return -1;
}

void process_destroy(Process *process)
{
  if (process != NULL)
  {
    if (process->is_running)
    {
      process_kill(process);
      process_wait(process);
    }

    free(process);
  }
}