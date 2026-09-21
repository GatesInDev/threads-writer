#include "../../include/plataform.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>

struct Process
{
    HANDLE hProcess;
    HANDLE hThread;
    DWORD pid;
    int is_running;
};

int process_create(Process *process)
{
    if (process == NULL)
    {
        return -1;
    }

    process->hProcess = NULL;
    process->hThread = NULL;
    process->pid = 0;
    process->is_running = 0;

    return 0;
}

Process* process_new(void)
{
  Process *p = (Process *)malloc(sizeof(struct Process));
  if (p != NULL)
  {
    p->hProcess = NULL;
    p->hThread = NULL;
    p->pid = 0;
    p->is_running = 0;
  }
  return p;
}

void process_exec(Process *process, const char *command, char *const argv[])
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    char cmdline[1024];
    int i;

    if (process == NULL || command == NULL)
    {
        return;
    }

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    cmdline[0] = '\0';
    if (argv != NULL)
    {
        i = 0;
        while (argv[i] != NULL)
        {
            strcat(cmdline, argv[i]);
            strcat(cmdline, " ");
            i++;
        }
    }
    else
    {
        strcpy(cmdline, command);
    }

    if (CreateProcessA(
            NULL,
            cmdline,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi)
        != 0)
    {
        process->hProcess = pi.hProcess;
        process->hThread = pi.hThread;
        process->pid = pi.dwProcessId;
        process->is_running = 1;
    }
    else
    {
        process->is_running = 0;
    }
}

int process_wait(Process *process)
{
    DWORD exit_code;

    if (process == NULL || !process->is_running || process->hProcess == NULL)
    {
        return -1;
    }

    WaitForSingleObject(process->hProcess, INFINITE);

    if (GetExitCodeProcess(process->hProcess, &exit_code))
    {
        process->is_running = 0;

        if (process->hThread != NULL)
        {
            CloseHandle(process->hThread);
            process->hThread = NULL;
        }

        return (int)exit_code;
    }

    return -1;
}

int process_kill(Process *process)
{
    if (process == NULL || !process->is_running || process->hProcess == NULL)
    {
        return -1;
    }

    if (TerminateProcess(process->hProcess, 1))
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

        /* Libera o Handle do processo no SO */
        if (process->hProcess != NULL)
        {
            CloseHandle(process->hProcess);
            process->hProcess = NULL;
        }

        free(process);
    }
}