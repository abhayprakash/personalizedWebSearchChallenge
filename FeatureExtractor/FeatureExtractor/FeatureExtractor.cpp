// FeatureExtractor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdio>
#include <cstdlib>

#define SIZE 17*1024*1024*1024ull

int _tmain(int argc, _TCHAR* argv[])
{
	char *buff = (char*) malloc(SIZE);
	printf("allocated\n");
	int a;
	scanf("%d", &a);
	return 0;
}

