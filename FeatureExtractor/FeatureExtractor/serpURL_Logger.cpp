#include "stdafx.h"
#include "serpURL_Logger.h"
#include <cstring>
#include <cstdlib>

void serpURL_Logger::flush()
{
	printf("flushing SU file\n");
	fwrite(buffer, bytesUsedInBuffer, 1, fp);
	memset(buffer, 0, sizeOfBuffer);
	bytesUsedInBuffer = 0;
}

serpURL_Logger::serpURL_Logger()
{
	sizeOfBuffer = BUFF_SIZE_SU_FILE;
	fp = fopen(SERP_URL_FILE, "w");
	buffer = (char *)malloc(sizeOfBuffer);
	memset(buffer, 0, sizeOfBuffer);
		
	bytesUsedInBuffer = 0;
}

void serpURL_Logger::log(int sid, int urlid)
{
	char rowInLog[NUM_MAX_ROW_CHAR];
	memset(rowInLog, 0, NUM_MAX_ROW_CHAR);
	//prepare rowInLog
	sprintf(rowInLog, "%d,%d\n", sid, urlid);
		
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

void serpURL_Logger::wrapUp()
{
	fclose(fp);
	free(buffer);
	printf("SU file generation : Complete\n");
}

serpURL_Logger::~serpURL_Logger()
{
	wrapUp();
}
