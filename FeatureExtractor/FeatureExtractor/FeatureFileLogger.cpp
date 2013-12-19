#include "stdafx.h"
#include "FeatureFileLogger.h"
#include <cstring>
#include <cstdlib>

FeatureFileLogger::FeatureFileLogger(char* path, int test_or_train)
{
	fp = fopen(path, "w");
	strcpy(tempPath, path);
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
	
void FeatureFileLogger::logTrain(rowToLog &rowLog)
{
	char rowInLog[NUM_MAX_ROW_CHAR];
	memset(rowInLog, 0, NUM_MAX_ROW_CHAR);
	//prepare rowInLog
	sprintf(rowInLog, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", rowLog.user_id, rowLog.query_id, rowLog.url_id, rowLog.count_earlier_shown, rowLog.count_earlier_2, rowLog.count_earlier_1, rowLog.count_earlier_0, rowLog.url_position, rowLog.url_sameSession.exists, rowLog.url_sameSession.time_diff, rowLog.url_sameSession.grade, rowLog.url_beforeSession.exists, rowLog.url_beforeSession.time_diff, rowLog.url_beforeSession.grade, rowLog.query_sameSession.exists, rowLog.query_sameSession.time_diff, rowLog.query_sameSession.u_exists_for_q, rowLog.query_sameSession.grade, rowLog.query_beforeSession.exists, rowLog.query_beforeSession.time_diff, rowLog.query_beforeSession.u_exists_for_q, rowLog.query_beforeSession.grade, rowLog.resultGrade);
		
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

void FeatureFileLogger::logTest(rowToLog &rowLog)
{
	char rowInLog[NUM_MAX_ROW_CHAR];
	memset(rowInLog, 0, NUM_MAX_ROW_CHAR);
	//prepare rowInLog
	sprintf(rowInLog, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", , rowLog.user_id, rowLog.query_id, rowLog.url_id, rowLog.count_earlier_shown, rowLog.count_earlier_2, rowLog.count_earlier_1, rowLog.count_earlier_0, rowLog.url_position, rowLog.url_sameSession.exists, rowLog.url_sameSession.time_diff, rowLog.url_sameSession.grade, rowLog.url_beforeSession.exists, rowLog.url_beforeSession.time_diff, rowLog.url_beforeSession.grade, rowLog.query_sameSession.exists, rowLog.query_sameSession.time_diff, rowLog.query_sameSession.u_exists_for_q, rowLog.query_sameSession.grade, rowLog.query_beforeSession.exists, rowLog.query_beforeSession.time_diff, rowLog.query_beforeSession.u_exists_for_q, rowLog.query_beforeSession.grade);
		
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

void FeatureFileLogger::logMap(resultRow &rowLog) // to implement
{
	char rowInLog[NUM_MAX_ROW_CHAR];
	memset(rowInLog, 0, NUM_MAX_ROW_CHAR);
	//prepare rowInLog
	sprintf(rowInLog, "%d,%d,%d,%d\n", rowLog.rowNum, rowLog.session_id, rowLog.url_id, rowLog.url_pos); // use it with result column to print the result, url_pos is for sorting the same predicted class
		
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

void FeatureFileLogger::logValidate(rowToLog &rowLog) // to implement
{
	char rowInLog[NUM_MAX_ROW_CHAR];
	memset(rowInLog, 0, NUM_MAX_ROW_CHAR);
	//prepare rowInLog
	sprintf(rowInLog, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", , rowLog.user_id, rowLog.query_id, rowLog.url_id, rowLog.count_earlier_shown, rowLog.count_earlier_2, rowLog.count_earlier_1, rowLog.count_earlier_0, rowLog.url_position, rowLog.url_sameSession.exists, rowLog.url_sameSession.time_diff, rowLog.url_sameSession.grade, rowLog.url_beforeSession.exists, rowLog.url_beforeSession.time_diff, rowLog.url_beforeSession.grade, rowLog.query_sameSession.exists, rowLog.query_sameSession.time_diff, rowLog.query_sameSession.u_exists_for_q, rowLog.query_sameSession.grade, rowLog.query_beforeSession.exists, rowLog.query_beforeSession.time_diff, rowLog.query_beforeSession.u_exists_for_q, rowLog.query_beforeSession.grade);
		
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

void FeatureFileLogger::logVR(int actualResult) // to implement
{
	char rowInLog[NUM_MAX_ROW_CHAR];
	memset(rowInLog, 0, NUM_MAX_ROW_CHAR);
	//prepare rowInLog
	sprintf(rowInLog, "%d\n", actualResult);
		
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
	printf("flushing file %s\n", tempPath);
	fwrite(buffer, bytesUsedInBuffer, 1, fp);
	memset(buffer, 0, sizeOfBuffer);
	bytesUsedInBuffer = 0;
}

void FeatureFileLogger::wrapUp()
{
	flushToFile();
	fclose(fp);
	free(buffer);
	printf("feature file %s generation : Complete\n", tempPath);
}

FeatureFileLogger::~FeatureFileLogger()
{
	wrapUp();
}