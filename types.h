#ifndef _TYPES_H
#define _TYPES_H

#include <linux/limits.h>

typedef struct stat stat_t;
typedef struct timespec timespec_t;

typedef struct info
{
	int copied;
	int failed;
	int pathLevel;
	int byteFormatIndex;
	double byteTotal;
	char destPrefix[PATH_MAX];
	char srcPrefix[PATH_MAX];
} info_t;

#endif
