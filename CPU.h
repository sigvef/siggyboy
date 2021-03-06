#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "Memory.h"

typedef struct{


    union {
        uint16_t AF;
        struct{
            union {
                struct {
                    uint8_t bit0 : 1;
                    uint8_t bit1 : 1;
                    uint8_t bit2 : 1;
                    uint8_t bit3 : 1;
                    uint8_t cy   : 1; /* carry flag */
                    uint8_t h    : 1; /* half carry flag */
                    uint8_t n    : 1; /* add/sub flag */
                    uint8_t z    : 1; /* zero flag */
                };
                uint8_t F;
            };
            uint8_t A;
        };
    };

    union {
        uint16_t BC;
        struct {
            uint8_t C;
            uint8_t B;
        };
    };

    union {
        uint16_t DE;
        struct {
            uint8_t E;
            uint8_t D;
        };
    };

    union {
        uint16_t HL;
        struct {
            uint8_t L;
            uint8_t H;
        };
    };

    uint16_t SP;
    uint16_t PC;

    int stopped;

} CPU;

void CPU_reset(CPU*cpu);
void CPU_process_instruction(CPU*cpu, Memory* mem);
uint8_t CPU_op_add(CPU*cpu, uint8_t a, uint8_t b);
void CPU_op_push(CPU*cpu, Memory* mem, uint16_t value);

#endif
