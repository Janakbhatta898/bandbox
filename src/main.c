#include "namespaces.h"
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include "cgroups.h"

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

	return 0;
}
int main(){
	char current_hostname[64];

	if(gethostname(current_hostname, sizeof(current_hostname))==-1){
		perror("gethostname");
		return -1;
	}

	printf("Parent hostname: %s\n\n",current_hostname);

	printf("Parent: creating bandbox\n");

	if(start_bandbox(child_func, NULL) == -1){
		printf("faild to create a bandbox\n");
		return 1;
	}

	printf("Parent: child finished\n");

	if(gethostname(current_hostname, sizeof(current_hostname))==-1){
		perror("gethostname");
		return -1;
	}

	printf("\nParent hostname: %s\n", current_hostname);
	setup_cgroups();
	return 0;
}
