#ifndef VARIABLES_H_INCLUDED
#define VARIABLES_H_INCLUDED

// input files
extern char TRAIN_FILE[256];
extern char TEST_FILE[256];

// output files
extern char FEATURE_TRAIN_FILE[256];
extern char FEATURE_TEST_PHASE_FILE[256];
extern char FEATURE_VALIDATE_PHASE_FILE[256];

extern char DOMAIN_URL_FILE[256];
extern char SERP_URL_FILE[256];
extern char QUERY_TERM_FILE[256];

extern char RESULT_VALIDATE_FILE[256];
extern char RESULT_MAPPER_FILE[256];

#define MB_OR_GB 1ull

// memory allocation
extern unsigned long long BUFF_SIZE_INPUT_READ;

extern unsigned long long BUFF_SIZE_FEATURE_TRAIN_FILE;
extern unsigned long long BUFF_SIZE_FEATURE_TEST_FILE;
extern unsigned long long BUFF_SIZE_VALIDATE_FILE;

extern unsigned long long BUFF_SIZE_DU_FILE;
extern unsigned long long BUFF_SIZE_SU_FILE;
extern unsigned long long BUFF_SIZE_QT_FILE;

extern unsigned long long BUFF_SIZE_RESULT_MAPPER;

extern unsigned long long BUFF_SIZE_VALIDATE_RESULT;


// Upper limits
#define MAX_ROW_CLASS_TERM 4
#define MAX_ROW_2_TERMS 28
#define MAX_ROW_MORE_THAN_20_TERMS 192
#define MAX_ROW_AROUND_10_TERMS 40

// mutual communication
#define PARSE_TRAIN_FILE 0
#define PARSE_TEST_FILE 1
#define SEARCH_IN_SESSION 2
#define SEARCH_BEFORE_SESSION 3

// Computation parameters
#define SIMILAR_INDEX_THRESH_FOR_QUERY 0.75
#define UNCLICKED_CLASS -1 // make one submission with this value = 0 also
#define RAND_SEED 25 // or make if time(0), kept it constant to make results reproducible 
#define PROB_KEEP_UNCLICKED_URL 5

#endif // VARIABLES_H_INCLUDED
