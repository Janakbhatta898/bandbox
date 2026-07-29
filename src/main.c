#include<stdio.h>
#include "namespaces.h"
#include "cgroups.h"

int main(){
    printf("it's main.c working\n");
    initial_message();
    setup_cgroups();
    return 0;


}