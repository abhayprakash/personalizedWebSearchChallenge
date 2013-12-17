#pragma once
#include <vector>
#include <map>
#include "DomainURL_Logger.h"
#include "serpURL_Logger.h"

using namespace std;

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

// this collects data and orders by default - provide table populated as public so as to be used by Processor
class DataCollector{
	char *buffer;
	unsigned long long buffSize;
	DomainURL_Logger duLogger;
	serpURL_Logger suLogger;

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

public:
	sessionRec RecordOfDay[31]; // use it as RecordOfDay[day][session_id].{uid|queries[i].{qid|timeOfQuery|clickedURL[i].{urlid|timeOfClick|position}}}
	DataCollector(); 
	void parse(int test_1_train_0);
	void wrapUp();
};
