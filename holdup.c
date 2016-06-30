#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>
#include "fsys.h"

int main(int argc, char *argv[])
{
	fsys_copy(argv[1], argv[2]);
	printf("\n");
	return 0;
}
