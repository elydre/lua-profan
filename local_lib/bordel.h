#ifndef BORDEL_H
#define BORDEL_H

// bordel.c
long double strtold(const char *theString, char **end);

// strcoll
int strcoll(const char *s1, const char *s2);

// setlocale
char *setlocale(int category, const char *locale);

void (*signal(int sig, void (*func)(int)))(int);

#endif
