#ifndef _UTIL_H
#define _UTIL_H

#include "types.h"

#define GRN "\x1b[1;32m"
#define NRM "\x1b[0m"
#define STRLEN_FILESIZE 8

extern const char *byteSuffix[];

char* fileSizeString(double, char*, const unsigned int);
char* reverseNChar(char*, char, int);
void getPaths(char*, char*, char*, info_t*);

#endif
