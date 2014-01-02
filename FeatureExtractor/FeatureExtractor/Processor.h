#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include "FileLogger.h"
#include "global.h"
#include "variables.h"

class Processor{
	static bool qualifiesForValidation(int uid, vector<queryRec>::iterator it_query, vector<sessMetaData>::iterator it_session, map<int, bool> &queryInShortContextExists);
	static void updateLocal(rowToLog &logRow, int queryTime);
	static void getGlobalFeatures(rowToLog &logRow);
	static void getURLRelatedFeatures(rowToLog &logRow, int queryDay, int queryTime);
	static void getQueryRelatedFeatures(rowToLog &logRow, int queryDay, int queryTime);
	static void getGroundTruthWhenClicked(rowToLog &logRow, vector<queryRec>::iterator it_query, vector<sessMetaData>::iterator it_session, int i_qClickedUrl);
public:
	static void init_train();
	static void init_test();
	static void processTrain(userData* RecordOfUser);
	static void processTest(userData* RecordOfUser);
	static void wrapUp_train();
	static void wrapUp_test();
};

#endif
