#define _DEFAULT_SOURCE
#include <stdio.h>
#include <fts.h>
#include <sys/stat.h>
#include <unistd.h>
#include "fsys.h"
#include "holdup.h"
#include "util.h"

int main(int argc, char *argv[])
{
	if(argc < 3)
		return 1;

	FTS *fts = fts_open(argv + 2, FTS_COMFOLLOW | FTS_NOCHDIR, &byType);
	FTSENT *node;

	while((node = fts_read(fts)) != NULL)
	{
		switch(node->fts_info)
		{
		case FTS_D: // directory
			indent(node->fts_level);
			printf("%s/\n", node->fts_name);
			break;
		case FTS_F: // file
		case FTS_SL: // symlink
			process(node);
			break;
		case FTS_DNR: // inaccessable dir
		case FTS_ERR: // general error
			perror("");
		default:
			break;
		}
	}

	fts_close(fts);
	return 0;
}

/* TODO */
void process(FTSENT *node)
{
	// basic output
	indent(node->fts_level);
	char fileSize[STRLEN_FILESIZE];
	printf("%s: %s\n", node->fts_name,
		fileSizeString(node->fts_statp->st_size, fileSize, STRLEN_FILESIZE));
}

/* when processing an FTSENT, prioritise
	files over directories */
int byType(const FTSENT **a, const FTSENT **b)
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
	for(; i > 0; i-- )
		printf("  ");
}
