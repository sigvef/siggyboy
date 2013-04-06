#include "CPU.h"
#include "Memory.h"
#include "Catridge.h"

CPU cpu;
Memory* mem;
Cartridge* cart;

int main(){
    
    mem = Memory_create();

    cart = Cartridge_load("roms/TEST.GB");

    mem->ROM_bank_0 = cart;

    while(1){
        CPU_process_instruction(cpu, mem);
    }

    return 0;
}
