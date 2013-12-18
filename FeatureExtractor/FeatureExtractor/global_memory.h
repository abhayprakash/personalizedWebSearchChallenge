#ifndef _GLOBAL_MEMORY_H
#define _GLOBAL_MEMORY_H
#else

#include <vector>
#include <map>

int g_serpid = 0;
map<int, bool> usersInTest;
map<int, vector<int> > queryTerms; 
map<int, vector<int> > serpURLs;
struct daySession{
	int day, session_id;
	daySession(int x, int y)
	{
		day = x; session_id = y;
	}
};
map<int, vector<daySession> > userDaySessions; // use it as userDaySessions[user_id][i].{day|session}

// Data Collected in memory
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

#endif 