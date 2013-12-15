#include "stdafx.h"
#include "FeatureFileLogger.h"
#include <cstring>
#include <cstdlib>

FeatureFileLogger::FeatureFileLogger(void)
{
	fp = fopen(FEATURE_FILE, "w");
	buffer = (char *)malloc(sizeOfBuffer);
	memset(buffer, 0, sizeOfBuffer);
		
	bytesUsedInBuffer = 0;
}
	
void FeatureFileLogger::logRecord(int uid, int qid, int urlid, int urlpos, int max_rel, feature &u_ss, feature &u_bs, feature &q_ss, feature &q_bs, int r)
{
	char rowInLog[NUM_MAX_ROW_CHAR];
	memset(rowInLog, 0, NUM_MAX_ROW_CHAR);
	//prepare rowInLog
	sprintf(rowInLog, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", uid, qid, urlid, urlpos, max_rel, u_ss.exists, u_ss.time_diff, u_ss.grade, u_bs.exists, u_bs.time_diff, u_bs.grade, q_ss.exists, q_ss.time_diff, q_ss.grade, q_bs.exists, q_bs.time_diff, q_bs.grade, r);
		
	int bytesInRow = strlen(rowInLog);
	if(bytesInRow + bytesUsedInBuffer <= sizeOfBuffer)
	{
		strcat(buffer, rowInLog);
		bytesUsedInBuffer += bytesInRow;
	}
	else
	{
		flushToFile();
		strcat(buffer, rowInLog);
		bytesUsedInBuffer += bytesInRow;
	}
}

void FeatureFileLogger::flushToFile()
{
	printf("flushing feature train file\n");
	fwrite(buffer, bytesUsedInBuffer, 1, fp);
	memset(buffer, 0, sizeOfBuffer);
	bytesUsedInBuffer = 0;
}

void FeatureFileLogger::wrapUp()
{
	fclose(fp);
	free(buffer);
	printf("feature train file generation : Complete\n");
}

FeatureFileLogger::~FeatureFileLogger()
{
	wrapUp();
}