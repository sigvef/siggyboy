#include <stdint.h>
#include <stdlib.h>
#include "Memory.h"

Memory* Memory_create(){

    Memory* mem = (Memory*) malloc(sizeof(Memory));

    /* external memories */
    mem->ROM_bank_0 = NULL;
    mem->ROM_bank_n = NULL;
    mem->external_RAM = NULL;

    /* VIDEO RAM */
    mem->VRAM_banks = (uint8_t**) malloc(sizeof(uint8_t*) * N_SWITCHABLE_VRAM_BANKS);
    for(int i=0;i<N_SWITCHABLE_VRAM_BANKS;i++){
        mem->VRAM_banks[i] = (uint8_t*) malloc(sizeof(uint8_t) * VRAM_BANK_SIZE);
    }
    mem->VRAM = mem->VRAM_banks[0];


    /* WORKING RAM */
    mem->WRAM_banks = (uint8_t**) malloc(sizeof(uint8_t*) * N_SWITCHABLE_WRAM_BANKS);
    for(int i=0;i<N_SWITCHABLE_WRAM_BANKS+1;i++){
        mem->WRAM_banks[i] = (uint8_t*) malloc(sizeof(uint8_t) * WRAM_BANK_SIZE);
    }
    mem->WRAM_bank_0 = mem->WRAM_banks[0];
    mem->WRAM_bank_1 = mem->WRAM_banks[1];
    mem->ECHO        = mem->WRAM_bank_0;

    /* OTHER STUFF */
    mem->OAM         = (uint8_t*) malloc(sizeof(uint8_t) * IO_PORTS_SIZE);
    mem->IO_ports    = (uint8_t*) malloc(sizeof(uint8_t) * IO_PORTS_SIZE);
    mem->HRAM        = (uint8_t*) malloc(sizeof(uint8_t) * HRAM_SIZE);
    mem->IER = 0;

    return mem;
}


void Memory_read_16(Memory* mem, uint16_t location){
    /*
    uint16_t data = mem[location++];
    data = (data << 8) | mem[location];
    return data; 
    */
}
