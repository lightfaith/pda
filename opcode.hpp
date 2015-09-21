#ifndef PDA_OPCODE_H
#define PDA_OPCODE_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "platform.hpp"
#include "log.hpp"
class Opcode
{
	private:
		Platform::Type platform;
		unsigned long primary;
		long secondary;
		//unsigned long primaryreal;
		char primaryhex[8];
		//char primaryrealhex[8];
	public:
		Opcode(Platform::Type platform, unsigned long primary, long secondary/*, char opcoderbd=0*/);
		void Print() const;
		unsigned long GetPrimary() const;
		//unsigned long GetPrimaryReal() const;										// also considers opcode+rb/d instruction opcodes
		const char* GetPrimaryString() const;
		//const char* GetPrimaryRealString() const;
		long GetSecondary() const;
		bool DBitZero() const;
		bool SBitZero() const;
};


struct OpcodeComparer
{
	bool operator()(const Opcode* a, const Opcode* b) const 
	{
		return (b->GetPrimary()==a->GetPrimary()) ? (b->GetSecondary()>a->GetSecondary()) : (b->GetPrimary()>a->GetPrimary());
	}
};
#endif

