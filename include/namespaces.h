#ifndef NAMESPACES_H
#define NAMESPACES_H

#define STACKSIZE (1024 * 1024) // we will be using 1 MB stack

int start_sandbox(int (*func)(void *), void *arg);//it start bandbox(sandbox)

#endif
