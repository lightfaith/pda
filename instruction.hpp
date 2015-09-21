#ifndef PDA_INSTRUCTION_H
#define PDA_INSTRUCTION_H
#include "code.hpp"
#include "instructionset.hpp"
#include "log.hpp"

/* Describes exactly one instruction with parameters, both in machine and assembly form. */

class Instruction : public Code 
{
	private:
		const InstructionInfo *info;
		string assform;
		string machform;
		bool hasassform;
		bool hasmachform;
		
	public:
		Instruction(const Platform::Type platform, const string str, const Element desired); //unknown
		Instruction(const Platform::Type platform, const InstructionInfo *ii, const string str, const Element desired); //valid
		void Print();
		void Print(Element desired);
		
		
};
#endif
