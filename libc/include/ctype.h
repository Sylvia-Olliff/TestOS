#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

// Character classification functions
// int isalnum(int c);
// int isalpha(int c);
// int isblank(int c);
// int iscntrl(int c);
// int isdigit(int c);
// int isgraph(int c);
// int islower(int c);
// int isprint(int c);
// int ispunct(int c);
int isspace(int c);
// int isupper(int c);
// int isxdigit(int c);

#ifdef __cplusplus
}
#endif

#endif
