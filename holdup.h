#ifndef _HOLDUP_H
#define _HOLDUP_H

#include "types.h"

info_t info;

void process(FTSENT*);
int byType(const FTSENT**, const FTSENT**);
void indent(int);

#endif
