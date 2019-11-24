#include "Rob.h"
#include "Rob_Entry.h"
#include <array>
#include "Reg_Rat.h"
Rob::Rob(int rob_entries)
{
	num_rob = rob_entries;
	for (int i = 1; i <= rob_entries; ++i) {
		rob.push_back(i);
	}
	commit = 0;
	tail = 0;
	num_entries = 0;
}

int Rob::Reserve_Rob(int reg)
{
	if (num_entries == num_rob)
		return -1;

	if (num_entries == 0) {
		rob[tail].Reserve_Rob(reg);
		num_entries++;
		return tail;
	}
	else if(tail == num_rob-1){
		if (commit == 0) {
			return tail;
		}
		tail = 0;
		rob[tail].Reserve_Rob(reg);
		num_entries++;
		return tail;
	}
	else if (tail + 1 != commit) {
		rob[++tail].Reserve_Rob(reg);
		num_entries++;
		return tail;
	}
	return -1;
}

bool Rob::Update_Rob(int rob_index, int value)
{
	rob[rob_index].Update_Rob(value);
	return true;
}

bool Rob::Exception_Update(int rob_index)
{
	rob[rob_index].Exception_Update();
	return true;
}

bool Rob::Rob_Empty()
{
	if (num_entries == 0)
		return true;
	else
		return false;
}

bool Rob::Rob_Full()
{
	if (num_entries == num_rob)
		return true;
	else
		return false;
}


Commit_Tag Rob::Commit()
{
	Commit_Tag commit_msg;

	if (num_entries == 0)
		throw "ROB is empty";
	
	if (rob[commit].latency == 0)
		return;
	
	if (num_entries == 1) {
		commit_msg.reg_num = rob[commit].reg_num;
		commit_msg.value = rob[commit].value;
		rob[commit].Clear_Rob_Entry();
		num_entries--;
	}

	else {
		commit_msg.reg_num = rob[commit].reg_num;
		commit_msg.value = rob[commit].value;
		rob[commit].Clear_Rob_Entry();
		if (commit == num_rob - 1) {
			commit = 0;
		}
		else {
			commit++;
		}
		num_entries--;
	}
	
	return commit_msg;
}

bool Rob::Check_Exception()
{
	return rob[commit].Check_Exception();
}

void Rob::Handle_Exception()
{
	for (int i = 0; i < num_rob; ++i) {
		rob[i].Clear_Rob_Entry();
	}
}

void Rob::Update_Latency()
{
	if (num_entries == 0)
		return;

	else if (commit == tail) {
		rob[commit].latency++;
	}

	else if (commit > tail) {
		for (int i = commit; i <= num_rob; ++i) {
			rob[i].latency++;
		}
		for (int i = 0; i <= tail; ++i) {
			rob[i].latency++;
		}
	}

	else if (commit < tail) {
		for (int i = commit; i <= tail; ++i) {
			rob[i].latency++;
		}
	}
}

bool Rob::Commit_Ready()
{
	if (rob[commit].latency == 0)
		return false;
	else
		return true;
}

