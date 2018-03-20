#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <utime.h>
#include "dirent.h"

#include "sys_api.h"

void *__dso_handle = (void *)0;

extern char **environ;

/* stubs */

int dup2(int oldfd, int newfd) {
    errno = EIO;
    return -1;
}

int pipe(int pipefd[2]) {
    errno = ENFILE;
    return -1;
}

unsigned int alarm(unsigned int seconds) {
    return 0;
}

int rmdir(const char *path) {
    errno = EIO;
    return -1;
}

int chown(const char *path, uid_t owner, gid_t group) {
    errno = EIO;
    return -1;
}

int utime(const char *path, const struct utimbuf *times) {
    errno = EROFS;
    return -1;
}

int access(const char *path, int amode) {
    errno = EROFS;
    return -1;
}

long sysconf(int name) {
    errno = EINVAL;
    return -1;
}

int fcntl(int fildes, int cmd, ...) {
    errno = EINVAL;
    return -1;
}

mode_t umask(mode_t cmask) {
    return 0;
}

clock_t times(struct tms *buf) {
    return (clock_t)-1;
}

int isatty(int file) {
    return 0;
}

int kill(int pid, int sig) {
    errno = EINVAL;
    return -1;
}

int chmod(const char *path, mode_t mode) {
    errno = EROFS;
    return -1;
}

int stat(const char *file, struct stat *st) {
    errno = EIO;
    return -1;
}

int fstat(int file, struct stat *st) {
    errno = EIO;
    return -1;
}

int lstat(const char *path, struct stat *st) {
    errno = EIO;
    return -1;
}

int link(const char *old, const char *new) {
    errno = EROFS;
    return -1;
}

int unlink(const char *name) {
    errno = EROFS;
    return -1;
}

/* end of stubs */

unsigned int sleep(unsigned int seconds) {
    return OS_sleep(seconds);
}

int execve(const char *name, char * const argv[], char * const env[]) {
    if (OS_execve(name, argv, env)) {
        errno = ENOENT;
        return -1;
    }
}

int execv(const char *name, char * const argv[]) {
    return execve(name, argv, (const char **)environ);
}

int execvp(const char *name, char * const argv[]) {
    return execve(name, argv, (const char **)environ);
}

int chdir(const char *path) {
    if (OS_vfs_cd(path) == VFS_FAILURE) {
        errno = ENOENT;
        return -1;
    }

    return 0;
}

int mkdir(const char *path, mode_t mode) {
    if (OS_vfs_mkdir(path, mode) == VFS_FAILURE) {
        errno = ENOENT;
        return -1;
    }

    return 0;
}

DIR *opendir(const char *path) {
    vfs_metadata_t metadata;
    DIR *dirhandle;

    if (OS_vfs_get_metadata(path, &metadata, VFS_DIRECTORY_TYPE) == VFS_FAILURE) {
        errno = ENOENT;
        return (DIR *)0;
    }

    dirhandle = malloc(sizeof(DIR));
    if (!dirhandle) {
        errno = ENOENT;
        return (DIR *)0;
    }

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

    strcpy(handle->direntt.d_name, metadata.filename);
    handle->direntt.d_ino = handle->entry++;

    return &(handle->direntt);
}

char *getwd(char *path_name) {
    OS_pwd(path_name);
    return path_name;
}

void _exit(int status) {
    OS_exit(0);
}

int close(int file) {
    return OS_close(file);
}

int fork(void) {
    return OS_fork();
}

int getpid(void) {
    return OS_getpid();
}

off_t lseek(int file, off_t ptr, int dir) {
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
    int os_flags, fildes = 0;

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

    fildes = OS_open(name, os_flags, 0);

    if (fildes < 0) {
        errno = ENOENT;
        return -1;
    }

    return fildes;
}

int read(int file, void *ptr, size_t len) {
    return OS_read(file, ptr, len);
}

int write(int file, const void *ptr, size_t len) {
    return OS_write(file, ptr, len);
}

void *sbrk(ptrdiff_t size) {
    size_t ptr = OS_get_heap_base() + OS_get_heap_size();
    if (OS_resize_heap(OS_get_heap_size() + size) == -1) {
        errno = ENOMEM;
        return (void *)-1;
    }
    return (void *)ptr;
}

int wait(int *status) {
    return OS_wait(status);
}
