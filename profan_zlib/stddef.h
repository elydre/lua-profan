#ifndef STDDEF_H
#define STDDEF_H

// ptrdiff_t definition
typedef long ptrdiff_t;

// size_t definition
typedef unsigned int size_t;

// offsetof definition
#define offsetof(type, member) ((size_t) &((type *)0)->member)

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif
