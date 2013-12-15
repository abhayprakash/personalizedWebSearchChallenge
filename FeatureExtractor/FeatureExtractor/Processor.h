#pragma once
#include "FeatureFileLogger.h"
#include "DataCollector.h"
#include "usr_url.h"
#include "usr_qry.h"

// iterates over the collection by DataCollector seqentially to {fill, use the two DSs and put data in FeatureFileLogger}
class Processor{
public:
	void processTrain(DataCollector &dc_train, usr_url &store_usrURL, usr_qry &store_usrQry);
	void processTest(DataCollector &dc_test, usr_url &store_usrURL, usr_qry &store_usrQry);
};

