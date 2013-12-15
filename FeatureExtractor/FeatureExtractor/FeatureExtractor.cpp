#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include "variables.h"
#include "FeatureFileLogger.h"

using namespace std;
/*
Files required:
1.
user_id, query_id, url_id,count_earlier_shown,count_earlier_2,count_earlier_1,count_earlier_0,url_position,
{url_displayed_in_same_session(bool), time_difference_from_most_recent_display, grade_that_time(0/1/2)},
{url_displayed_before_current_session(bool), day_difference_from_most_recent, grade_that_time},
{similar_query_in_same_session(bool), time_diff_from_most_recent, this_url_shown(bool), grade_that_time},
{similar_query_before_current_session(bool), day_difference_from_most_recent, this_url_shown(bool), grade_that_time},
grade_this_time

2. 
query_id, term_id // iterate over map queryTerms to print all it

3.
domain_id, url_id // iterate over map domainURLs to print all it

4.
serp_id, url_id // iterate over map serpURLs to print all it
*/

// Global query -> {term}
map<int, vector<int> > queryTerms;
int g_serpid = 0;
			
// this collects data and orders by default - provide table populated as public so as to be used by Processor
class DataCollector{
	FILE* fp;
	char *buffer;
	unsigned long long buffSize;

	struct shownURL{
		int url_id, timeOfClick, position;
	};
	struct queryRec{
		int qid, timeOfQuery;
		vector<shownURL> clickedURL;
	};
	struct sessionRec{
		int uid, session_id; // session_id not to be output in any file
		vector<queryRec> queries;
	};
	
	shownURL tempURL;
	queryRec tempQuery;
	sessionRec tempSession;
public:
	vector<sessionRec> RecordOfDay[31];

	DataCollector(char* path) // phase I : provide path to train file, phase II : provide path to test file
	{
		fp = fopen(path, "r");
		buffSize = BUFF_SIZE_INPUT_READ;
		buffer = (char *)malloc(buffSize);
	}

	void parseFile()
	{
		int nRead;
		while((nRead = fread(buffer, 1, buffSize, fp)) > 0)
		{
			printf("Read a chunk of file\n");
			if(nRead == buffSize)
			{
				int movedBack = 0;
				while(buffer[nRead-1] != '\n')
				{
					nRead--;
					movedBack++;
				}
				fseek(fp, -movedBack, SEEK_CUR);
			}
			buffer[nRead] = 0;
			// do operation on buffer
			istringstream buff_in(buffer);
			string rowInLog;
			
			//temp
			int temp_sid, temp_day, temp_time, temp_uid, temp_serp, temp_qid, temp_term, temp_url, temp_domain;
			string temp_typeOrTime;
			
			while(getline(buff_in, rowInLog))
			{
				istringstream sin(rowInLog);
				sin>>temp_sid>>temp_typeOrTime;
				if(temp_typeOrTime == "M")
				{
					sin>>temp_day>>temp_uid;
					// sid, day, uid
					continue;
				}
				temp_time = atoi(temp_typeOrTime.c_str());
				sin>>temp_typeOrTime;
				if(temp_typeOrTime == "Q"
			}
		}
	}

	void wrapUp()
	{
		fclose(fp);
		free(buffer);
		printf("data populated\n");
	}
};

// iterates over the collection by DataCollector seqentially to {fill, use the two DSs and put data in FeatureFileLogger}
class Processor{
public:
	void its_main()
	{
	}
};

int main()
{
	usr_url storeRoom_usr_url;
	usr_query storeRoom_usr_query;

	DataCollector dc;
	dc.parseFile();

	Processor P;
	P.its_main();

	storeRoom_usr_url.flushLastData();
	storeRoom_usr_url.wrapUp();

	storeRoom_usr_query.flushLastData();
	storeRoom_usr_query.wrapUp();
}

int main()
{
    stringstream train_fin(buff);
	
	while(getline(train_fin, rowInLog))
    {
        stringstream sin(rowInLog);
        sin>>tempSess>>tempForTypeOrTime;
        if(tempForTypeOrTime == "M")
        {
            row_i++;
            outRow[row_i].session_id = tempSess;
            if(!firstLogRow)
            {
                if(clickedAnyURLForThisQuery)
                {
                    outRow[row_i-1].num_of_relevant_2_click++;
                }
            }
            firstLogRow = false;

            sin >> outRow[row_i].query_day >> outRow[row_i].user_id;
			madeSomeQueryForThisSession = false;
            outRow[row_i].num_of_query_in_current_session = 0;
            //cout<<"in row:"<<row_i<<endl;
            //cout << outRow[row_i].session_id<<" "<<tempForTypeOrTime<<" "<<outRow[row_i].query_day<<" "<<outRow[row_i].user_id<<endl;
            continue;
        }
        stringstream streamTime(tempForTypeOrTime);
        int temp_Time;
        streamTime >> temp_Time;
        sin>>tempForTypeOrTime >> SERPID;

        if(tempForTypeOrTime == "Q")
        {
            row_i++;
            outRow[row_i].session_id = tempSess;
            outRow[row_i].query_day = outRow[row_i-1].query_day;
            outRow[row_i].user_id = outRow[row_i-1].user_id;
            outRow[row_i].time_of_action = temp_Time;
            outRow[row_i].num_of_query_in_current_session++;
            if(madeSomeQueryForThisSession)
            {
                if(clickedAnyURLForThisQuery)
                {
                    int timeSpentOnLastLink = outRow[row_i].time_of_action - prevTimePassed;
                    int grade = 0;
                    if(timeSpentOnLastLink >= 50)
                        grade = 1;
                    if(timeSpentOnLastLink >= 400)
                        grade = 2;

					if(grade == 2)
                        outRow[row_i - 1].num_of_relevant_2_click++;
                    else if(grade == 1)
                        outRow[row_i - 1].num_of_relevant_1_click++;
                    else
                        outRow[row_i - 1].num_of_irrelevant_click++;
                }
            }
            else
                madeSomeQueryForThisSession = true;

            sin>>outRow[row_i].query_id;

            sin>>listOfTerms;
            istringstream tokenStream(listOfTerms);
            string s_termID;
            map<string, int> table_term;
            while(getline(tokenStream, s_termID, ','))
            {
                if(!table_term[s_termID])
                    outRow[row_i].num_of_unique_query_term++;
                table_term[s_termID] = 1;
            }

            map<int, int> table_domain;
            url_rank.clear();
            for(int rank = 1; rank <= 10; rank++)
            {
                sin>>pairOfURLandDomain;
                tokenStream.clear();
                tokenStream.str(pairOfURLandDomain);
                string s_URLID;
                getline(tokenStream, s_URLID, ',');
                URLID = atoi(s_URLID.c_str());
                url_rank[URLID] = rank;
                tokenStream>>DomainID;
                if(!table_domain[DomainID])
                    outRow[row_i].num_of_unique_domain++;
                table_domain[DomainID] = 1;
            }
            clickedAnyURLForThisQuery = false;
            continue;
        }
        if(tempForTypeOrTime == "C")
        {
            outRow[row_i].last_action_time = temp_Time;
            clickedAnyURLForThisQuery = true;
            sin>>URLID;
            //cout << "here url : "<<URLID<<" "<<url_rank[URLID]<<endl;
            outRow[row_i].clicked_ranks_in_click_order.push_back(url_rank[URLID]);
            //cout<<"just pushed to "<<row_i<<" "<<outRow[row_i].clicked_ranks_in_click_order[outRow[row_i].clicked_ranks_in_click_order.size()-1]<<endl;
            outRow[row_i].num_of_clicked_result_for_this_query++;

            if(outRow[row_i].num_of_clicked_result_for_this_query > 1)
            {
                int timeSpentOnLastLink = temp_Time - prevTimePassed;
                int grade = 0;
                if(timeSpentOnLastLink >= 50)
                    grade = 1;
                if(timeSpentOnLastLink >= 400)
                    grade = 2;

                if(grade == 2)
                    outRow[row_i].num_of_relevant_2_click++;
                else if(grade == 1)
                    outRow[row_i].num_of_relevant_1_click++;
                else
                    outRow[row_i].num_of_irrelevant_click++;
            }
            prevTimePassed = temp_Time;
            continue;
        }
    }
/*
    fprintf(fp, "            user_id\t\tsession_id\t\tquery_day\t\tnum_of_query_in_current_session\t\tlength of session\t\t");
    fprintf(fp, "query id\t\tnum_of_clicked_result_for_this_query\t\t");
    fprintf(fp, "num_of_unique_query_term\t\tnum_of_unique_domain\t\t");
    fprintf(fp, "num_of_relevant_2_click\t\tnum_of_relevant_1_click\t\tnum_of_irrelevant_click\t\tclicked_ranks_in_click_order\n");
*/
    for(int i = 0; i <= row_i; i++)
    {
        if(i==0 || outRow[i].session_id != outRow[i-1].session_id)
        {
            int count_queries = 0, length_sess = 0;
            for(int j = i+1; j < MAXROW; j++)
            {
                if(outRow[j].session_id != outRow[i].session_id)
                    break;
                count_queries++;
                length_sess = outRow[j].last_action_time;
            }
            //fprintf(fp,"Session : ");
            fprintf(fp_sessions, "%d,%d,%d,%d,%d\n", outRow[i].user_id, outRow[i].session_id, outRow[i].query_day, count_queries, length_sess);
        }
        else
        {
            //fprintf(fp,"          ");
            fprintf(fp_queries, "%d,%d,%d,", outRow[i].user_id, outRow[i].session_id, outRow[i].query_day);//, outRow[i].num_of_query_in_current_session,outRow[i].time_of_action);
            fprintf(fp_queries, "%d,%d,", outRow[i].query_id, outRow[i].num_of_clicked_result_for_this_query);
            fprintf(fp_queries, "%d,%d,",outRow[i].num_of_unique_query_term,outRow[i].num_of_unique_domain);
            fprintf(fp_queries, "%d,%d,%d\n", outRow[i].num_of_relevant_2_click,outRow[i].num_of_relevant_1_click,outRow[i].num_of_irrelevant_click);
            for(int j = 0; j < outRow[i].num_of_clicked_result_for_this_query;  j++)
                fprintf(fp_click, "%d,%d,%d,%d\n",outRow[i].user_id,outRow[i].session_id, outRow[i].query_id ,outRow[i].clicked_ranks_in_click_order[j]);
        }
    }
    fclose(fp_sessions);
    fclose(fp_queries);
    fclose(fp_click);
}
