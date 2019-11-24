#include "Rob_Entry.h"

Rob_Entry::Rob_Entry(int index)
{
	value = 0;
	reg_num = 0;
	exception = 0;
	rob_num = index;
	done = false;
	latency = 0;

}

bool Rob_Entry::Check_Exception()
{
	if (exception == 1)
		return true;
	else
		return false;
}

void Rob_Entry::Clear_Rob_Entry()
{
	reg_num = 0;
	value = 0;
	done = false;
	exception = 0;
	latency = 0;
}

bool Rob_Entry::Reserve_Rob(int reg)
{
	reg_num = reg;
	return true;
}

void Rob_Entry::Update_Rob(int new_value)
{
	value = new_value;
	done = true;
	exception = 0;
}

void Rob_Entry::Exception_Update()
{
	exception = 1;
	done = true;
}
