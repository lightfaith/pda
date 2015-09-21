#include "instructioninfo.hpp"

using namespace std;

// Constructor for empty instruction info
InstructionInfo::InstructionInfo()
{
	platform=Platform::Type::NULLPlatform;
	opcode=new Opcode(platform, 0, -1);
	assparamnum=0;
	machparamnum=0;
	this->meaning = "Empty function";
	anycomment=false;
	anyflag=false;
}

/*
InstructionInfo::InstructionInfo(int pl, unsigned long op, unsigned char pn, string meaning)
{
	platform=pl;
	opcode=op;
	paramnum=pn;
	this->meaning = meaning;
	anycomment=false;
	anyflag=false;
}
*/

// Complete constructor
//InstructionInfo::InstructionInfo(int pl, unsigned long op, unsigned char pn, string meaning, vector<string> comments, vector<string> flags)
InstructionInfo::InstructionInfo(Platform::Type pl, Opcode *opcode, string meaning, unsigned char apn, vector<string> comments, unsigned char mpn, vector<string> mpt, vector<string> flags)
{
	platform=pl;
	this->opcode=opcode;
	assparamnum=apn;
	machparamnum=mpn;
	machparamtypes=mpt;
	this->meaning = meaning;
	this->comments=comments;
	this->flags=flags;
	anycomment=false;
	anyflag=false;
	for(int i=0; i<comments.size(); i++)
		if(comments[i].size()>0)
		{
			anycomment=true;
			break;
		}
	for(int i=0; i<flags.size(); i++)
		if(flags[i].size()>0)
		{
			anyflag=true;
			break;
		}
}

/*
void InstructionInfo::AddComment(int comid, string comment)
{
	if(comid<paramnum+1)
	{
		comments[comid]=comment;
		if(comments[comid].size()>0)
			anycomment=true;
	}
	
}
*/

// Is here any comment?
bool InstructionInfo::AnyComment() const
{
	return anycomment;
}

// Is here any flag modification?
bool InstructionInfo::AnyFlag() const
{
	return anyflag;
}

// Meaning getter
string InstructionInfo::GetMeaning() const
{
	return meaning;
}

// Print function
void InstructionInfo::Print()
{
	printf("---\n");
	//BASIC INFO
	opcode->Print();
	printf("\n");
	//printf("Platform: %s\n", Platform::Names[platform]);
	//printf("No. of parameters: %d\n",  paramnum);
	printf("Meaning: %s\n", meaning.c_str());
	
	
	//COMMENTS
	if(AnyComment())
	{
		printf("Comments:\n");
		for(int i=0; i<comments.size(); i++)
		{
			printf("  %d) %s\n", i, comments[i].c_str());
		}
	}
	
	//MACHINE STUFF
	printf("Machine parameters to load: %d\n", machparamnum);
	for(int i=0; i<machparamnum; i++)
		printf("  %d: %s\n", i+1, machparamtypes[i].c_str());
	
	//FLAGS
	if(AnyFlag())
	{
		printf("Flags:\n");
		for(int i=0; i<flags.size() && i<sizeof(x86::Flags)/sizeof(x86::Flags[0]); i++)
		{
			if(flags[i].size()>0)
				printf("  %s%s %s\n", x86::Flags[i], ((flags[i].c_str()[0]==' ')?"":":"), flags[i].c_str());
		}
	}
	printf("-------\n");
}

// Returns number of assembly arguments
unsigned char InstructionInfo::GetAssParamNum() const
{
	return assparamnum;
}

// Returns number of machine arguments
unsigned char InstructionInfo::GetMachParamNum() const
{
	return machparamnum;
}

// Returns types of machine arguments
vector<string> InstructionInfo::GetMachParamTypes() const
{
	return machparamtypes;
}

Opcode* InstructionInfo::GetOpcode() const
{
	return opcode;
}

bool InstructionInfo::DBitZero() const
{
	return opcode->DBitZero();
}

bool InstructionInfo::SBitZero() const
{
	return opcode->SBitZero();
}

bool InstructionInfo::IsOpcodeRBD() const
{
	if(GetMachParamNum()>0)
		if(GetMachParamTypes()[0].compare("opcode+rd")==0 || GetMachParamTypes()[0].compare("opcode+rb")==0)
			return true;
	return false;
}
//TODO 0x66 & 0x67 support for number of bytes to return!!!!
