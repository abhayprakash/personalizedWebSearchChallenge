#include "FileLogger.h"
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#define _snprintf snprintf

FileLogger::FileLogger(char* path, unsigned long long size_buff, int size_row)
{
	printf("opening %s\n", path);
	fp = fopen(path, "w");
	strcpy(tempPath, path);
	sizeOfBuffer = size_buff;
	sizeOfRow = size_row;

	buffer = (char *)malloc(sizeOfBuffer);
	memset(buffer, 0, sizeOfBuffer);
	bytesUsedInBuffer = 0;
	fprintf(fp,"SessionID,URLID\n");
}

void FileLogger::checkAndFlush(unsigned long long &bytesUsedInBuffer)
{
	if (bytesUsedInBuffer >= sizeOfBuffer - sizeOfRow)
	{
		printf("flushing %s\n", tempPath);
		fwrite(buffer, bytesUsedInBuffer, 1, fp);
		memset(buffer, 0, sizeOfBuffer);
		bytesUsedInBuffer = 0;
	}
}

void FileLogger::logSubmit(int SessionID, int URLID)
{
	bytesUsedInBuffer += _snprintf( buffer + bytesUsedInBuffer, sizeOfRow, "%d,%d\n", SessionID, URLID);

	checkAndFlush(bytesUsedInBuffer);
}

void FileLogger::wrapUp()
{
	fwrite(buffer, bytesUsedInBuffer, 1, fp);
	fclose(fp);
	free(buffer);
	printf("submit file %s generation : Complete\n", tempPath);
}

FileLogger::~FileLogger()
{
	wrapUp();
}
