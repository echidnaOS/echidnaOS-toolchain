#include <fcntl.h>

#include "sys_api.h"

int main(int, char **, char **);
void _init_signal(void);

extern char **environ;

void _start(void) {

    int argc = *((int *)0x1001000);
    char **argv = (char **)0x1001010;
    environ = (char **)0x1002000;

    _init_signal();

    int exit_code = main(argc, argv, environ);

    OS_exit(exit_code);

}
