#ifndef USR_URL_H_INCLUDED
#define USR_URL_H_INCLUDED

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
		int count_shown, count_r[3], last_time_day, latest_grade; // count_r[grade] contains the time this url was relevant of 'grade' - so have count of click too indirectly
		rec_url()
		{
			count_shown = 0; 
			count_r[2] = 0; count_r[1] = 0; count_r[0] = 0;
			last_time_day = 0;
			latest_grade = UNCLICKED_CLASS; // we never need to put explicitly for un clicked urls
		}
	};

	void updateTable_Shown(int uid, int urlid, int day);
	void updateTable_Click(int uid, int urlid, int grade);
public:
	map<int, map<int, rec_url> > table, local;        // <user, url> -> {count, ...}
													  // local: <usr,url> -> {time, grade} for the usr in current session
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
	void getCountR(int uid, int urlid, int obtained[3]);
	int getLatestDayGrade(int uid, int urlid);
	int getLastDay(int uid, int urlid);

	// get Same Session params
	int getLastTime(int uid, int urlid);
	int getLatestTimeGrade(int uid, int urlid);
};

#endif