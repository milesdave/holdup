#ifndef _FSYS_H
#define _FSYS_H

#include "types.h"

int fsys_copy(const char*, const char*, info_t*);
int fsys_exists(const char*);
int fsys_mkpath(const char*);

#endif
