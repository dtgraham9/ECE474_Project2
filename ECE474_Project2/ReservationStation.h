#pragma once
#include <string>
class ReservationStation //Abbrv to RS
{
public:
	std::string name;
	int index;
	bool busy;
	int tag1;
	int tag2;
	int value1;
	int value2;
	int op;
	int result;
	bool result_ready;
	int ext_lat; //Track execution of RS in execute
	int issue_lat;  //track issue latency
	int broadcast_lat; //track broadcast latency
	int reg_loc;
	std::string disp;

	//**** Methods
public:
	ReservationStation();
	ReservationStation(int);
	void ClearResrvStat();
	bool Check_Dispatch_Ready(int);
	void Recieve_Broadcast(int, int);
	std::string Print_Tag(int);
};


