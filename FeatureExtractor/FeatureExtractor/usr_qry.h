#ifndef USR_QRY_H_INCLUDED
#define USR_QRY_H_INCLUDED

#include "variables.h"
#include <map>
#include <vector>
#include <list>

using namespace std;

/*
{similar_query_in_same_session(bool), time_diff_from_most_recent, this_url_shown(bool), grade_that_time},
{similar_query_before_current_session(bool), day_difference_from_most_recent, this_url_shown(bool), grade_that_time},
*/

// DS for getting similar query for particular user before session
class usr_qry{
	typedef map<int, int> map_urlGrade; // url -> grade // all 10 urls so at shown time make it -1
	struct q_rec{
		list<int>::iterator pointerToQID;
		int last_time_day;
		map_urlGrade urlLastGrade; 
	};
	
	struct query_list{
		list<int> queries; // q1 q2 q3 ... : arrange in the way that most recent is in front
		map<int, q_rec> queryMetadata; // queryId -> Query's metadata
	};
	map<int, query_list> user_queries; // contains all the queries of the user before session

	query_list local_user_queries; // contains the queries of the user in current session
	/*
	for each session
	-> for each query
		-> get global data
		-> get session data
		-> update local shown links, clicked links
	-> copy local to global
	*/
	void updateShown_userQueries(int uid, int qid, int urlid, int day);
	void updateClicked_userQueries(int uid, int qid, int urlid, int grade);
public:
	void updateShown_local(int qid, int urlid, int time);
	void updateClicked_local(int qid, int urlid, int grade);
	void copyLocalToGlobal_and_ClearLocal(int day, int uid);
	bool getRecentSimilarQueryData(int session_or_day, int uid, int actual_qid, int urlid, int &time_or_day, bool &URLshown, int &grade_that_time); // it will return the data for nearest qid qualifying similarity criteria with true, false if doesn't exist
	int getOldestQueryDay(int uid);
};

#endif