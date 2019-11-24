#include <iostream>
#include <vector>
#include "ReservationStation.h"
#include "Instruction.h"
#include <fstream>
#include <array>
#include "Reg_Rat.h"
#include "Executor.h"
const int ADD_SUB_RS = 3;
const int MUL_DIV_RS = 2;

//Latencies for different stages
const int ISSUE_LAT = 1;
const int BROADCAST_LAT = 1;
//Reservation Station Latency for different opcodes at execution
const int ADD_SUB_LAT = 2;
const int MULT_LAT = 10;
const int DIV_LAT = 40;
// Opcode Values
const int AddOp = 0;
const int SubOp = 1;
const int MultOp = 2;
const int DivOp = 3;

const int OPERAND_READY = -1;
const int OPERAND_EMPTY = -2;
const int CLEAR_OPERAND = -3;


const int NUM_REG = 8;
//Used for bounds of the Reservation stations
struct RS_DIVIDER {
	int ADD_START = 0;
	int ADD_END = ADD_SUB_RS;
	int MUL_START = ADD_SUB_RS;
	int MUL_END = MUL_DIV_RS + ADD_SUB_RS;
}RS_Bounds;

int Issue(std::vector<Instruction> & instructions, 
	std::array<ReservationStation, MUL_DIV_RS + ADD_SUB_RS> & res_unit,
	std::array <Reg_Rat, NUM_REG> & registers,
	int instr_counter);
void Dispatch(std::array<ReservationStation, MUL_DIV_RS + ADD_SUB_RS> & res_unit,
	std::array <Reg_Rat, NUM_REG> & registers,
	std::array <Executor, 2> & executors);
void Broadcast(std::array<ReservationStation, MUL_DIV_RS + ADD_SUB_RS> & res_unit,
	std::array <Reg_Rat, NUM_REG> & registers,
	std::array <Executor, 2> & executors);
void printRegister(std::array <Reg_Rat, NUM_REG> & registers);
void printReservationStations(std::array<ReservationStation, MUL_DIV_RS + ADD_SUB_RS> & res_unit);

std::string Opcode(int op) {
	switch (op)
	{
	case AddOp:
		return "Add";
		break;
	case SubOp:
		return "Sub";
		break;
	case MultOp:
		return "MUL";
		break;
	case DivOp:
		return "DIV";
		break;
	default:
		return "";
		break;
	}
}


int main(int argc, char* argv[])
{
	
	/*
	Parse arguments to ensure program will work correctly.  Number of processes is parsed later.
	*/
	if (argc == 1) {
		std::cout << "Missing arguments <file>\n./ECE474_Project2.exe <file>\n" << std::endl;
		return 1;
	}
	else if (argc >= 3) {
		std::cout << "Something wrong with arguments\n./main <file>\n" << std::endl;
		return 1;
	}


	std::ifstream instr_file;
	instr_file.open(argv[1], std::ios_base::in);
	if (!instr_file) {
		std::cerr << "Unable to open file";
		exit(1);   // call system to stop
	}
	int amount;
	int num_cycles;
	instr_file >> amount;
	const int instr_amount = amount;
	instr_file >> num_cycles;
	Executor adder = Executor();
	Executor mult = Executor();
	std::array <Executor, 2> executors = { adder, mult };
	// Load all instructions into memory
	std::vector<Instruction> instructions;
	for (int i = 0; i < instr_amount; ++i) {
		int Op, Rd, Rs, Rt;
		instr_file >> Op >> Rd >> Rs >> Rt;
		instructions.push_back(Instruction(Op, Rd, Rs, Rt));
	}
	// load all register values into memory
	std::array <Reg_Rat, NUM_REG> registers;
	for (int i = 0; i < NUM_REG; ++i) {
		int value;
		instr_file >> value;
		registers[i] = Reg_Rat(value, "R" + std::to_string(i));
	}

	instr_file.close();

	std::cout << "Instructions:" << std::endl;
	// Print Instruction queue
	for (int i = 0; i < instructions.size(); ++i) {
		std::string op = Opcode(instructions[i].op);
		if (instructions[i].op == -1)
			std::cout << "EMPTY" << std::endl;
		else
			std::cout << op << " R" << instructions[i].rd << ", R" << instructions[i].rs << ", R" << instructions[i].rt << std::endl;
	}

	//debugging
	/*std::cout << "\tRF\t\tRAT" << std::endl;
	for (int i = 0; i < NUM_REG; ++i) {
		std::cout << "R" << i << ":\t" << registers[i].value << "\t\t" << registers[i].Name_Resolver() << std::endl;
	}*/

	std::array<ReservationStation, ADD_SUB_RS + MUL_DIV_RS> reservation_unit;
	//Create Reservation Unit of reservation stations
	bool no_instr = false; 
	for (int i = 0; i < ADD_SUB_RS+MUL_DIV_RS; ++i) {
		reservation_unit[i] = ReservationStation(i);
	}

	int instr_counter = 0;
	for (int i = 1; i <= num_cycles; ++i) {
		instr_counter = Issue(instructions, reservation_unit, registers, instr_counter);
		Dispatch(reservation_unit, registers, executors);
		Broadcast(reservation_unit, registers, executors);
	}
	std::cout << std::endl;
	std::cout << "Cycle: " << num_cycles << "\n" << std::endl;
	printRegister(registers);
	std::cout << std::endl;
	printReservationStations(reservation_unit);
	std::cout << "\nInstruction Queue" << std::endl;
	for (int i = 0; i < instructions.size(); ++i) {
		std::string op = Opcode(instructions[i].op);
		if (instructions[i].op == -1)
			std::cout << "EMPTY" << std::endl;
		else
			std::cout << op << " R" << instructions[i].rd << ", R" << instructions[i].rs << ", R" << instructions[i].rt << std::endl;
	}

}
/*
return values of instr_counter meaning
-1:  instr_counter has completed all instructions
instr_counter: waiting for RS to become free or has incremented by 1

Pass all Instructions, may convert from vector to queue.
*/
int Issue(std::vector<Instruction> & instructions,
	std::array<ReservationStation, MUL_DIV_RS + ADD_SUB_RS> & res_unit,
	std::array <Reg_Rat, NUM_REG> & registers, int instr_counter) {
	if (instructions.size() <= instr_counter) {
		return -1;
	}
	
	if (instr_counter == -1) {
		return -1;
	}
	bool RS_free = false;
	int opcode = instructions[instr_counter].op;
	int rs_slot;
	//If add or sub loop through looking for free RS.
	if (opcode == 0 || opcode == 1) {
		for (int i = RS_Bounds.ADD_START; i < RS_Bounds.ADD_END; ++i) {
			if (!res_unit[i].busy) {
				instructions[instr_counter].op = -1;
				rs_slot = i;
				++instr_counter;
				res_unit[i].op = opcode; //Will assign either add or sub
				RS_free = true;
				break;
			}
		}
		if (!RS_free) {
			return instr_counter;
		}
	}
	//If Mul or div loop through looking for free RS.
	else if (opcode == 2 || opcode == 3) {
		for (int i = RS_Bounds.MUL_START; i < RS_Bounds.MUL_END; i++) {
			if (!res_unit[i].busy) {
				instructions[instr_counter].op = -1;
				rs_slot = i;
				++instr_counter;
				res_unit[i].op = opcode; //Will assign either Mul or Div
				RS_free = true;
				break;
			}
		}
		if (!RS_free) {
			return instr_counter;
		}
	}
	//Implement logic from page 180
	// Tackle RS 
	Instruction instr = instructions[instr_counter - 1];
	if (registers[instr.rs].rat == OPERAND_READY) {
		res_unit[rs_slot].value1 = registers[instr.rs].value;
		res_unit[rs_slot].tag1 = OPERAND_READY;
		res_unit[rs_slot].reg_loc = instr.rd;
	}
	else {
		res_unit[rs_slot].tag1 = registers[instr.rs].rat;
		res_unit[rs_slot].reg_loc = instr.rd;
	}

	//Tackle Rd
	if (registers[instr.rt].rat == OPERAND_READY) {
		res_unit[rs_slot].value2 = registers[instr.rt].value;
		res_unit[rs_slot].tag2 = OPERAND_READY;
		res_unit[rs_slot].reg_loc = instr.rd;
	}
	else {
		res_unit[rs_slot].tag2 = registers[instr.rt].rat;
		res_unit[rs_slot].reg_loc = instr.rd;
	}

	res_unit[rs_slot].busy = true;
	res_unit[rs_slot].issue_lat = 0;
	registers[instr.rd].rat = rs_slot;
	return instr_counter;

}
/*
Future improvement take in only one RS per type of operation opposed to checking them all
*/
void Dispatch(std::array<ReservationStation, MUL_DIV_RS + ADD_SUB_RS> & res_unit,
	std::array <Reg_Rat, NUM_REG> & registers, std::array <Executor, 2> & executors) {
	
	if (!executors[0].busy) {
		for (int i = RS_Bounds.ADD_START; i < RS_Bounds.ADD_END; ++i) {
			if (res_unit[i].Check_Dispatch_Ready(ISSUE_LAT)) {
				res_unit[i].disp = "1";
				executors[0].Prime_Executor(res_unit[i]);
				break;
			}
		}

	}
	else {
		executors[0].Execute();
	}
	if (!executors[1].busy) {
		for (int i = RS_Bounds.MUL_START; i < RS_Bounds.MUL_END; ++i) {
			if (res_unit[i].Check_Dispatch_Ready(ISSUE_LAT)) {
				res_unit[i].disp = "1";
				executors[1].Prime_Executor(res_unit[i]);
				break;
			}
		}
	}
	else {
		executors[1].Execute();
	}
	for (int i = RS_Bounds.ADD_START; i < RS_Bounds.MUL_END; ++i) {
		if (!res_unit[i].Check_Dispatch_Ready(ISSUE_LAT)) {
			res_unit[i].issue_lat++;
		}
	}
}
/*
Check the executors and if mult/div executor is done first it will broadcast first.
Stuff done:
Update register
update RAT
update reservation stations if matched
clear reservation station and executor
*/
void Broadcast(std::array<ReservationStation, MUL_DIV_RS + ADD_SUB_RS> & res_unit,
	std::array <Reg_Rat, NUM_REG> & registers,
	std::array <Executor, 2> & executors) {
	/*
	Check Mult and Div executor first so it can broadcast if ready
	*/
	if (executors[1].Broadcast_Ready(BROADCAST_LAT)) {
		int reg_index = executors[1].dest_tag;
		int executor_from_tag = executors[1].from_tag;
		int executor_result = executors[1].result;
		res_unit[executor_from_tag].ClearResrvStat(); //clear RS and executor for next RS dispatch
		executors[1].Reset();
		//See if another RS is ready to be dispatched
		for (int i = RS_Bounds.MUL_START; i < RS_Bounds.MUL_END; ++i) {
			if (res_unit[i].Check_Dispatch_Ready(ISSUE_LAT)) {
				res_unit[i].disp = "1";
				executors[1].Prime_Executor(res_unit[i]);
				break;
			}
		}
		if (registers[reg_index].rat == executor_from_tag) {
			registers[reg_index].Set_New(executor_result);
		}
		//Broadcast value to RSs
		for (int i = 0; i < ADD_SUB_RS + MUL_DIV_RS; ++i) {
			res_unit[i].Recieve_Broadcast(executor_from_tag, executor_result);
		}
		
		return ;
	}

	if (executors[0].Broadcast_Ready(BROADCAST_LAT)) {
		int reg_index = executors[0].dest_tag;
		int executor_from_tag = executors[0].from_tag;
		int executor_result = executors[0].result;
		res_unit[executor_from_tag].ClearResrvStat(); //clear RS and executor for next RS
		executors[0].Reset();
		//See if another RS is ready to be dispatched
		for (int i = RS_Bounds.ADD_START; i < RS_Bounds.ADD_END; ++i) {
			if (res_unit[i].Check_Dispatch_Ready(ISSUE_LAT)) {
				res_unit[i].disp = "1";
				executors[0].Prime_Executor(res_unit[i]);
				break;
			}
		}
		if (registers[reg_index].rat == executor_from_tag) {
			registers[reg_index].Set_New(executor_result);
		}
		//Broadcast value to RSs
		for (int i = 0; i < ADD_SUB_RS + MUL_DIV_RS; ++i) {
			res_unit[i].Recieve_Broadcast(executor_from_tag, executor_result);
		}
		res_unit[executor_from_tag].ClearResrvStat();
		
		return;
	}

}

//Print registers and Rat
void printRegister(std::array <Reg_Rat, NUM_REG> & registers) {
	std::cout << "Register Status: " << std::endl;
	std::cout << "RF\t\tRAT" << std::endl;
	for (int i = 0; i < registers.size(); ++i) {
		std::cout << registers[i].name << ": ";
		std::cout << registers[i].value << "\t\t";
		std::cout << registers[i].Name_Resolver() << std::endl;
	}
}
//Print reservation stations
void printReservationStations(std::array<ReservationStation, MUL_DIV_RS + ADD_SUB_RS> & res_unit) {
	std::cout << "Reservation Stations:" << std::endl;
	std::cout << "RS\tOP\tTag1\tTag2\tValue1\tValue2\tBusy\tDisp" << std::endl;
	for (int i = 0; i < res_unit.size(); ++i) {
		//Print nothing if RS unit is not busy
		if (res_unit[i].op == -1) {
			std::cout << "RS" << i + 1 << ":\t" << Opcode(res_unit[i].op) << "\t" <<
				"" << "\t" << "" << "\t" <<
				"" << "\t" << "" << "\t" <<
				res_unit[i].busy << "\t" << res_unit[i].disp << std::endl;
		}
		//Print info if RS is busy
		else {
			std::cout << "RS" << i + 1 << ":\t" << Opcode(res_unit[i].op) << "\t" <<
				res_unit[i].Print_Tag(1) << "\t" << res_unit[i].Print_Tag(2) << "\t" <<
				res_unit[i].value1 << "\t" << res_unit[i].value2 << "\t" <<
				res_unit[i].busy << "\t" << res_unit[i].disp << std::endl;
		}
		
	}
}


