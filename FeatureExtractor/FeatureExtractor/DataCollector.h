#ifndef DATACOLLECTOR_H_INCLUDED
#define DATACOLLECTOR_H_INCLUDED

#include "variables.h"
#include <sstream>
#include "FileLogger.h"
//#include <map>

using namespace std;

// this collects data and orders by default - provide table populated as public so as to be used by Processor
class DataCollector{
	char *buffer;
	unsigned long long buffSize;

	FileLogger* duLogger, *suLogger;
public:
	DataCollector(); 
	void parse(int test_1_train_0);
	void wrapUp();
};

#endif