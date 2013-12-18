#pragma once
#include "variables.h"
#include "DomainURL_Logger.h"
#include "serpURL_Logger.h"
#include <sstream>
//#include <map>

using namespace std;

// this collects data and orders by default - provide table populated as public so as to be used by Processor
class DataCollector{
	char *buffer;
	unsigned long long buffSize;
	DomainURL_Logger duLogger;
	serpURL_Logger suLogger;

public:
	DataCollector(); 
	void parse(int test_1_train_0);
	void wrapUp();
};
