#ifndef PDA_PROGRAM_H
#define PDA_PROGRAM_H
#include "block.hpp"
#include "instruction.hpp"
#include "lib.hpp"


using namespace std;

enum ProgramType { HEX_STRING, INSTR_STRING, HEX_FILE, INSTR_FILE };
static const char* ProgramTypeNames[]={"Hex format as string", "Instructions as string", "Hex format from file", "Instructions from file"};

// Special top-level Block
class Program : public Block
{
	private:
		ProgramType type;																						//
		string form;																							//
		void ConvertTox86Instruction();																			//
		InstructionSet *is;																						//
	public:
		Program(const Platform::Type platform, const string str, const ProgramType type, InstructionSet* is);	//
		void Convert(const Element desired);																	//
		void Print();																							//
		void Print(const Element desired);																		//

};
#endif
