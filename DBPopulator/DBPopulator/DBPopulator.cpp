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
	unsigned int SessionID, Day, USERID;
    char TypeOfRecord;
    unsigned int TimePassed, SERPID, QueryID, URLID, DomainID, TermID;
    
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
			char queryToExecute[256];
			sprintf(queryToExecute, "INSERT INTO session VALUES(%d)",);
			mysql_query(connect, queryToExecute); 
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