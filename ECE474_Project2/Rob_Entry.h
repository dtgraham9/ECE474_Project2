#pragma once
class Rob_Entry
{
public:
	int reg_num;
	int value;
	bool done;
	int exception;
	int rob_num;
public:
	Rob_Entry(int);
	bool Check_Exception();
};

