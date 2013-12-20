#include "stdafx.h"
#include "queryTerm_Logger.h"
#include <cstring>
#include <cstdlib>

void queryTerm_Logger::flush()
{
	printf("flushing QT file\n");
	fwrite(buffer, bytesUsedInBuffer, 1, fp);
	memset(buffer, 0, sizeOfBuffer);
	bytesUsedInBuffer = 0;
}

queryTerm_Logger::queryTerm_Logger()
{
	sizeOfBuffer = BUFF_SIZE_QT_FILE;
	fp = fopen(QUERY_TERM_FILE, "w");
	buffer = (char *)malloc(sizeOfBuffer);
	memset(buffer, 0, sizeOfBuffer);
		
	bytesUsedInBuffer = 0;
}

void queryTerm_Logger::logAll(map<int, vector<int> > &queryTerms)
{
	map<int, vector<int> >::iterator it;
	for(it = queryTerms.begin(); it!=queryTerms.end(); it++)
	{
		for(unsigned int i = 0; i < it->second.size(); i++)
			log(it->first, it->second[i]);
	}
}

void queryTerm_Logger::log(int qid, int termid)
{
	char rowInLog[NUM_MAX_ROW_CHAR];
	memset(rowInLog, 0, NUM_MAX_ROW_CHAR);
	//prepare rowInLog
	sprintf(rowInLog, "%d,%d\n", qid, termid);
		
	int bytesInRow = strlen(rowInLog);
	if(bytesInRow + bytesUsedInBuffer <= sizeOfBuffer)
	{
		strcat(buffer, rowInLog);
		bytesUsedInBuffer += bytesInRow;
	}
	else
	{
		flush();
		strcat(buffer, rowInLog);
		bytesUsedInBuffer += bytesInRow;
	}
}

void queryTerm_Logger::wrapUp()
{
	fclose(fp);
	free(buffer);
	printf("QT file generation : Complete\n");
}

queryTerm_Logger::~queryTerm_Logger()
{
	wrapUp();
}