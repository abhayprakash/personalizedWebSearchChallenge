#ifndef VARIABLES_H_INCLUDED
#define VARIABLES_H_INCLUDED

char TRAIN_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\small_train_dataset";
char FEATURE_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\feature.txt";
char URL_USR_DATA_FILE[256] = "E:\\Projects\\WSDM_PersonalisedWebSearchChallenge\\Data\\usr_url_data.txt"; // required in test phase for before session feature

#define BUFF_SIZE_FEATURE_FILE 10*1024*1024*1024ull //10 GB
#define BUFF_SIZE_USR_URL_FILE 10*1024*1024*1024ull // not required

#define NUM_MAX_ROW_CHAR 150
#define NUM_MAX_WORD_CHAR 15
#define SIMILAR_INDEX_THRESH_FOR_QUERY 100

#endif // VARIABLES_H_INCLUDED
