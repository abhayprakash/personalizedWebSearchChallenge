#pragma once
#include <vector>
#include <map>

// id for serp for each query
int g_serpid = 0;

// whether the user has record in test file
map<int, bool> usersInTest;

// query to its terms
map<int, vector<int> > queryTerms;

// g_serpid to its urls
map<int, vector<int> > serpURLs;

// to retrieve which records for *this user
/*
struct daySession{
	int day, session_id;
	daySession(int x, int y)
	{
		day = x; session_id = y;
	}
};
map<int, vector<daySession> > userDaySessions; // use it as userDaySessions[user_id][i].{day|session}
*/

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
sessionRec RecordOfDay[31]; // use it as RecordOfDay[day][session_id].{uid|queries[i].{qid|timeOfQuery|clickedURL[i].{urlid|timeOfClick|position}}}
