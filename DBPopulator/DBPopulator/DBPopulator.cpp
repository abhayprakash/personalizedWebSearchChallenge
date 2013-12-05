/**
Author : Abhay Prakash
email : abhay.prakash1@gmail.com

Desc: Code to parse the user log. Made a single one to parse any of the train or test file.
Reads the given file and puts the data in the connected database.
**/
#include <stdafx.h>
#include "variables.h"

#include "my_global.h" // Include this file first to avoid problems
#include "mysql.h" // MySQL Include File

#include <vector>
#include <map>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "ap11"
#define DATABASE "new_userlog"

int main()
{
    MYSQL *connect;
    connect=mysql_init(NULL);
    if(!connect)
    {
        fprintf(stderr,"MySQL Initialization Failed");
        system("pause");
		return 1;
    }

    connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);

    if(connect){
        printf("Connection Succeeded\n");
    }
    else{
        printf("Connection Failed!\n");
		system("pause");
		return 0;
    }
    //MYSQL_RES *res_set; /* Create a pointer to recieve the return value.*/
    //MYSQL_ROW row;  /* Assign variable for rows. */
	/***********************Parser********************/
	unsigned int SessionID, Day, USERID, TimePassed;
	unsigned int SERPID, QueryID, URLID;
	unsigned int DomainID, TermID;
	char TypeOfRecord;

    bool firstLogRow = true;
    unsigned int prevSessionID, prevQueryID, prevURLID, prevSERPID;
    unsigned int prevTimePassed;
    char queryToExecute[256];

    //ifstream train_fin(SHORT_TRAIN_FILE_PATH);
    string rowInLog, tempForTypeOrTime;
    string listOfTerms, pairOfURLandDomain;
    bool clickedAnyURLForThisQuery;
    bool madeSomeQueryForThisSession;
    map<int, bool> userHasPreviousQueriesBeforeThisQuery;

    fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *buff = (char*)malloc(fsize + 1);
	fread(buff, fsize, 1, fp);
	fclose(fp);
	buff[fsize] = 0;
	stringstream train_fin(buff);
	while(getline(train_fin, rowInLog))
    {
        stringstream sin(rowInLog);
        sin>>SessionID>>tempForTypeOrTime;
        if(tempForTypeOrTime == "M")
        {
            if(!firstLogRow)
            {
                //alter queryShowedLinks to set grade = 2 acc. to prevSessionID, prevQueryID, prevURLID, prevSERPID
                    //check whether clicked for last query
                if(clickedAnyURLForThisQuery)
                {
					/**checked**/
                    sprintf(queryToExecute, "UPDATE queryshowedlinks SET Grade=2 WHERE SessionID=%d AND QueryID=%d AND URLID=%d AND SERPID=%d",prevSessionID, prevQueryID, prevURLID, prevSERPID);
                    mysql_query(connect, queryToExecute);
                }
            }
            firstLogRow = false;

            sin >> Day >> USERID;
			/**checked**/
            sprintf(queryToExecute, "INSERT INTO sessions VALUES(%d, %d, %d)",SessionID, USERID, Day);
			mysql_query(connect, queryToExecute);
            madeSomeQueryForThisSession = false;
            continue;
        }
        stringstream streamTime(tempForTypeOrTime);
        streamTime >> TimePassed;
        sin>>tempForTypeOrTime >> SERPID;

        //got SessionID, TimePassed, SERPID
        if(tempForTypeOrTime == "Q" || tempForTypeOrTime == "T") // for T, test file has to be opened.
        {
            if(madeSomeQueryForThisSession)
            {
                // alter queryShowedLinks for prev values
                    //check whether clicked for previous query
                if(clickedAnyURLForThisQuery)
                {
                    int timeSpentOnLastLink = TimePassed - prevTimePassed;
                    int grade = 0;
                    if(timeSpentOnLastLink >= 50)
                        grade = 1;
                    if(timeSpentOnLastLink >= 400)
                        grade = 2;
					/**checked**/
                    sprintf(queryToExecute, "UPDATE queryshowedlinks SET TimeSpent=%d,Grade=%d WHERE SessionID=%d AND QueryID=%d AND URLID=%d AND SERPID=%d",timeSpentOnLastLink,grade,prevSessionID, prevQueryID, prevURLID, prevSERPID);
                    mysql_query(connect, queryToExecute);
                }
            }
            else
                madeSomeQueryForThisSession = true;

            sin>>QueryID;
            //insert into query
			/**checked**/
            sprintf(queryToExecute, "INSERT INTO query(QueryID,SessionID,QueryMadeAtTime) VALUES(%d, %d, %d)",QueryID, SessionID, TimePassed);
			mysql_query(connect, queryToExecute);

            //if day >= 25 and <= 27 insert into validate query
            if(Day >= 25 && Day <= 27)
            {
                if(userHasPreviousQueriesBeforeThisQuery[USERID])
                {
                    /**checked**/
                    sprintf(queryToExecute, "INSERT INTO validate_query VALUES(%d, %d)", SessionID, QueryID);
                    mysql_query(connect, queryToExecute);
                }
            }

            //Put all with 'Q' in train query
            if(tempForTypeOrTime == "Q")
            {
                userHasPreviousQueriesBeforeThisQuery[USERID] = true;
                /**checked**/
                sprintf(queryToExecute, "INSERT INTO train_query VALUES(%d, %d)", SessionID, QueryID);
                mysql_query(connect, queryToExecute);
            }
            else // 'T'
            {
				/**checked**/
				sprintf(queryToExecute, "INSERT INTO test_query VALUES(%d, %d)", SessionID, QueryID);
                mysql_query(connect, queryToExecute);
            }

            sin>>listOfTerms;
            istringstream tokenStream(listOfTerms);
            string s_termID;
            while(getline(tokenStream, s_termID, ','))
            {
                TermID = atoi(s_termID.c_str());//could have kept this variable as string - as currently used only in query, but for genericity kept as int and used conversion
                //insert into query_has_terms
				/**checked**/
                sprintf(queryToExecute, "INSERT INTO queryhasterms VALUES(%d, %d)", QueryID, TermID);
                mysql_query(connect, queryToExecute);
            }

            for(int rank = 1; rank <= 10; rank++)
            {
                sin>>pairOfURLandDomain;
                tokenStream.clear();
                tokenStream.str(pairOfURLandDomain);
                string s_URLID;
                getline(tokenStream, s_URLID, ',');
                URLID = atoi(s_URLID.c_str());
                tokenStream>>DomainID;

                //insert into URL
				/**checked**/
                sprintf(queryToExecute, "INSERT INTO url VALUES(%d, %d)", URLID, DomainID);
                mysql_query(connect, queryToExecute);

                //insert into queryshowedlinks except timespent, WasClicked and grade
				/**checked**/
                sprintf(queryToExecute, "INSERT INTO queryshowedlinks(SessionID,QueryID,URLID,SERPID,DisplayRank) VALUES(%d,%d,%d,%d,%d)", SessionID, QueryID, URLID, SERPID, rank);
                mysql_query(connect, queryToExecute);
            }
            prevTimePassed = TimePassed;
            clickedAnyURLForThisQuery = false;
            continue;
        }
        if(tempForTypeOrTime == "C")
        {
            sin>>URLID;
            //update queryshowedlinks set wasClicked = 1
			/**checked**/
            sprintf(queryToExecute, "UPDATE queryshowedlinks SET WasClicked=1 WHERE SessionID=%d AND QueryID=%d AND URLID=%d AND SERPID=%d", SessionID,QueryID,URLID,SERPID);
            mysql_query(connect, queryToExecute);

            if(!clickedAnyURLForThisQuery)//i.e it is the first click
            {
                int timeForFirstClick = TimePassed - prevTimePassed;
                //alter query with queryID, sessionID, prevTimePassed to set clickedAnyLink=true, TimeForFirstClick = timeForFirstClick
				/**checked**/
                sprintf(queryToExecute, "UPDATE query SET ClickedAnyLink=1,TimeForFirstClick=%d WHERE QueryID=%d AND SessionID=%d AND QueryMadeAtTime=%d",timeForFirstClick,QueryID,SessionID,prevTimePassed);
                mysql_query(connect, queryToExecute);

                clickedAnyURLForThisQuery = true;
            }
            else
            {
                int timeSpentOnLastLink = TimePassed - prevTimePassed;
                int grade = 0;
                if(timeSpentOnLastLink >= 50)
                    grade = 1;
                if(timeSpentOnLastLink >= 400)
                    grade = 2;

                //alter previous in queryshowedlinks
				/**checked**/
                sprintf(queryToExecute, "UPDATE queryshowedlinks SET TimeSpent=%d,Grade=%d WHERE SessionID=%d AND QueryID=%d AND URLID=%d AND SERPID=%d",timeSpentOnLastLink,grade,prevSessionID, prevQueryID, prevURLID, prevSERPID);
                mysql_query(connect, queryToExecute);
            }
            prevSessionID = SessionID;
            prevQueryID = QueryID;
            prevSERPID = SERPID;
            prevURLID = URLID;
            prevTimePassed = TimePassed;
            continue;
        }
    }

    mysql_close(connect);   /* Close and shutdown */
	system("pause");
    return 0;
}
/***************EXAMPLE****************************
    mysql_query(connect,"SELECT * FROM query");
    // Send a query to the database.
    unsigned int i = 0; // Create a counter for the rows

    res_set = mysql_store_result(connect); // Receive the result and store it in res_set

    unsigned int numrows = mysql_num_rows(res_set); // Create the count to print all rows

    // This while is to print all rows and not just the first row found,

    while ((row = mysql_fetch_row(res_set)) != NULL){
        printf("%s\n",row[i] != NULL ?
        row[i] : "NULL"); // Print the row data
    }
***************************************************/
