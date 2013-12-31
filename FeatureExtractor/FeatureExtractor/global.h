#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include "variables.h"
#include "usr_url.h"
#include "usr_qry.h"
#include <vector>
#include <map>

using namespace std;

// id for serp for each query
extern int g_serpid;

// query to its terms
extern map<int, vector<int> > queryTerms;

// g_serpid to its urls
extern map<int, vector<int> > table_serpURLs;

// whether the user is in test file or not
extern map<int, bool> userExistsInTest;

// global ds to store temporal features
extern usr_url store_usrURL;
extern usr_qry store_usrQry;

// Data Collected in memory : RecordOfDay[]
struct shownURL{
	int url_id, timeOfClick, position;
	shownURL(int u, int t, int p)
	{
		url_id = u; timeOfClick = t; position = p;
	}
};
struct queryRec{
	int qid, timeOfQuery, shownSERP;
	vector<shownURL> clickedURL;
	queryRec(int q, int time, int gSerp)
	{
		qid = q; timeOfQuery = time; shownSERP = gSerp;
	}
};
struct sessMetaData{
	int day, sessionId; 
	vector<queryRec> queries;
	sessMetaData(int d, int sid)
	{
		day = d;
		sessionId = sid;
	}
};
struct userData{
	int uid;
	vector<sessMetaData> session;
};

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