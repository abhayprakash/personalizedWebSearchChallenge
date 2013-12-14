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
user_id, query_id, url_id, url_position, max_rel_of_this_url_for_this_user,
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
map<int, vector<int> > domainURLs;
map<int, vector<int> > serpURLs;

// DS for getting url data for particular user before session
class usr_url{
	struct rec{
		int max_grade, latest_grade, last_day; 
	};
	rec temp;

	FILE* fp;

public:
	map<int, map<int, rec> > table_usr_url; // public: for the case i want to use it directly

	void update(int uid, int urlid, int mg, int lg, int ld)
	{
		temp.max_grade = mg;
		temp.latest_grade = lg;
		temp.last_day = ld;

		table_usr_url[uid][urlid] = temp;
	}

	void updateDayGrade(int uid, int urlid, int day, int grade)
	{
		if(table_usr_url.find(uid) == table_usr_url.end() || table_usr_url[uid].find(urlid) == table_usr_url[uid].end())
		{
			update(uid, urlid, grade, grade, day);
			return;
		}
		table_usr_url[uid][urlid].max_grade = max(table_usr_url[uid][urlid].max_grade, grade);
		if(table_usr_url[uid][urlid].last_day < day)
		{
			table_usr_url[uid][urlid].last_day = day;
			table_usr_url[uid][urlid].latest_grade = grade;
		}
	}

	int getMaxGrade(int uid, int urlid)
	{
		return table_usr_url[uid][urlid].max_grade;
	}

	int getLatestGrade(int uid, int urlid)
	{
		return table_usr_url[uid][urlid].latest_grade;
	}

	int getLastDay(int uid, int urlid)
	{
		return table_usr_url[uid][urlid].last_day;
	}
	
	// call at end
	void flushLastData()
	{
		fp = fopen(URL_USR_DATA_FILE,"w");
		char* buffer = (char *)malloc(); 
		fclose(fp);
	}

	void wrapUp()
	{
	}
};

// DS for getting similar query for particular user before session
class usr_query{
	struct rec{
		int max_grade, latest_grade, last_day;
	};
	rec temp;

	int getRecentSimilarQuery(int qid)
	{
		if(SIMILAR_INDEX_THRESH_FOR_QUERY == 100)
			return qid;
		// to implement below

	}
public:
	map<int, map<int, rec> > table_usr_qry; // public: for the case i want to use it directly
	
	void update(int uid, int qid, int mg, int lg, int ld)
	{
		temp.max_grade = mg;
		temp.latest_grade = lg;
		temp.last_day = ld;

		table_usr_qry[uid][qid] = temp;
	}

	void updateDayGrade(int uid, int qid, int day, int grade)
	{
		if(table_usr_qry.find(uid) == table_usr_qry.end() || table_usr_qry[uid].find(qid) == table_usr_qry[uid].end())
		{
			update(uid, qid, grade, grade, day);
			return;
		}
		table_usr_qry[uid][qid].max_grade = max(table_usr_qry[uid][qid].max_grade, grade);
		if(table_usr_qry[uid][qid].last_day < day)
		{
			table_usr_qry[uid][qid].last_day = day;
			table_usr_qry[uid][qid].latest_grade = grade;
		}
	}

	int getMaxGrade_forSimilar(int uid, int qid)
	{
		int recent_similar_query = getRecentSimilarQuery(qid);
		return table_usr_qry[uid][recent_similar_query].max_grade;
	}

	int getLatestGrade_forSimilar(int uid, int qid)
	{
		int recent_similar_query = getRecentSimilarQuery(qid);
		return table_usr_qry[uid][recent_similar_query].latest_grade;
	}

	int getLastDay_forSimilar(int uid, int qid)
	{
		int recent_similar_query = getRecentSimilarQuery(qid);
		return table_usr_qry[uid][recent_similar_query].last_day;
	}

	void flushLastData()
	{
		//To implement
	}

	void wrapUp()
	{
	}
};

// this collects data and orders by default - provide table populated as public so as to be used by Processor
class DataCollector{
	FILE* fp;

	struct shownURL{
		int url_id, timeOfClick, grade;
	};
	shownURL tempURL;

	struct queryRec{
		int qid, timeOfQuery;
		vector<shownURL> clickedURL;
	};
	queryRec tempQuery;
	
	struct sessionRec{
		int uid, session_id; // session_id not to be output in any file
		vector<queryRec> queries;
	};
	sessionRec tempSession;

public:
	vector<sessionRec> RecordOfDay[31];

	DataCollector()
	{
		fp = fopen(TRAIN_FILE, "r");
	}

	void parseFile()
	{

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

	Processor p;
	P.its_main();

	storeRoom_usr_url.flushLastData();
	storeRoom_usr_url.wrapUp();

	storeRoom_usr_query.flushLastData();
	storeRoom_usr_query.wrapUp();
}

struct rowToPrint{
    int user_id;
    int session_id;
    int query_id;
    int query_day;
    int num_of_query_in_current_session;
    int num_of_clicked_result_for_this_query;
    vector<int> clicked_ranks_in_click_order;
    int num_of_relevant_2_click;
    int num_of_relevant_1_click;
    int num_of_irrelevant_click;
    int num_of_unique_query_term;
    int num_of_unique_domain;
    int time_of_action;
    int last_action_time;
    rowToPrint()
    {
        num_of_query_in_current_session = 0;
        num_of_clicked_result_for_this_query = 0;
        num_of_relevant_2_click = 0;
        num_of_relevant_1_click = 0;
        num_of_irrelevant_click = 0;
        num_of_unique_query_term = 0;
        num_of_unique_domain = 0;
        time_of_action = 0;
        last_action_time = 0;
    }
};

int main()
{
    //FILE* fp_in = fopen("verySmall", "r");
    FILE* fp_in = fopen("small_train_dataset", "r");

    fseek(fp_in, 0, SEEK_END);
	long fsize = ftell(fp_in);
	fseek(fp_in, 0, SEEK_SET);

    char* buff = (char *)malloc(fsize + 1);
    fread(buff, fsize, 1, fp_in);
    buff[fsize] = 0;
    fclose(fp_in);

    stringstream train_fin(buff);

    FILE* fp_sessions = fopen("sessions.txt", "w");
    FILE* fp_queries = fopen("queries.txt", "w");
    FILE* fp_click = fopen("clicks_in_order.txt", "w");

    #define MAXROW 1000
    rowToPrint* outRow = new rowToPrint[MAXROW];

    bool firstLogRow = true;
    unsigned int prevTimePassed;
    bool clickedAnyURLForThisQuery;
    bool madeSomeQueryForThisSession;

    int row_i = -1;
    string rowInLog, tempForTypeOrTime, SERPID;
    string listOfTerms, pairOfURLandDomain;
    int URLID, DomainID;
    map<int, int> url_rank;
    int tempSess;
    while(getline(train_fin, rowInLog))
    {
        if(row_i == 1000)
            break;
        //cout<<"HERE1\n";
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
