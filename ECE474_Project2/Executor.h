#pragma once
#include "ReservationStation.h"
class Executor
{
public:
	int value1;
	int value2;
	int op;
	int result;
	int from_tag; //index for RS
	int result_ready;
	int latency;
	int dest_tag; //index for register
	bool busy;
	int ext_lat;
	int broadcast_lat;
	int internal_result;
public:
	void Prime_Executor(ReservationStation &);
	Executor();
	void Execute();
	void Reset();
	bool Broadcast_Ready(int);
	bool Busy();
};

