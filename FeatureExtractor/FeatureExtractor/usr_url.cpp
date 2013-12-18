#include "stdafx.h"
#include "usr_url.h"

void usr_url::updateTable_Shown(int uid, int urlid, int day)
{
	if(table.find(uid) == table.end() || table[uid].find(urlid) == table[uid].end())
	{
		rec_url temp;
		table[uid][urlid] = temp;
	}
	table[uid][urlid].count_shown++;
	table[uid][urlid].last_day = day;
}

void usr_url::updateTable_Click(int uid, int urlid, int grade)
{
	// shown must have put the entry
	table[uid][urlid].latest_day_grade = grade;
	table[uid][urlid].count_r[grade]++;
}

void usr_url::updateLocal_Shown(int uid, int urlid, int time)
{
	if(local.find(uid) == local.end() || local[uid].find(urlid) == local[uid].end())
	{
		rec_time_grade temp;
		local[uid][urlid] = temp;
	}
	local[uid][urlid].time = time;
}

void usr_url::updateLocal_Click(int uid, int urlid, int grade)
{
	local[uid][urlid].grade = grade;
}

void usr_url::copyLocalToGlobal_and_ClearLocal(int day)
{
	map<int, map<int, rec_time_grade> >::iterator it;
	map<int, rec_time_grade>::iterator it2;
	for(it = local.begin(); it != local.end(); ++it)
	{
		for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			updateTable_Shown(it->first, it2->first, day);
		}
	}
	local.clear();
}

bool usr_url::existsBeforeSession(int uid, int urlid)
{
	if(table.find(uid) == table.end() || table[uid].find(urlid) == table[uid].end())
		return false;
	return true;
}

// for below functions assumed that entry : table[uid][urlid] exists
// so before using them check exists(...) 
int usr_url::getCountShown(int uid, int urlid)
{
	return table[uid][urlid].count_shown;
}

int usr_url::getCountR(int uid, int urlid, int grade)
{
	return table[uid][urlid].count_r[grade];
}

int usr_url::getLatestDayGrade(int uid, int urlid)
{
	return table[uid][urlid].latest_day_grade;
}

int usr_url::getLastDay(int uid, int urlid)
{
	return table[uid][urlid].last_day;
}

// local ds and operations
bool usr_url::existsCurrentSession(int uid, int urlid)
{
	if(local.find(uid) == local.end() || local[uid].find(urlid) == local[uid].end())
		return false;
	return true;
}

int usr_url::getLastTime(int uid, int urlid)
{
	return local[uid][urlid].time;
}

int usr_url::getLatestTimeGrade(int uid, int urlid)
{
	return local[uid][urlid].grade;
}