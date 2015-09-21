#ifndef PDA_INSTRUCTIONSET_H
#define PDA_INSTRUCTIONSET_H

#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<map>
#include<sstream>
#include<string>
#include<fstream>
#include<string.h>

#include "instructioninfo.hpp"
#include "lib.hpp"
#include "log.hpp"
#include "opcode.hpp"

#define DELIMITER ';'

/* Class describing lists of known instructions by platform. */

using namespace std;

typedef pair<Opcode*, InstructionInfo*> InstructionEntry;
class InstructionSet
{
	private:
		void Load(const Platform::Type platform, map<Opcode*, InstructionInfo*, OpcodeComparer> &set);
	public:
		map<Opcode*, InstructionInfo*, OpcodeComparer> x86Set;
		map<Opcode*, InstructionInfo*, OpcodeComparer> ARMSet;
		InstructionSet();
		void Print(const Platform::Type platform);
		void Print(const Platform::Type platform, const char* meaning);
		InstructionInfo* GetInstructionInfo(Platform::Type platform, char opcode); //program loads by bytes
	
};

#endif
