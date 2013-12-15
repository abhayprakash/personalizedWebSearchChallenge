#ifndef VARIABLES_H_INCLUDED
#define VARIABLES_H_INCLUDED

// input files
char TRAIN_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\small_train_dataset";
char TEST_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\test";

// output files
char FEATURE_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\feature_train.txt";
char FEATURE_TEST_PHASE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\feature_test.txt";
char DOMAIN_URL_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\DU_file.txt";
char SERP_URL_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\SU_file.txt";
char QUERY_TERM_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\QT_file.txt";

// memory allocation
#define BUFF_SIZE_FEATURE_FILE 10*1024*1024*1024ull //10 GB
#define BUFF_SIZE_USR_URL_FILE 10*1024*1024*1024ull // not required

// Upper limits
#define NUM_MAX_ROW_CHAR 150
#define NUM_MAX_WORD_CHAR 15

// Computation parameters
#define SIMILAR_INDEX_THRESH_FOR_QUERY 100

#endif // VARIABLES_H_INCLUDED
