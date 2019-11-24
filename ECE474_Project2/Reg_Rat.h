#pragma once
#include <string>
class Reg_Rat
{
public:
	bool busy;
	int value;
	int rat;
	std::string name;
public:
	Reg_Rat();
	Reg_Rat(int val, std::string);
	std::string Name_Resolver();
	void Set_New(int);
};

