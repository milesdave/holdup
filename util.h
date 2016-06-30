#ifndef _UTIL_H
#define _UTIL_H

#define GRN "\x1b[1;32m"
#define NRM "\x1b[0m"
#define STRLEN_FILESIZE 8

extern const char *byteSuffix[];

typedef struct stat stat_t;
typedef struct timespec timespec_t;

char* fileSizeString(double, char*, const unsigned int);

#endif
