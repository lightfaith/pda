#include "lib.hpp"

using namespace std;

vector<string> &split(const string &s, char delim, vector<string> &elems)
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

string lower(string a)
{
	string result;
	for(int i=0; i<a.size(); i++)
		result+=tolower(a[i]);
	return result;
}

string upper(string a)
{
	string result;
	for(int i=0; i<a.size(); i++)
		result+=toupper(a[i]);
	return result;
}

char* loadfromfile(const char* path)
{
	FILE *f = fopen(path, "r");
	int len=0;
	if(f==NULL)
		Log::Error("Cannot load file %s! #Lib", path);
	fseek(f, 0, SEEK_END);
	len=ftell(f);
	fseek(f, 0, SEEK_SET);
	char * result = (char*)malloc(len+1);
	char c;
	for(int i=0; i< len; i++)
	{
		c = fgetc(f);
		result[i]=c;
	}
	result[len]=0;
	fclose(f);
	return result;
		
}

char* getpath()
{
    char *buf = (char*)malloc(Lib::MAXFILENAMELEN*sizeof(char));
	readlink("/proc/self/exe", buf, Lib::MAXFILENAMELEN);
	//just use directory
	for(int i=strlen(buf); i>=1; i--)
		if(buf[i-1]=='/')
		{
			buf[i]='\0';
			break;
		}
	return buf;
}

char* getfilename(const char* file)
{
    char* abs = (char*)malloc(Lib::MAXFILENAMELEN*sizeof(char));
	snprintf(abs, Lib::MAXFILENAMELEN, "%s/%s", getpath(), file); //todo check
	return abs;
}
