#ifndef _HOLDUP_H
#define _HOLDUP_H

#include "types.h"

info_t info;

void process(FTSENT *node);
void copy(const char *src, const char *dest);
void indent(int i);
void printSize(const size_t size);

int byType(const FTSENT *const *a, const FTSENT *const *b);

#endif
