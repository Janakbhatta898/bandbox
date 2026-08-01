#include<stdio.h>
#include<unistd.h> //for linux kernal porcess like fork
#include<stdlib.h> 
#include<sys/stat.h> //for mkdir
#include<sys/types.h>

#include "cgroups.h"

#define cgroup_root "/sys/fs/cgroup"
#define cgroup_path "/sys/fs/cgroup/bandbox"



int mkdir_cgroups(){
    if(mkdir(cgroup_path, 0755)==-1){
        perror("cgroup for bandbox is already exists");
        return -1;
    }
    return 0;

}

static int write_to_file(const char *path,const char *value){
    FILE *fp=fopen(path,"w");
    if(fp==NULL){
        perror("Failed to write  file");
        return -1;

    }
    fprintf(fp,"%s",value);
    printf("Sucessfully implemented to: %s\n",path);
    fclose(fp);
    return 0;
}

void max_pids(){
    write_to_file(cgroup_path"/pids.max","100\n");
   return;
}

int attach_process_to_cgroup_procs(pid_t pid) {
   // 1. Format the integer PID into a string
   char pid_str[32];
    snprintf(pid_str, sizeof(pid_str), "%d\n", pid); // or sprintf -> can cause memory overflow
    
    // linux store every data in character so need to convert integer pid to str which above line of code does
    return write_to_file("/sys/fs/cgroup/bandbox/cgroup.procs", pid_str);
}

int setup_cgroups(){

   
//enable controllers in parent cgroup i.e cgroup.subtree_control so that the child process can excess the controllers of banbox
    if(write_to_file(cgroup_root"/cgroup.subtree_control","+cpu +memory +pids")==-1) return -1;
    mkdir_cgroups();   

    max_pids();

   
    return 0;
}
