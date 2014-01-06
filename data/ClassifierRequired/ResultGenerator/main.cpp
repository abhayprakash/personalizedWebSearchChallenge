#include <iostream>
#include "variables.h"
#include "FileLogger.h"
#include <algorithm>
#include <iostream>
#include <math.h>

using namespace std;

struct answerRow{
    int sid, urlid, pos, result_class;
    double result[4];
    void getClass()
    {
        result_class = 0;
        double mx = result[0];
        for(int i = 0; i < 4; i++)
        {
            if(mx <= result[i])
            {
                mx = result[i];
                result_class = i;
            }
        }
    }
};

bool myfn(answerRow i, answerRow j)
{
    if(i.result_class > j.result_class)
        return true;

    if(i.result_class == j.result_class)
        if(i.pos < j.pos)
            return true;

    return false;
}

int main()
{
    FILE* fp_map = fopen(RESULT_MAPPER_FILE, "r");
    FILE* fp_result = fopen(PREDICTED_RESULT_FILE, "r");

    char* map_buff = (char*)malloc(BUFF_SIZE_INPUT_READ);
    char* result_buff = (char*)malloc(BUFF_SIZE_INPUT_READ);

    FileLogger logger(SUBMIT_FILE, BUFF_SIZE_OUT_FILE, MAX_ROW_2_TERMS); //char* path, unsigned long long size_buff, int size_row)

    /**Read data and bring to buffers**/
    int nRead = fread(map_buff, 1, BUFF_SIZE_INPUT_READ, fp_map);
    map_buff[nRead] = 0;
    printf("Read a chunk - %d bytes of given file\n", nRead);

    nRead = fread(result_buff, 1, BUFF_SIZE_INPUT_READ, fp_result);
    result_buff[nRead] = 0;
    printf("Read a chunk - %d bytes of given file\n", nRead);

    fclose(fp_map);
    fclose(fp_result);
    /** Buffers Made **/


    // do operation on buffer
    istringstream map_in(map_buff);
    istringstream result_in(result_buff);

    string rowInLog, temp_s_term;
    int rowNum, countRow = 0;;

    //int iterated_count = 0;
    while(map_in)
    {
        //printf("here\n");
        if(countRow == 10000)
        {
            //printf("Done 10000\n");
            countRow = 0;
        }
        int tobreak = false;
        answerRow shownURL[10];
        for(int i = 0; i < 10; i++)
        {
            getline(map_in, rowInLog);
            if(!map_in)
            {
                tobreak = true;
                break;
            }
            //cout<<rowInLog<<endl;

            istringstream tokenStream(rowInLog);

            int iteration = 0;
            //cout<<"Tokens: ";
            while(getline(tokenStream, temp_s_term, ','))
            {
                //cout<<temp_s_term<<" ";
                if(iteration == 0)
                    rowNum = atoi(temp_s_term.c_str());
                else if(iteration == 1)
                    shownURL[i].sid = atoi(temp_s_term.c_str());
                else if(iteration == 2)
                    shownURL[i].urlid = atoi(temp_s_term.c_str());
                else if(iteration == 3)
                {
                    shownURL[i].pos = atoi(temp_s_term.c_str());
                    //printf("%d %d\n", i, shownURL[i].pos);
                }
                iteration++;
            }
            //cout<<endl;
            for(int j = 0; j < 4; j++)
            {
                result_in>>shownURL[i].result[j];
            }
            shownURL[i].getClass();
            //cout<<"Originally:\n";
            //cout<<shownURL[i].sid<<"-------"<<shownURL[i].urlid<<"------"<<shownURL[i].result<<"-----"<<shownURL[i].pos<<endl;
        }
        sort(shownURL, shownURL+10, myfn);

        for(int i = 0; i < 10; i++)
        {
            if(tobreak)
                break;
            //cout<<"Logging: "<<shownURL[i].sid<<" "<<shownURL[i].urlid<<endl;
            logger.logSubmit(shownURL[i].sid,shownURL[i].urlid);
        }
        countRow += 10;
        //iterated_count++;
        //cout<<iterated_count<<endl;
    }
    //logger.wrapUp();
    return 0;
}
