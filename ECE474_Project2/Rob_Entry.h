#pragma once
class Rob_Entry
{
public:
	int reg_num;
	int value;
	bool done;
	int exception;
	int rob_num;
	int latency;
public:
	Rob_Entry(int);
	bool Check_Exception();
	void Clear_Rob_Entry();
	bool Reserve_Rob(int);
	void Update_Rob(int);
	void Exception_Update();
};

