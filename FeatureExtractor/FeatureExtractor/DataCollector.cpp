#include "stdafx.h"
#include "DataCollector.h"
#include "global.h"

DataCollector::DataCollector() // phase I : provide path to train file, phase II : provide path to test file
{
	buffSize = BUFF_SIZE_INPUT_READ;
	buffer = (char *)malloc(buffSize);
}

void DataCollector::collectTestUserList()
{
	printf("opening %s\n", TEST_FILE);
	fp = fopen(TEST_FILE, "r");
	while((nRead = fread(buffer, 1, buffSize, fp)) > 0)
	{
		buffer[nRead] = 0;
		printf("Read a chunk - %d bytes of given file\n", nRead);
		
		istringstream buff_in(buffer);
		string testForM;
		while(buff_in)
		{
			buff_in>>testForM;
			if(testForM == "M")
			{
				buff_in>>temp_day>>temp_uid;
				userExistsInTest[temp_uid] = true;
			}
		}
	}
	printf("Test Users' list collected\n");
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
				if(temp_uid != prev_uid && considerUser == true)
				{
					if(test_1_train_0)
						Processor::processTest(RecordOfUser); // other side just for safety keep a check for null
					else
						Processor::processTrain(RecordOfUser);
					prev_uid = temp_uid;
					RecordOfUser = new userData;
				}
				// temp_sid, temp_day, temp_uid
				if(userExistsInTest[temp_uid])
				{
					considerUser = true;
					RecordOfUser->uid = temp_uid;
					index_session = RecordOfUser->session.size();
					RecordOfUser->session.push_back(sessMetaData(temp_day, temp_sid));
				}
				else
				{
					considerUser = false;
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
					index_q = RecordOfUser->session[index_session].queries.size();
					RecordOfUser->session[index_session].queries.push_back(queryRec(temp_qid, temp_time, g_serpid));
				}
				sin>>temp_list;
				istringstream tokenStream(temp_list);
				while(getline(tokenStream, temp_s_term, ','))
				{
					//temp_sid temp_time temp_serp temp_qid temp_term
					/*
					 * we can keep Queries of worthful users only but i think it will be used to get a separate SOM, so let me keep all queries 
					 * NOTE: queryid means the TEXTUAL query only
					 * wait that will cause over 100 Mn rows - do it for worthful users only
					 */
					if(considerUser)
					{
						temp_term = atoi(temp_s_term.c_str());
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
					RecordOfUser->session[index_session].queries[index_q].clickedURL.push_back(shownURL(temp_url, temp_time, urlRank[temp_url]));
				}
			}
		}
	}
}

void DataCollector::parse(int test_1_train_0)
{
	considerUser = false;
	RecordOfUser = new userData;

	if(test_1_train_0)
	{
		Processor::init_test();
		considerUser = false;
		printf("opening %s\n", TEST_FILE);
		fp = fopen(TEST_FILE, "r");
		processOneFile(test_1_train_0);
		
		//process last record
		if(considerUser == true)
		{
			Processor::processTest(RecordOfUser);
			prev_uid = temp_uid;
			RecordOfUser = NULL;
		}

		printf("test file parsing complete\n");
		Processor::wrapUp_test();
		fclose(fp);
	}
	else
	{
		Processor::init_train();
		for(int fileNum = 1; fileNum <= 10; fileNum++)
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
		if(considerUser == true)
		{
			Processor::processTrain(RecordOfUser);
			prev_uid = temp_uid;
			RecordOfUser = NULL;
		}
		Processor::wrapUp_train();
		printf("train file parsing complete\n");
	}
}

void DataCollector::wrapUp()
{
	free(buffer);
	printf("Feature Files generation complete\n");
}