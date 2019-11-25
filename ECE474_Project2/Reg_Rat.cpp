#include "Reg_Rat.h"
#include <string>
Reg_Rat::Reg_Rat() {
	busy = false;
	rat = -1;
}
Reg_Rat::Reg_Rat(int val, std::string df_name) {
	busy = false;
	rat = -1;
	value = val;
	name = df_name;
}

std::string  Reg_Rat::Name_Resolver()
{
	if (rat == -1) {
		return name;
	}
	else {
		return "ROB" + std::to_string(rat+1); // since array starts at 0; +1 to = RS1-5
	}
}

void Reg_Rat::Set_New(int new_value, int rob_index)
{
	if (rat == rob_index) {
		rat = -1;
		value = new_value;
	}
	else {
		value = new_value;
	}
	
}
