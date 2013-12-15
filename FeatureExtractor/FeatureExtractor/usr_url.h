#pragma once
#include <map>
using namespace std;

// DS for getting url data for particular user before session
class usr_url{
	struct rec_url{
		int count_shown, count_r[3], latest_grade, last_day; 
		rec_url()
		{
			count_shown = 0; 
			count_r[2] = 0; count_r[1] = 0; count_r[0] = 0;
			latest_grade = 0;
			last_day = 0;
		}
	};
public:
	map<int, map<int, rec_url> > table;

	void updateShown(int uid, int urlid, int day);
	void updateClick(int uid, int urlid, int grade);
	bool exists(int uid, int urlid);
	int getCountShown(int uid, int urlid);
	int getCountR(int uid, int urlid, int grade);
	int getLatestGrade(int uid, int urlid);
	int getLastDay(int uid, int urlid);
};