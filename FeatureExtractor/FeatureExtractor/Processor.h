#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include "FeatureFileLogger.h"
#include "usr_url.h"
#include "usr_qry.h"
#include "variables.h"
// iterates over the collection by DataCollector seqentially to {fill, use the two DSs and put data in FeatureFileLogger}
/*
 * records of [1,27] days  -> processTrain -> {FeatureTrainLogger}
 * records of [28,30] days -> processTest  -> {FeatureTestLogger, FeatureValidateLogger, map<rowInFeatureTest,sessionID>, validate_result}
 */
class Processor{
	bool Processor::qualifiesForValidation(vector<queryRec>::iterator it_query, sessMetaData &session_data, map<int, bool> &queryInShortContextExists, usr_qry &store_usrQry);
	void Processor::updateLocal(usr_url &store_usrURL, usr_qry &store_usrQry, rowToLog &logRow, int queryTime);
	void Processor::getGlobalFeatures(rowToLog &logRow, usr_url &store_usrURL);
	void Processor::getURLRelatedFeatures(rowToLog &logRow, usr_url &store_usrURL, int queryDay, int queryTime);
	void Processor::getQueryRelatedFeatures(rowToLog &logRow, usr_qry &store_usrQry, int queryDay, int queryTime);
	void Processor::getGroundTruthWhenClicked(rowToLog &logRow, vector<queryRec>::iterator it_query, sessMetaData &session_data, int i_qClickedUrl);
public:
	void processTrain(usr_url &store_usrURL, usr_qry &store_usrQry);
	void processTest(usr_url &store_usrURL, usr_qry &store_usrQry);
};

#endif