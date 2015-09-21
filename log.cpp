#include "log.hpp"

using namespace std;

const char* Log::Files[] = {"logs/debug.txt", "logs/info.txt", "logs/warn.txt", "logs/error.txt"};
const char Log::Chars[] = {'D', 'I', 'W', 'E'};
const char* Log::Path="logs";
unsigned int Log::indent;

void Log::Init()
{
	Log::indent=0;
	mkdir(Log::Path, 0640);
	for(int i=0; i<sizeof(Files)/sizeof(Files[0]); i++)
	{
		FILE *f = fopen(Files[i], "w"); //TODO TEST
		fclose(f); //TODO TEST
	}
}

void Log::Add(int type, const char* format, va_list args)
{
	FILE *f = fopen(Log::Files[type], "a"); //TODO TEST
	
	//va_list args;
	//va_start(args, format);
	fprintf(f, "%c>%*s", Log::Chars[type], indent, "");
	vfprintf(f, format, args);
	fprintf(f, "\n");
	//va_end(args);
	fclose(f); //TODO TEST
}

void Log::Indent()
{
	if(Log::indent<50)
		Log::indent+=Lib::LOGINDENTSTEP;
}

void Log::Deindent()
{
	if(Log::indent>0)
		Log::indent-=Lib::LOGINDENTSTEP;
}

void Log::ZeroIndent()
{
	Log::indent=0;
}

void Log::Debug(int level, const char* format, ...)
{
	if(level>Lib::DEBUGLEVEL)
		return;
	va_list args;
	va_start(args, format);
	Log::Add(DEBUG, format, args);
	va_end(args);
}

void Log::Info(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	Log::Add(INFO, format, args);
	va_end(args);
}

void Log::Warn(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	Log::Add(WARN, format, args);
	va_end(args);
}

void Log::Error(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	Log::Add(ERROR, format, args);
	va_end(args);
	// Terminate program in this case
	printf("Application is suffering from an Exception. See logs/error.txt for more details.");
	exit(1);
}
