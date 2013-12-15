#include "stdafx.h"
#include "DataCollector.h"
#include "variables.h"
#include <sstream>

DataCollector::DataCollector() // phase I : provide path to train file, phase II : provide path to test file
{
	buffSize = BUFF_SIZE_INPUT_READ;
	buffer = (char *)malloc(buffSize);
}

void DataCollector::parseFile(char *path)
{
	fp = fopen(path, "r");
	int nRead;
	while((nRead = fread(buffer, 1, buffSize, fp)) > 0)
	{
		printf("Read a chunk of file\n");
		if(nRead == buffSize)
		{
			int movedBack = 0;
			while(buffer[nRead-1] != '\n')
			{
				nRead--;
				movedBack++;
			}
			fseek(fp, -movedBack, SEEK_CUR);
		}
		buffer[nRead] = 0;
		// do operation on buffer
		istringstream buff_in(buffer);
		string rowInLog;
			
		//temp
		int temp_sid, temp_day, temp_time, temp_uid, temp_serp, temp_qid, temp_term, temp_url, temp_domain;
		string temp_typeOrTime;
			
		while(getline(buff_in, rowInLog))
		{
			istringstream sin(rowInLog);
			sin>>temp_sid>>temp_typeOrTime;
			if(temp_typeOrTime == "M")
			{
				sin>>temp_day>>temp_uid;
				// sid, day, uid
				continue;
			}
			temp_time = atoi(temp_typeOrTime.c_str());
			sin>>temp_typeOrTime;
			if(temp_typeOrTime == "Q"
		}
	}
	wrapUp();
}

void DataCollector::wrapUp()
{
	fclose(fp);
	free(buffer);
	printf("data populated\n");	
}