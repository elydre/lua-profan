#include <syscall.h>
#include <string.h>
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

int str_len(char s[]) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

void str_reverse(char s[]) {
    int i = 0;
    int j = str_len(s) - 1;
    char tmp;
    while (i < j) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

void int2str(int n, char s[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) s[i++] = '-';
    s[i] = '\0';

    str_reverse(s);
}

void hex2str(uint32_t n, char s[]) {
    int i = 0;
    int tmp;
    char hex[] = "0123456789abcdef";
    do {
        tmp = n % 16;
        s[i++] = hex[tmp];
    } while ((n /= 16) > 0);
    s[i] = '\0';
    str_reverse(s);
}

void serial_debug(char *fmt, ...) {
    char *args = (char *) &fmt;
    args += 4;
    int i = 0;
    char *char_buffer = malloc(0x1000);
    int buffer_i = 1;
    char_buffer[0] = ' ';
    while (fmt[i] != '\0') {
        if (fmt[i] == '%') {
            i++;
            if (fmt[i] == 's') {
                char *s = *((char **) args);
                args += 4;
                for (int j = 0; s[j] != '\0'; j++) {
                    char_buffer[buffer_i] = s[j];
                    buffer_i++;
                }                
            } else if (fmt[i] == 'c') {
                char c = *((char *) args);
                args += 4;
                char_buffer[buffer_i] = c;
                buffer_i++;
            } else if (fmt[i] == 'd') {
                int n = *((int *) args);
                args += 4;
                char s[20];
                int2str(n, s);
                for (int j = 0; s[j] != '\0'; j++) {
                    char_buffer[buffer_i] = s[j];
                    buffer_i++;
                }
            } else if (fmt[i] == 'x') {
                uint32_t n = *((int *) args);
                args += 4;
                char s[20];
                hex2str(n, s);
                for (int j = 0; s[j] != '\0'; j++) {
                    char_buffer[buffer_i] = s[j];
                    buffer_i++;
                }
            } else if (fmt[i] == '%') {
                char_buffer[buffer_i] = '%';
                buffer_i++;
            }
        } else {
            char_buffer[buffer_i] = fmt[i];
            buffer_i++;
        }
        i++;
    }
    char_buffer[buffer_i] = '\0';
    c_serial_print(SERIAL_PORT_A, char_buffer);
    free(char_buffer);
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
