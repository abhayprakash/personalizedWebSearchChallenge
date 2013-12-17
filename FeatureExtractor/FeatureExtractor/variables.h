#ifndef VARIABLES_H_INCLUDED
#define VARIABLES_H_INCLUDED

// input files
char TRAIN_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\small_train_dataset";
char TEST_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\test";

// output files
char FEATURE_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\feature_train.txt";
char FEATURE_TEST_PHASE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\feature_test.txt";
char FEATURE_VALIDATE_PHASE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\feature_validate.txt";

char DOMAIN_URL_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\DU_file.txt";
char SERP_URL_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\SU_file.txt";
char QUERY_TERM_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\QT_file.txt";

char RESULT_VALIDATE_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\result_validate.txt";

// memory allocation
#define BUFF_SIZE_INPUT_READ 4*1024*1024*1024ull			// 4 GB
#define BUFF_SIZE_FEATURE_FILE 5*1024*1024*1024ull			// 5 GB
#define BUFF_SIZE_FEATURE_TEST_FILE 1*1024*1024*1024ull		// 1 GB
#define BUFF_SIZE_DU_FILE 1*1024*1024*1024ull				// 1 GB
#define BUFF_SIZE_SU_FILE 1*1024*1024*1024ull				// 1 GB
#define BUFF_SIZE_QT_FILE 5*1024*1024*1024ull				// 5 GB

// Upper limits
#define NUM_MAX_ROW_CHAR 150
#define NUM_MAX_WORD_CHAR 15

// Computation parameters
#define SIMILAR_INDEX_THRESH_FOR_QUERY 100

#endif // VARIABLES_H_INCLUDED
