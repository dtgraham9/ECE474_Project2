#pragma once
#include <string>
class Instruction
{
public:
	int rd; 
	int rs;
	int rt;
	int op; 
public:
	Instruction();
	Instruction(int, int, int, int);
};

