#ifndef __DIRENT_H__
#define __DIRENT_H__

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct dirent {
    ino_t d_ino;
    char d_name[128];
};

typedef struct {
    char dir[128];
    int entry;
    struct dirent direntt;
} DIR;

DIR *opendir(const char *);
int closedir(DIR *);
struct dirent *readdir(DIR *);

#ifdef __cplusplus
}
#endif

#endif
