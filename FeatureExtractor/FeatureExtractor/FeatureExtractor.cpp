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

#include <map>
#include <vector>
#include "DataCollector.h"
#include "variables.h"
#include "Processor.h"
#include "usr_url.h"
#include "usr_qry.h"
#include "queryTerm_Logger.h"

using namespace std;

int main()
{
	DataCollector dc;
	usr_url store_usrURL;
	usr_qry store_usrQry;
	Processor P;
	
	dc.parseFile(TRAIN_FILE);
	P.processTrain(dc_train, store_usrURL, store_usrQry);
	dc.wrapUp();
	dc.parseFile(TEST_FILE);
	dc_test.parseFile();
	P.processTest(dc_test, store_usrURL, store_usrQry);
	
	queryTerm_Logger qt_Logger;
	qt_Logger.logAll();
}