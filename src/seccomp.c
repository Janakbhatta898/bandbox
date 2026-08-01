#include <stdio.h>
#include <string.h>
                         
#include <seccomp.h>
#include <sys/prctl.h>
#include <errno.h>
#include "seccomp_filter.h"

int apply_seccomp_filter(void) {

    // Stop from gaining more privilages
    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) == -1){
        perror("prctl");
        return -1;
    }

    // We implement a denylist strategy: default action is ALLOW, and we
    // explicitly block the syscalls below. (Earlier attempts with
    // SCMP_ACT_KILL / SCMP_ACT_ERRNO as the default action killed bash
    // outright, since a shell needs far more syscalls than a short
    // allowlist accounts for.)

    scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
    if (ctx == NULL) {
        fprintf(stderr, "Failed to initialize seccomp context\n");
        return -1;
    }

    int syscalls_to_block[] = {
        // Kernel & System Manipulation
        SCMP_SYS(reboot),
        SCMP_SYS(kexec_load),
        SCMP_SYS(kexec_file_load),
                                     
        SCMP_SYS(init_module),
        SCMP_SYS(delete_module),
        SCMP_SYS(finit_module),

        // Kernel Keyring & Security
        SCMP_SYS(keyctl),
        SCMP_SYS(add_key),
        SCMP_SYS(request_key),

        // Clock & Time Manipulation
        SCMP_SYS(adjtimex),
        SCMP_SYS(clock_adjtime),
        SCMP_SYS(clock_settime),
        SCMP_SYS(settimeofday),

        // Swap and Filesystem Operations
        SCMP_SYS(swapon),
        SCMP_SYS(swapoff),
        SCMP_SYS(pivot_root),
        SCMP_SYS(mount),
        SCMP_SYS(umount2),

        // Namespace isolation
        SCMP_SYS(unshare), 
        SCMP_SYS(setns),    
                             

        // Process Tracing & Kernel Inspection
        SCMP_SYS(ptrace),
        SCMP_SYS(process_vm_readv),
        SCMP_SYS(process_vm_writev),
        SCMP_SYS(bpf),
        SCMP_SYS(userfaultfd),
        SCMP_SYS(perf_event_open),
        SCMP_SYS(syslog)
    };

    size_t num_syscalls = sizeof(syscalls_to_block) / sizeof(syscalls_to_block[0]);

    for (size_t i = 0; i < num_syscalls; i++) {
        int ret = seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), syscalls_to_block[i], 0);
        if (ret < 0) {
            fprintf(stderr, "Failed to add seccomp rule for syscall %d: %s\n",
                    syscalls_to_block[i], strerror(-ret));
            seccomp_release(ctx);
            return -1;
        }
    }

    // Load ctx to kernel via BPF (Berkeley Packet Filter)
    int ret = seccomp_load(ctx);
    if (ret < 0){
        perror("seccomp_load");
        seccomp_release(ctx);
        return -1;
    }

    // Free up allocated ctx
    seccomp_release(ctx);
    return 0;
}