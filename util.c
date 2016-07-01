#define _DEFAULT_SOURCE
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

	for(int i = strlen(string); i > 0; i++)
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
