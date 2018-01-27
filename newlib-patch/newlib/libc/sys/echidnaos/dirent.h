#ifndef __DIRENT_H__
#define __DIRENT_H__

#include <sys/types.h>

struct dirent {
    ino_t d_ino;
    char d_name[128];
};

typedef struct {
    char dir[128];
    int entry;
} DIR;

DIR *opendir(const char *);
int closedir(DIR *);
struct dirent *readdir(DIR *);

#endif
