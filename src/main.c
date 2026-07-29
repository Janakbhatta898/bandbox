//Temporary test only

#include "namespaces.h"
#include<stdio.h>

int test_child(void *arg){
	printf("We are inside sandbox child process\n");
	return 0;
}
int main(){
	printf("Parent: creating bandbox\n");
	if(start_bandbox(test_child, NULL) == -1){
		printf("faild to create a bandbox\n");
		return 1;
	}
	printf("Parent: child finished\n");
	return 0;
}

