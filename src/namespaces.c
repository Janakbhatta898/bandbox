
#include "namespaces.h"

#include<sched.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdio.h>

int start_bandbox(int (*child_func)(void *), void *arg){
	char *stack = malloc(STACKSIZE);

	if(stack == NULL){
		perror("malloc");
		return -1;
	}

	char *stack_top = stack + STACKSIZE;

	pid_t child_pid = clone(
			child_func,
			stack_top,
			CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWUTS | CLONE_NEWIPC | SIGCHLD,
			arg
			);
	
	if(child_pid == -1){
		perror("clone");
		free(stack);
		return -1;
	}

	if(waitpid(child_pid, NULL, 0) == -1){
		perror("waitpid");
		free(stack);
		return -1;
	}

	free(stack);
	return 0;

}
