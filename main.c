#include "CPU.h"
#include "Memory.h"
#include "Cartridge.h"

CPU cpu;
Memory* mem;
uint8_t* cart;

int main(){
    
    mem = Memory_create();

    cart = Cartridge_load("roms/TEST.GB");

    mem->ROM_bank_0 = cart;

    while(1){
        CPU_process_instruction(&cpu, mem);
    }

    return 0;
}
