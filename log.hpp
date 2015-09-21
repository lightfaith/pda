#ifndef PDA_LOG_H
#define PDA_LOG_H
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<stdarg.h>
#include<sys/stat.h>
#include<sys/types.h>
#include "lib.hpp"

/* Class for log creating. */

using namespace std;

enum LogLevel {DEBUG, INFO, WARN, ERROR};

class Log
{
	private:
		static const char* Path;										// Folder with log files
		static const char* Files[];										// Log filenames
		static const char Chars[];										// Prompt characters
		static unsigned int indent;										// Actual indent level
		static void Add(int type, const char* format, va_list args);	// New entry
	public:

		static void Init();												// Init function (file creation etc.)
		
		static void Indent();
		static void Deindent();
		static void ZeroIndent();
		static void Debug(int level, const char* format, ...);
		static void Info(const char* format, ...);
		static void Warn(const char* format, ...);
		static void Error(const char* format, ...);
};


#endif
