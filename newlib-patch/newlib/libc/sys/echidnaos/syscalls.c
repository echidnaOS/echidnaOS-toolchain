/* note these headers are all provided by newlib - you don't need to provide them */
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <stdio.h>
#include "dirent.h"

#include "sys_api.h"

void *__dso_handle = (void *)0;

typedef struct dirent dirent_t;

static dirent_t dirent_struct;

extern char **environ;

/* stubs to compile gcc */

int dup2(int oldfd, int newfd) {
    return -1;
}

int pipe(int pipefd[2]) {
    return -1;
}

unsigned int sleep(unsigned int seconds) {
    return seconds;
}

unsigned alarm(unsigned seconds) {
    return 0;
}

/* end of stubs */

int execvp(const char *file, char *const argv[]) {
    return execve(file, argv, environ);
}

int chdir(const char *path) {
    if (OS_vfs_cd(path) == VFS_FAILURE)
        return -1;

    return 0;
}

int mkdir(const char *path, mode_t mode) {
    if (OS_vfs_mkdir(path, mode) == VFS_FAILURE)
        return -1;

    return 0;
}

DIR *opendir(const char *path) {
    vfs_metadata_t metadata;
    DIR *dirhandle;

    if (OS_vfs_get_metadata(path, &metadata, VFS_DIRECTORY_TYPE) == VFS_FAILURE)
        return (DIR *)0;

    dirhandle = malloc(sizeof(DIR));
    if (!dirhandle)
        return (DIR *)0;

    dirhandle->entry = 0;
    strcpy(dirhandle->dir, path);

    return dirhandle;
}

int closedir(DIR *handle) {
    free(handle);
    return 0;
}

struct dirent *readdir(DIR *handle) {
    vfs_metadata_t metadata;

    if (OS_vfs_list(handle->dir, &metadata, handle->entry) == VFS_FAILURE)
        return (struct dirent *)0;

    strcpy(dirent_struct.d_name, metadata.filename);
    dirent_struct.d_ino = handle->entry++;

    return &dirent_struct;
}

int rmdir(const char *path) {

    return -1;

}

int chown(const char *path, uid_t owner, gid_t group) {

    return 0;

}

int utime(const char *path, const struct utimbuf *times) {

    return 0;
    
}

int access(const char *path, int amode) {

    return 0;

}

char *getwd(char *path_name) {

    OS_pwd(path_name);
    return path_name;

}

int lstat(const char *restrict path, struct stat *restrict buf) {

    return stat(path, buf);

}

long sysconf(int name) {

    return -1;
    
}

int fcntl(int fildes, int cmd, ...) {

    return -1;

}

mode_t umask(mode_t cmask) {

    return 0;

}

int chmod(const char *path, mode_t mode) {

    return 0;

}
 
void _exit(void) {

    OS_exit(0);

}

int close(int file) {

    return OS_close(file);

}

int execve(char *name, char **argv, char **env) {

    return OS_execve(name, argv, env);
    
}

int execv(char *name, char **argv) {

    return OS_execve(name, argv, environ);

}

int fork(void) {

    return OS_fork();

}

int fstat(int file, struct stat *st) {

    st->st_mode = S_IFCHR;
    return 0;

}

int getpid(void) {

    return OS_getpid();

}

int isatty(int file) {

    return 0;

}

int kill(int pid, int sig) {

    return -1;

}

int link(char *old, char *new) {

    return -1;

}

int lseek(int file, int ptr, int dir) {

    switch (dir) {
        case SEEK_SET:
            return OS_lseek(file, ptr, ECH_SEEK_SET);
        case SEEK_END:
            return OS_lseek(file, ptr, ECH_SEEK_END);
        case SEEK_CUR:
            return OS_lseek(file, ptr, ECH_SEEK_CUR);
        default:
            return -1;
    }

}

int open(const char *name, int flags, ...) {

    int os_flags = 0;
    
    if (flags & O_RDONLY)
        os_flags |= ECH_O_RDONLY;
    if (flags & O_WRONLY)
        os_flags |= ECH_O_WRONLY;
    if (flags & O_RDWR)
        os_flags |= ECH_O_RDWR;
    
    if (flags & O_APPEND)
        os_flags |= ECH_O_APPEND;
    if (flags & O_CREAT)
        os_flags |= ECH_O_CREAT;
    if (flags & O_TRUNC)
        os_flags |= ECH_O_TRUNC;


    return OS_open(name, os_flags, 0);

}

int read(int file, char *ptr, int len) {

    return OS_read(file, ptr, len);

}

caddr_t sbrk(int size) {

    size_t ptr = OS_get_heap_base() + OS_get_heap_size();
    if (OS_resize_heap(OS_get_heap_size() + size) == -1)
        return -1;
    return (caddr_t)ptr;

}

int stat(const char *file, struct stat *st) {

    st->st_mode = S_IFCHR;
    return 0;

}

clock_t times(struct tms *buf) {

    return -1;

}

int unlink(char *name) {

    if (OS_vfs_remove(name) == VFS_FAILURE)
        return -1;
    
    return 0;

}

int wait(int *status) {

    return OS_wait(status);

}

int write(int file, char *ptr, int len) {

    return OS_write(file, ptr, len);

}
