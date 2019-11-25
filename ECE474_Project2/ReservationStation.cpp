#include "ReservationStation.h"
static const int OPERAND_READY = -1;
static const int OPERAND_EMPTY = -2;
static const int CLEAR_OPERAND = -3;
void ReservationStation::ClearResrvStat()
{
	busy = false;
	result_ready = false;
	tag1 = OPERAND_EMPTY;
	tag2 = OPERAND_EMPTY;
	issue_lat = 0;
	ext_lat = 0;
	broadcast_lat = 0;
	disp = 0;
	op = -1;
}

ReservationStation::ReservationStation(){
    busy = false;
    op = 0;
    result = 0;
    result_ready = false;
    tag1 = OPERAND_EMPTY;
    tag2 = OPERAND_EMPTY;
    value1 = 0;
    value2 = 0;
    issue_lat = 0;
	ext_lat = 0;
    broadcast_lat= 0;
	disp = 0;
}

//Constructor for the reservation station
ReservationStation::ReservationStation(int index_value)
{
	disp = 0;
	index = index_value;
	busy = false;
	op = -1;
	result = 0;
	result_ready = false;
	tag1 = OPERAND_EMPTY;
	tag2 = OPERAND_EMPTY;
	value1 = 0;
	value2 = 0;
	issue_lat = 0;
	ext_lat = 0;
	broadcast_lat = 0;
	name = "RS" + std::to_string(index_value + 1);
}

bool ReservationStation::Check_Dispatch_Ready(int issue_latency) {

	return tag1 == OPERAND_READY && tag2 == OPERAND_READY && busy && issue_lat >= issue_latency;
}

void ReservationStation::Recieve_Broadcast(int tag, int value)
{
	if (tag1 == tag) {
		value1 = value;
		tag1 = OPERAND_READY;
	}
	if (tag2 == tag) {
		value2 = value;
		tag2 = OPERAND_READY;
	}
}
//If tags are empty or ready return empty string or return RS#
std::string ReservationStation::Print_Tag(int tag)
{
	if (tag == 1) {
		if (tag1 == OPERAND_READY || tag1 == OPERAND_EMPTY) {
			return "";
		}
		else {
			return "ROB" + std::to_string(tag1 + 1);
		}
	}
	if (tag == 2) {
		if (tag2 == OPERAND_READY || tag2 == OPERAND_EMPTY) {
			return "";
		}
		else {
			return "ROB" + std::to_string(tag2 + 1);
		}
	}
	return "";
}
