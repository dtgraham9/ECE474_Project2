#include "Rob_Entry.h"
//Assignes a ROB number to it
Rob_Entry::Rob_Entry(int index)
{
	value = 0;
	reg_num = -1;
	exception = 0;
	rob_num = index;
	done = false;
	latency = 0;

}
//Check if an error occured
bool Rob_Entry::Check_Exception()
{
	if (exception == 1)
		return true;
	else
		return false;
}
//clear ROB 
void Rob_Entry::Clear_Rob_Entry()
{
	reg_num = -1;
	value = 0;
	done = false;
	exception = 0;
	latency = 0;
}
//Assign a Register for ROB to write back to
bool Rob_Entry::Reserve_Rob(int reg)
{
	reg_num = reg;
	return true;
}
//Give ROB (assumed to already be assigned a register) an updated value from executor
void Rob_Entry::Update_Rob(int new_value)
{
	value = new_value;
	done = true;
	exception = 0;
}
//If the executor reports an execption occurred.  
void Rob_Entry::Exception_Update()
{
	exception = 1;
	done = true;
}

