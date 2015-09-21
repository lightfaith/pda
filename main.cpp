#include<stdio.h>
#include<vector>
#include "instructionset.hpp"
#include "log.hpp"
#include "program.hpp"
//#include "lib.hpp"

InstructionSet *is;
Program *p;

void PrintUsage();

int main(int argc, char** argv)
{
	Log::Init();
	is = new InstructionSet();
	
	//no parameters or help
	if(argc<2 || argc==2 && (strcmp(argv[1],"--help")==0 || strcmp(argv[1], "-h")==0))
	{
		PrintUsage();
		exit(0);
	}
	//info
	else if(strcmp(argv[1],"info")==0)
		if(argc<3)
			is->Print(Platform::Type::x86);
		else
			is->Print(Platform::Type::x86, argv[2]);
	//decodestr
	else if (strcmp(argv[1], "decodestr")==0)
	{
		if(argc<3)
		{
			PrintUsage();
			exit(1);
		}
		//const char* testchar="\x50\x57\x53\x51\x56\xb0\x46\x55\x52\x54";
		//const char* testchar="\x89\xe3\x89\xdc\x89\xe3";
		//const char* testchar="\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60";
		//const char* testchar="\x68\x2f\x2f\x73\x68\x50\x50\x51\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80";
		//const char* testchar="\x23\x41\x0c\x22\x41\x0c\x21\x41\x0c\x20\x41\x0c";
		//const char* testchar="\x21\x45\xf0\x23\x45\x08\x20\x45\xf0\x22\x45\x08";
		//p = new Program(Platform::Type::x86, testchar, HEX_STRING, is);
		p = new Program(Platform::Type::x86, argv[2], HEX_STRING, is);
		
		p->Convert(INSTRUCTION);
		p->Print();
	}
	else if (strcmp(argv[1], "decodefile")==0)
	{
		if(argc<3)
		{
			PrintUsage();
			exit(1);
		}
		p = new Program(Platform::Type::x86, argv[2], HEX_FILE, is); 
		p->Convert(INSTRUCTION);
		p->Print();
	}
	
	// no valid parameters - last resort
	else
		PrintUsage();
	return 0;
}

void PrintUsage()
{
	printf(" __________________________________________________________\n");
	printf("|                                                          |\n");
	printf("| Primo DisAssembler v%-36s |\n", Lib::VERSION); 
	printf("| Help:                      pda                           |\n");
	printf("|                            pda -h                        |\n");
	printf("|                            pda --help                    |\n");
	printf("| DB search:                 pda info <opcode|instruction> |\n");
	printf("| Decode opcode string:      pda decodestr <string>        |\n");
	printf("| Decode opcode file:        pda decodefile <path>         |\n");
	printf("| Encode instruction string: pda codestr <string>          |\n");
	printf("| Encode instruction file:   pda codefile <path>           |\n");
	printf("|__________________________________________________________|\n");
}
