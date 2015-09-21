#include "instructionset.hpp"


using namespace std;

InstructionSet::InstructionSet()
{
	//x86Set.resize(512, new InstructionInfo());
	//ARMSet.resize(512, NULL);
	Load(Platform::Type::x86, x86Set);
	//Log::Add(DEBUG, "ending InstructionSet ctor..");
}

// Loads instructions of a chosen platform from appropriate file
void InstructionSet::Load(const Platform::Type platform, map<Opcode*, InstructionInfo*, OpcodeComparer> &set)
{
	string line;
	
	ifstream infile(Platform::Files[platform]);
	if(!infile)
		Log::Error("Attempt to load instructions from non-existent file %s! #InstructionSet", Platform::Files[platform]);
	
	
	while (getline(infile, line))
	{
    	//istringstream iss(line);
		
		//comment?
		if(line[0]=='#')
			continue;
			
	    vector<string> data = split(line, DELIMITER);
	    
	    unsigned long opcode;
	    long secopcode;
	    unsigned char assparamnum, machparamnum;
	    string meaning;
	    vector<string> comments;
	    vector<string> machparamtypes;
	    vector<string> flags;
		Opcode *op;
		// PARSE LINE (0x000000op;name;assparamnum;comments;machparamnum;machparamtype;c;p;a;z;s;o)
		if(data.size()>=6)
		{
			try
			{
				//opcodes, name, number of assembly parameters, comments for instruction and parameters
				if(data[0].size()==0)
					continue;

				opcode=strtoul(data[0].c_str(), NULL, 16);
				secopcode=(data[1].size()>0) ? strtoul(data[1].c_str(), NULL, 10) : -1;
				op = new Opcode(platform, opcode, secopcode);
				meaning=data[2].c_str();
				assparamnum=atoi(data[3].c_str());
				for(int i=0; i<assparamnum+1 && i<data.size(); i++)
					comments.push_back(data[i+4]);
				//number of machine parameters, types of machine parameters, flags cpazso
				machparamnum=atoi(data[5+assparamnum].c_str());
				for(int i=0; i<machparamnum; i++)
					machparamtypes.push_back(data[i+6+assparamnum]);
				for(int i=6+assparamnum+machparamnum; i<data.size(); i++)
					flags.push_back(data[i]);
					
				
				//set.insert(InstructionEntry(opcode, new InstructionInfo(platform, opcode, paramnum, meaning, comments, flags)));
				set.insert(InstructionEntry(op, new InstructionInfo(platform, op, meaning, assparamnum, comments, machparamnum, machparamtypes, flags)));
			}
			catch(...)
			{
				Log::Warn("invalid instruction entry \"%s\" in file \"%s\"! #InstructionSet", line.c_str(), Platform::Files[platform]);
			}
		}
		else
		{
			if(line.size()!=0)
				Log::Warn("instruction entry \"%s\" in file \"%s\" too short! #InstructionSet", line.c_str(), Platform::Files[platform]);
		}
	}
	Log::Debug(1, "Loaded %d %s instructions. #InstructionSet", set.size(), Platform::Names[platform]);
}

// Print all instructions
void InstructionSet::Print(const Platform::Type platform)
{
	Print(platform, "");
}

// Print instructions with optional Meaning filter
void InstructionSet::Print(const Platform::Type platform, const char* meaning)
{
	map<Opcode*, InstructionInfo*, OpcodeComparer> *set;
	switch(platform)
	{
		case Platform::Type::x86: 
		{
			set=&x86Set; 
			break;
		}
		default:
		{
			Log::Error("Attempt to print instructions from invalid platform! #InstructionSet");
			break;
		}
	}
	if(strcmp(meaning, "")==0)
	{
		for(map<Opcode*, InstructionInfo*>::iterator it = set->begin(); it!=set->end(); ++it)
			it->second->Print();
		Log::Debug(1, "Printed %d %s instructions. #InstructionSet", set->size(), Platform::Names[platform]);
	}
	else
	{
		bool exact=true;	//should be entries shown by exact match?
		for(int i=0; i<strlen(meaning); i++)
			if(isupper(meaning[i])==0)
			{
				exact=false;
				break;
			}
		
	
		int counter=0;
		char* check;
		bool isopcodevalue=true;
		unsigned long opcodevalue=strtoul(meaning, &check, 16);
		if(/*check!=meaning+strlen(meaning)*/*check!='\0')
		{
			isopcodevalue=false;
			Log::Debug(5, "%s is not opcode value", meaning);
		}
		else
			Log::Debug(5, "%s parsed as opcodevalue=0x%02x", meaning, opcodevalue);
			
		for(map<Opcode*, InstructionInfo*>::iterator it = set->begin(); it!=set->end(); ++it)
		{
			//if meaning is in capital the exatch match is shown, else any occurence is displayed
			if(exact && (upper(it->second->GetMeaning()).compare(upper(string(meaning)))==0)) //exact meaning match
			{
				Log::Debug(5, "Got exact match!");
				it->second->Print();
				counter++;
			}
			if(!exact && upper(it->second->GetMeaning()).find(upper(string(meaning)))!=string::npos) //partial meaning  match
			{
				Log::Debug(5, "Got partial match!");
				it->second->Print();
				counter++;
			}
			if(isopcodevalue)
			{
				Log::Debug(5, "Is opcode, opcodevalue=0x%02x, negated= 0x%02x", opcodevalue, opcodevalue&(-0x10));
				if((it->second->IsOpcodeRBD() ? (opcodevalue&(-0x08)) : opcodevalue)==it->first->GetPrimary()) //TODO shouldnt be 0x07????
				/*strcmp(meaning, it->first->GetPrimaryString())==0) || (strlen(meaning)>2 && strcmp(meaning+2, it->first->GetPrimaryString())==0)*/ //opcode
				{
					Log::Debug(5, "Got opcode match, IsOpcodeRBD()=%s", it->second->IsOpcodeRBD() ? "true" : "false");
					it->second->Print();
					counter++;
				}
			}
		}
		Log::Debug(1, "Printed %d/%d %s instructions. #InstructionSet", counter, set->size(), Platform::Names[platform]);
	}
	//for(const InstructionEntry& kv : set)
}

// Opcode filter 
InstructionInfo* InstructionSet::GetInstructionInfo(Platform::Type platform, char opcode)
{
	map<Opcode*, InstructionInfo*, OpcodeComparer> *set;
	//int primarycodecount=0; //counts number of occurences of primary opcode (to warn about secondary)
	InstructionInfo *result; 
	//choose correct set
	switch(platform)
	{
		case Platform::Type::x86: 
		{
			set=&x86Set; 
			break;
		}
		default:
		{
			Log::Error("Attempt to get instructioninfo from invalid platform! #InstructionSet");
			break;
		}
	}
	//return result
	for(map<Opcode*, InstructionInfo*>::iterator it = set->begin(); it!=set->end(); ++it)
	{
		//find first occurence, client will deduce possible duplicities from Opcode::HasSecondaryOpcode()
		//also look for same opcodes with null lsBytes, to detect opcode+rb/d instructions
		//TODO make work with unsigned long!!!!!!!!!!!
		if((it->first->GetPrimary()&255)==(opcode&255))
		{
			return it->second;
		}
		Log::Debug(5, "Opcode: 0x%02x, negated = 0x%02x, compared to 0x%02x #InstructionSet", opcode&255, (opcode&-0x10)&255, it->first->GetPrimary()&255);
		if((it->first->GetPrimary()&255)==(opcode&(-0x08)&255))
		{
			Log::Debug(5, "ALMOST FOUND MATCH!");
			if(it->second->IsOpcodeRBD())
				return it->second;
			Log::Debug(5, "aftermatch!");
		}

	}
	return NULL;
}

