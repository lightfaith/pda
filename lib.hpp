#ifndef PDA_LIB_H
#define PDA_LIB_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<iostream>
#include<vector>
#include<sstream>
#include<string.h>
#include "log.hpp"

using namespace std;

namespace Lib
{
	//TODO change to appropriate values before release
	static const char* VERSION="0.0";
	static int DEBUGLEVEL=5;
	static int LOGINDENTSTEP=2;
	static int MAXFILENAMELEN=256;

}

vector<string> &split(const string &s, char delim, vector<string> &elems);	// String splitting support method
vector<string> split(const string &s, char delim);							// String splitting method
string lower(string a);														// tolower() function
string upper(string a);														// TOUPPER() FUNCTION
char* loadfromfile(const char* path);										// char* loading from file
char* getpath();
char* getfilename(const char* file);

#endif

