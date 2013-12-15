#pragma once
#include "variables.h"

class serpURL_Logger
{
	FILE* fp;
	void flush();
public:
	serpURL_Logger();
	void log(int sid, int urlid);
	void wrapUp();
	~serpURL_Logger();
};

