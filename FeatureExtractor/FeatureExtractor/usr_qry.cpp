#include "usr_qry.h"
#include "global.h"

void usr_qry::updateShown_userQueries(int uid, int qid, int urlid, int day)
{
	if(user_queries[uid].queryMetadata.find(qid) != user_queries[uid].queryMetadata.end()) // no entry for it
	{
		user_queries[uid].queries.erase(user_queries[uid].queryMetadata[qid].pointerToQID);
	}
	user_queries[uid].queries.push_front(qid);
	user_queries[uid].queryMetadata[qid].pointerToQID = user_queries[uid].queries.begin();
	user_queries[uid].queryMetadata[qid].last_time_day = day;
	user_queries[uid].queryMetadata[qid].urlLastGrade[urlid] = UNCLICKED_CLASS;
}

void usr_qry::updateClicked_userQueries(int uid, int qid, int urlid, int grade)
{
	user_queries[uid].queryMetadata[qid].urlLastGrade[urlid] = grade;
}

void usr_qry::updateShown_local(int qid, int urlid, int time)
{
	if(local_user_queries.queryMetadata.find(qid) == local_user_queries.queryMetadata.end()) // no entry for it
	{
		local_user_queries.queries.push_front(qid);
	}
	else
	{
		local_user_queries.queries.erase(local_user_queries.queryMetadata[qid].pointerToQID);
		local_user_queries.queries.push_front(qid);
	}
	local_user_queries.queryMetadata[qid].pointerToQID = local_user_queries.queries.begin();
	local_user_queries.queryMetadata[qid].last_time_day = time;
	local_user_queries.queryMetadata[qid].urlLastGrade[urlid] = UNCLICKED_CLASS;
}

void usr_qry::updateClicked_local(int qid, int urlid, int grade)
{
	local_user_queries.queryMetadata[qid].urlLastGrade[urlid] = grade;
}

void usr_qry::copyLocalToGlobal_and_ClearLocal(int day, int uid)
{
	list<int>::iterator list_it;
	map<int, int>::iterator url_it;
	for(list_it = local_user_queries.queries.begin();list_it != local_user_queries.queries.end(); ++list_it)
	{
		for(url_it = local_user_queries.queryMetadata[*list_it].urlLastGrade.begin(); url_it != local_user_queries.queryMetadata[*list_it].urlLastGrade.end(); ++url_it)
		{
			updateShown_userQueries(uid, *list_it, url_it->first , day);
			updateClicked_userQueries(uid, *list_it, url_it->first, url_it->second);
		}
	}
	local_user_queries.queries.clear();
	local_user_queries.queryMetadata.clear();
}

// it will return the data for nearest qid qualifying similarity criteria with true, false if doesn't exist
bool usr_qry::getRecentSimilarQueryData(int session_or_day, int uid, int actual_qid, int urlid, int &time_or_day, bool &URLshown, int &grade_that_time)
{
	//int checkedQueries;
	list<int>::iterator it;
	int matchThisQuery, count;
	map<int, bool> q1_has_term;
	if(session_or_day == SEARCH_IN_SESSION)
	{
		if(local_user_queries.queryMetadata.find(actual_qid) == local_user_queries.queryMetadata.end())
        {
            time_or_day = 0;
            URLshown = false;
            grade_that_time = UNCLICKED_CLASS;
            return false;
        }
        else
        {
            time_or_day = local_user_queries.queryMetadata[actual_qid].last_time_day;
            if(local_user_queries.queryMetadata[actual_qid].urlLastGrade.find(urlid) == local_user_queries.queryMetadata[actual_qid].urlLastGrade.end())
            {
                URLshown = false;
                grade_that_time = UNCLICKED_CLASS;
            }
            else
            {
                URLshown = true;
                grade_that_time = local_user_queries.queryMetadata[actual_qid].urlLastGrade[urlid];
            }
        }
	}
	else
	{
        if(user_queries[uid].queryMetadata.find(actual_qid) == user_queries[uid].queryMetadata.end())
        {
            time_or_day = 0;
            URLshown = false;
            grade_that_time = UNCLICKED_CLASS;
            return false;
        }
        else
        {
            time_or_day = user_queries[uid].queryMetadata[actual_qid].last_time_day;
            if(user_queries[uid].queryMetadata[actual_qid].urlLastGrade.find(urlid) == user_queries[uid].queryMetadata[actual_qid].urlLastGrade.end())
            {
                URLshown = false;
                grade_that_time = UNCLICKED_CLASS;
            }
            else
            {
                URLshown = true;
                grade_that_time = user_queries[uid].queryMetadata[actual_qid].urlLastGrade[urlid];
            }
        }
		return false;
	}
}

int usr_qry::getOldestQueryDay(int uid)
{
	if(user_queries[uid].queries.size() != 0)
		return user_queries[uid].queryMetadata[user_queries[uid].queries.back()].last_time_day;
	else
		return 0;
}
