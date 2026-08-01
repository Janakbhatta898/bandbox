#include "namespaces.h"
#include "cgroups.h"

#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct
{
    int (*child_func)(void *);
    void *arg;
    int sync_pipe[2];
} trampoline_args;

static int trampoline(void *raw_args)
{
    trampoline_args *targs = (trampoline_args *)raw_args;

    close(targs->sync_pipe[1]);   // Child only reads.

    char dummy;

    // wait until the parent finishes attaching us to the cgroup.
    if (read(targs->sync_pipe[0], &dummy, 1) == -1)
    {
        perror("read");
        close(targs->sync_pipe[0]);
        return -1;
    }

    close(targs->sync_pipe[0]);

    return targs->child_func(targs->arg);
}

int start_bandbox(int (*child_func)(void *), void *arg)
{
    char *stack = malloc(STACKSIZE);

    if (stack == NULL)
    {
        perror("malloc");
        return -1;
    }

    trampoline_args targs;
    targs.child_func = child_func;
    targs.arg = arg;

    if (pipe(targs.sync_pipe) == -1)
    {
        perror("pipe");
        free(stack);
        return -1;
    }

    char *stack_top = stack + STACKSIZE;

    pid_t child_pid = clone(
        trampoline,
        stack_top,
        CLONE_NEWPID |
        CLONE_NEWNS |
        CLONE_NEWUTS |
        CLONE_NEWIPC |
		CLONE_NEWNET |
        SIGCHLD,
        &targs);

    if (child_pid == -1)
    {
        perror("clone");
        close(targs.sync_pipe[0]);
        close(targs.sync_pipe[1]);
        free(stack);
        return -1;
    }

    close(targs.sync_pipe[0]);  

    // attach the child to the cgroup before it executes user code
    if (attach_process_to_cgroup_procs(child_pid) == -1)
    {
        perror("attach_process_to_cgroup_procs");

        close(targs.sync_pipe[1]);
        waitpid(child_pid, NULL, 0);
        free(stack);
        return -1;
    }

 
    close(targs.sync_pipe[1]);

    if (waitpid(child_pid, NULL, 0) == -1)
    {
        perror("waitpid");
        free(stack);
        return -1;
    }

    free(stack);
    return 0;
}
