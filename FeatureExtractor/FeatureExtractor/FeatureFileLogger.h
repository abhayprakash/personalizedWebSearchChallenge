#pragma once
#include "variables.h"

/* File Format:
user_id, query_id, url_id, url_position,
{url_displayed_in_same_session(bool), time_difference_from_most_recent_display, clicked(bool), grade_that_time(0/1/2)},
{url_displayed_before_current_session(bool), day_difference_from_most_recent, clicked(bool), grade_that_time},
{similar_query_in_same_session(bool), time_diff_from_most_recent, this_url_shown(bool), grade_that_time},
{similar_query_before_current_session(bool), day_difference_from_most_recent, this_url_shown(bool), grade_that_time},
grade_this_time
*/

struct feature{
	bool exists;
	int time_diff; // treat as day_diff for the case of before session
	bool clicked;
	int grade;
	
	feature(){
		clicked = false;
		time_diff = 0;
		grade = 0;
	}
};

class FeatureFile{
	int user_id, query_id, url_id, url_position;
	feature url_sameSession, url_beforeSession, query_sameSession, query_beforeSession; // these will be replaced to similarity instead of exact
	int resultGrade;
	
	FILE* fp;
	char *buffer;
	const unsigned long long sizeOfBuffer = BUFF_SIZE_FEATURE_FILE;
	unsigned long long bytesUsedInBuffer;

public:
	FeatureFile();	
	void logRecord(int uid, int qid, int urlid, int urlpos, feature &u_ss, feature &u_bs, feature &q_ss, feature &q_bs, int r);
	void flushToFile();
};

