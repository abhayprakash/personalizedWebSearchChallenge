#include "stdafx.h"
#include "urs_url.h"

void usr_url::updateShown(int uid, int urlid, int day)
{
	if(table.find(uid) == table.end() || table[uid].find(urlid) == table[uid].end())
	{
		rec temp;
		table[uid][urlid] = temp;
	}
	table[uid][urlid].count_shown++;
	table[uid][urlid].last_day = day;
}

void usr_url::updateClick(int uid, int urlid, int grade)
{
	// shown must have put the entry
	table[uid][urlid].latest_grade = grade;
	table[uid][urlid].count_r[grade]++;
}

bool usr_url::exists(int uid, int urlid)
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

int usr_url::getLatestGrade(int uid, int urlid)
{
	return table[uid][urlid].latest_grade;
}

int usr_url::getLastDay(int uid, int urlid)
{
	return table[uid][urlid].last_day;
}