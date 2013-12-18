#include "stdafx.h"
#include "usr_qry.h"


void usr_qry::updateShown_userQueries(int uid, int qid, int urlid, int day)
{
	list<int>::iterator it;
	for(it = user_queries[uid].queries.begin(); it!= user_queries[uid].queries.end(); ++it)
	{
		if(*it == qid)
			break;
	}

	if(it == user_queries[uid].queries.end()) // no entry for it
	{
		user_queries[uid].queries.push_front(qid);
		user_queries[uid].queryMetadata[qid].last_time_day = day;
		user_queries[uid].queryMetadata[qid].urlLastGrade[urlid] = UNCLICKED_CLASS;
	}
	else
	{
		user_queries[uid].queries.erase(it);
		user_queries[uid].queries.push_front(qid);
		user_queries[uid].queryMetadata[qid].last_time_day = day;
		user_queries[uid].queryMetadata[qid].urlLastGrade[urlid] = UNCLICKED_CLASS;
	}
}

void usr_qry::updateClicked_userQueries(int uid, int qid, int urlid, int grade)
{

}

void usr_qry::updateShown_local(int uid, int qid, int urlid, int time)
{

}

void usr_qry::updateClicked_local(int uid, int qid, int urlid, int grade)
{

}

void usr_qry::copyLocalToGlobal_and_ClearLocal(int day)
{

}

// it will return the data for nearest qid qualifying similarity criteria with true, false if doesn't exist
bool usr_qry::getRecentSimilarQueryData(int session_or_day, int uid, int actual_qid, int &time_or_day, bool &URLshown, int &grade_that_time)
{

}