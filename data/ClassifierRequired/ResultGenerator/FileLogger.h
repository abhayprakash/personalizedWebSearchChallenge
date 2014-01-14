#ifndef FEATURE_FILE_LOGGER_H_INCLUDED
#define FEATURE_FILE_LOGGER_H_INCLUDED

#include "variables.h"

class FileLogger{
	FILE* fp;
	char *buffer;
	char tempPath[256];

	unsigned long long sizeOfBuffer;
	unsigned long long bytesUsedInBuffer;
	int sizeOfRow;

	void checkAndFlush(unsigned long long &bytesUsedInBuffer);
public:
	FileLogger(char* path, unsigned long long size_buff, int size_row);
	void logSubmit(int SessionID, int URLID);
	void wrapUp();
	~FileLogger();
};

#endif
