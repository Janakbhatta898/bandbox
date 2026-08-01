#ifndef CGROUPS_H
#define CGROUPS_H
#include<sys/types.h>


int setup_cgroups();
int mkdir_cgroups();
void max_pids();
int attach_process_to_cgroup_procs(pid_t pid);

#endif
