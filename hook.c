#define _GNU_SOURCE
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>

typedef int (*orig_open_f_type)(const char *pathname, int flags, ...);

int open(const char *pathname, int flags, ...) {
    orig_open_f_type orig_open;
    orig_open = (orig_open_f_type)dlsym(RTLD_NEXT, "open");

    if (strncmp(pathname, "/tmp", 4) == 0) {
        printf("[HOOK] Blocked access to: %s\n", pathname);
        return -1;
    }

    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode_t mode = va_arg(args, mode_t);
        va_end(args);
        return orig_open(pathname, flags, mode);
    } else {
        return orig_open(pathname, flags);
    }
}

