#ifndef FEATURE_FILE_LOGGER_H_INCLUDED
#define FEATURE_FILE_LOGGER_H_INCLUDED

#include "variables.h"
#include "global.h"

/* File Format:
user_id, query_id, url_id,
count_earlier_shown,count_earlier_2,count_earlier_1,count_earlier_0, // these till time of this query : long + short
url_position,
{url_displayed_in_same_session(bool), time_difference_from_most_recent_display, grade_that_time(0/1/2)},
{url_displayed_before_current_session(bool), day_difference_from_most_recent, grade_that_time},
{similar_query_in_same_session(bool), time_diff_from_most_recent, this_url_shown(bool), grade_that_time},
{similar_query_before_current_session(bool), day_difference_from_most_recent, this_url_shown(bool), grade_that_time},
grade_this_time
*/

class FileLogger{
	FILE* fp;
	char *buffer;
	char tempPath[256];
	
	unsigned long long sizeOfBuffer;
	unsigned long long bytesUsedInBuffer;
	int sizeOfRow;

	void checkAndFlush(unsigned long long &bytesUsedInBuffer);
public:
	FileLogger(char* path, unsigned long long size_buff, int size_row);
	void logTrain(rowToLog &rowLog);
	void logDU(int did, int urlid);
	void logSU(int sid, int urlid);
	void logQT(int qid, int termid);
	void logQT_All(map<int, vector<int> > &queryTerms);
	void logTest(rowToLog &rowLog);
	void logMap(resultRow &result_Row);
	void logValidate(rowToLog &rowLog); 
	void logVR(int actualResult); 
	void wrapUp();
	~FileLogger();
};

#endif