#include <syscall.h>
#include <string.h>
#include <profan.h>
#include <stdlib.h>
#include <stdio.h>

#include <bordel.h>

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif


int unlink(const char *pathname) {
    serial_debug("unlink: %s\n", (char *) pathname);
    return 0;
}

int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}

int isdigit(int c) {
    return c >= '0' && c <= '9';
}

int toupper(int c) {
    if (islower(c)) {
        return c - 32;
    }
    return c;
}

int isxdigit(int c) {
    return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int isspace(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

int tolower(int c) {
    if (isupper(c)) {
        return c + 32;
    }
    return c;
}

int isalpha(int c) {
    return isupper(c) || islower(c);
}

int iscntrl(int c) {
    return c < 32 || c == 127;
}

int isgraph(int c) {
    return c > 32 && c < 127;
}

int islower(int c) {
    return c >= 'a' && c <= 'z';
}

int ispunct(int c) {
    return isgraph(c) && !isalnum(c);
}

int isupper(int c) {
    return c >= 'A' && c <= 'Z';
}

long double strtold(const char *theString, char **end) {
    puts("strtold is not implemented yet...\n");
    return 0;
}

// localeconv
struct lconv *localeconv(void) {
    puts("localeconv is not implemented yet...\n");
    return NULL;
}

// strcoll
int strcoll(const char *s1, const char *s2) {
    return strcmp(s1, s2);
}

// setlocale
char *setlocale(int category, const char *locale) {
    puts("setlocale is not implemented yet...\n");
    return NULL;
}

// signal
void (*signal(int sig, void (*func)(int)))(int) {
    serial_debug("signal is not implemented yet...\n");
    return func;
}
