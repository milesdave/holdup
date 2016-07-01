#ifndef _UTIL_H
#define _UTIL_H

#include "types.h"

#define RED "\x1b[1;31m"
#define GRN "\x1b[1;32m"
#define NRM "\x1b[0m"
#define STRLEN_FILESIZE 8
#define STRLEN_TIME 20

extern const char *byteSuffix[];
extern const char *timeSuffix[];

char* fileSizeString(double, char*, const unsigned int);
char* reverseNChar(char*, char, int);
char* timeString(double, char*, const unsigned int);
double timeDiff(const timespec_t*, const timespec_t*);
double addBytes(double, double, int*);
void getPaths(char*, char*, char*, info_t*);

#endif
