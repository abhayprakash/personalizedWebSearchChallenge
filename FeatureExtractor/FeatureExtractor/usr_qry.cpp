#include "stdafx.h"
#include "usr_qry.h"


int usr_qry::getRecentSimilarQuery(int qid)
{
	if(SIMILAR_INDEX_THRESH_FOR_QUERY == 100)
		return qid;
	// to implement below

}
	
void usr_qry::update(int uid, int qid, int mg, int lg, int ld)
{
	temp.max_grade = mg;
	temp.latest_grade = lg;
	temp.last_day = ld;

	table_usr_qry[uid][qid] = temp;
}

void usr_qry::updateDayGrade(int uid, int qid, int day, int grade)
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

bool usr_qry::exists(int uid, int qid)
{
	if(table_usr_qry.find(uid) == table_usr_qry.end() || table_usr_qry[uid].find(qid) == table_usr_qry[uid].end())
		return false;
	return true;
}

int usr_qry::getMaxGrade_forSimilar(int uid, int qid)
{
	int recent_similar_query = getRecentSimilarQuery(qid);
	return table_usr_qry[uid][recent_similar_query].max_grade;
}

int usr_qry::getLatestGrade_forSimilar(int uid, int qid)
{
	int recent_similar_query = getRecentSimilarQuery(qid);
	return table_usr_qry[uid][recent_similar_query].latest_grade;
}

int usr_qry::getLastDay_forSimilar(int uid, int qid)
{
	int recent_similar_query = getRecentSimilarQuery(qid);
	return table_usr_qry[uid][recent_similar_query].last_day;
}