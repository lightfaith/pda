#ifndef PDA_PLATFORM_H
#define PDA_PLATFORM_H
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<iostream>
#include "lib.hpp"

using namespace std;

namespace Platform
{
	enum Type
	{
		NULLPlatform=-1, x86, ARM
	};

	static const char* Files[]={getfilename("sets/x86.txt"), getfilename("sets/arm.txt")};
	static const char* Names[]={"x86", "ARM"};
}

enum Element { OPCODE, INSTRUCTION, BOTH };

namespace x86
{
	static const char* Flags[]={"CF", "PF", "AF", "ZF", "SF", "OF"};
	static const char* Registers8b[]={"AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"};
	static const char* Registers16b[]={"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"};
	static const char* Registers32b[]={"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
	static const char* Registers64b[]={"RAX", "RCX", "RDX", "RBX", "RSP", "RBP", "RSI", "RDI"};
	static const char* ParamTypes[]={"imm8", "imm16", "imm32", "moffs8", "moffs32", "modrm8", "modrm32", "opcode+rb", "opcode+rd"};
	enum ParamType {IMM8, IMM16, IMM32, MOFFS8, MOFFS32, MODRM8, MODRM32, OPCODERB, OPCODERD};
	ParamType GetParamType(const string type); // translation between ParamTypes[] and ParamType
}


#endif
