#include <stdio.h>
#include "CPU.h"
#include "Memory.h"
#include "Cartridge.h"
#include "LCD.h"
#include "ASCIIRenderer.h"

CPU cpu;
Memory* mem;
LCD* lcd;
uint8_t* cart;

int main(){
    
    mem = Memory_create();

    cart = Cartridge_load("roms/cpu_instrs/cpu_instrs.gb");
    if(cart == NULL){
        printf("Error loading cartridge!\n");
        return -1;
    }

    mem->ROM_bank_0 = cart;
    mem->ROM_bank_n = cart + 0x4000;
    mem->external_RAM = cart + 0xA000;
    mem->cart = cart;

    lcd = (LCD*) mem->VRAM;

    CPU_reset(&cpu);

    while(!cpu.stopped){
        printf("PC: 0x%X\n", cpu.PC);
        CPU_process_instruction(&cpu, mem);
        render(lcd);
    }

    return 0;
}
