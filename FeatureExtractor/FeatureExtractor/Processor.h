#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include "FileLogger.h"
#include "global.h"
#include "variables.h"

class Processor{
	static bool Processor::qualifiesForValidation(int uid, vector<queryRec>::iterator it_query, vector<sessMetaData>::iterator it_session, map<int, bool> &queryInShortContextExists);
	static void Processor::updateLocal(rowToLog &logRow, int queryTime);
	static void Processor::getGlobalFeatures(rowToLog &logRow);
	static void Processor::getURLRelatedFeatures(rowToLog &logRow, int queryDay, int queryTime);
	static void Processor::getQueryRelatedFeatures(rowToLog &logRow, int queryDay, int queryTime);
	static void Processor::getGroundTruthWhenClicked(rowToLog &logRow, vector<queryRec>::iterator it_query, vector<sessMetaData>::iterator it_session, int i_qClickedUrl);
public:
	static void init_train();
	static void init_test();
	static void processTrain(userData* RecordOfUser);
	static void processTest(userData* RecordOfUser);
	static void wrapUp_train();
	static void wrapUp_test();
};

#endif