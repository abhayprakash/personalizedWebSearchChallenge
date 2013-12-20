#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include "variables.h"
#include <vector>
#include <map>

using namespace std;

// id for serp for each query
extern int g_serpid;

// whether the user has record in test file
extern map<int, bool> usersInTest;

// query to its terms
extern map<int, vector<int> > queryTerms;

// g_serpid to its urls
extern map<int, vector<int> > table_serpURLs;

// Data Collected in memory : RecordOfDay[]
struct shownURL{
	int url_id, timeOfClick, position;
};
struct queryRec{
	int qid, timeOfQuery, shownSERP;
	vector<shownURL> clickedURL;
};
struct sessMetaData{
	int uid; 
	vector<queryRec> queries;
};
typedef map<int, sessMetaData> sessionRec; // first int is session_id
extern sessionRec RecordOfDay[31]; // use it as RecordOfDay[day][session_id].{uid|queries[i].{qid|timeOfQuery|clickedURL[i].{urlid|timeOfClick|position}}}

// Feature
struct feature{
	bool exists;
	int time_diff; // treat as day_diff for the case of before session
	int grade;

	bool u_exists_for_q; // use for q_ss and q_bs only
	
	feature(){
		exists = false;
		time_diff = 0;
		grade = UNCLICKED_CLASS;
		u_exists_for_q = false;
	}
};

// to log
struct rowToLog{
	int user_id, query_id, url_id, count_earlier_shown, count_earlier_2, count_earlier_1, count_earlier_0, url_position;
	feature url_sameSession, url_beforeSession, query_sameSession, query_beforeSession;
	int resultGrade;
};

struct resultRow{
	int rowNum, session_id, url_id, url_pos;
};

#endif