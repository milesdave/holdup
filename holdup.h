#ifndef _HOLDUP_H
#define _HOLDUP_H

#include <stdbool.h>
#include "types.h"

info_t info;

void process(FTSENT *node);
void copy(const char *src, const char *dest);
void indent(int);

int byType(const FTSENT **a, const FTSENT **b);

#endif
