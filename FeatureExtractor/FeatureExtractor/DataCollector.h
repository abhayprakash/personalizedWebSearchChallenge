#pragma once
#include <vector>
#include "DomainURL_Logger.h"
#include "serpURL_Logger.h"

using namespace std;

int g_serpid = 0;
map<int, vector<int> > queryTerms; 

// this collects data and orders by default - provide table populated as public so as to be used by Processor
class DataCollector{
	FILE* fp;
	char *buffer;
	unsigned long long buffSize;
	DomainURL_Logger duLogger;
	serpURL_Logger suLogger;

	// Data Collected in memory
	struct shownURL{
		int url_id, timeOfClick, position;
	};
	struct queryRec{
		int qid, timeOfQuery;
		vector<shownURL> clickedURL;
	};
	struct sessionRec{
		int uid, session_id; 
		vector<queryRec> queries;
	};

	shownURL tempURL;
	queryRec tempQuery;
	sessionRec tempSession;
public:
	vector<sessionRec> RecordOfDay[31];
	DataCollector(); 
	void parseFile(char *path);
	void wrapUp();
};
