#ifndef _HOLDUP_H
#define _HOLDUP_H

void process(FTSENT*);
int byType(const FTSENT**, const FTSENT**);
void indent(int);

#endif
