#include "variables.h"

// input files
char TRAIN_FILE[256] = "/home/abhay/pwsc/data/input/";
char TEST_FILE[256] = "/home/abhay/pwsc/data/test";

// output files
char FEATURE_TRAIN_FILE[256] = "/home/abhay/pwsc/data/Result/feature_train.txt";
char FEATURE_TEST_PHASE_FILE[256] = "/home/abhay/pwsc/data/Result/feature_test.txt";
char FEATURE_VALIDATE_PHASE_FILE[256] = "/home/abhay/pwsc/data/Result/feature_validate.txt";

char DOMAIN_URL_FILE[256] = "/home/abhay/pwsc/data/Result/DU_file.txt";
char SERP_URL_FILE[256] = "/home/abhay/pwsc/data/Result/SU_file.txt";
char QUERY_TERM_FILE[256] = "/home/abhay/pwsc/data/Result/QT_file.txt";
char USER_TERM_FILE[256] = "/home/abhay/pwsc/data/Result/USER_TERM_file.txt";
char USER_TERM_COUNT_FILE[256] = "/home/abhay/pwsc/data/Result/USER_TERM_COUNT_file.txt";
char URL_TERM_FILE[256] = "/home/abhay/pwsc/data/Result/URL_TERM_file.txt";

char RESULT_VALIDATE_FILE[256] = "/home/abhay/pwsc/data/Result/result_validate.txt";
char RESULT_MAPPER_FILE[256] = "/home/abhay/pwsc/data/Result/result_mapper.txt";

/**********************************************************************************************************************************/

// input buffer
unsigned long long BUFF_SIZE_INPUT_READ         = 1*1024*1024*MB_OR_GB;

// 20+ terms
unsigned long long BUFF_SIZE_FEATURE_TRAIN_FILE = 512*1024*MB_OR_GB;
unsigned long long BUFF_SIZE_FEATURE_TEST_FILE  = 512*1024*MB_OR_GB;
unsigned long long BUFF_SIZE_VALIDATE_FILE      = 512*1024*MB_OR_GB;

// 2 terms
unsigned long long BUFF_SIZE_DU_FILE            = 512*1024*MB_OR_GB;
unsigned long long BUFF_SIZE_SU_FILE            = 512*1024*MB_OR_GB;
unsigned long long BUFF_SIZE_QT_FILE            = 512*1024*MB_OR_GB;

// ~5 terms
unsigned long long BUFF_SIZE_RESULT_MAPPER      = 512*1024*MB_OR_GB;

// 1 term
unsigned long long BUFF_SIZE_VALIDATE_RESULT    = 512*1024*MB_OR_GB;
