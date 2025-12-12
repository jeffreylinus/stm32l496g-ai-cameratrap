/*
 * syscalls.c
 *
 *  Created on: Nov 25, 2025
 *      Author: Sumeeth
 */

#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

// ----------------------------------------------------------------------
// Existing minimal implementations
// ----------------------------------------------------------------------

int _close(int file) {
    (void)file;
    return -1;
}

int _fstat(int file, struct stat *st) {
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file) {
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _read(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

int _write(int file, char *ptr, int len) {
    // If you later want printf over UART, implement it here.
    (void)file;
    (void)ptr;
    return len;
}

// ----------------------------------------------------------------------
// New stubs needed by libc_nano / TinyJPEG
// ----------------------------------------------------------------------

int _open(char *path, int flags, int mode) {
    (void)path;
    (void)flags;
    (void)mode;
    errno = ENOSYS;
    return -1;
}

int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _getpid(void) {
    return 1;
}

// Optional, but silences some toolchains' references
void _exit(int status) {
    (void)status;
    while (1) {
        // hang here
    }
}

int _raise(int sig) {
    (void)sig;
    return 0;
}
