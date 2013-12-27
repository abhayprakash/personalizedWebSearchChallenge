#include "stdafx.h"
#include "DataCollector.h"
#include "global.h"

DataCollector::DataCollector() // phase I : provide path to train file, phase II : provide path to test file
{
	buffSize = BUFF_SIZE_INPUT_READ;
	buffer = (char *)malloc(buffSize);
//	duLogger =  new FileLogger(DOMAIN_URL_FILE, BUFF_SIZE_DU_FILE, MAX_ROW_2_TERMS);
//	suLogger = new FileLogger(SERP_URL_FILE, BUFF_SIZE_SU_FILE, MAX_ROW_2_TERMS);
}

void DataCollector::processOneFile(int test_1_train_0)
{
	while((nRead = fread(buffer, 1, buffSize, fp)) > 0)
	{
		buffer[nRead] = 0;
		printf("Read a chunk - %d bytes of given file\n", nRead);
		
		// do operation on buffer
		istringstream buff_in(buffer);
		int countRow = 0;
		while(getline(buff_in, rowInLog))
		{
			countRow++;
			if(countRow == 1000000)
			{
				printf("done 1000000\n");
				countRow = 0;
			}
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
					//userDaySessions[temp_uid].push_back(daySession(temp_day, temp_sid));
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
					index_q = RecordOfDay[temp_day][temp_sid].queries.size() - 1;
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
//						duLogger->logDU(temp_domain, temp_url);
//						suLogger->logSU(g_serpid, temp_url); // could have flushed at one time as query-term but maintained the initial design
						table_serpURLs[g_serpid].push_back(temp_url);
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
					int index_u = RecordOfDay[temp_day][temp_sid].queries[index_q].clickedURL.size() - 1;
					RecordOfDay[temp_day][temp_sid].queries[index_q].clickedURL[index_u].position = urlRank[temp_url];
					RecordOfDay[temp_day][temp_sid].queries[index_q].clickedURL[index_u].url_id = temp_url;
					RecordOfDay[temp_day][temp_sid].queries[index_q].clickedURL[index_u].timeOfClick = temp_time;
				}
			}
		}
	}
}

void DataCollector::parse(int test_1_train_0)
{
	if(test_1_train_0)
	{
		printf("opening %s\n", TEST_FILE);
		fp = fopen(TEST_FILE, "r");
		processOneFile(test_1_train_0);
		printf("test file parsing complete\n");
		fclose(fp);
	}
	else
	{
		for(int fileNum = 1; fileNum <= 6; fileNum++)
		{
			char path[256];
			strcpy(path, TRAIN_FILE);
			string append[19] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18"};
			strcat(path, append[fileNum].c_str());
			printf("opening %s\n", path);
			fp = fopen(path, "r");
			processOneFile(test_1_train_0);
			fclose(fp);
		}
		printf("train file parsing complete\n");
	}
}

void DataCollector::wrapUp()
{
	free(buffer);
//	duLogger->wrapUp();
//	suLogger->wrapUp();
	printf("data populated\n");	
}