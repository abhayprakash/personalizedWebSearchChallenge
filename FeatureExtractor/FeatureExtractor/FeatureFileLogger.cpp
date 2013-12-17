#include "stdafx.h"
#include "FeatureFileLogger.h"
#include <cstring>
#include <cstdlib>

FeatureFileLogger::FeatureFileLogger(char* path, int test_or_train)
{
	fp = fopen(path, "w");
	if(test_or_train == CALL_FROM_PROCESS_TRAIN)
	{
		sizeOfBuffer = BUFF_SIZE_FEATURE_FILE;
	}
	else
	{
		sizeOfBuffer = BUFF_SIZE_FEATURE_TEST_FILE;
	}
	buffer = (char *)malloc(sizeOfBuffer);
	memset(buffer, 0, sizeOfBuffer);
		
	bytesUsedInBuffer = 0;
}
	
void FeatureFileLogger::logTrain(int uid, int qid, int urlid, int count_earlier_shown, int count_earlier_2, int count_earlier_1, int count_earlier_0, int urlpos, feature &u_ss, feature &u_bs, feature &q_ss, feature &q_bs, int r)
{
	char rowInLog[NUM_MAX_ROW_CHAR];
	memset(rowInLog, 0, NUM_MAX_ROW_CHAR);
	//prepare rowInLog
	sprintf(rowInLog, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", uid, qid, urlid, count_earlier_shown, count_earlier_2, count_earlier_1, count_earlier_0, urlpos, u_ss.exists, u_ss.time_diff, u_ss.grade, u_bs.exists, u_bs.time_diff, u_bs.grade, q_ss.exists, q_ss.time_diff, q_ss.u_exists_for_q, q_ss.grade, q_bs.exists, q_bs.time_diff, q_bs.u_exists_for_q, q_bs.grade, r);
		
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

void FeatureFileLogger::logTest(int uid, int qid, int urlid, int count_earlier_shown, int count_earlier_2, int count_earlier_1, int count_earlier_0, int urlpos, feature &u_ss, feature &u_bs, feature &q_ss, feature &q_bs)
{
	char rowInLog[NUM_MAX_ROW_CHAR];
	memset(rowInLog, 0, NUM_MAX_ROW_CHAR);
	//prepare rowInLog
	sprintf(rowInLog, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", uid, qid, urlid, count_earlier_shown, count_earlier_2, count_earlier_1, count_earlier_0, urlpos, u_ss.exists, u_ss.time_diff, u_ss.grade, u_bs.exists, u_bs.time_diff, u_bs.grade, q_ss.exists, q_ss.time_diff, q_ss.u_exists_for_q, q_ss.grade, q_bs.exists, q_bs.time_diff, q_bs.u_exists_for_q, q_bs.grade);
		
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
	printf("flushing feature file\n");
	fwrite(buffer, bytesUsedInBuffer, 1, fp);
	memset(buffer, 0, sizeOfBuffer);
	bytesUsedInBuffer = 0;
}

void FeatureFileLogger::wrapUp()
{
	fclose(fp);
	free(buffer);
	printf("feature file generation : Complete\n");
}

FeatureFileLogger::~FeatureFileLogger()
{
	wrapUp();
}