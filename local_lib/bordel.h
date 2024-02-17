#ifndef BORDEL_H
#define BORDEL_H

#include <type.h>

// errno variable
static int errno;

// bordel.c
long double strtold(const char *theString, char **end);

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
