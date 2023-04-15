#ifndef BORDEL_H
#define BORDEL_H

#include <type.h>

// uintptr_t
#define uintptr_t unsigned long

// errno variable
static int errno;

#define O_RDONLY 0
#define O_WRONLY 1
#define O_CREAT 2
#define O_TRUNC 4
#define O_BINARY 8

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

// define jmp_buf type
typedef int jmp_buf[6];

struct timeval {
    long tv_sec;
    long tv_usec;
};

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

// define off_t type
#define off_t long

// define size_t type
#define ssize_t long

#ifdef exit
#undef exit
#endif

#ifdef open
#undef open
#endif

#ifdef close
#undef close
#endif

#ifdef read
#undef read
#endif

#ifdef lseek
#undef lseek
#endif

#ifdef memcpy
#undef memcpy
#endif

#ifdef fputs
#undef fputs
#endif

#ifdef strncmp
#undef strncmp
#endif

#ifdef unlink
#undef unlink
#endif

#ifdef fputc
#undef fputc
#endif

// entry.c
void exit(int status);

// bordel.c
void init_bordel();

void serial_debug(char *fmt, ...);

int strncmp(const char *s1, const char *s2, size_t n);
long double strtold(const char *theString, char **end);

int open(const char *pathname, ...);
int close(int fd);
ssize_t read(int fd, void *buf, size_t count);
off_t lseek(int fd, off_t offset, int whence);

int unlink(const char *pathname);

void *memcpy(void *dest, const void *src, size_t n);

int fseek(FILE *stream, long offset, int whence);
FILE *fdopen(int fd, const char *mode);

int fputs(const char *s, FILE *stream);
int fputc(int c, FILE *stream);

void SLogMsg(const char *msg, ...);

int GetTickCount(void);


#endif
