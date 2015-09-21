#include "program.hpp"

using namespace std;

Program::Program(const Platform::Type platform, const string str, const ProgramType type, InstructionSet* is)
{
	this->type=type;
	this->platform=platform;
	this->is=is;
	
	if(type == HEX_STRING || type == INSTR_STRING)
	{
		form=str;
		Log::Info("Loading %s: #Program\n %s\n ", ProgramTypeNames[type], form.c_str());
	}
	else
	{
		form=loadfromfile(str.c_str());
		Log::Info("Loading %s (%s): #Program\n %s\n", ProgramTypeNames[type], str.c_str(), form.c_str());
	}
	
}

void Program::Convert(const Element desired)
{
	code.clear();
	switch(platform)
	{
		case Platform::Type::x86:
		{
			if(desired==OPCODE)
			{
			
			}
			else if (desired==INSTRUCTION)
			{
				ConvertTox86Instruction();
			}
			break;
		}
		case Platform::Type::ARM:
		{
			//TODO arm encode/decode
			break;
		}
		default:
		{
			Log::Warn("Attempt to convert program of invalid platform %d! #Program", platform);
			break;
		}
	} // end of platform switch
}

void Program::Print()
{
	Print(BOTH);
}

void Program::Print(const Element desired)
{
	Log::Debug(1, "Printing program code (%d instructions)! #Program", code.size());
	Log::Indent();
	for(int i=0; i<code.size(); i++)
		code[i]->Print(desired);
	Log::Deindent();
}

void Program::ConvertTox86Instruction()
{
	const char* cform=form.c_str(); //char* from of the whole code
	unsigned char opcode;			//1st byte of every instruction
	unsigned long bytesloaded=0;	
	unsigned long bytestoload;		//how many bytes should be loaded for actual instruction
	string actualinstruction;		//actual instruction bytes to store
	Instruction *in;				//actual instruction object to store
	InstructionInfo *ii;			//info about actual instruction

	Log::Debug(1, "--Converting %dB program! #Program", form.size());
	//Log::Add(DEBUG, "---Converting %dB program! #Program", form.size());
	Log::Indent();
	while(bytesloaded<form.size())
	{
		opcode=cform[bytesloaded];
		bytesloaded++;
		
		actualinstruction.push_back(opcode);
		
		ii = is->GetInstructionInfo(platform, opcode);
		if(ii==NULL)
		{
			in = new Instruction(platform, actualinstruction, INSTRUCTION); //undefined
			Log::Warn("Undefined function (0x%02x) has been found. #Program", opcode);
		}
		else
		{
			Log::Debug(3, "Found Instruction %s (now at byte %d)! #Program", ii->GetMeaning().c_str(), bytesloaded);
			//TODO remember the 0x66 instruction when determining bytes to load!
			bytestoload=0;
			Log::Indent();
			//determine number of bytes to load
			Log::Debug(3, "Number of machine parameters: %d #Program", ii->GetMachParamNum());
			Log::Indent();
			for(int i=0; i<ii->GetMachParamNum(); i++)
			{
				Log::Debug(3, "Param %d: %s #Program", i, ii->GetMachParamTypes()[i].c_str());
				switch(x86::GetParamType(ii->GetMachParamTypes()[i]))
				{
					case x86::IMM8:
					{
						bytestoload+=1; 
						break;
					}
					case x86::IMM16:
					{
						bytestoload+=2; 
						break;
					}
					case x86::IMM32:
					{
						bytestoload+=4; 
						break;
					}
					case x86::MOFFS8:
					{
						bytestoload+=1; 
						break;
					}
					case x86::MOFFS32:
					{
						bytestoload+=4; 
						break;
					}
					case x86::MODRM8:
					case x86::MODRM32:
					{
						bytestoload+=1;
						//peek at next 2 bits
						char peeker=(cform[bytesloaded] >>6) & 0x3; // 1100 0000
						Log::Debug(4, "Byte=0x%02x, Peeker=%d #Program", cform[bytesloaded], peeker);
						if(peeker==0) //esp problem //TODO any ebp problem?
						{
							peeker=cform[bytesloaded+1]&7;
							if(peeker==4) //ESP there, space for 24!
								bytestoload+=1;
						
						}
						else if(peeker==1) //01, one byte addressing
							bytestoload+=1;
						else if(peeker==2) //10, four byte addressing
							bytestoload+=4;
						break;
					}
					case x86::OPCODERB:
					case x86::OPCODERD: {break;}
					default:
					{
						Log::Warn("Attempt to parse unknown machine parameter type %s for %s! #Program", ii->GetMachParamTypes()[i].c_str(), ii->GetMeaning().c_str());
						break;
					}
				}
			}
			Log::Deindent();
			//load additional bytes
			if(bytesloaded+bytestoload>form.size()) //TODO check off by one
				Log::Error("Attempt to read missing bytes (loaded: %d, toload: %d, size: %d)! #Program", bytesloaded, bytestoload, form.size());
			for(int i=0; i<bytestoload; i++)
				actualinstruction.push_back(cform[bytesloaded++]);
			in = new Instruction(platform, ii, actualinstruction, INSTRUCTION); //valid instruction
			Log::Debug(4, "actualinstruction=%s, bytestoload=%d #Program", actualinstruction.c_str(), bytestoload);
			Log::Deindent();
		}
		code.push_back(in);
		actualinstruction.clear();
		//Log::Debug("FORM SIZE: %d", form.size());
	}
	Log::Deindent();
	//all instructions has been loaded
	Log::Debug(1, "--Conversion complete! #Program");
}
