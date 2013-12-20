#include "stdafx.h"
#include "global.h"


int g_serpid = 0;
map<int, bool> usersInTest;
map<int, vector<int> > queryTerms;
map<int, vector<int> > table_serpURLs;
sessionRec RecordOfDay[31];