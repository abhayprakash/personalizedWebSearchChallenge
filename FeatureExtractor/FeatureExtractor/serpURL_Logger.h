#pragma once
#include "variables.h"

class serpURL_Logger
{
	FILE* fp;
	char *buffer;
	unsigned long long sizeOfBuffer;
	unsigned long long bytesUsedInBuffer;
	void flush();
public:
	serpURL_Logger();
	void log(int sid, int urlid);
	void wrapUp();
	~serpURL_Logger();
};

