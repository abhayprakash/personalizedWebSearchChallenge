#pragma once
#include "variables.h"

class DomainURL_Logger
{
	FILE* fp;
	char *buffer;
	unsigned long long sizeOfBuffer;
	unsigned long long bytesUsedInBuffer;
	void flush();
public:
	DomainURL_Logger();
	void log(int did, int urlid);
	void wrapUp();
	~DomainURL_Logger();
};

