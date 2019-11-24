#include "Instruction.h"

Instruction::Instruction() {
	rd = 0;
	rs = 0;
	rt = 0;
	op = 0;

}

Instruction::Instruction(int OP, int RD, int RS, int RT) {
	rd = RD;
	rs = RS;
	rt = RT;
	op = OP;
}
