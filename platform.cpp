#include "platform.hpp"

using namespace std;

x86::ParamType x86::GetParamType(const string type)
{
	if(type.compare("imm8")==0)
		return x86::IMM8;
	if(type.compare("imm16")==0)
		return x86::IMM16;	
	if(type.compare("imm32")==0)
		return x86::IMM32;
	if(type.compare("moffs8")==0)
		return x86::MOFFS8;
	if(type.compare("moffs32")==0)
		return x86::MOFFS32;
	if(type.compare("modrm8")==0)
		return x86::MODRM8;		
	if(type.compare("modrm32")==0)
		return x86::MODRM32;
	if(type.compare("opcode+rb")==0)
		return x86::OPCODERB;
	if(type.compare("opcode+rd")==0)
		return x86::OPCODERD;
}
