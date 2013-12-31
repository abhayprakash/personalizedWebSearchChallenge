#include "stdafx.h"
#include "Processor.h"
#include "global.h"

// iterates over the collection by DataCollector seqentially to {fill, use the two DSs and put data in FeatureFileLogger}
/*
 * records of [1,27] days  -> processTrain -> {FeatureTrainLogger}
 * records of [28,30] days -> processTest  -> {FeatureTestLogger, FeatureValidateLogger, map<rowInFeatureTest,sessionID>, validate_result}
 */

/*
for each session
-> for each url
	-> get the day stuff by all get things
	-> get the session stuff
	-> update local
-> for each url
	-> copy local to global
*/

Processor::Processor()
{
	logger_feature_train = new FileLogger(FEATURE_TRAIN_FILE, BUFF_SIZE_FEATURE_TRAIN_FILE, MAX_ROW_MORE_THAN_20_TERMS);
	logger_feature_test = new FileLogger(FEATURE_TEST_PHASE_FILE, BUFF_SIZE_FEATURE_TEST_FILE, MAX_ROW_MORE_THAN_20_TERMS);
	logger_resultMapper = new FileLogger(RESULT_MAPPER_FILE, BUFF_SIZE_RESULT_MAPPER, MAX_ROW_AROUND_10_TERMS);
	logger_feature_validate = new FileLogger(FEATURE_VALIDATE_PHASE_FILE, BUFF_SIZE_VALIDATE_FILE, MAX_ROW_MORE_THAN_20_TERMS);
	logger_validate_result = new FileLogger(RESULT_VALIDATE_FILE, BUFF_SIZE_VALIDATE_RESULT, MAX_ROW_CLASS_TERM);
}

// check whether the considered second last is qualified to be taken in validation set
bool Processor::qualifiesForValidation(int uid, vector<queryRec>::iterator it_query, vector<sessMetaData>::iterator it_session, map<int, bool> &queryInShortContextExists)
{
	if(it_query+2 != it_session->queries.end())
		return false;

	// statement: "If the sampled query does not have any short-term context"
	// if it has, include
	if(queryInShortContextExists[it_query->qid])
		return true;

	// statement: "user that asked this query has no search sessions in the training period"
	// if it has, include in validation set 
	// for this check the day of last query in the user's query list if it is <= 27 return true
	// a little probability of missing: latest day of oldest query may be in test days whereas its first day could be in train time
	// but this would just miss some cases but not include any wrong one - resulting a bit small validation set that could have been got
	// this is probably better than allocating 4 byte data to each query for its first day or any other way of iteration over 27 days
	int lastestDayOfOldestQuery = store_usrQry.getOldestQueryDay(uid);
	if( lastestDayOfOldestQuery > 0 && lastestDayOfOldestQuery <= 27)
		return true;
	
	return false;
}

// expects set logRow.user_id, logRow.url_id, logRow.query_id, logRow.resultGrade
void Processor::updateLocal(rowToLog &logRow, int queryTime)
{
	store_usrURL.updateLocal_Shown(logRow.url_id, queryTime);
	store_usrQry.updateShown_local(logRow.query_id, logRow.url_id, queryTime);
	store_usrURL.updateLocal_Click(logRow.url_id, logRow.resultGrade);
	store_usrQry.updateClicked_local(logRow.query_id, logRow.url_id, logRow.resultGrade);
}

// expects set logRow.user_id, logRow.url_id
void Processor::getGlobalFeatures(rowToLog &logRow) 
{
	// store(update) features both global and session : global things
	int obtained[3];
	store_usrURL.getCountR(logRow.user_id, logRow.url_id, obtained);
	logRow.count_earlier_0 = obtained[0];
	logRow.count_earlier_1 = obtained[1];
	logRow.count_earlier_2 = obtained[2];
	logRow.count_earlier_shown = store_usrURL.getCountShown(logRow.user_id, logRow.url_id);
}

// expects set logRow.user_id, logRow.url_id
void Processor::getURLRelatedFeatures(rowToLog &logRow, int queryDay, int queryTime) 
{
	// store(update) features global
	logRow.url_beforeSession.exists = store_usrURL.existsBeforeSession(logRow.user_id, logRow.url_id);
	logRow.url_beforeSession.grade = store_usrURL.getLatestDayGrade(logRow.user_id, logRow.url_id);
	
	if(logRow.url_beforeSession.exists)
	{
		logRow.url_beforeSession.time_diff = queryDay - store_usrURL.getLastDay(logRow.user_id, logRow.url_id);
	}
	else
	{
		logRow.url_beforeSession.time_diff = 0;
	}
	
	// store(update) features local
	logRow.url_sameSession.exists = store_usrURL.existsCurrentSession(logRow.url_id);
	logRow.url_sameSession.grade = store_usrURL.getLatestTimeGrade(logRow.url_id);
	
	if(logRow.url_sameSession.exists)
	{
		logRow.url_sameSession.time_diff = queryTime - store_usrURL.getLastTime(logRow.url_id);
	}
	else
	{
		logRow.url_sameSession.time_diff = 0;
	}
}

// expects logRow.user_id, logRow.query_id, logRow.url_id
void Processor::getQueryRelatedFeatures(rowToLog &logRow, int queryDay, int queryTime) 
{
	int time_day_of_found_query;
	
	// store(update) features global
	if(store_usrQry.getRecentSimilarQueryData(SEARCH_BEFORE_SESSION, logRow.user_id, logRow.query_id, logRow.url_id, time_day_of_found_query, logRow.query_beforeSession.u_exists_for_q, logRow.query_beforeSession.grade))
	{
		logRow.query_beforeSession.exists =  true;
		logRow.query_beforeSession.time_diff = queryDay - time_day_of_found_query;
	}
	else
	{
		logRow.query_beforeSession.exists =  false;
		logRow.query_beforeSession.time_diff = 0;
		logRow.query_beforeSession.u_exists_for_q = false;
		logRow.query_beforeSession.grade = 0;
	}

	// store(update) features local
	if(store_usrQry.getRecentSimilarQueryData(SEARCH_IN_SESSION, logRow.user_id, logRow.query_id, logRow.url_id, time_day_of_found_query, logRow.query_sameSession.u_exists_for_q, logRow.query_sameSession.grade))
	{
		logRow.query_sameSession.exists =  true;
		logRow.query_sameSession.time_diff = queryTime - time_day_of_found_query;
	}
	else
	{
		logRow.query_sameSession.exists =  false;
		logRow.query_sameSession.time_diff = 0;
		logRow.query_sameSession.u_exists_for_q = false;
		logRow.query_sameSession.grade = 0;
	}
}

// expects nothing in logRow
void Processor::getGroundTruthWhenClicked(rowToLog &logRow, vector<queryRec>::iterator it_query, vector<sessMetaData>::iterator it_session, int i_qClickedUrl)
{
	int timeGivenOnURL, time_nextActivity;
	// if last click of session
	if(it_query+1 == it_session->queries.end() && i_qClickedUrl+1 == it_query->clickedURL.size()) 
	{
		timeGivenOnURL = 404; // a dummy value greater than 400
	}
	// if last click of query
	else if(i_qClickedUrl+1 == it_query->clickedURL.size()) 
	{
		time_nextActivity = (it_query+1)->timeOfQuery;
		timeGivenOnURL = time_nextActivity - it_query->clickedURL[i_qClickedUrl].timeOfClick;
	}
	// same query has next click
	else 
	{
		time_nextActivity = it_query->clickedURL[i_qClickedUrl+1].timeOfClick;
		timeGivenOnURL = time_nextActivity - it_query->clickedURL[i_qClickedUrl].timeOfClick;
	}

	logRow.resultGrade = 0;
	if( timeGivenOnURL >= 50) 
	{
		logRow.resultGrade = 1;
	}
	if( timeGivenOnURL >= 400) 
	{
		logRow.resultGrade = 2;
	}
}

void Processor::processTrain(userData* RecordOfUser)
{
	// just a precaution that RE not arise
	if(RecordOfUser == NULL)
		return;

	rowToLog logRow;
	logRow.user_id = RecordOfUser->uid;
	
	// temp
	map<int, bool> url_logged;
	srand(RAND_SEED);

	// iteration over sessions
	for(vector<sessMetaData>::iterator it_session = RecordOfUser->session.begin(); it_session != RecordOfUser->session.end(); ++it_session)
	{
		//iteration over queries in a session
		for(vector<queryRec>::iterator it_query = it_session->queries.begin(); it_query != it_session->queries.end() && it_query->clickedURL.size() != 0 ; ++it_query)
		{
			url_logged.clear();
			logRow.query_id = it_query->qid;

			// iteration over the urls in each query
			// operation for clicked urls
			for(unsigned int i_qClickedUrl = 0; i_qClickedUrl < it_query->clickedURL.size(); ++i_qClickedUrl)
			{
				logRow.url_id = it_query->clickedURL[i_qClickedUrl].url_id;
				logRow.url_position = it_query->clickedURL[i_qClickedUrl].position;

				// get all features
				getGlobalFeatures(logRow);

				getURLRelatedFeatures(logRow, it_session->day, it_query->timeOfQuery);
				getQueryRelatedFeatures(logRow, it_session->day, it_query->timeOfQuery);

				
				getGroundTruthWhenClicked(logRow, it_query, it_session, i_qClickedUrl);

				// log and mark logged
				url_logged[logRow.url_id] = true;
				logger_feature_train->logTrain(logRow);

				// update local
				updateLocal(logRow, it_query->timeOfQuery);
			}

			// operation for nonclicked urls
			for(unsigned int i_serpurl = 0; i_serpurl < table_serpURLs[it_query->shownSERP].size(); ++i_serpurl)
			{
				logRow.url_id = table_serpURLs[it_query->shownSERP][i_serpurl];
				// check if logged : continue, proceed to log for 20% unclicked urls
				if(url_logged[logRow.url_id])
				{
					continue;
				}

				// ground truth
				logRow.resultGrade = UNCLICKED_CLASS;
				if((rand() % 100) < PROB_KEEP_UNCLICKED_URL)
				{
					logRow.url_position = i_serpurl + 1;

					// get all features
					getGlobalFeatures(logRow);

					getURLRelatedFeatures(logRow, it_session->day, it_query->timeOfQuery);
					getQueryRelatedFeatures(logRow, it_session->day, it_query->timeOfQuery);

					// log this url un train
					logger_feature_train->logTrain(logRow);
				}

				// update local
				updateLocal(logRow, it_query->timeOfQuery);
			}
		}
		// copy local to global
		store_usrQry.copyLocalToGlobal_and_ClearLocal(it_session->day, logRow.user_id);
		store_usrURL.copyLocalToGlobal_and_ClearLocal(it_session->day, logRow.user_id);
	}
	delete RecordOfUser;
}

void Processor::processTest(userData* RecordOfUser)
{
	rowToLog logRow; //logTest, logValidate
	resultRow result_Row;
	logRow.user_id = RecordOfUser->uid;

	// temp
	int rowsLoggedInTest = 0;
	map<int, bool> queryInShortContextExists;

	// iterate over sessions
	for(vector<sessMetaData>::iterator it_session = RecordOfUser->session.begin(); it_session != RecordOfUser->session.end(); ++it_session)
	{
		queryInShortContextExists.clear();
		result_Row.session_id = it_session->sessionId;
		
		//iterate over queries in a session
		for(vector<queryRec>::iterator it_query = it_session->queries.begin(); it_query != it_session->queries.end(); ++it_query)
		{
			logRow.query_id = it_query->qid;
			if(it_query+1 == it_session->queries.end()) // last query -> test
			{
				// iterate over all 10 urls in T query
				for(unsigned int i_serpurl = 0; i_serpurl < table_serpURLs[it_query->shownSERP].size(); ++i_serpurl)
				{
					logRow.url_id = table_serpURLs[it_query->shownSERP][i_serpurl];
					logRow.url_position = i_serpurl + 1;
					result_Row.url_id = logRow.url_id;
					result_Row.url_pos = i_serpurl + 1;

					// get features
					getGlobalFeatures(logRow);
					getURLRelatedFeatures(logRow, it_session->day, it_query->timeOfQuery);
					getQueryRelatedFeatures(logRow, it_session->day, it_query->timeOfQuery);

					// log row in test file
					logger_feature_test->logTest(logRow);
						
					result_Row.rowNum = rowsLoggedInTest;
					rowsLoggedInTest++;
					// log in result mapper so as to output session_id and URLid(ordered by class and urlpos)
					logger_resultMapper->logMap(result_Row);			
				}
			}
			else
			{
				bool flag_qualifiesValidation = false;
				map<int, int> i_of_ClickedURL;
				for(unsigned int i_qClickedUrl = 0; i_qClickedUrl < it_query->clickedURL.size(); ++i_qClickedUrl)
				{
					i_of_ClickedURL[it_query->clickedURL[i_qClickedUrl].url_id] = i_qClickedUrl;
					getGroundTruthWhenClicked(logRow, it_query, it_session, i_qClickedUrl);
					if(logRow.resultGrade > 0)
						flag_qualifiesValidation = true;
				}
				if(flag_qualifiesValidation)
				{
					flag_qualifiesValidation = qualifiesForValidation(logRow.user_id, it_query, it_session, queryInShortContextExists);
				}
				queryInShortContextExists[it_query->qid] = true;
				for(unsigned int i_serpurl = 0; i_serpurl < table_serpURLs[it_query->shownSERP].size(); ++i_serpurl) // all 10 url even for non clicked
				{
					logRow.url_id = table_serpURLs[it_query->shownSERP][i_serpurl];
						
					// if clicked, get class
					if(i_of_ClickedURL.find(logRow.url_id) != i_of_ClickedURL.end())
					{
						getGroundTruthWhenClicked(logRow, it_query, it_session, i_of_ClickedURL[logRow.url_id]);
					}
					else
						logRow.resultGrade = UNCLICKED_CLASS; // if not clicked class := -1
						
					if(flag_qualifiesValidation) // validation set
					{
						logRow.url_position = i_serpurl + 1;
						
						// get features
						getGlobalFeatures(logRow);
						getURLRelatedFeatures(logRow, it_session->day, it_query->timeOfQuery);
						getQueryRelatedFeatures(logRow, it_session->day, it_query->timeOfQuery);

						// log row in validate file
						logger_feature_validate->logValidate(logRow);

						// log to logVR
						logger_validate_result->logVR(logRow.resultGrade);
					}
					//update local
					updateLocal(logRow, it_query->timeOfQuery);
				}					
			}
		}
		// copy local to global
		store_usrQry.copyLocalToGlobal_and_ClearLocal(it_session->day, logRow.user_id);
			store_usrURL.copyLocalToGlobal_and_ClearLocal(it_session->day, logRow.user_id);
	}
	delete RecordOfUser;
}

void Processor::wrapUp()
{
	logger_feature_train->wrapUp();
	logger_feature_test->wrapUp();
	logger_resultMapper->wrapUp();
	logger_feature_validate->wrapUp();
	logger_validate_result->wrapUp();
}
