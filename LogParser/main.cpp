#include "variables.h"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct URL_Domain_Pair
{
    unsigned int URLID, DomainID;
};

int main()
{
    //variables in a row
    unsigned int SessionID, Day, USERID;
    char TypeOfRecord;
    unsigned int TimePassed, SERPID, QueryID, URLID, DomainID, TermID;
    vector<unsigned int> ListOfTerms;
    vector<URL_Domain_Pair> ListOfURLsAndDomains;


    ifstream train_fin(SHORT_TRAIN_FILE_PATH);
    string rowInLog;
    string temp;
    while(getline(train_fin, rowInLog))
    {
        stringstream sin(rowInLog);
        sin>>SessionID;
        sin>>temp;
        if(temp == "M")
        {
            sin >> Day >> USERID;
            //Insert in DB acc to 1
            continue;
        }
        else
        {
            stringstream streamTime(temp);
            streamTime >> TimePassed;
            sin>>temp >> SERPID;
            if(temp == "C") // for T, test file has to be opened.
            {
                sin>>URLID;
                //Insert in DB acc to 3
            }
            else
            {
               sin>>QueryID;

            }
        }
    }

    train_fin.close();
}
