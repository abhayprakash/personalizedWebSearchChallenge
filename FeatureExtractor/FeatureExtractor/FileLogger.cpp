#include "FileLogger.h"
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#define _snprintf snprintf

FileLogger::FileLogger(char* path, unsigned long long size_buff, int size_row)
{
	printf("opening %s\n", path);
	fp = fopen(path, "w");
	strcpy(tempPath, path);
	sizeOfBuffer = size_buff;
	sizeOfRow = size_row;

	buffer = (char *)malloc(sizeOfBuffer);
	memset(buffer, 0, sizeOfBuffer);
	bytesUsedInBuffer = 0;
}

void FileLogger::checkAndFlush(unsigned long long &bytesUsedInBuffer)
{
	if (bytesUsedInBuffer >= sizeOfBuffer - sizeOfRow)
	{
		printf("flushing %s\n", tempPath);
		fwrite(buffer, bytesUsedInBuffer, 1, fp);
		memset(buffer, 0, sizeOfBuffer);
		bytesUsedInBuffer = 0;
	}
}

void FileLogger::logQT(int qid, int termid)
{
	bytesUsedInBuffer += _snprintf( buffer + bytesUsedInBuffer, sizeOfRow, "%d,%d\n", qid, termid);

	checkAndFlush(bytesUsedInBuffer);
}

void FileLogger::logQT_All(map<int, vector<int> > &queryTerms)
{
	map<int, vector<int> >::iterator it;
	for(it = queryTerms.begin(); it!=queryTerms.end(); it++)
	{
		for(unsigned int i = 0; i < it->second.size(); i++)
			logQT(it->first, it->second[i]);
	}
}

void FileLogger::logSU(int sid, int urlid)
{
	bytesUsedInBuffer += _snprintf( buffer + bytesUsedInBuffer, sizeOfRow, "%d,%d\n", sid, urlid);

	checkAndFlush(bytesUsedInBuffer);
}

void FileLogger::logDU(int did, int urlid)
{
	bytesUsedInBuffer += _snprintf( buffer + bytesUsedInBuffer, sizeOfRow, "%d,%d\n", did, urlid);

	checkAndFlush(bytesUsedInBuffer);
}

void FileLogger::logTrain(rowToLog &rowLog)
{
	bytesUsedInBuffer += _snprintf( buffer + bytesUsedInBuffer, sizeOfRow, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", rowLog.user_id, rowLog.query_id, rowLog.url_id, rowLog.count_earlier_shown, rowLog.count_earlier_2, rowLog.count_earlier_1, rowLog.count_earlier_0, rowLog.url_position, rowLog.url_sameSession.exists, rowLog.url_sameSession.time_diff, rowLog.url_sameSession.grade, rowLog.url_beforeSession.exists, rowLog.url_beforeSession.time_diff, rowLog.url_beforeSession.grade, rowLog.query_sameSession.exists, rowLog.query_sameSession.time_diff, rowLog.query_sameSession.u_exists_for_q, rowLog.query_sameSession.grade, rowLog.query_beforeSession.exists, rowLog.query_beforeSession.time_diff, rowLog.query_beforeSession.u_exists_for_q, rowLog.query_beforeSession.grade, rowLog.resultGrade);

	checkAndFlush(bytesUsedInBuffer);
}

void FileLogger::logTest(rowToLog &rowLog)
{
	bytesUsedInBuffer += _snprintf( buffer + bytesUsedInBuffer, sizeOfRow, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", rowLog.user_id, rowLog.query_id, rowLog.url_id, rowLog.count_earlier_shown, rowLog.count_earlier_2, rowLog.count_earlier_1, rowLog.count_earlier_0, rowLog.url_position, rowLog.url_sameSession.exists, rowLog.url_sameSession.time_diff, rowLog.url_sameSession.grade, rowLog.url_beforeSession.exists, rowLog.url_beforeSession.time_diff, rowLog.url_beforeSession.grade, rowLog.query_sameSession.exists, rowLog.query_sameSession.time_diff, rowLog.query_sameSession.u_exists_for_q, rowLog.query_sameSession.grade, rowLog.query_beforeSession.exists, rowLog.query_beforeSession.time_diff, rowLog.query_beforeSession.u_exists_for_q, rowLog.query_beforeSession.grade);

	checkAndFlush(bytesUsedInBuffer);
}

// use rowNum to get the class of this row from the other result file produced by classifier
// result_Row.rowNum, result_Row.session_id, result_Row.url_id, result_Row.url_pos
void FileLogger::logMap(resultRow &result_Row)
{
	bytesUsedInBuffer += _snprintf( buffer + bytesUsedInBuffer, sizeOfRow, "%d,%d,%d,%d\n", result_Row.rowNum, result_Row.session_id, result_Row.url_id, result_Row.url_pos); // use it with result column to print the result, url_pos is for sorting the same predicted class

	checkAndFlush(bytesUsedInBuffer);
}

void FileLogger::logValidate(rowToLog &rowLog) // to implement
{
	bytesUsedInBuffer += _snprintf( buffer + bytesUsedInBuffer, sizeOfRow, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", rowLog.user_id, rowLog.query_id, rowLog.url_id, rowLog.count_earlier_shown, rowLog.count_earlier_2, rowLog.count_earlier_1, rowLog.count_earlier_0, rowLog.url_position, rowLog.url_sameSession.exists, rowLog.url_sameSession.time_diff, rowLog.url_sameSession.grade, rowLog.url_beforeSession.exists, rowLog.url_beforeSession.time_diff, rowLog.url_beforeSession.grade, rowLog.query_sameSession.exists, rowLog.query_sameSession.time_diff, rowLog.query_sameSession.u_exists_for_q, rowLog.query_sameSession.grade, rowLog.query_beforeSession.exists, rowLog.query_beforeSession.time_diff, rowLog.query_beforeSession.u_exists_for_q, rowLog.query_beforeSession.grade);

	checkAndFlush(bytesUsedInBuffer);
}

void FileLogger::logVR(int actualResult) // to implement
{
	bytesUsedInBuffer += _snprintf( buffer + bytesUsedInBuffer, sizeOfRow, "%d\n", actualResult);

	checkAndFlush(bytesUsedInBuffer);
}

void FileLogger::wrapUp()
{
    if(fp)
    {
        fwrite(buffer, bytesUsedInBuffer, 1, fp);
        fclose(fp);
        fp = NULL;
        free(buffer);
        printf("feature file %s generation : Complete\n", tempPath);
    }
}

FileLogger::~FileLogger()
{
	wrapUp();
}
