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

class FeatureFileLogger{
	FILE* fp;
	char *buffer;
	unsigned long long sizeOfBuffer;
	unsigned long long bytesUsedInBuffer;

	char tempPath[256];

	void flushToFile();
public:
	FeatureFileLogger(char* path, int test_or_train);	
	void logTrain(rowToLog &rowLog);
	void logTest(rowToLog &rowLog);
	void logMap(resultRow &rowLog); 
	void logValidate(rowToLog &rowLog); 
	void logVR(int actualResult); 
	void wrapUp();
	~FeatureFileLogger();
};

#endif