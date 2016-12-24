#ifndef _UTIL_H
#define _UTIL_H

#include "types.h"

#define RED "\x1b[1;31m"
#define GRN "\x1b[1;32m"
#define BLU "\x1b[1;34m"
#define NRM "\x1b[0m"

#define STRLEN_FILESIZE 8
#define STRLEN_TIME 20
#define STRLEN_TRUNC 50
#define TRUNC_LEN 20

#define ERROR printf(" %s", RED); perror(""); printf("%s", NRM);

extern const char *byteSuffix[];
extern const char *timeSuffix[];

char* fileSizeString(double bytes, char *buffer, const unsigned int bufferSize);
char* reverseNChar(char *string, char x, int n);
char* timeString(double time, char *buffer, const unsigned int bufferSize);
char* stringTrunc(char *string);

double timeDiff(const timespec_t *start, const timespec_t *end);
double addBytes(double total, double add, int *index);

void getPaths(char *destPath, char *destFile, char *fileName, info_t *info);

#endif
