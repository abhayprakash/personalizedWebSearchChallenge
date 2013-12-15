#pragma once
#include "variables.h"
#include <map>

using namespace std;

/*
{similar_query_in_same_session(bool), time_diff_from_most_recent, this_url_shown(bool), grade_that_time},
{similar_query_before_current_session(bool), day_difference_from_most_recent, this_url_shown(bool), grade_that_time},
*/

// DS for getting similar query for particular user before session
class usr_qry{
	int getRecentSimilarQuery(int qid);
public:
	map<int, int> ; // public: for the case i want to use it directly
	
	void update(int uid, int qid, int mg, int lg, int ld);
	void updateDayGrade(int uid, int qid, int day, int grade);
	bool exists(int uid, int qid);
	int getMaxGrade_forSimilar(int uid, int qid);
	int getLatestGrade_forSimilar(int uid, int qid);
	int getLastDay_forSimilar(int uid, int qid);
};
