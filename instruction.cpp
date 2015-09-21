#include "instruction.hpp"

// Constructor for existing but unknown instructions
Instruction::Instruction(const Platform::Type platform, const string str, const Element desired)
{
	info=NULL;
	assform="N/A";
	machform=machform;
	bool hasassform=true; //although obviously incorrect
	bool hasmachform=true;
	Log::Warn("Unknown instruction %s has occured! #Instruction", machform.c_str());
}

// Constructor for known instructions 
Instruction::Instruction(const Platform::Type platform, const InstructionInfo *ii, const string str, const Element desired)
{
	this->platform=platform;
	info=ii;
	bool hasmodrm=false; //does instruction have modrm parameter? (can operand order change?)
	if(desired==OPCODE)
	{	
		assform=str;
		hasassform=true;
		//TODO encode
	}
	else if(desired==INSTRUCTION)
	{
		machform=str;
		hasmachform=true;
		//TODO decode	

		vector<string> assparams;
		bool validparamtype;
		for(int i=0; i<ii->GetMachParamNum(); i++)
		{
			validparamtype=true;
			Log::Debug(3, "Parsing %s instruction (%d bytes). #Instruction", ii->GetMeaning().c_str(), machform.size());
			Log::Indent();
			Log::Debug(3,  "Trying to decode next machine parameter (%s) #Instruction", ii->GetMachParamTypes()[i].c_str());
			//TODO what about assparamtypes?
			switch(x86::GetParamType(ii->GetMachParamTypes()[i]))
			{
				case x86::IMM8:
				{
					//TODO assuming it is the second byte
					unsigned char value = machform.c_str()[1];
					char hexval[5]={0};
					snprintf(hexval, 5, "0x%02x", value);
					assparams.push_back(hexval);
					break;
				}
				case x86::IMM16:
				{
					//TODO assuming it is the second byte
					unsigned short value=0;
					for(int i=0; i<2; i++)
					{
						value=(value<<8)+machform.c_str()[1+i];
					}
					char hexval[7]={0};
					snprintf(hexval, 7, "0x%04x", value);
					assparams.push_back(hexval);
					break;
				}
				case x86::IMM32:
				{
					//TODO assuming it is the second byte
					unsigned long value=0;
					for(int i=4; i>0; i--)  //LITTLE ENDIAN!!
					{
						value=(value<<8)+machform.c_str()[i];
					}
					char hexval[11]={0};
					snprintf(hexval, 11, "0x%08x", value);
					assparams.push_back(hexval);
					break;
				}
				case x86::MODRM8:
				case x86::MODRM32:
				{
					hasmodrm=true;
					static const char** regarr;
					if(x86::GetParamType(ii->GetMachParamTypes()[i])==x86::MODRM8)
						regarr=x86::Registers8b; //TODO here test for 16b modifier
					else
						regarr=x86::Registers32b;
					//TODO now assuming it is the second byte
					unsigned char modrm=machform.c_str()[1];
					char mod = modrm >> 6; //first 2 bits
					char reg = (modrm >> 3) & 0x7; // following 3 bits
					char rm = modrm & 0x7; //last 3 bits
					assparams.push_back(lower(regarr[reg]));
					Log::Debug(3, "Mod=%x", mod);
					Log::Debug(3, "Reg=%x (%s)", reg, regarr[reg]);
					Log::Debug(3, "R/M=%x (%s)", rm, regarr[rm]);
					switch(mod)
					{
						case 0: // 0b00, address with no offset
						{
							assparams.push_back(string("[")+lower(regarr[rm])+"]");
							break;
						}
						case 1: // 0b01, address with 1B offset
						{
							//TODO assuming it is third byte
							unsigned char value=machform.c_str()[2];
							char sign='+';
							if(value >>7!=0)
							{
								sign='-';
								value=-value;
							}
							char hexval[5]={0}; //TODO \0??
							snprintf(hexval, 5, "0x%02x", value);
							assparams.push_back(string("DWORD PTR [")+lower(regarr[rm])+sign+hexval+"]");
							break;
						}
						case 2: //0x10, address with 4B offset
						{
							//TODO assuming it is third-sixth byte
							unsigned long value=0;
							for(int i=0; i<4; i++)
							{
								value=(value<<8)+machform.c_str()[2+i];
							}
							char sign='+';
							if(value >>31!=0)
							{
								sign='-';
								value=-value;
							}
							char hexval[11]={0}; //TODO \0??
							snprintf(hexval, 11, "0x%08x", value);
							assparams.push_back(string("DWORD PTR [")+lower(regarr[rm])+sign+hexval+"]");
							break;
						}
						case 3: // 0b11, direct
						{
							assparams.push_back(lower(regarr[rm]));
							break;
						}
						default:
						{
							Log::Warn("Invalid MOD (%d) while parsing instruction! #Instruction", mod);
							break;
						}
					} //end of mod switch
					break;
				}
				case x86::OPCODERB:
				{
					unsigned char rb=str.c_str()[0]-ii->GetOpcode()->GetPrimary();
					assparams.push_back(lower(x86::Registers8b[rb])); 
					break;
				}
				case x86::OPCODERD:
				{
					unsigned char rd=str.c_str()[0]-ii->GetOpcode()->GetPrimary();
					assparams.push_back(lower(x86::Registers32b[rd])); 
					break;
				}
				default:
				{
					validparamtype=false;
					Log::Warn("Attempt to decode instruction %s with unknown type! #Instruction", ii->GetMeaning().c_str());
				}
			}//end of paramtype switch
			if(assparams.size()>0 && validparamtype) //no invalid mode, can show new parameter in debug
			{
				Log::Indent();
				Log::Debug(3, "Parameter decoded: %s #Instruction", assparams[assparams.size()-1].c_str());
				Log::Deindent();
			}
			Log::Deindent();
		} //all parameters loaded
		// determine operand order
		switch(assparams.size())
		{
			case 0:
			{
				Log::Debug(4, "Instruction has 0 parameters.");
				/*if(ii->GetAssParamNum()==1) // one assembly operand, but none loaded? This must be proprietary for register!
					assform=lower(ii->GetMeaning()+" "+lower(x86::Registers32b[ii->GetOpcode()->GetPrimary()&0x07]));
				else*/
					assform=lower(ii->GetMeaning());
				break;
			}
			case 1:
			{
				if(ii->GetAssParamNum()==2) // two assembly operands, but only one loaded? This must be AL/AX/EAX/RAX proprietary!
				{
					if(ii->SBitZero())
						assform=lower(ii->GetMeaning())+" al, "+assparams[0];
					else //TODO 16b check
						assform=lower(ii->GetMeaning())+" eax, "+assparams[0];
					//TODO 64b eventually
					Log::Debug(4, "Instruction has 1 parameter, but should have 2. Treated as AL/EAX proprietary.");
				}
				else
				{
					Log::Debug(4, "Instruction has 1 parameter.");
					assform=lower(ii->GetMeaning())+" "+assparams[0];
				}
				break;
			}
			case 2:
			{
				if(hasmodrm) //TODO only changeable order if modrm?	
				{
					if(ii->DBitZero()) 
						assform=lower(ii->GetMeaning())+" "+assparams[1]+", "+assparams[0];
				}
				else
					assform=lower(ii->GetMeaning())+" "+assparams[0]+", "+assparams[1];
				Log::Debug(4, "Instruction has 2 parameters.");
				break;
			}
			default:
			{
				Log::Warn("Weird number of assembly parameters for instruction %s! #Instruction", ii->GetMeaning().c_str());
				break;
			}
		} //end of assparams.size() switch
		hasassform=true;
		Log::Debug(3, "Instruction in assembly form: '%s' #Instruction", assform.c_str());
	}
}

// Ordinary printing (machine form)
void Instruction::Print()
{
	Print(BOTH);
}

// Printing with form choosing
void Instruction::Print(const Element desired)
{
	if(desired==OPCODE && hasmachform)
	{
		for(int i=0; i<machform.size(); i++)
			printf("%02x ", machform.c_str()[i]&255); //print one byte of instruction
		printf("\n");
	}
	else if(desired==INSTRUCTION && hasassform)
	{
		printf("%s\n", assform.c_str());
	}
	else if(desired==BOTH && hasassform && hasmachform)
	{
		string hex;
		for(int i=0; i<machform.size(); i++)
			printf("%02x ", machform.c_str()[i]&255); //print one byte of instruction
		printf("%*s", 22-(machform.size()*3), "");
		printf("%s\n", assform.c_str());	
		//printf("\n");
	}
	else
	{
		Log::Warn("Attempt to print undefined instruction %s! #Instruction", machform.c_str());
	}
}
