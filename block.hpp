#ifndef PDA_BLOCK_H
#define PDA_BLOCK_H
#include "code.hpp"

/* Describes list of instructions or inner blocks. */

class Block : public Code 
{
	protected:
		vector<Code*> code;
	public:
		
};
#endif
