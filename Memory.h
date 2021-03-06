#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define ROM_BANK_SIZE 0x4000
#define VRAM_BANK_SIZE 0x2000
#define WRAM_BANK_SIZE 0x1000
#define OAM_SIZE 0xA0
#define IO_PORTS_SIZE 0x80
#define HRAM_SIZE 0x7F

#define N_SWITCHABLE_WRAM_BANKS 7
#define N_SWITCHABLE_VRAM_BANKS 2

typedef struct {
    uint8_t* ROM_bank_0;
    uint8_t* ROM_bank_n;
    uint8_t* VRAM;
    uint8_t** VRAM_banks;
    uint8_t* external_RAM;
    uint8_t* WRAM_bank_0;
    uint8_t* WRAM_bank_1;
    uint8_t** WRAM_banks;
    uint8_t* ECHO;
    uint8_t* OAM;
    uint8_t* IO_ports;
    uint8_t* HRAM;
    uint8_t* IER;
    uint8_t* cart;
} Memory;

Memory* Memory_create();
uint16_t Memory_read_16(Memory* mem, uint16_t location);
uint8_t Memory_read_8(Memory* mem, uint16_t location);
void Memory_write_16(Memory* mem, uint16_t location, uint16_t value);
void Memory_write_8(Memory* mem, uint16_t location, uint8_t value);
uint8_t* Memory_get_source(Memory*mem, uint16_t* location);

#endif
