#ifndef BORDEL_H
#define BORDEL_H

#include <type.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

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

#ifdef lseek
#undef lseek
#endif

#ifdef fputs
#undef fputs
#endif

// bordel.c

void serial_debug(char *fmt, ...);

long double strtold(const char *theString, char **end);

int fseek(FILE *stream, long offset, int whence);
int fputs(const char *s, FILE *stream);

int isalnum(int c);
int isdigit(int c);
int toupper(int c);
int isxdigit(int c);
int isspace(int c);
int tolower(int c);
int isalpha(int c);
int iscntrl(int c);
int isgraph(int c);
int islower(int c);
int ispunct(int c);
int isupper(int c);

// strcoll
int strcoll(const char *s1, const char *s2);

// setlocale
char *setlocale(int category, const char *locale);

void (*signal(int sig, void (*func)(int)))(int);

#endif
