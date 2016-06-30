#define _DEFAULT_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "fsys.h"

#define BUFFER_SIZE 512

typedef struct stat stat_t;

void fsys_copy(const char *src, const char *dest)
{
	int srcFD, destFD;
	char buffer[BUFFER_SIZE];
	off_t bufferSize, fileSize, written, remaining;

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

	// copy file BUFFER_SIZE at a time
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
