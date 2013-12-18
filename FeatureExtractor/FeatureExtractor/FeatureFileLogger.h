#pragma once
#include "variables.h"

/* File Format:
user_id, query_id, url_id,count_earlier_shown,count_earlier_2,count_earlier_1,count_earlier_0,url_position,
{url_displayed_in_same_session(bool), time_difference_from_most_recent_display, grade_that_time(0/1/2)},
{url_displayed_before_current_session(bool), day_difference_from_most_recent, grade_that_time},
{similar_query_in_same_session(bool), time_diff_from_most_recent, this_url_shown(bool), grade_that_time},
{similar_query_before_current_session(bool), day_difference_from_most_recent, this_url_shown(bool), grade_that_time},
grade_this_time
*/

struct feature{
	bool exists;
	int time_diff; // treat as day_diff for the case of before session
	int grade;

	bool u_exists_for_q; // use for q_ss and q_bs only
	
	feature(){
		exists = false;
		time_diff = 0;
		grade = 0;
		u_exists_for_q = false;
	}
};

class FeatureFileLogger{
	int user_id, query_id, url_id, url_position, max_rel_of_this_url_for_this_user;
	feature url_sameSession, url_beforeSession, query_sameSession, query_beforeSession; // these will be replaced to similarity instead of exact
	int resultGrade;
	
	FILE* fp;
	char *buffer;
	unsigned long long sizeOfBuffer;
	unsigned long long bytesUsedInBuffer;

	char tempPath[256];

	void flushToFile();
public:
	FeatureFileLogger(char* path, int test_or_train);	
	void logTrain(int uid, int qid, int urlid, int count_earlier_shown, int count_earlier_2, int count_earlier_1, int count_earlier_0, int urlpos, feature &u_ss, feature &u_bs, feature &q_ss, feature &q_bs, int r);
	void logTest(int uid, int qid, int urlid, int count_earlier_shown, int count_earlier_2, int count_earlier_1, int count_earlier_0, int urlpos, feature &u_ss, feature &u_bs, feature &q_ss, feature &q_bs);
	void logMap(int rowNum, int session_id, int url_id, int url_pos); // to implement
	void logValidate(int uid, int qid, int urlid, int count_earlier_shown, int count_earlier_2, int count_earlier_1, int count_earlier_0, int urlpos, feature &u_ss, feature &u_bs, feature &q_ss, feature &q_bs); // to implement
	void logVR(int actualResult); // to implement
	void wrapUp();
	~FeatureFileLogger();
};

