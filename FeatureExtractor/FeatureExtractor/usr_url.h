#pragma once
#include <map>
#include "variables.h"
using namespace std;

/**
{url_displayed_in_same_session(bool), time_difference_from_most_recent_display, grade_that_time(0/1/2)},
{url_displayed_before_current_session(bool), day_difference_from_most_recent, grade_that_time},
**/

// DS for getting url data for particular user before session
class usr_url{
	struct rec_url{
		int count_shown, count_r[3], last_day, latest_day_grade; // count_r[grade] contains the time this url was relevant of 'grade' - so have count of click too indirectly
		rec_url()
		{
			count_shown = 0; 
			count_r[2] = 0; count_r[1] = 0; count_r[0] = 0;
			last_day = 0;
			latest_day_grade = UNCLICKED_CLASS; // we never need to put explicitly for un clicked urls
		}
	};

	struct rec_time_grade{
		int time, grade;
		rec_time_grade()
		{
			time = 0; grade = UNCLICKED_CLASS; // we never need to put explicitly for un clicked urls
		}
	};

	void updateTable_Shown(int uid, int urlid, int day);
	void updateTable_Click(int uid, int urlid, int grade);
public:
	map<int, map<int, rec_url> > table;        // <user, url> -> {count, ...}
	map<int, map<int, rec_time_grade> > local; // <usr, url> -> {time, grade}
	/*
	for each session
	-> for each url
	    -> get the day stuff by all get things
		-> get the session stuff
		-> update url record in local
	-> copy local to global
	*/
	void updateLocal_Shown(int uid, int urlid, int time);
	void updateLocal_Click(int uid, int urlid, int grade);

	void copyLocalToGlobal_and_ClearLocal(int day);

	bool existsBeforeSession(int uid, int urlid);
	bool existsCurrentSession(int uid, int urlid);

	// check exists before calling these
	// get Before Session params
	int getCountShown(int uid, int urlid);
	int getCountR(int uid, int urlid, int grade);
	int getLatestDayGrade(int uid, int urlid);
	int getLastDay(int uid, int urlid);

	// get Same Session params
	int getLastTime(int uid, int urlid);
	int getLatestTimeGrade(int uid, int urlid);
};