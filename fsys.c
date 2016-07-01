#define _DEFAULT_SOURCE
#include <fcntl.h>
#include <linux/limits.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "fsys.h"
#include "types.h"
#include "util.h"

#define BUFFER_SIZE sysconf(_SC_PAGESIZE)

void printPercentage(off_t, off_t, off_t*);

/* copies the source file to the destination file in
	BUFFER_SIZE sections at a time
	- prints percentage complete */
int fsys_copy(const char *src, const char *dest, info_t *info)
{
	int srcFD, destFD;
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

	// buffer aligned to page size
	char *buffer = memalign(sysconf(_SC_PAGESIZE), BUFFER_SIZE);

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

	free(buffer);
	close(srcFD);
	close(destFD);
	info->copied++;
	info->byteTotal = addBytes(info->byteTotal, written, &info->byteFormatIndex);
	return 0;

RW_ERR:
	free(buffer);
	close(destFD);
	remove(dest);
W_OPEN_ERR:
	close(srcFD);
R_OPEN_ERR:
	info->failed++;
	perror(" ");
	return -1;
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

/* returns the result of stat()
	-1 if the path doesn't exist
	0 if the path does exist */
int fsys_exists(const char *path)
{
	stat_t st;
	return stat(path, &st);
}

/* creates all non-existent directories
	in a full path */
int fsys_mkpath(const char *path)
{
	/* need a copy of the path because it will
		be modifed in the process */
	char pathCopy[PATH_MAX];
	strcpy(pathCopy, path);

	char newPath[PATH_MAX] = "";
	char *dir = strtok(pathCopy, "/");

	// absolute path from root
	if(pathCopy[0] == '/')
		strcat(newPath, "/");

	while(dir != NULL)
	{
		strcat(newPath, dir);

		if((fsys_exists(newPath)) == -1) // path doesn't exist
			if((mkdir(newPath, 0777)) == -1) // create it
				return -1; // mkdir() failed

		// add next dir to path
		strcat(newPath, "/");
		dir = strtok(NULL, "/");
	}

	return 0;
}

/* fills 'time' with the file's
	last modified time */
int fsys_mtime(const char *path, time_t *time)
{
	stat_t st;

	if((stat(path, &st)) == -1)
		return -1;

	*time = st.st_mtime;
	return 0;
}
