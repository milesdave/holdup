#ifndef _HOLDUP_H
#define _HOLDUP_H

#include "types.h"

info_t info;

void process(FTSENT*);
void copy(const char*, const char*);
int byType(const FTSENT**, const FTSENT**);
void indent(int);

#endif
