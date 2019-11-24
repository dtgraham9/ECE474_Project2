#pragma once

class ReservationStation {
public:
	bool busy;
	int tag1;  //point to other RS station
	int tag2;  //point to other RS station
	int value1; //Operand rs
	int value2;  //Operand rt
	int excute_lat; // time counter for execute stage
	int opcode;  
	int result;
	bool resultReady;
	int issue_lat;  //time to hold in issue
	int broadcast_lat; //time to hold for broadcast

public:
	ReservationStation();
	ReservationStation(int, int);
	ReservationStation(int);
	ClearResrvStat();
};