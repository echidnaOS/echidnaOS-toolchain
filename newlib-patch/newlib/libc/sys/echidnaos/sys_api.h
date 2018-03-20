/*
 *  echidnaOS kernel API
 */

#ifndef __SYS_API_H__
#define __SYS_API_H__

#include <stdint.h>

typedef struct {
    char filename[2048];
    int filetype;
    uint64_t size;
} vfs_metadata_t;

#define VFS_FILE_TYPE 0
#define VFS_DIRECTORY_TYPE 1
#define VFS_DEVICE_TYPE 2
#define VFS_SUCCESS 0
#define VFS_FAILURE -2

#define ECH_O_RDONLY        0b0001
#define ECH_O_WRONLY        0b0010
#define ECH_O_RDWR          0b0100

#define ECH_O_APPEND        0b001000
#define ECH_O_CREAT         0b010000
#define ECH_O_TRUNC         0b100000

#define ECH_SEEK_SET 0
#define ECH_SEEK_END 1
#define ECH_SEEK_CUR 2

#define OS_open(path, flags, mode) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x2a, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)         \
                     : "c" (path),  \
                       "d" (flags), \
                       "D" (mode) \
                     :  );         \
    return_val;                                \
})

#define OS_execve(arg1, arg2, arg3) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x03, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)         \
                     : "c" (arg1),  \
                       "d" (arg2), \
                       "D" (arg3) \
                     :  );         \
    return_val;                                \
})

#define OS_wait(arg1) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x04, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)         \
                     : "c" (arg1)  \
                     : "rdx" );         \
    return_val;                                \
})

#define OS_sleep(arg1) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x06, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)         \
                     : "c" (arg1)  \
                     : "rdx" );         \
    return_val;                                \
})

#define OS_close(handle) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x2b, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)         \
                     : "c" (handle) \
                     : "rdx" );         \
    return_val;                                \
})

#define OS_read(handle, buf, len) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x2c, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)         \
                     : "c" (handle),  \
                       "d" (buf), \
                       "D" (len) \
                     :  );         \
    return_val;                                \
})

#define OS_write(handle, buf, len) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x2d, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)         \
                     : "c" (handle),  \
                       "d" (buf), \
                       "D" (len) \
                     :  );         \
    return_val;                                \
})

#define OS_lseek(a, b, c) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x2e, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)         \
                     : "c" (a),  \
                       "d" (b), \
                       "D" (c) \
                     :  );         \
    return_val;                                \
})

#define OS_signal(sig, handler) ({ \
    int ret; \
    asm volatile (  "movq $0x16, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (ret)                 \
                     : "c" (sig),        \
                       "d" (handler)    \
                     :  );         \
    ret; \
})

#define OS_getpid() ({              \
    int val;                         \
    asm volatile (  "movq $0x15, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (val)      \
                     : \
                     : "rdx" );         \
    val;                               \
})

#define OS_get_heap_base() ({              \
    uint64_t val;                         \
    asm volatile (  "movq $0x10, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (val)      \
                     : \
                     : "rdx" );         \
    val;                               \
})

#define OS_get_heap_size() ({              \
    uint64_t val;                         \
    asm volatile (  "movq $0x11, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (val)      \
                     : \
                     : "rdx" );         \
    val;                               \
})

#define OS_resize_heap(val) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x12, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)      \
                     : "c" (val)  \
                     : "rdx" );         \
    return_val;                                \
})

#define OS_pwd(value) ({                \
    asm volatile (  "movq $0x1a, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     :                  \
                     : "c" (value)      \
                     : "rax", "rdx" );  \
})

#define OS_vfs_list(path, metadata, entry) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x32, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)         \
                     : "c" (path),  \
                       "d" (metadata), \
                       "D" (entry) \
                     :  );         \
    return_val;                                \
})

#define OS_vfs_get_metadata(path, metadata, type) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x33, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)         \
                     : "c" (path),  \
                       "d" (metadata), \
                       "D" (type) \
                     :  );         \
    return_val;                                \
})

#define OS_vfs_mkdir(path, perms) ({ \
    int ret; \
    asm volatile (  "movq $0x35, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (ret)                 \
                     : "c" (path),        \
                       "d" (perms)    \
                     :  );         \
    ret; \
})

#define OS_vfs_create(path, perms) ({ \
    int ret; \
    asm volatile (  "movq $0x36, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (ret)                 \
                     : "c" (path),        \
                       "d" (perms)    \
                     :  );         \
    ret; \
})

#define OS_vfs_cd(path) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x2f, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)      \
                     : "c" (path)  \
                     : "rdx" );         \
    return_val;                                \
})

#define OS_vfs_remove(path) ({  \
    int return_val;                            \
    asm volatile (  "movq $0x34, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (return_val)      \
                     : "c" (path)  \
                     : "rdx" );         \
    return_val;                                \
})

#define OS_exit(value) ({               \
    asm volatile (  "movq $0x00, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : \
                     : "c" (value)      \
                     : "rax", "rdx" );  \
})

#define OS_fork() ({               \
    int ret; \
    asm volatile (  "movq $0x05, %%rax\n\t"    \
                    "int $0x80\n\t"         \
                     : "=a" (ret) \
                     :     \
                     : "rdx" );  \
    ret; \
})

#endif
