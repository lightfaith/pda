#ifndef PDA_CODE_H
#define PDA_CODE_H
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<iostream>
#include "platform.hpp"
#include "log.hpp"
#include "instructionset.hpp"

/* Generic class describing instruction or block determined by specific platform. */

class Code
{
	protected:
		Platform::Type platform;
	public:
		virtual void Print()=0;
		virtual void Print(const Element desired)=0;
};
#endif
