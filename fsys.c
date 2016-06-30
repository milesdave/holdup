#define _DEFAULT_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "fsys.h"
#include "util.h"

#define BUFFER_SIZE 512

void printPercentage(off_t, off_t, off_t*);

/* copies the source file to the destination file in
	BUFFER_SIZE sections at a time
	- prints percentage complete */
void fsys_copy(const char *src, const char *dest)
{
	int srcFD, destFD;
	char buffer[BUFFER_SIZE];
	off_t bufferSize, fileSize, written, remaining, last;

	// open source
	if((srcFD = open(src, O_RDONLY)) == -1)
		goto R_OPEN_ERR;

	// get source file size and permissions
	stat_t fileInfo;
	if((stat(src, &fileInfo)) == -1)
		goto R_OPEN_ERR;

	// open destination
	if((destFD = open(dest, O_WRONLY | O_TRUNC | O_CREAT, fileInfo.st_mode)) == -1)
		goto W_OPEN_ERR;

	bufferSize = written = 0;
	remaining = fileSize = fileInfo.st_size;
	last = -1; // -1 means nothing written yet - used in printPercentage()

	// copy source BUFFER_SIZE at a time to destination
	while(written != fileSize)
	{
		// file or remainder of file could be < BUFFER_SIZE
		bufferSize = remaining < BUFFER_SIZE ? remaining : BUFFER_SIZE;

		if((read(srcFD, buffer, bufferSize)) != bufferSize)
			goto RW_ERR;
		if((write(destFD, buffer, bufferSize)) != bufferSize)
			goto RW_ERR;

		written += bufferSize;
		remaining -= bufferSize;
		printPercentage(written, fileSize, &last);
	}

	close(srcFD);
	close(destFD);
	return;

RW_ERR:
	close(destFD);
	remove(dest);
W_OPEN_ERR:
	close(srcFD);
R_OPEN_ERR:
	perror("");
}

/* prints the percentage of the file copied, overwriting the
	previous output each time using '*last' as a reference
	for how much to backspace each time
	- output of '100%' is coloured green */
void printPercentage(off_t written, off_t fileSize, off_t *last)
{
	off_t percent = ((double)written / (double)fileSize) * 100.0;

	if(*last == -1) // first print
		percent == 100 ? printf("%s100%%%s", GRN, NRM)
			: printf("%ld%%", percent);
	else if(*last < 10)
		percent == 100 ? printf("\b\b%s100%%%s", GRN, NRM)
			: printf("\b\b%ld%%", percent);
	else if(*last < 100)
		percent == 100 ? printf("\b\b\b%s100%%%s", GRN, NRM)
			: printf("\b\b\b%ld%%", percent);

	fflush(stdout);
	*last = percent;
}
