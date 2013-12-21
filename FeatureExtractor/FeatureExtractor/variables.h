#ifndef VARIABLES_H_INCLUDED
#define VARIABLES_H_INCLUDED

// input files
extern char TRAIN_FILE[256];
extern char TEST_FILE[256];

// output files
extern char FEATURE_FILE[256];
extern char FEATURE_TEST_PHASE_FILE[256];
extern char FEATURE_VALIDATE_PHASE_FILE[256];

extern char DOMAIN_URL_FILE[256];
extern char SERP_URL_FILE[256];
extern char QUERY_TERM_FILE[256];

extern char RESULT_VALIDATE_FILE[256];
extern char RESULT_MAPPER_FILE[256];

// memory allocation
#define BUFF_SIZE_INPUT_READ 4*1024*1024*1024ull;			// 4 GB
#define BUFF_SIZE_FEATURE_FILE 5*1024*1024*1024ull;			// 5 GB - objects (1 in processTrain)
#define BUFF_SIZE_FEATURE_TEST_FILE 1*1024*1024*1024ull;	// 1 GB - objects (4 in processTest)
#define BUFF_SIZE_DU_FILE 1*1024*1024*1024ull;				// 1 GB
#define BUFF_SIZE_SU_FILE 1*1024*1024*1024ull;				// 1 GB
#define BUFF_SIZE_QT_FILE 5*1024*1024*1024ull;				// 5 GB

// Upper limits
#define NUM_MAX_ROW_CHAR 150
#define NUM_MAX_WORD_CHAR 15

// mutual communication
#define PARSE_TRAIN_FILE 0
#define PARSE_TEST_FILE 1
#define CALL_FROM_PROCESS_TRAIN 2
#define CALL_FROM_PROCESS_TEST 3
#define SEARCH_IN_SESSION 4
#define SEARCH_BEFORE_SESSION 5

// Computation parameters
#define SIMILAR_INDEX_THRESH_FOR_QUERY 0.75
#define UNCLICKED_CLASS -1 // make one submission with this value = 0 also
#define RAND_SEED 25 // or make if time(0), kept it constant to make results reproducible 
#define PROB_KEEP_UNCLICKED_URL 10

#endif // VARIABLES_H_INCLUDED
