#include "CPU.h"

void CPU_reset(CPU*cpu){
    cpu->AF = 0;
    cpu->BC = 0;
    cpu->DE = 0;
    cpu->HL = 0;
    cpu->SP = 0;
    cpu->PC = 0;
}
