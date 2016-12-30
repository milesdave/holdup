#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <fts.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "fsys.h"
#include "holdup.h"
#include "util.h"

int main(int argc, char *argv[])
{
	if(argc < 3)
		return 1;

	// strip trailing '/' for source(s)/destination
	for(int i = 1; i < argc; i++)
		if(argv[i][strlen(argv[i]) - 1] == '/')
			argv[i][strlen(argv[i]) - 1] = '\0';

	// disable buffering on stdout
	setbuf(stdout, NULL);

	// set info inital values
	strcpy(info.destPrefix, argv[1]);
	info.copied = 0;
	info.failed = 0;
	info.byteFormatIndex = 0;
	info.byteTotal = 0.0;

	FTS *fts = fts_open(argv + 2, FTS_COMFOLLOW | FTS_NOCHDIR, &byType);
	FTSENT *node;

	// start time
	timespec_t start, end;
	clock_gettime(CLOCK_REALTIME, &start);

	while((node = fts_read(fts)) != NULL)
	{
		switch(node->fts_info)
		{
		case FTS_D: // directory
			indent(node->fts_level);
			printf("%s/\n", node->fts_name);
			strcpy(info.srcPrefix, node->fts_path);
			info.pathLevel = node->fts_level;
			break;
		case FTS_F: // file
		case FTS_SL: // symlink
			process(node);
			break;
		case FTS_DNR: // inaccessable dir
		case FTS_ERR: // general error
			ERROR;
		default:
			break;
		}
	}

	fts_close(fts);

	// total files
	info.copied == 1 ? printf("\n1 file, ")
		: printf("\n%d files, ", info.copied);

	// total bytes
	info.byteFormatIndex == 0 ? printf("%.0f%s, ", info.byteTotal, byteSuffix[info.byteFormatIndex])
		: printf("%.1f%s, ", info.byteTotal, byteSuffix[info.byteFormatIndex]);

	// total time
	clock_gettime(CLOCK_REALTIME, &end);
	char timeStr[STRLEN_TIME];
	printf("%s", timeString(timeDiff(&start, &end), timeStr, STRLEN_TIME));

	// total fails
	info.failed > 0 ? printf(", %s%d failed%s\n", RED, info.failed, NRM) : printf("\n");

	return 0;
}

/* determines what should be done with a file
	- doesn't exist: mkpath() and copy()
	- does exist: check last modified and copy() */
void process(FTSENT *node)
{
	char fileName[PATH_MAX];

	indent(node->fts_level);
	printf("%s ", stringTrunc(node->fts_name, fileName));

	char destPath[PATH_MAX]; // without filename - used for mkpath()
	char destFile[PATH_MAX]; // with filename - used for copy()
	getPaths(destPath, destFile, node->fts_name, &info);

	// destFile doesn't exist
	if((fsys_exists(destFile)) == -1)
	{
		// make path to destFile
		if((fsys_mkpath(destPath)) == -1)
		{
			ERROR;
			return;
		}

		printSize(node->fts_statp->st_size);
		copy(node->fts_path, destFile);
	}
	else // destFile does exist
	{
		// get destintation file's mtime
		time_t destTime;
		if((fsys_mtime(destFile, &destTime)) == -1)
		{
			ERROR;
			return;
		}

		// source has been mdofied since backup
		if(node->fts_statp->st_mtime > destTime)
		{
			printSize(node->fts_statp->st_size);
			copy(node->fts_path, destFile);
			return;
		}

		printf("\n");
	}
}

/* prints the size of a file in a human-readable
	format and coloured blue */
void printSize(const size_t size)
{
	char fileSize[STRLEN_FILESIZE];
	printf("%s%s%s ", BLU,
		fileSizeString(size, fileSize, STRLEN_FILESIZE), NRM);
}

/* copies the file to the destination and
	records how long it took */
void copy(const char *src, const char *dest)
{
	// start time
	timespec_t start, end;
	clock_gettime(CLOCK_REALTIME, &start);

	if((fsys_copy(src, dest, &info)) != -1)
	{
		// time output
		clock_gettime(CLOCK_REALTIME, &end);
		char timeStr[STRLEN_TIME];
		printf("\b\b\b\b%s%s%s\n", GRN,
			timeString(timeDiff(&start, &end), timeStr, STRLEN_TIME), NRM);
	}
}

/* when processing an FTSENT, prioritise
	files over directories */
int byType(const FTSENT *const *a, const FTSENT *const *b)
{
	if(((*a)->fts_info == FTS_F) && ((*b)->fts_info != FTS_F))
		return -1;
	else if(((*a)->fts_info != FTS_F) && ((*b)->fts_info == FTS_F))
		return 1;

	return 0;
}

/* used to indent the directory traversal output
	according to the file/dir depth */
void indent(int i)
{
	for(; i > 0; i--)
		printf("  ");
}
