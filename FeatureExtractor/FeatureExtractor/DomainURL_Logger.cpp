#include "stdafx.h"
#include "DomainURL_Logger.h"
#include <cstring>
#include <cstdlib>

void DomainURL_Logger::flush()
{
	printf("flushing DU\n");
	fwrite(buffer, bytesUsedInBuffer, 1, fp);
	memset(buffer, 0, sizeOfBuffer);
	bytesUsedInBuffer = 0;
}

DomainURL_Logger::DomainURL_Logger()
{
	sizeOfBuffer = BUFF_SIZE_DU_FILE;
	fp = fopen(DOMAIN_URL_FILE, "w");
	buffer = (char *)malloc(sizeOfBuffer);
	memset(buffer, 0, sizeOfBuffer);
		
	bytesUsedInBuffer = 0;
}

void DomainURL_Logger::log(int did, int urlid)
{
	char rowInLog[NUM_MAX_ROW_CHAR];
	memset(rowInLog, 0, NUM_MAX_ROW_CHAR);
	//prepare rowInLog
	sprintf(rowInLog, "%d,%d\n", did, urlid);
		
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

void DomainURL_Logger::wrapUp()
{
	fclose(fp);
	free(buffer);
	printf("DU file generation : complete\n");
}

DomainURL_Logger::~DomainURL_Logger()
{
	wrapUp();
}
