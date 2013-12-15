#include "stdafx.h"
#include "variables.h"
#include "Processor.h"

void Processor::processTrain(DataCollector &dc_train, usr_url &store_usrURL, usr_qry &store_usrQry)
{
	FeatureFileLogger logger(TRAIN_FILE);
	
	logger.wrapUp();
}

void Processor::processTest(DataCollector &dc_test, usr_url &store_usrURL, usr_qry &store_usrQry)
{
	FeatureFileLogger logger(TEST_FILE);

	logger.wrapUp();
}