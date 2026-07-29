#include<stdio.h>
#include<unistd.h> //for linux kernal porcess like fork
#include<stdlib.h> 
#include<sys/stat.h> //for mkdir
#include<sys/types.h>

#include "cgroups.h"

#define cgroup_path "/sys/fs/cgroup/testing"



void mkdir_cgroups(){
    if(mkdir(cgroup_path, 0755)==-1){
        perror("cgroup for bandbox is already exists");
    }
    return;

}
void max_pids(){
    FILE *fp=fopen(cgroup_path"/pids.max","w");
    if(fp==NULL){
        perror("max process setup failed in pids.max");
        return;
    }
    fprintf(fp,"9\n");
    fclose(fp);
    return;
}
void setup_cgroups(void){

    mkdir_cgroups();
    max_pids();
   
    return;
}
