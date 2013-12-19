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

void Processor::processTrain(usr_url &store_usrURL, usr_qry &store_usrQry)
{
	FeatureFileLogger logger_feature_train(FEATURE_FILE, CALL_FROM_PROCESS_TEST);
	rowToLog logRow;
	
	// temp
	int session_id;
	int time_nextActivity;
	int timeGivenOnURL;
	sessMetaData session_data;
	map<int, bool> url_logged;
	srand(RAND_SEED);

	for(int i_day = 1; i_day <= 27; i_day++)
	{
		// use it as RecordOfDay[day][session_id].{uid|queries[i].{qid|timeOfQuery|clickedURL[i].{urlid|timeOfClick|position}}}
		map<int, sessMetaData>::iterator it_session;
		for(it_session = RecordOfDay[i_day].begin(); it_session != RecordOfDay[i_day].end(); ++it_session)
		{
			session_id = it_session->first;
			session_data = it_session->second;
			logRow.user_id = session_data.uid;
			for(vector<queryRec>::iterator it_query = session_data.queries.begin(); it_query != session_data.queries.end(); ++it_query)
			{
				url_logged.clear();
				logRow.query_id = it_query->qid;
				// operation for clicked urls
				for(int i_qClickedUrl = 0; i_qClickedUrl < it_query->clickedURL.size(); ++i_qClickedUrl)
				{
					logRow.url_id = it_query->clickedURL[i_qClickedUrl].url_id;
					logRow.url_position = it_query->clickedURL[i_qClickedUrl].position;

					// store(update) features both global and session : global things
					logRow.count_earlier_0 = store_usrURL.getCountR(logRow.user_id, logRow.url_id, 0);
					logRow.count_earlier_1 = store_usrURL.getCountR(logRow.user_id, logRow.url_id, 1);
					logRow.count_earlier_2 = store_usrURL.getCountR(logRow.user_id, logRow.url_id, 2);
					logRow.count_earlier_shown = store_usrURL.getCountShown(logRow.user_id, logRow.url_id);

					// store(update) features both global and session : url related
					logRow.url_beforeSession.exists = store_usrURL.existsBeforeSession(logRow.user_id, logRow.url_id);
					logRow.url_beforeSession.grade = store_usrURL.getLatestDayGrade(logRow.user_id, logRow.url_id);
					if(logRow.url_beforeSession.exists)
					{
						logRow.url_beforeSession.time_diff = i_day - store_usrURL.getLastDay();
					}
					else
					{
						logRow.url_beforeSession.time_diff = 0;
					}
					logRow.url_sameSession.exists = store_usrURL.existsCurrentSession(logRow.user_id, logRow.url_id);
					logRow.url_sameSession.grade = store_usrURL.getLatestTimeGrade(logRow.user_id, logRow.url_id);
					if(logRow.url_sameSession.exists)
					{
						logRow.url_sameSession.time_diff = it_query->timeOfQuery - store_usrURL.getLastTime(logRow.user_id, logRow.url_id);
					}
					else
					{
						logRow.url_sameSession.time_diff = 0;
					}
					// store(update) features both global and session : query related
					int time_day_of_found_query;
					if(store_usrQry.getRecentSimilarQueryData(SEARCH_BEFORE_SESSION, logRow.user_id, logRow.query_id, logRow.url_id, time_day_of_found_query, logRow.query_beforeSession.u_exists_for_q, logRow.query_beforeSession.grade))
					{
						logRow.query_beforeSession.exists =  true;
						logRow.query_beforeSession.time_diff = i_day - time_day_of_found_query;
					}
					else
					{
						logRow.query_beforeSession.exists =  false;
						logRow.query_beforeSession.time_diff = 0;
						logRow.query_beforeSession.u_exists_for_q = false;
						logRow.query_beforeSession.grade = 0;
					}

					if(store_usrQry.getRecentSimilarQueryData(SEARCH_IN_SESSION, logRow.user_id, logRow.query_id, logRow.url_id, time_day_of_found_query, logRow.query_sameSession.u_exists_for_q, logRow.query_sameSession.grade))
					{
						logRow.query_sameSession.exists =  true;
						logRow.query_sameSession.time_diff = it_query->timeOfQuery - time_day_of_found_query;
					}
					else
					{
						logRow.query_sameSession.exists =  false;
						logRow.query_sameSession.time_diff = 0;
						logRow.query_sameSession.u_exists_for_q = false;
						logRow.query_sameSession.grade = 0;
					}

					// ground truth
					if(it_query+1 == session_data.queries.end() && i_qClickedUrl+1 == it_query->clickedURL.size()) // if last click of session
					{
						timeGivenOnURL = 404;
					}
					else if(i_qClickedUrl+1 == it_query->clickedURL.size()) // if last click of query
					{
						time_nextActivity = (it_query+1)->timeOfQuery;
						timeGivenOnURL = time_nextActivity - it_query->clickedURL[i_qClickedUrl].timeOfClick;
					}
					else // same query has next click
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

					// log and mark logged
					url_logged[logRow.url_id] = true;
					logger_feature_train.logTrain(logRow);

					// update local
					store_usrURL.updateLocal_Shown(logRow.user_id, logRow.url_id, it_query->timeOfQuery);
					store_usrQry.updateShown_local(logRow.user_id, logRow.query_id, logRow.url_id, it_query->timeOfQuery);
					store_usrURL.updateLocal_Click(logRow.user_id, logRow.url_id, logRow.resultGrade);
					store_usrQry.updateClicked_local(logRow.user_id, logRow.query_id, logRow.url_id, logRow.resultGrade);
				}

				// operation for nonclicked urls
				for(int i_serpurl = 0; i_serpurl < table_serpURLs[it_query->shownSERP].size(); ++i_serpurl)
				{
					logRow.url_id = table_serpURLs[it_query->shownSERP][i_serpurl];
					// check if logged : continue, proceed to log for 20% unclicked urls
					if(url_logged[logRow.url_id])
					{
						continue;
					}

					if(rand() > 1.0 - PROB_KEEP_UNCLICKED_URL)
					{
						logRow.url_position = i_serpurl + 1;

						// store(update) features both global and session : global things
						logRow.count_earlier_0 = store_usrURL.getCountR(logRow.user_id, logRow.url_id, 0);
						logRow.count_earlier_1 = store_usrURL.getCountR(logRow.user_id, logRow.url_id, 1);
						logRow.count_earlier_2 = store_usrURL.getCountR(logRow.user_id, logRow.url_id, 2);
						logRow.count_earlier_shown = store_usrURL.getCountShown(logRow.user_id, logRow.url_id);

						// store(update) features both global and session : url related
						logRow.url_beforeSession.exists = store_usrURL.existsBeforeSession(logRow.user_id, logRow.url_id);
						logRow.url_beforeSession.grade = store_usrURL.getLatestDayGrade(logRow.user_id, logRow.url_id);
						if(logRow.url_beforeSession.exists)
						{
							logRow.url_beforeSession.time_diff = i_day - store_usrURL.getLastDay();
						}
						else
						{
							logRow.url_beforeSession.time_diff = 0;
						}
						logRow.url_sameSession.exists = store_usrURL.existsCurrentSession(logRow.user_id, logRow.url_id);
						logRow.url_sameSession.grade = store_usrURL.getLatestTimeGrade(logRow.user_id, logRow.url_id);
						if(logRow.url_sameSession.exists)
						{
							logRow.url_sameSession.time_diff = it_query->timeOfQuery - store_usrURL.getLastTime(logRow.user_id, logRow.url_id);
						}
						else
						{
							logRow.url_sameSession.time_diff = 0;
						}
						// store(update) features both global and session : query related
						int time_day_of_found_query;
						if(store_usrQry.getRecentSimilarQueryData(SEARCH_BEFORE_SESSION, logRow.user_id, logRow.query_id, logRow.url_id, time_day_of_found_query, logRow.query_beforeSession.u_exists_for_q, logRow.query_beforeSession.grade))
						{
							logRow.query_beforeSession.exists =  true;
							logRow.query_beforeSession.time_diff = i_day - time_day_of_found_query;
						}
						else
						{
							logRow.query_beforeSession.exists =  false;
							logRow.query_beforeSession.time_diff = 0;
							logRow.query_beforeSession.u_exists_for_q = false;
							logRow.query_beforeSession.grade = 0;
						}

						if(store_usrQry.getRecentSimilarQueryData(SEARCH_IN_SESSION, logRow.user_id, logRow.query_id, logRow.url_id, time_day_of_found_query, logRow.query_sameSession.u_exists_for_q, logRow.query_sameSession.grade))
						{
							logRow.query_sameSession.exists =  true;
							logRow.query_sameSession.time_diff = it_query->timeOfQuery - time_day_of_found_query;
						}
						else
						{
							logRow.query_sameSession.exists =  false;
							logRow.query_sameSession.time_diff = 0;
							logRow.query_sameSession.u_exists_for_q = false;
							logRow.query_sameSession.grade = 0;
						}

						// ground truth
						logRow.resultGrade = UNCLICKED_CLASS;					

						// log
						logger_feature_train.logTrain(logRow);
					}
					// update local
					store_usrQry.updateShown_local(logRow.user_id, it_query->qid, table_serpURLs[it_query->shownSERP][i_serpurl],it_query->timeOfQuery);
					store_usrURL.updateLocal_Shown(logRow.user_id, table_serpURLs[it_query->shownSERP][i_serpurl],it_query->timeOfQuery);
				}
			}
			// copy local to global
			store_usrQry.copyLocalToGlobal_and_ClearLocal(i_day);
			store_usrURL.copyLocalToGlobal_and_ClearLocal(i_day);
		}
	}
}

void Processor::processTest(usr_url &store_usrURL, usr_qry &store_usrQry)
{
	FeatureFileLogger logger_feature_test(FEATURE_TEST_PHASE_FILE, CALL_FROM_PROCESS_TEST);
	FeatureFileLogger logger_resultMapper(RESULT_MAPPER_FILE, CALL_FROM_PROCESS_TEST);
	FeatureFileLogger logger_feature_validate(FEATURE_VALIDATE_PHASE_FILE, CALL_FROM_PROCESS_TEST);
	FeatureFileLogger logger_validate_result(RESULT_VALIDATE_FILE, CALL_FROM_PROCESS_TEST);
	
	rowToLog logRow; //logTest, logValidate
	resultRow result_Row;
	// temp
	int timeGivenOnURL;
	int time_nextActivity;
	int session_id;
	int time_previousActivity;
	sessMetaData session_data;

	for(int i_day = 28; i_day <= 30; i_day++)
	{
		// use it as RecordOfDay[day][session_id].{uid|queries[i].{qid|timeOfQuery|clickedURL[i].{urlid|timeOfClick|position}}}
		map<int, sessMetaData>::iterator it_session;
		for(it_session = RecordOfDay[i_day].begin(); it_session != RecordOfDay[i_day].end(); ++it_session)
		{
			session_id = it_session->first;
			session_data = it_session->second;
			logRow.user_id = session_data.uid;
			for(vector<queryRec>::iterator it_query = session_data.queries.begin(); it_query != session_data.queries.end(); ++it_query)
			{
				if(it_query+1 == session_data.queries.end()) // last query -> test
				{
					for() // all 10 urls
					{
						// get features

						logger_feature_test.logTest(logRow);
						logger_resultMapper.logMap();
						//update shown
					}
				}
				else if(it_query+2 == session_data.queries.end()) // second last query -> validate
				{					
					for() // all 10 url
					{
						// get feature

						logger_feature_validate.logValidate(logRow);
						int tempGrade = UNCLICKED_CLASS;
						// compute the gade

						logger_validate_result.logVR(tempGrade);
						//update shown and click
					}					
				}
				else
				{
					for() // all clicked
					{
					}
					for() // non clicked
					{
					}
				}
			}
			// copy local to global
			store_usrQry.copyLocalToGlobal_and_ClearLocal(i_day);
			store_usrURL.copyLocalToGlobal_and_ClearLocal(i_day);
		}
	}
}