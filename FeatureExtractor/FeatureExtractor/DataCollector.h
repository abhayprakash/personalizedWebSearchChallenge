#ifndef DATACOLLECTOR_H_INCLUDED
#define DATACOLLECTOR_H_INCLUDED

#include "variables.h"
#include <sstream>
#include "FileLogger.h"
#include "Processor.h"

using namespace std;

// this collects data and orders by default - provide table populated as public so as to be used by Processor
class DataCollector{
	char *buffer;
	unsigned long long buffSize;
	FILE* fp;

	userData* RecordOfUser;

	/***** Temp globals *****/
	int nRead;
	string rowInLog;
	map<int, int> urlRank;
	int index_q;

	//temp vars
	int prev_uid, index_session;
	int temp_sid, temp_day, temp_time, temp_uid, temp_serp, temp_qid, temp_term, temp_url, temp_domain;
	string temp_typeOrTime, temp_list, temp_s_term, temp_s_URLDomain, temp_s_URLID;
	
	bool considerUser;
	/************************/
	
	void processOneFile(int test_1_train_0);
public:
	DataCollector(); 
	void parse(int test_1_train_0);
	void wrapUp();
	void collectTestUserList();
};

#endif