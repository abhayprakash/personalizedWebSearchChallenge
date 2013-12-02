#include <stdafx.h>
#include "variables.h"

#include "my_global.h" // Include this file first to avoid problems
#include "mysql.h" // MySQL Include File

#include <vector>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "ap11"
#define DATABASE "short_userlog"

int main()
{
    MYSQL *connect; // Create a pointer to the MySQL instance
    connect=mysql_init(NULL); // Initialise the instance
    /* This If is irrelevant and you don't need to show it. I kept it in for Fault Testing.*/
    if(!connect)    /* If instance didn't initialize say so and exit with fault.*/
    {
        fprintf(stderr,"MySQL Initialization Failed");
        system("pause");
		return 1;
    }
    /* Now we will actually connect to the specific database.*/

    connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);
    /* Following if statements are unneeded too, but it's worth it to show on your
    first app, so that if your database is empty or the query didn't return anything it
    will at least let you know that the connection to the mysql server was established. */

    if(connect){
        printf("Connection Succeeded\n");
    }
    else{
        printf("Connection Failed!\n");
		system("pause");
		return 0;
    }
    MYSQL_RES *res_set; /* Create a pointer to recieve the return value.*/
    MYSQL_ROW row;  /* Assign variable for rows. */
	/***********************Parser********************/
	unsigned int SessionID, Day, USERID, TimePassed;
	unsigned int SERPID, QueryID, URLID;
	unsigned int DomainID, TermID;
	char TypeOfRecord;

    bool firstLogRow = true;
    unsigned int prevSessionID, prevQueryID, prevURLID, prevSERPID;
    unsigned int prevTimePassed;
    char queryToExecute[256];

    ifstream train_fin(SHORT_TRAIN_FILE_PATH);
    string rowInLog, tempForTypeOrTime;
    string listOfTerms, pairOfURLandDomain;
    bool firstClick;

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
                firstLogRow = false;
            }
            prevTimePassed = 0; // no use of initializing in M
            sin >> Day >> USERID;
            sprintf(queryToExecute, "INSERT INTO session VALUES(%d, %d, %d)",SessionID, USERID, Day);
			mysql_query(connect, queryToExecute);
            continue;
        }
        stringstream streamTime(tempForTypeOrTime);
        streamTime >> TimePassed;
        sin>>tempForTypeOrTime >> SERPID;

        //got SessionID, TimePassed, SERPID
        if(tempForTypeOrTime == "Q" || tempForTypeOrTime == "T") // for T, test file has to be opened.
        {
            sin>>QueryID;
            //insert into query
            //if day <= 24 insert into train_query
            //if day >= 25 and <= 27 insert into validate query
            //if day >= 28 insert into test query
            sin>>listOfTerms;
            istringstream tokenStream(listOfTerms);
            while(getline(tokenStream, TermID, ','))
            {
                //insert into query_has_terms

            }

            for(int rank = 1; rank <= 10; rank++)
            {
                sin>>pairOfURLandDomain;
                tokenStream.clear();
                tokenStream.str(pairOfURLandDomain);
                getline(tokenStream, URLID, ',');
                tokenStream>>DomainID;
                //insert into URL
                //insert into queryshowedlinks except timespent and grade
            }
            prevTimePassed = TimePassed;
            firstClick = true;
            continue;
        }
        if(tempForTypeOrTime == "C")
        {
            if(firstClick)
            {
                int timeForFirstClick = TimePassed - prevTimePassed;
                //alter query with queryID, sessionID, prevTimePassed to set clickedAnyLink=true, TimeForFirstClick = timeForFirstClick

                firstClick = false;
            }
            else
            {
                int timeOnPreviousLink = TimePassed - prevTimePassed;

            }
            prevTimePassed = TimePassed;
            continue;
        }
    }

    train_fin.close();
	/*************************************************/
    mysql_query(connect,"SELECT * FROM query");
    /* Send a query to the database. */
    unsigned int i = 0; /* Create a counter for the rows */

    res_set = mysql_store_result(connect); /* Receive the result and store it in res_set */

    unsigned int numrows = mysql_num_rows(res_set); /* Create the count to print all rows */

    /* This while is to print all rows and not just the first row found, */

    while ((row = mysql_fetch_row(res_set)) != NULL){
        printf("%s\n",row[i] != NULL ?
        row[i] : "NULL"); /* Print the row data */
    }
    mysql_close(connect);   /* Close and shutdown */
	system("pause");
    return 0;
}
