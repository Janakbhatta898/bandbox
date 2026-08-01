#include "namespaces.h"
#include "cgroups.h"
#include "seccomp_filter.h"

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/mount.h>
#include<errno.h>

int child_func(void *arg){
	printf("We are inside sandbox child process\n");

	
	const char *hostname = "bandbox";

	if(sethostname(hostname, strlen(hostname))==-1){
		perror("sethostname");
		return -1;
	}

	char check_changed_hostname[64];

	if(gethostname(check_changed_hostname, sizeof(check_changed_hostname))==-1){
		perror("gethostname");
		return -1;
	}

	printf("Bandbox hostname: %s\n", check_changed_hostname);

	if(mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) == -1){
		perror("mount MS_PRIVATE");
		return -1;
	}

	if(umount2("/proc", MNT_DETACH) == -1){
		if(errno!=EINVAL){//if proc is absent ignore unmount
			perror("umount2 /proc");
			return -1;
		}
	}


	if(mount("proc", "/proc", "proc", 0, NULL) == -1){
		perror("mount");
		return -1;
	}

    if (apply_seccomp_filter() == -1){
        perror("Failed to apply seccomp filter.");
        return -1;
    }

	char **args = (char **)arg;
	fflush(stdout);

	if(execvp(args[1], &args[1]) == -1){
		perror("execvp");
		return -1;
	}
	return 0;
}


int main(int argc, char *argv[]){
	//the thing inside comment was for testing only if you want you can do too
	/*char current_hostname[64];

	if(gethostname(current_hostname, sizeof(current_hostname))==-1){
		perror("gethostname");
		return -1;
	}

	printf("Parent hostname: %s\n\n",current_hostname);

	printf("Parent: creating bandbox\n");
	*/

	if(argc < 2){
		printf("Usage: %s <program> [arguments...]\n", argv[0]);
		return 1;
	}
	setup_cgroups();



	if(start_bandbox(child_func, argv) == -1){
		printf("failed to create a bandbox\n");
		return 1;
	}
	/*

	printf("Parent: child finished\n");

	if(gethostname(current_hostname, sizeof(current_hostname))==-1){
		perror("gethostname");
		return -1;
	}

	printf("\nParent hostname: %s\n", current_hostname);
	setup_cgroups();
	*/
	return 0;
}
