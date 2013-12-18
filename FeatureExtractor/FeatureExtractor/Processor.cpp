#include "stdafx.h"
#include "variables.h"
#include "Processor.h"

/*
for each session
-> for each url
	-> get the day stuff by all get things
	-> get the session stuff
	-> update url record in local
-> for each url
	-> update table
*/

void Processor::processTrain(usr_url &store_usrURL, usr_qry &store_usrQry)
{
	FeatureFileLogger logger_feature_train(FEATURE_FILE, CALL_FROM_PROCESS_TEST);
	// use logger_feature_train.logTrain() here
	for(int i_day = 1; i_day <= 27; i_day++)
	{
		// use it as RecordOfDay[day][session_id].{uid|queries[i].{qid|timeOfQuery|clickedURL[i].{urlid|timeOfClick|position}}}
		
	}
}

void Processor::processTest(usr_url &store_usrURL, usr_qry &store_usrQry)
{
	FeatureFileLogger logger_feature_test(FEATURE_TEST_PHASE_FILE, CALL_FROM_PROCESS_TEST);
	// use logger_feature_test.logTest() for this

	FeatureFileLogger logger_resultMapper(RESULT_MAPPER_FILE, CALL_FROM_PROCESS_TEST);
	// use logger_resultMapper.logMap() for this

	FeatureFileLogger logger_feature_validate(FEATURE_VALIDATE_PHASE_FILE, CALL_FROM_PROCESS_TEST);
	// use logger_feature_validate.logValidate() for this

	FeatureFileLogger logger_validate_result(RESULT_VALIDATE_FILE, CALL_FROM_PROCESS_TEST);
	// use logger_validate_result.logVR() for this

}