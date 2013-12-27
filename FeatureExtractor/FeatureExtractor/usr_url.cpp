#include "stdafx.h"
#include "usr_url.h"

void usr_url::updateTable_Shown(int uid, int urlid, int day)
{
	/*
	if(table.find(uid) == table.end() || table[uid].find(urlid) == table[uid].end())
	{
		rec_url temp;
		table[uid][urlid] = temp;
	}
	*/
	table[uid][urlid].count_shown++;
	table[uid][urlid].last_time_day = day;
}

void usr_url::updateTable_Click(int uid, int urlid, int grade)
{
	// shown must have put the entry
	table[uid][urlid].latest_grade = grade;
	table[uid][urlid].count_r[grade]++;
}

void usr_url::updateLocal_Shown(int urlid, int time)
{
	/*
	if(local.find(uid) == local.end() || local[uid].find(urlid) == local[uid].end())
	{
		rec_url temp;
		local[uid][urlid] = temp;
	}
	*/
	local[urlid].last_time_day = time;
	local[urlid].count_shown++;
	local[urlid].latest_grade = UNCLICKED_CLASS;
}

void usr_url::updateLocal_Click(int urlid, int grade)
{
	local[urlid].latest_grade = grade;
	local[urlid].count_r[grade]++;
}

void usr_url::copyLocalToGlobal_and_ClearLocal(int day, int uid)
{
	map<int, rec_url>::iterator it;

	for(it = local.begin(); it != local.end(); ++it)
	{
		updateTable_Shown(uid, it->first, day);
	}
	local.clear();
}

bool usr_url::existsBeforeSession(int uid, int urlid)
{
	if(table[uid].find(urlid) == table[uid].end())
		return false;
	return true;
}

// for below functions assumed that entry : table[uid][urlid] exists
// so before using them check exists(...) 
int usr_url::getCountShown(int uid, int urlid)
{
	return table[uid][urlid].count_shown + local[urlid].count_shown;
}

void usr_url::getCountR(int uid, int urlid, int obtained[3])
{
	obtained[0] = table[uid][urlid].count_r[0] + local[urlid].count_r[0];
	obtained[1] = table[uid][urlid].count_r[1] + local[urlid].count_r[1];
	obtained[2] = table[uid][urlid].count_r[2] + local[urlid].count_r[2];
}

int usr_url::getLatestDayGrade(int uid, int urlid)
{
	return table[uid][urlid].latest_grade;
}

int usr_url::getLastDay(int uid, int urlid)
{
	return table[uid][urlid].last_time_day;
}

// local ds and operations
bool usr_url::existsCurrentSession(int urlid)
{
	if(local.find(urlid) == local.end())
		return false;
	return true;
}

int usr_url::getLastTime(int urlid)
{
	return local[urlid].last_time_day;
}

int usr_url::getLatestTimeGrade(int urlid)
{
	return local[urlid].latest_grade;
}