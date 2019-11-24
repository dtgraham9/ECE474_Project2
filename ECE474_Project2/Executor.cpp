#include "Executor.h"
//Reservation Station Latency for different opcodes at execution
//calculate both the result and latency and track to and from tags
void Executor::Prime_Executor(ReservationStation & RS)
{
	const int ADD_SUB_LAT = 2;
	const int MULT_LAT = 10;
	const int DIV_LAT = 40;
	// Opcode Values
	const int AddOp = 0;
	const int SubOp = 1;
	const int MultOp = 2;
	const int DivOp = 3;
	
	busy = true;
	value1 = RS.value1;
	value2 = RS.value2;
	op = RS.op;
	from_tag = RS.index;
	latency = 0;
	dest_tag = RS.reg_loc;
	broadcast_lat = 1;
	internal_result = 0;

	switch (op) {
	case 0: //add
		internal_result = value1 + value2;
		ext_lat = ADD_SUB_LAT;
		break;
	case 1: //sub
		internal_result = value1 - value2;
		ext_lat = ADD_SUB_LAT;
		break;
	case 2: //mult
		internal_result = value1 * value2;
		ext_lat = MULT_LAT;
		break;
	case 3://div
		internal_result = value1 / value2;
		ext_lat = DIV_LAT;
		break;
	default:
		break;
	}

	if (ext_lat - 1 < latency) {
		result_ready = true;
		result = internal_result;
	}
	++latency;
}

Executor::Executor()
{
	busy = false;
	result_ready = false;
	broadcast_lat = 1;
	from_tag = -1;

}
//increment latency
void Executor::Execute()
{
	
	if (ext_lat -1 < latency) {
		result_ready = true;
		result = internal_result;
	}
	++latency;
	
}

void Executor::Reset() {
	op = -1;
	result = -1;
	from_tag = -1;
	result_ready = false;
	latency = 0;
	dest_tag = -1;
	busy = false;
	ext_lat = 0;
	broadcast_lat = 1;
}

bool Executor::Broadcast_Ready(int broadcast)
{
	if(result_ready) {
		return true;
	}
	else {
		return false;
	}
}

bool Executor::Busy()
{
	if (ext_lat - 1 < latency) {
		return true;
	}
	return false;
}
