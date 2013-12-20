#pragma once
#include "variables.h"
#include <map>
#include <vector>

using namespace std;

class queryTerm_Logger
{
	FILE* fp;
	char *buffer;
	unsigned long long sizeOfBuffer;
	unsigned long long bytesUsedInBuffer;
	void flush();
public:
	queryTerm_Logger();
	void logAll(map<int, vector<int> > &queryTerms);
	void log(int qid, int termid);
	void wrapUp();
	~queryTerm_Logger();
};