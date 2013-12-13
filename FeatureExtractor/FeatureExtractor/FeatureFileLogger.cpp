#include "stdafx.h"
#include "FeatureFileLogger.h"
#include <cstring>
#include <cstdlib>

FeatureFile::FeatureFile(void)
{
	fp = fopen(FEATURE_FILE, "w");
	buffer = (char *)malloc(sizeOfBuffer);
	memset(buffer, 0, sizeOfBuffer);
		
	bytesUsedInBuffer = 0;
}
	
void FeatureFile::logRecord(int uid, int qid, int urlid, int urlpos, feature &u_ss, feature &u_bs, feature &q_ss, feature &q_bs, int r)
{
	char rowInLog[NUM_MAX_ROW_CHAR];
	memset(rowInLog, 0, NUM_MAX_ROW_CHAR);
	//prepare rowInLog
	sprintf(rowInLog, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", uid, qid, urlid, urlpos, u_ss.exists, u_ss.time_diff, u_ss.clicked, u_ss.grade, u_bs.exists, u_bs.time_diff, u_bs.clicked, u_bs.grade, q_ss.exists, q_ss.time_diff, q_ss.clicked, q_ss.grade, q_bs.exists, q_bs.time_diff, q_bs.clicked, q_bs.grade, r);
		
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

void FeatureFile::flushToFile()
{
	fwrite(buffer, bytesUsedInBuffer, 1, fp);
	memset(buffer, 0, sizeOfBuffer);
	bytesUsedInBuffer = 0;
}