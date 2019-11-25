#pragma once
#include "Rob_Entry.h"
#include <vector>

typedef struct {
	int reg_num;
	int value;
	int rob_index;
} Commit_Tag;

class Rob
{
private:
	int num_rob;
public:
	int commit;
	int tail;
	int num_entries;
	std::vector<Rob_Entry> rob;
public:
	Rob(int);
	int Reserve_Rob(int);
	bool Update_Rob(int, int);
	bool Exception_Update(int);
	bool Rob_Empty();
	bool Rob_Full();
	Commit_Tag Commit();
	bool Check_Exception();
	void Handle_Exception();
	void Update_Latency();
	bool Commit_Ready();
};

