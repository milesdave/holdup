#define _DEFAULT_SOURCE
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

const char *byteSuffix[] = {"B", "K", "M", "G"};

/* converts a file size in bytes into a string
	with a more human readable format, eg: "1.4M" */
char* fileSizeString(double bytes, char *buffer, const unsigned int bufferSize)
{
	int i = 0;

	// reduce bytes to appropriate format
	while(bytes > 1024 && i < 3)
	{
		bytes /= 1024;
		i++;
	}

	// 'B' won't have decimal places
	i == 0 ? snprintf(buffer, bufferSize, "%.0f%s", bytes, byteSuffix[i])
		: snprintf(buffer, bufferSize, "%.1f%s", bytes, byteSuffix[i]);

	return buffer;
}

/* finds the 'n'th occurance of 'x' char
	from the end of a string */
char* reverseNChar(char *string, char x, int n)
{
	int count = 0;

	for(int i = strlen(string); i > 0; i--)
	{
		if(string[i] == x)
		{
			count++;
			if(count == n)
				return string + i;
		}
	}

	return NULL;
}

/* builds and returns a destination path
	and a destination path + filename */
void getPaths(char *destPath, char *destFile, char *fileName, info_t *info)
{
	/* gets the 'root' path of the source path to
		append to the destination path */
	char *destSuffix = reverseNChar(info->srcPrefix, '/', info->pathLevel + 1);

	// build the full, new destination path
	snprintf(destPath, PATH_MAX, "%s%s", info->destPrefix, destSuffix);

	// build the destination path + filename
	snprintf(destFile, PATH_MAX, "%s/%s", destPath, fileName);
}
