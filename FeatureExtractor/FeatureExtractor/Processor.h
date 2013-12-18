#pragma once
#include "FeatureFileLogger.h"
#include "DataCollector.h"
#include "global.h"
#include "usr_url.h"
#include "usr_qry.h"

// iterates over the collection by DataCollector seqentially to {fill, use the two DSs and put data in FeatureFileLogger}
/*
 * records of [1,27] days  -> processTrain -> {FeatureTrainLogger}
 * records of [28,30] days -> processTest  -> {FeatureTestLogger, FeatureValidateLogger, map<rowInFeatureTest,sessionID>, validate_result}
 */
class Processor{
public:
	void processTrain(usr_url &store_usrURL, usr_qry &store_usrQry);
	void processTest(usr_url &store_usrURL, usr_qry &store_usrQry);
};

