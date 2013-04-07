#include <stdint.h>
#include <stdio.h>
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
    mem->IER         = (uint8_t*) malloc(sizeof(uint8_t) * 1);
    mem->cart = NULL;

    return mem;
}


uint16_t Memory_read_16(Memory* mem, uint16_t location){

    //printf("Memory_read_16( mem: %p, location: 0x%X)\n", mem, location);

    uint8_t* source = Memory_get_source(mem, &location);

    uint16_t data = source[location++];
    data = (source[location] << 8) | data;
    return data; 
}


uint8_t Memory_read_8(Memory* mem, uint16_t location){
    
    printf("Memory_read_8( mem: %p, location: 0x%X)\n", mem, location);

    uint8_t* source = Memory_get_source(mem, &location);

    return source[location]; 
}


void Memory_write_16(Memory* mem, uint16_t location, uint16_t value){

    if(location <= 0x1FFF){
        /* enable/disable external ram */
    }else if(location <= 0x3FFF){
        int bank_n = value & 0x1F;
        if(bank_n == 0){
            bank_n = 1;
        }
        mem->ROM_bank_n = &mem->cart[0x4000 + ROM_BANK_SIZE * (bank_n-1)];
    }

    uint8_t* source = Memory_get_source(mem, &location);

    source[location++] = value & 0xFF;
    source[location] = value >> 8;
}


void Memory_write_8(Memory* mem, uint16_t location, uint8_t value){
    if(location <= 0x1FFF){
        /* enable/disable external ram */
    }else if(location <= 0x3FFF){
        int bank_n = value & 0x1F;
        if(bank_n == 0){
            bank_n = 1;
        }
        mem->ROM_bank_n = &mem->cart[0x4000 + ROM_BANK_SIZE * (bank_n-1)];
    }

    uint8_t* source = Memory_get_source(mem, &location);
    source[location] = value;
}


uint8_t* Memory_get_source(Memory*mem, uint16_t* location) {

    uint8_t* source;

    if(*location <= 0x3FFF){
        source = mem->ROM_bank_0; 

    }else if(*location <= 0x7FFF){
        source = mem->ROM_bank_n;
        *location -= 0x4000;

    }else if(*location <= 0x9FFF){
        source = mem->VRAM;
        *location -= 0x8000;

    }else if(*location <= 0xBFFF){
        source = mem->external_RAM;
        *location -= 0xA000;

    }else if(*location <= 0xCFFF){
        source = mem->WRAM_bank_0;
        *location -= 0xC000;

    }else if(*location <= 0xDFFF){
        source = mem->WRAM_bank_1;
        *location -= 0xD000;

    }else if(*location <= 0xFDFF){
        source = mem->ECHO;
        *location -= 0xE000;

    }else if(*location <= 0xFE9F){
        source = mem->OAM;
        *location -= 0xFE00; 

    }else if(*location <= 0xFF7F){
        source = mem->IO_ports;
        *location -= 0xFF00;

    }else if(*location <= 0xFFFE){
        source = mem->HRAM; 
        *location -= 0xFF80;
    }else{
        source = mem->IER;
        *location -= 0xFFFF;
    }

    return source;
}
