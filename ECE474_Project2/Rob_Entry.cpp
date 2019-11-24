#include "Rob_Entry.h"

Rob_Entry::Rob_Entry(int index)
{
	value = 0;
	reg_num = 0;
	exception = 0;
	rob_num = index;

}

bool Rob_Entry::Check_Exception()
{
	if (exception == 1)
		return true;
	else
		return false;
}
