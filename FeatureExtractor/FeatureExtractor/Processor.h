#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include "FileLogger.h"
#include "global.h"
#include "variables.h"

class Processor{
	FileLogger* logger_feature_train, *logger_feature_test, *logger_resultMapper, *logger_feature_validate, *logger_validate_result;

	bool Processor::qualifiesForValidation(int uid, vector<queryRec>::iterator it_query, vector<sessMetaData>::iterator it_session, map<int, bool> &queryInShortContextExists);
	void Processor::updateLocal(rowToLog &logRow, int queryTime);
	void Processor::getGlobalFeatures(rowToLog &logRow);
	void Processor::getURLRelatedFeatures(rowToLog &logRow, int queryDay, int queryTime);
	void Processor::getQueryRelatedFeatures(rowToLog &logRow, int queryDay, int queryTime);
	void Processor::getGroundTruthWhenClicked(rowToLog &logRow, vector<queryRec>::iterator it_query, vector<sessMetaData>::iterator it_session, int i_qClickedUrl);
public:
	Processor();
	void processTrain(userData* RecordOfUser);
	void processTest(userData* RecordOfUser);
	void wrapUp();
};

#endif