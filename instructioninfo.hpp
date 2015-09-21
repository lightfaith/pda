#ifndef PDA_INSTRUCTIONINFO_H
#define PDA_INSTRUCTIONINFO_H
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<iostream>
#include "platform.hpp"
#include "opcode.hpp"


using namespace std;

/*
	This class describes one single instruction (based on platform and opcode. Meaning is the opcoded instruction (e.g. XOR), comments describe opcode meaning and all of its parameters);
*/

class InstructionInfo
{
	private:
		Platform::Type platform;		// platform (just in case)
		Opcode *opcode; 				// instruction primary (1-3bytes) and secondary opcode

		unsigned char assparamnum;		// number of parameters of assembly form
		unsigned char machparamnum;		// number of parameters of machine code form
		string meaning;					// actual name of the instruction
		vector<string> comments;		// comments of the instruction and assembly parameters
		vector<string> machparamtypes;  // types of machine parameters
		vector<string> flags;			// flags commentary
		bool anycomment;				// ---for output only
		bool anyflag;					// ---for output only

	public:
		InstructionInfo();
		//InstructionInfo(int pl, unsigned long op, unsigned char pn, string meaning);
		InstructionInfo(Platform::Type pl, Opcode *opcode, string meaning, unsigned char apn, vector<string> comments, unsigned char mpn, vector<string> mpt, vector<string> flags);
		bool AnyComment() const;
		bool AnyFlag() const;
		//void AddComment(int comid, string comment);
		void Print();
		string GetMeaning() const;
		unsigned char GetAssParamNum() const;
		unsigned char GetMachParamNum() const;
		vector<string> GetMachParamTypes() const;
		Opcode* GetOpcode() const;
		bool DBitZero() const;
		bool SBitZero() const;
		bool IsOpcodeRBD() const;
};
#endif
