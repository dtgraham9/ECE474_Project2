#include "Rob.h"
#include "Rob_Entry.h"
#include <array>
#include "Reg_Rat.h"
#include  <iostream>
//Create ROB table
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
//Reserve an open ROB, assumed that it was checked before hand trying to reserve
int Rob::Reserve_Rob(int reg)
{
	//Safety measure
	if (num_entries == num_rob)
		return -1;
	//If ROB is empty
	if (num_entries == 0) {
		rob[tail].Reserve_Rob(reg);
		num_entries++;
		return tail;
	}
	//If ROB's tail is at end and needs to warp around
	else if(tail == num_rob-1){
		//Check if commit head is still at the beginning
		if (commit == 0) {
			return tail;
		}
		
		tail = 0;
		rob[tail].Reserve_Rob(reg);
		num_entries++;
		return tail;
	}
	//Prevent tail from overwritting commit
	else if (tail + 1 != commit) {
		rob[++tail].Reserve_Rob(reg);
		num_entries++;
		return tail;
	}
	return -1;
}
//Update a given reserved ROB with executor value
bool Rob::Update_Rob(int rob_index, int value)
{
	rob[rob_index].Update_Rob(value);
	return true;
}
//Update a given ROB if execption occurred in executor
bool Rob::Exception_Update(int rob_index)
{
	rob[rob_index].Exception_Update();
	return true;
}
//Check if ROB is empty
bool Rob::Rob_Empty()
{
	if (num_entries == 0)
		return true;
	else
		return false;
}
//Check if ROB is full
bool Rob::Rob_Full()
{
	if (num_entries == num_rob)
		return true;
	else
		return false;
}

//Properly change commit pointer, clear commit ROB, and return value and register
//Assumed to be able to commit
Commit_Tag Rob::Commit()
{
	Commit_Tag commit_msg;
	//Should not happen
	if (num_entries == 0)
		throw "ROB is empty"; 
	//should not happen
	if (rob[commit].latency == 0)
		throw "ROB hasn't delayed";
	//Prevent changing commit
	if (num_entries == 1) {
		commit_msg.reg_num = rob[commit].reg_num;
		commit_msg.value = rob[commit].value;
		commit_msg.rob_index = rob[commit].rob_num-1;
		rob[commit].Clear_Rob_Entry();
		num_entries--;
	}
	//update commit when more then one entry is stored
	else {
		commit_msg.reg_num = rob[commit].reg_num;
		commit_msg.value = rob[commit].value;
		commit_msg.rob_index = rob[commit].rob_num-1;
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
//check for commit exception
bool Rob::Check_Exception()
{
	return rob[commit].Check_Exception();
}
//Clear ROB if exception was present
//Assumed that a check was performed
void Rob::Handle_Exception()
{
	for (int i = 0; i < num_rob; ++i) {
		rob[i].Clear_Rob_Entry();
	}
	commit = 0;
	tail = 0;
}
//Commit requires one cycle after broadcast
void Rob::Update_Latency()
{	
	//DO nothing
	if (num_entries == 0)
		return;
	//Only happens if 1 entry is present
	else if (commit == tail) {
		if (rob[commit].done == true)
			rob[commit].latency++;
	}
	//This accounts if tail wrapped around
	else if (commit > tail) {
		for (int i = commit; i < num_rob; ++i) {
			if(rob[i].done == true)
				rob[i].latency++;
		}
		for (int i = 0; i <= tail; ++i) {
			if (rob[i].done == true)
				rob[i].latency++;
		}
	}
	//Tail hasn't wrapped around
	else if (commit < tail) {
		for (int i = commit; i <= tail; ++i) {
			if (rob[i].done == true)
				rob[i].latency++;
		}
	}
}
//Check if ROB can commit
bool Rob::Commit_Ready()
{
	if (rob[commit].latency != 0 && rob[commit].done == true)
		return true;
	else
		return false;
}
//Check if ROB's entry is ready
bool Rob::Check_Entry_Ready(int rob_entry)
{
	return rob[rob_entry].done == 1;
}
//Get the value of a ROB entry
int Rob::Get_Rob_Entry(int rob_entry)
{
	return rob[rob_entry].value;
}



