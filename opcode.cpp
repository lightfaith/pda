#include "opcode.hpp"

Opcode::Opcode(Platform::Type platform, unsigned long primary, long secondary/*, char opcoderbd*/)
{
	this->platform=platform;
	this->primary=primary;
	//this->primaryreal=primary+opcoderbd;
	this->secondary=secondary;
	snprintf(primaryhex, 8, "%x", primary);
	//snprintf(primaryrealhex, 8, "%x", primaryreal);
}

void Opcode::Print() const
{
	printf("0x%0*x ", (strlen(primaryhex)+1)&-2, primary);
	if(secondary!=-1)
		printf(" /%u", secondary);
}

unsigned long Opcode::GetPrimary() const 
{
	return primary;
}

/*unsigned long Opcode::GetPrimaryReal() const 
{
	return primaryreal;
}*/

const char* Opcode::GetPrimaryString() const
{
	return primaryhex;
}

/*const char* Opcode::GetPrimaryRealString() const
{
	return primaryrealhex;
}*/

long Opcode::GetSecondary() const 
{
	return secondary;
}

bool Opcode::DBitZero() const
{
	Log::Debug(3, "DBitQuery of instruction %02x = %d #Opcode", primary, primary&0x02);
	return ((primary&0x02)==0);
}

bool Opcode::SBitZero() const
{
	Log::Debug(3, "SBitQuery of instruction %02x = %d #Opcode", primary, primary&0x01);
	return ((primary&0x01)==0);
}
