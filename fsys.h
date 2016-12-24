#ifndef _FSYS_H
#define _FSYS_H

#include "types.h"

int fsys_copy(const char *src, const char *dest, info_t *info);
int fsys_exists(const char *path);
int fsys_mkpath(const char *path);
int fsys_mtime(const char *path, time_t *time);

#endif
