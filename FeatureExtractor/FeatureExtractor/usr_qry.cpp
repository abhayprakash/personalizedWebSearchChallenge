#include "stdafx.h"
#include "usr_qry.h"
#include "global.h"

void usr_qry::updateShown_userQueries(int uid, int qid, int urlid, int day)
{
	if(user_queries.find(uid) == user_queries.end() || user_queries[uid].queryMetadata.find(qid) == user_queries[uid].queryMetadata.end()) // no entry for it
	{
		user_queries[uid].queries.push_front(qid);
		user_queries[uid].queryMetadata[qid].pointerToQID = user_queries[uid].queries.begin();
		user_queries[uid].queryMetadata[qid].last_time_day = day;
		user_queries[uid].queryMetadata[qid].urlLastGrade[urlid] = UNCLICKED_CLASS;
	}
	else
	{
		user_queries[uid].queries.erase(user_queries[uid].queryMetadata[qid].pointerToQID);
		user_queries[uid].queries.push_front(qid);
		user_queries[uid].queryMetadata[qid].pointerToQID = user_queries[uid].queries.begin();
		user_queries[uid].queryMetadata[qid].last_time_day = day;
		user_queries[uid].queryMetadata[qid].urlLastGrade[urlid] = UNCLICKED_CLASS;
	}
}

void usr_qry::updateClicked_userQueries(int uid, int qid, int urlid, int grade)
{
	user_queries[uid].queryMetadata[qid].urlLastGrade[urlid] = grade;
}

void usr_qry::updateShown_local(int uid, int qid, int urlid, int time)
{
	if(local_user_queries.find(uid) == local_user_queries.end() || local_user_queries[uid].queryMetadata.find(qid) == local_user_queries[uid].queryMetadata.end()) // no entry for it
	{
		local_user_queries[uid].queries.push_front(qid);
	}
	else
	{
		local_user_queries[uid].queries.erase(local_user_queries[uid].queryMetadata[qid].pointerToQID);
		local_user_queries[uid].queries.push_front(qid);
	}
	local_user_queries[uid].queryMetadata[qid].pointerToQID = local_user_queries[uid].queries.begin();
	local_user_queries[uid].queryMetadata[qid].last_time_day = time;
	local_user_queries[uid].queryMetadata[qid].urlLastGrade[urlid] = UNCLICKED_CLASS;
}

void usr_qry::updateClicked_local(int uid, int qid, int urlid, int grade)
{
	local_user_queries[uid].queryMetadata[qid].urlLastGrade[urlid] = grade;
}

void usr_qry::copyLocalToGlobal_and_ClearLocal(int day)
{
	list<int>::iterator list_it;
	map<int, query_list>::iterator table_it;
	map<int, int>::iterator url_it;
	for(table_it = local_user_queries.begin(); table_it != local_user_queries.end(); ++table_it)
	{
		for(list_it = table_it->second.queries.begin();list_it != table_it->second.queries.end(); ++list_it)
		{
			for(url_it = table_it->second.queryMetadata[*list_it].urlLastGrade.begin(); url_it != table_it->second.queryMetadata[*list_it].urlLastGrade.end(); ++url_it)
			{
				updateShown_userQueries(table_it->first, *list_it, url_it->first , day);
				updateClicked_userQueries(table_it->first, *list_it, url_it->first, url_it->second);
			}
		}
	}
	local_user_queries.clear();
}

// it will return the data for nearest qid qualifying similarity criteria with true, false if doesn't exist
bool usr_qry::getRecentSimilarQueryData(int session_or_day, int uid, int actual_qid, int urlid, int &time_or_day, bool &URLshown, int &grade_that_time)
{
	list<int>::iterator it;
	int matchThisQuery, count;
	map<int, bool> q1_has_term;
	if(session_or_day == SEARCH_IN_SESSION)
	{
		for(it = local_user_queries[uid].queries.begin(); it != local_user_queries[uid].queries.end(); ++it)
		{
			matchThisQuery = *it;
			count = 0;
			q1_has_term.clear();
			for(int i = 0; i < queryTerms[matchThisQuery].size(); ++i)
			{
				q1_has_term[queryTerms[matchThisQuery][i]] = true;
			}
			for(int i = 0; i < queryTerms[actual_qid].size(); ++i)
			{
				if(q1_has_term[queryTerms[actual_qid][i]])
				{
					count++;
				}
			}
			if(count >= SIMILAR_INDEX_THRESH_FOR_QUERY * queryTerms[actual_qid].size())
			{
				// matchThisQuery has qualified
				time_or_day = local_user_queries[uid].queryMetadata[matchThisQuery].last_time_day;
				if(local_user_queries[uid].queryMetadata[matchThisQuery].urlLastGrade.find(urlid) != local_user_queries[uid].queryMetadata[matchThisQuery].urlLastGrade.end())
				{
					URLshown = true;
					grade_that_time = local_user_queries[uid].queryMetadata[matchThisQuery].urlLastGrade[urlid];
				}
				else
				{
					URLshown = false;
					grade_that_time = 0;
				}
				return true;
			}
		}
		return false;
	}
	else
	{
		for(it = user_queries[uid].queries.begin(); it != user_queries[uid].queries.end(); ++it)
		{
			matchThisQuery = *it;
			count = 0;
			q1_has_term.clear();
			for(int i = 0; i < queryTerms[matchThisQuery].size(); ++i)
			{
				q1_has_term[queryTerms[matchThisQuery][i]] = true;
			}
			for(int i = 0; i < queryTerms[actual_qid].size(); ++i)
			{
				if(q1_has_term[queryTerms[actual_qid][i]])
				{
					count++;
				}
			}
			if(count >= SIMILAR_INDEX_THRESH_FOR_QUERY * queryTerms[actual_qid].size())
			{
				// matchThisQuery has qualified
				time_or_day = user_queries[uid].queryMetadata[matchThisQuery].last_time_day;
				if(user_queries[uid].queryMetadata[matchThisQuery].urlLastGrade.find(urlid) != user_queries[uid].queryMetadata[matchThisQuery].urlLastGrade.end())
				{
					URLshown = true;
					grade_that_time = user_queries[uid].queryMetadata[matchThisQuery].urlLastGrade[urlid];
				}
				else
				{
					URLshown = false;
					grade_that_time = 0;
				}
				return true;
			}
		}
		return false;
	}
}