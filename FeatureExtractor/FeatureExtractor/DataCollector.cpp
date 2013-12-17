#include "stdafx.h"
#include "DataCollector.h"
#include "variables.h"
#include <sstream>

DataCollector::DataCollector() // phase I : provide path to train file, phase II : provide path to test file
{
	buffSize = BUFF_SIZE_INPUT_READ;
	buffer = (char *)malloc(buffSize);
}

void DataCollector::parse(int test_1_train_0)
{
	FILE* fp;
	if(test_1_train_0)
		fp = fopen(TEST_FILE, "r");
	else
		fp = fopen(TRAIN_FILE, "r");

	int nRead;
	while((nRead = fread(buffer, 1, buffSize, fp)) > 0)
	{
		if(test_1_train_0)
			printf("Read a chunk of test file\n");
		else
			printf("Read a chunk of train file\n");

		if(nRead == buffSize)
		{
			int movedBack = 0;
			while(buffer[nRead-1] != '\n')
			{
				nRead--;
				movedBack++;
			}
			fseek(fp, -movedBack, SEEK_CUR);
		}
		buffer[nRead] = 0;
		// do operation on buffer
		istringstream buff_in(buffer);
		string rowInLog;
		map<int, int> urlRank;
		int index_q;

		//temp vars
		int temp_sid, temp_day, temp_time, temp_uid, temp_serp, temp_qid, temp_term, temp_url, temp_domain;
		string temp_typeOrTime, temp_list, temp_s_term, temp_s_URLDomain, temp_s_URLID;;
		
		bool considerUser;
		while(getline(buff_in, rowInLog))
		{
			istringstream sin(rowInLog);
			sin>>temp_sid>>temp_typeOrTime;
			if(temp_typeOrTime == "M")
			{
				sin>>temp_day>>temp_uid;
				// temp_sid, temp_day, temp_uid
				if(test_1_train_0)
				{
					usersInTest[temp_uid] = true;
					considerUser = true;
				}
				else
				{
					if(usersInTest[temp_uid])
						considerUser = true;
					else
						considerUser = false;
				}
				if(considerUser)
				{
					userDaySessions[temp_uid].push_back(daySession(temp_day, temp_sid));
					RecordOfDay[temp_day][temp_sid].uid = temp_uid;
				}
				continue;
			}
			temp_time = atoi(temp_typeOrTime.c_str());
			sin>>temp_typeOrTime;
			
			if(temp_typeOrTime == "Q" || temp_typeOrTime == "T") // way of identifing T queries: last query of each session with day 28|29|30
			{
				sin>>temp_serp>>temp_qid;
				//temp_sid temp_time temp_serp temp_qid
				if(considerUser)
				{
					RecordOfDay[temp_day][temp_sid].queries.push_back(queryRec());
					index_q = RecordOfDay[temp_day][temp_sid].queries.size();
					RecordOfDay[temp_day][temp_sid].queries[index_q].qid = temp_qid;
					RecordOfDay[temp_day][temp_sid].queries[index_q].timeOfQuery = temp_time;
					RecordOfDay[temp_day][temp_sid].queries[index_q].shownSERP = g_serpid;
				}
				sin>>temp_list;
				istringstream tokenStream(temp_list);
				while(getline(tokenStream, temp_s_term, ','))
				{
					temp_term = atoi(temp_s_term.c_str());
					//temp_sid temp_time temp_serp temp_qid temp_term
					/*
					 * we can keep Queries of worthful users only but i think it will be used to get a separate SOM, so let me keep all queries 
                     * NOTE: queryid means the TEXTUAL query only
					 * wait that will cause over 100 Mn rows - do it for worthful users only
					 */
					if(considerUser)
					{
						queryTerms[temp_qid].push_back(temp_term); 
					}
				}

				urlRank.clear();
				for(int rank = 1; rank <= 10; rank++)
				{
					sin>>temp_s_URLDomain;
					tokenStream.clear();
					tokenStream.str(temp_s_URLDomain);
					getline(tokenStream, temp_s_URLID, ',');
					temp_url = atoi(temp_s_URLID.c_str());
					tokenStream>>temp_domain;
					//temp_sid temp_time temp_serp temp_qid temp_url temp_domain
					if(considerUser)
					{
						duLogger.log(temp_domain, temp_url);
						suLogger.log(g_serpid, temp_url); // could have flushed at one time as query-term but maintained the initial design
						serpURLs[g_serpid].push_back(temp_url);
						urlRank[temp_url] = rank;
					}
				}
				if(considerUser)
				{
					g_serpid++;
				}
				continue;
			}
			if(temp_typeOrTime == "C")
			{
				sin>>temp_serp>>temp_url;
				// temp_sid temp_time temp_serp temp_url
				if(considerUser)
				{
					RecordOfDay[temp_day][temp_sid].queries[index_q].clickedURL.push_back(shownURL());
					int index_u = RecordOfDay[temp_day][temp_sid].queries[index_q].clickedURL.size();
					RecordOfDay[temp_day][temp_sid].queries[index_q].clickedURL[index_u].position = urlRank[temp_url];
					RecordOfDay[temp_day][temp_sid].queries[index_q].clickedURL[index_u].url_id = temp_url;
					RecordOfDay[temp_day][temp_sid].queries[index_q].clickedURL[index_u].timeOfClick = temp_time;
				}
			}
		}
	}
	if(test_1_train_0)
		printf("test file parsing complete\n");
	else
		printf("train file parsing complete\n");
	fclose(fp);
}

void DataCollector::wrapUp()
{
	free(buffer);
	printf("data populated\n");	
}