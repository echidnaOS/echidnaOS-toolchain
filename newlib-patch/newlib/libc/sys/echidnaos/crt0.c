#include <fcntl.h>

asm (
    ".global _start\n\t"
    "_start:\n\t"
    "jmp _start_c\n\t"
);

#include "sys_api.h"

int main(int, char **, char **);
void _init_signal(void);

extern char **environ;

void _start_c(void) {

    int argc = *((int *)0x1001000);
    char **argv = (char **)0x1001010;
    environ = (char **)0x1001020;

    _init_signal();

    int exit_code = main(argc, argv, environ);

    OS_exit(exit_code);

}
