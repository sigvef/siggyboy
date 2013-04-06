#include "CPU.h"
#include "Memory.h"


void CPU_reset(CPU*cpu){
    cpu->AF = 0;
    cpu->BC = 0;
    cpu->DE = 0;
    cpu->HL = 0;
    cpu->SP = 0xFFFE;
    cpu->PC = 0x100;

    cpu->stopped = 0;
}


uint8_t CPU_op_add(CPU*cpu, uint8_t a, uint8_t b){
    uint8_t result = a + b;
    cpu->z = result ? 0 : 1;
	cpu->h = (0x0F - (a & 0x0F) < (b & 0x0F)) ? 1 : 0;
    cpu->n = 0;
	cpu->cy = (0xFF - a < b) ? 1 : 0;
    return result;
}


uint8_t CPU_op_sub(CPU*cpu, uint8_t a, uint8_t b){
    uint8_t result = a - b;
    cpu->z = result ? 0 : 1;
	cpu->h = ((a & 0x0F) < (b & 0x0F)) ? 1 : 0;
    cpu->n = 1;
	cpu->cy = a < b ? 1 : 0;
    return result;
}


uint8_t CPU_op_and(CPU*cpu, uint8_t a, uint8_t b){
    uint8_t result = a & b;
    cpu->z =  result ? 0 : 1;
	cpu->h = 1;
    cpu->n = 0;
	cpu->cy = 0;
    return result;
}


uint8_t CPU_op_or(CPU*cpu, uint8_t a, uint8_t b){
    uint8_t result = a | b;
    cpu->z =  result ? 0 : 1;
	cpu->h = 0;
    cpu->n = 0;
	cpu->cy = 0;
    return result;
}


uint8_t CPU_op_xor(CPU*cpu, uint8_t a, uint8_t b){
    uint8_t result = a ^ b;
    cpu->z =  result ? 0 : 1;
	cpu->h = 0;
    cpu->n = 0;
	cpu->cy = 0;
    return result;
}


void CPU_op_cp(CPU*cpu, uint8_t a, uint8_t b){
    uint8_t result = a - b;
    cpu->z =  result ? 0 : 1;
	cpu->h = ((a & 0x0F) < (b & 0x0F)) ? 1 : 0;
    cpu->n = 1;
	cpu->cy = a < b ? 1 : 0;
}


uint8_t CPU_op_inc(CPU*cpu, uint8_t a){
    uint8_t result = a + 1;
    cpu->z = result ? 0 : 1;
    cpu->h = (a & 0x0F) == 0 ? 0 : 1;
    cpu->n = 0;
    return result;
}


uint8_t CPU_op_dec(CPU*cpu, uint8_t a){
    uint8_t result = a - 1;
    cpu->z = result ? 0 : 1;
    cpu->h = (a & 0x0F) == 0 ? 1 : 0;
    cpu->n = 1;
    return result;
}


uint16_t CPU_op_add_16(CPU*cpu, uint16_t a, uint16_t b){
    uint16_t result = a + b;
    cpu->h =  (0x0FFF - (a & 0x0FFF) < (b & 0x0FFF)) ? 1 : 0;
    cpu->n = 0;
    cpu->cy = 0xFFFF - a < b ? 1 : 0;
    return result;
}

void CPU_op_push(CPU*cpu, Memory* mem, uint16_t value){
    uint8_t low, high;
    low = value & 0xFF;
    high = (value >> 8) & 0xFF;
    Memory_write_8(mem, --cpu->SP, low);
    Memory_write_8(mem, --cpu->SP, high);
}

uint16_t CPU_op_pop(CPU*cpu, Memory* mem){
    uint8_t low, high;
    high = Memory_read_8(mem, cpu->SP++);
    low = Memory_read_8(mem, cpu->SP++);
    return (high << 8) | low;
}


/* inspired by https://gbemulator.googlecode.com/svn/trunk/src/core.c */

/* a good resource is page 84- of http://chrisantonellis.com/files/gameboy/gb-programming-manual.pdf */


void CPU_process_instruction(CPU*cpu, Memory* mem){
    int opcode = Memory_read_8(mem, cpu->PC++);
    printf("opcode: 0x%X\n", opcode);
    switch(opcode){
        /* 8 bit loads, immediate to register */
        /* TODO: actually read next byte, increment PC etc */
        case 0x06:
            cpu->B = Memory_read_8(mem, cpu->PC++);
            break;

        case 0x0E:
            cpu->C = Memory_read_8(mem, cpu->PC++);
            break;

        case 0x16:
            cpu->D = Memory_read_8(mem, cpu->PC++);
            break;

        case 0x1E:
            cpu->E = Memory_read_8(mem, cpu->PC++);
            break;

        case 0x26:
            cpu->H = Memory_read_8(mem, cpu->PC++);
            break;

        case 0x2E:
            cpu->L = Memory_read_8(mem, cpu->PC++);
            break;


            /* 8 bit loads, register to register */

        case 0x7F: /* LD A, A */
            break;
        case 0x78: /* LD A, B */
            cpu->A = cpu->B;
            break;
        case 0x79: /* LD A, C */
            cpu->A = cpu->C;
            break;
        case 0x7A: /* LD A, D */
            cpu->A = cpu->D;
            break;
        case 0x7B: /* LD A, E */
            cpu->A = cpu->E;
            break;
        case 0x7C: /* LD A, H */
            cpu->A = cpu->H;
            break;
        case 0x7D: /* LD A, L */
            cpu->A = cpu->L;
            break;
        case 0x7E: /* LD A, (HL) */
            cpu->A = Memory_read_8(mem, cpu->HL);
            break;

        case 0x40:  /* LD B, B */
            break;
        case 0x41:  /* LD B, C */
            cpu->B = cpu->C;
            break;
        case 0x42:  /* LD B, D */
            cpu->B = cpu->D;
            break;
        case 0x43:  /* LD B, E */
            cpu->B = cpu->E;
            break;
        case 0x44:  /* LD B, H */
            cpu->B = cpu->H;
            break;
        case 0x45:  /* LD B, L */
            cpu->B = cpu->L;
            break;
        case 0x46:  /* LD B, (HL) */
            cpu->B = Memory_read_8(mem, cpu->HL);
            break;

        case 0x48:  /* LD C, B */
            cpu->C = cpu->B;
            break;
        case 0x49:  /* LD C, C */
            break;
        case 0x4A:  /* LD C, D */
            cpu->C = cpu->D;
            break;
        case 0x4B:  /* LD C, E */
            cpu->C = cpu->E;
            break;
        case 0x4C:  /* LD C, H */
            cpu->C = cpu->H;
            break;
        case 0x4D:  /* LD C, L */
            cpu->C = cpu->L;
            break;
        case 0x4E:  /* LD C, (HL) */
            cpu->C = Memory_read_8(mem, cpu->HL);
            break;

        case 0x50:  /* LD D, B */
            cpu->D = cpu->B;
            break;
        case 0x51:  /* LD D, C */
            cpu->D = cpu->C;
            break;
        case 0x52:  /* LD D, D */
            break;
        case 0x53:  /* LD D, E */
            cpu->D = cpu->E;
            break;
        case 0x54:  /* LD D, H */
            cpu->D = cpu->H;
            break;
        case 0x55:  /* LD D, L */
            cpu->D = cpu->L;
            break;
        case 0x56:  /* LD D, (HL) */
            cpu->D = Memory_read_8(mem, cpu->HL);
            break;

        case 0x58:  /* LD E, B */
            cpu->E = cpu->B;
            break;
        case 0x59:  /* LD E, C */
            cpu->E = cpu->C;
            break;
        case 0x5A:  /* LD E, D */
            cpu->E = cpu->D;
            break;
        case 0x5B:  /* LD E, E */
            break;
        case 0x5C:  /* LD E, H */
            cpu->E = cpu->H;
            break;
        case 0x5D:  /* LD E, L */
            cpu->E = cpu->L;
            break;
        case 0x5E:  /* LD E, (HL) */
            cpu->E = Memory_read_8(mem, cpu->HL);
            break;

        case 0x60:  /* LD H, B */
            cpu->H = cpu->B;
            break;
        case 0x61:  /* LD H, C */
            cpu->H = cpu->C;
            break;
        case 0x62:  /* LD H, D */
            cpu->H = cpu->D;
            break;
        case 0x63:  /* LD H, E */
            cpu->H = cpu->E;
            break;
        case 0x64:  /* LD H, H */
            break;
        case 0x65:  /* LD H, L */
            cpu->H = cpu->L;
            break;
        case 0x66:  /* LD H, (HL) */
            cpu->H = Memory_read_8(mem, cpu->HL);
            break;

        case 0x68:  /* LD L, B */
            cpu->L = cpu->B;
            break;
        case 0x69:  /* LD L, C */
            cpu->L = cpu->C;
            break;
        case 0x6A:  /* LD L, D */
            cpu->L = cpu->D;
            break;
        case 0x6B:  /* LD L, E */
            cpu->L = cpu->E;
            break;
        case 0x6C:  /* LD L, H */
            cpu->L = cpu->H;
            break;
        case 0x6D:  /* LD L, L */
            break;
        case 0x6E:  /* LD L, (HL) */
            cpu->L = Memory_read_8(mem, cpu->HL);
            break;


            /* 8bit loads: reg -> (HL) */
        case 0x70:  /* LD (HL), B */
            Memory_write_8(mem, cpu->HL, cpu->B);
            break;
        case 0x71:  /* LD (HL), C */
            Memory_write_8(mem, cpu->HL, cpu->C);
            break;
        case 0x72:  /* LD (HL), D */
            Memory_write_8(mem, cpu->HL, cpu->D);
            break;
        case 0x73:  /* LD (HL), E */
            Memory_write_8(mem, cpu->HL, cpu->E);
            break;
        case 0x74:  /* LD (HL), H */
            Memory_write_8(mem, cpu->HL, cpu->H);
            break;
        case 0x75:  /* LD (HL), L */
            Memory_write_8(mem, cpu->HL, cpu->L);
            break;
        case 0x36:  /* LD (HL), n */
            Memory_write_8(mem, cpu->HL, Memory_read_8(mem, cpu->PC++));
            break;
        case 0x0A:  /* LD A, (BC) */
            cpu->A = Memory_read_8(mem, cpu->BC);
            break;
        case 0x1A:  /* LD A, (DE) */
            cpu->A = Memory_read_8(mem, cpu->DE);
            break;
        case 0xFA:  /* LD A, (nn) */
            cpu->A = Memory_read_16(mem, cpu->PC);
            cpu->PC += 2;
            break;
        case 0x3E:  /* LD A, n */
            cpu->A = Memory_read_8(mem, cpu->PC++);
            break;
        case 0x47:  /* LD B, A */
            cpu->B = cpu->A;
            break;
        case 0x4F:  /* LD C, A */
            cpu->C = cpu->A;
            break;
        case 0x57:  /* LD D, A */
            cpu->D = cpu->A;
            break;
        case 0x5F:  /* LD E, A */
            cpu->E = cpu->A;
            break;
        case 0x67:  /* LD H, A */
            cpu->H = cpu->A;
            break;
        case 0x6F:  /* LD L, A */
            cpu->L = cpu->A;
            break;
        case 0x02:  /* LD (BC), A */
            Memory_write_8(mem, cpu->BC, cpu->A);
            break;
        case 0x12:  /* LD (DE), A */
            Memory_write_8(mem, cpu->DE, cpu->A);
            break;
        case 0x77:  /* LD (HL), A */
            Memory_write_8(mem, cpu->HL, cpu->A);
            break;
        case 0xEA:  /* LD (nn), A */
            Memory_write_8(mem, Memory_read_16(mem, cpu->PC), cpu->A);
            cpu->PC += 2;
            break;
        case 0xF2:  /* LD A, (C) */
            cpu->A = Memory_read_8(mem, cpu->C);
            break;
        case 0xE2:  /* LD (C), A */
            Memory_write_8(mem, cpu->C, cpu->A);
            break;


            /* 8bit loads/dec/inc, also called LD A, (HLI) for instance */
        case 0x3A:  /* LDD A, (HL) */
            cpu->A = Memory_read_8(mem, cpu->HL--);
            break;
        case 0x32:  /* LDD (HL), A */
            Memory_write_8(mem, cpu->HL--, cpu->A);
            break;
        case 0x2A:  /* LDI A, (HL) */
            cpu->A = Memory_read_8(mem, cpu->HL++);
            break;
        case 0x22:  /* LDI (HL), A */
            Memory_write_8(mem, cpu->HL++, cpu->A);
            break;
        case 0xE0:  /* LDH (n), A */
            break;
        case 0xF0:  /* LDH A, (n) */
            break;


            /* 16bit loads */
        case 0x01:  /* LD BC, nn */
            cpu->BC = Memory_read_16(mem, cpu->PC);
            cpu->PC += 2;
            break;
        case 0x11:  /* LD DE, nn */
            cpu->DE = Memory_read_16(mem, cpu->PC);
            cpu->PC += 2;
            break;
        case 0x21:  /* LD HL, nn */
            cpu->HL = Memory_read_16(mem, cpu->PC);
            cpu->PC += 2;
            break;
        case 0x31:  /* LD SP, nn */
            cpu->SP = Memory_read_16(mem, cpu->PC);
            cpu->PC += 2;
            break;
        case 0xF9:  /* LD SP, HL */
            cpu->SP = cpu->HL;
            break;
        case 0xF8:  /* LDHL SP, n */
            cpu->HL = cpu->SP + Memory_read_8(mem, cpu->PC++);
            cpu->z = 0;
            cpu->h = 0; /* TODO: h should not always be 0 */
            cpu->n = 0;
            cpu->cy = 0; /* TODO: cy should not always be 0 */
            break;
        case 0x08: // LD (nn), SP
            Memory_write_16(mem, Memory_read_16(mem, cpu->PC), cpu->SP);
            cpu->PC += 2;
            break;


        case 0xF5:	// PUSH AF
            CPU_op_push(cpu, mem, cpu->AF);
            break;
        case 0xC5:	// PUSH BC
            CPU_op_push(cpu, mem, cpu->BC);
            break;
        case 0xD5:	// PUSH DE
            CPU_op_push(cpu, mem, cpu->DE);
            break;
        case 0xE5:	// PUSH HL
            CPU_op_push(cpu, mem, cpu->HL);
            break;
        case 0xF1:	// POP AF
            cpu->AF = CPU_op_pop(cpu, mem);
            break;
        case 0xC1:	// POP BC
            cpu->BC = CPU_op_pop(cpu, mem);
            break;
        case 0xD1:	// POP DE
            cpu->DE = CPU_op_pop(cpu, mem);
            break;
        case 0xE1:	// POP HL
            cpu->HL = CPU_op_pop(cpu, mem);
            break;


        /* ADD */
        case 0x87:	// ADD A, A
            cpu->A = CPU_op_add(cpu, cpu->A, cpu->A);
            break;
        case 0x80:	// ADD A, B
            cpu->A = CPU_op_add(cpu, cpu->A, cpu->B);
            break;
        case 0x81:	// ADD A, C
            cpu->A = CPU_op_add(cpu, cpu->A, cpu->C);
            break;
        case 0x82:	// ADD A, D
            cpu->A = CPU_op_add(cpu, cpu->A, cpu->D);
            break;
        case 0x83:	// ADD A, E
            cpu->A = CPU_op_add(cpu, cpu->A, cpu->E);
            break;
        case 0x84:	// ADD A, H
            cpu->A = CPU_op_add(cpu, cpu->A, cpu->H);
            break;
        case 0x85:	// ADD A, L
            cpu->A = CPU_op_add(cpu, cpu->A, cpu->L);
            break;
        case 0x86:	// ADD A, (HL)
            cpu->A = CPU_op_add(cpu, cpu->A, Memory_read_8(mem, cpu->HL));
            break;
        case 0xC6:	// ADD A, n
            cpu->A = CPU_op_add(cpu, cpu->A, Memory_read_8(mem, cpu->PC++));
            break;


        case 0x8F:	// ADC A, A
            cpu->A = CPU_op_add(cpu, cpu->A+cpu->cy, cpu->A);
            break;
        case 0x88:	// ADC A, B
            cpu->A = CPU_op_add(cpu, cpu->A+cpu->cy, cpu->B);
            break;
        case 0x89:	// ADC A, C
            cpu->A = CPU_op_add(cpu, cpu->A+cpu->cy, cpu->C);
            break;
        case 0x8A:	// ADC A, D
            cpu->A = CPU_op_add(cpu, cpu->A+cpu->cy, cpu->D);
            break;
        case 0x8B:	// ADC A, E
            cpu->A = CPU_op_add(cpu, cpu->A+cpu->cy, cpu->E);
            break;
        case 0x8C:	// ADC A, H
            cpu->A = CPU_op_add(cpu, cpu->A+cpu->cy, cpu->H);
            break;
        case 0x8D:	// ADC A, L
            cpu->A = CPU_op_add(cpu, cpu->A+cpu->cy, cpu->L);
            break;
        case 0x8E:	// ADC A, (HL)
            cpu->A = CPU_op_add(cpu, cpu->A+cpu->cy, Memory_read_8(mem, cpu->HL));
            break;
        case 0xCE:	// ADC A, n
            cpu->A = CPU_op_add(cpu, cpu->A+cpu->cy, Memory_read_8(mem, cpu->PC++));
            break;


        case 0x97:	// SUB A, A
            cpu->A = CPU_op_sub(cpu, cpu->A, cpu->A);
            break;
        case 0x90:	// SUB A, B
            cpu->A = CPU_op_sub(cpu, cpu->A, cpu->B);
            break;
        case 0x91:	// SUB A, C
            cpu->A = CPU_op_sub(cpu, cpu->A, cpu->C);
            break;
        case 0x92:	// SUB A, D
            cpu->A = CPU_op_sub(cpu, cpu->A, cpu->D);
            break;
        case 0x93:	// SUB A, E
            cpu->A = CPU_op_sub(cpu, cpu->A, cpu->E);
            break;
        case 0x94:	// SUB A, H
            cpu->A = CPU_op_sub(cpu, cpu->A, cpu->H);
            break;
        case 0x95:	// SUB A, L
            cpu->A = CPU_op_sub(cpu, cpu->A, cpu->L);
            break;
        case 0x96:	// SUB A, (HL)
            cpu->A = CPU_op_sub(cpu, cpu->A, Memory_read_8(mem, cpu->HL));
            break;
        case 0xD6:	// SUB A, n
            cpu->A = CPU_op_sub(cpu, cpu->A, Memory_read_8(mem, cpu->PC++));
            break;


        case 0x9F:	// SBC A, A
            cpu->A = CPU_op_sub(cpu, cpu->A - cpu->cy, cpu->A);
            break;
        case 0x98:	// SBC A, B
            cpu->A = CPU_op_sub(cpu, cpu->A - cpu->cy, cpu->B);
            break;
        case 0x99:	// SBC A, C
            cpu->A = CPU_op_sub(cpu, cpu->A - cpu->cy, cpu->C);
            break;
        case 0x9A:	// SBC A, D
            cpu->A = CPU_op_sub(cpu, cpu->A - cpu->cy, cpu->D);
            break;
        case 0x9B:	// SBC A, E
            cpu->A = CPU_op_sub(cpu, cpu->A - cpu->cy, cpu->E);
            break;
        case 0x9C:	// SBC A, H
            cpu->A = CPU_op_sub(cpu, cpu->A - cpu->cy, cpu->H);
            break;
        case 0x9D:	// SBC A, L
            cpu->A = CPU_op_sub(cpu, cpu->A - cpu->cy, cpu->L);
            break;
        case 0x9E:	// SBC A, (HL)
            cpu->A = CPU_op_sub(cpu, cpu->A - cpu->cy, Memory_read_8(mem, cpu->HL));
            break;
        case 0xDE:	// SBC A, n
            cpu->A = CPU_op_sub(cpu, cpu->A - cpu->cy, Memory_read_8(mem, cpu->PC++));
            break;


        case 0xA7:	// AND A
            cpu->A = CPU_op_and(cpu, cpu->A, cpu->A);
            break;
        case 0xA0:	// AND B
            cpu->A = CPU_op_and(cpu, cpu->A, cpu->B);
            break;
        case 0xA1:	// AND C
            cpu->A = CPU_op_and(cpu, cpu->A, cpu->C);
            break;
        case 0xA2:	// AND D
            cpu->A = CPU_op_and(cpu, cpu->A, cpu->D);
            break;
        case 0xA3:	// AND E
            cpu->A = CPU_op_and(cpu, cpu->A, cpu->E);
            break;
        case 0xA4:	// AND H
            cpu->A = CPU_op_and(cpu, cpu->A, cpu->H);
            break;
        case 0xA5:	// AND L
            cpu->A = CPU_op_and(cpu, cpu->A, cpu->L);
            break;
        case 0xA6:	// AND (HL)
            cpu->A = CPU_op_and(cpu, cpu->A, Memory_read_8(mem, cpu->HL));
            break;
        case 0xE6:	// AND n
            cpu->A = CPU_op_and(cpu, cpu->A, Memory_read_8(mem, cpu->PC++));
            break;


        case 0xB7:	// OR A
            cpu->A = CPU_op_or(cpu, cpu->A, cpu->A);
            break;
        case 0xB0:	// OR B
            cpu->A = CPU_op_or(cpu, cpu->A, cpu->B);
            break;
        case 0xB1:	// OR C
            cpu->A = CPU_op_or(cpu, cpu->A, cpu->C);
            break;
        case 0xB2:	// OR D
            cpu->A = CPU_op_or(cpu, cpu->A, cpu->D);
            break;
        case 0xB3:	// OR E
            cpu->A = CPU_op_or(cpu, cpu->A, cpu->E);
            break;
        case 0xB4:	// OR H
            cpu->A = CPU_op_or(cpu, cpu->A, cpu->H);
            break;
        case 0xB5:	// OR L
            cpu->A = CPU_op_or(cpu, cpu->A, cpu->L);
            break;
        case 0xB6:	// OR (HL)
            cpu->A = CPU_op_or(cpu, cpu->A, Memory_read_8(mem, cpu->HL));
            break;
        case 0xF6:	// OR n
            cpu->A = CPU_op_or(cpu, cpu->A, Memory_read_8(mem, cpu->PC++));
            break;


        case 0xAF:	// XOR A
            cpu->A = CPU_op_xor(cpu, cpu->A, cpu->A);
            break;
        case 0xA8:	// XOR B
            cpu->A = CPU_op_xor(cpu, cpu->A, cpu->B);
            break;
        case 0xA9:	// XOR C
            cpu->A = CPU_op_xor(cpu, cpu->A, cpu->C);
            break;
        case 0xAA:	// XOR D
            cpu->A = CPU_op_xor(cpu, cpu->A, cpu->D);
            break;
        case 0xAB:	// XOR E
            cpu->A = CPU_op_xor(cpu, cpu->A, cpu->E);
            break;
        case 0xAC:	// XOR H
            cpu->A = CPU_op_xor(cpu, cpu->A, cpu->H);
            break;
        case 0xAD:	// XOR L
            cpu->A = CPU_op_xor(cpu, cpu->A, cpu->L);
            break;
        case 0xAE:	// XOR (HL)
            cpu->A = CPU_op_xor(cpu, cpu->A, Memory_read_8(mem, cpu->HL));
            break;
        case 0xEE:	// XOR n
            cpu->A = CPU_op_xor(cpu, cpu->A, Memory_read_8(mem, cpu->PC++));
            break;


        case 0xBF:	// CP A
            CPU_op_cp(cpu, cpu->A, cpu->A);
            break;
        case 0xB8:	// CP B
            CPU_op_cp(cpu, cpu->A, cpu->B);
            break;
        case 0xB9:	// CP C
            CPU_op_cp(cpu, cpu->A, cpu->C);
            break;
        case 0xBA:	// CP D
            CPU_op_cp(cpu, cpu->A, cpu->D);
            break;
        case 0xBB:	// CP E
            CPU_op_cp(cpu, cpu->A, cpu->E);
            break;
        case 0xBC:	// CP H
            CPU_op_cp(cpu, cpu->A, cpu->H);
            break;
        case 0xBD:	// CP L
            CPU_op_cp(cpu, cpu->A, cpu->L);
            break;
        case 0xBE:	// CP (HL)
            CPU_op_cp(cpu, cpu->A, Memory_read_8(mem, cpu->HL));
            break;
        case 0xFE:	// CP n
            CPU_op_cp(cpu, cpu->A, Memory_read_8(mem, cpu->PC++));
            break;


        case 0x3C:	// INC A
            cpu->A = CPU_op_inc(cpu, cpu->A);
            break;
        case 0x04:	// INC B
            cpu->B = CPU_op_inc(cpu, cpu->B);
            break;
        case 0x0C:	// INC C
            cpu->C = CPU_op_inc(cpu, cpu->C);
            break;
        case 0x14:	// INC D
            cpu->D = CPU_op_inc(cpu, cpu->D);
            break;
        case 0x1C:	// INC E
            cpu->E = CPU_op_inc(cpu, cpu->E);
            break;
        case 0x24:	// INC H
            cpu->H = CPU_op_inc(cpu, cpu->H);
            break;
        case 0x2C:	// INC L
            cpu->L = CPU_op_inc(cpu, cpu->L);
            break;
        case 0x34:	// INC (HL)
            Memory_write_8(mem, cpu->HL, CPU_op_inc(cpu, Memory_read_8(mem, cpu->HL)));
            break;


        case 0x3D:	// DEC A
            cpu->A = CPU_op_dec(cpu, cpu->A);
            break;
        case 0x05:	// DEC B
            cpu->B = CPU_op_dec(cpu, cpu->B);
            break;
        case 0x0D:	// DEC C
            cpu->C = CPU_op_dec(cpu, cpu->C);
            break;
        case 0x15:	// DEC D
            cpu->D = CPU_op_dec(cpu, cpu->D);
            break;
        case 0x1D:	// DEC E
            cpu->E = CPU_op_dec(cpu, cpu->E);
            break;
        case 0x25:	// DEC H
            cpu->H = CPU_op_dec(cpu, cpu->H);
            break;
        case 0x2D:	// DEC L
            cpu->L = CPU_op_dec(cpu, cpu->L);
            break;
        case 0x35:	// DEC (HL)
            Memory_write_8(mem, cpu->HL, CPU_op_dec(cpu, Memory_read_8(mem, cpu->HL)));
            break;


        case 0x09:	// ADD HL, BC
            cpu->HL = CPU_op_add_16(cpu, cpu->HL, cpu->BC);
            break;
        case 0x19:	// ADD HL, DE
            cpu->HL = CPU_op_add_16(cpu, cpu->HL, cpu->DE);
            break;
        case 0x29:	// ADD HL, HL
            cpu->HL = CPU_op_add_16(cpu, cpu->HL, cpu->HL);
            break;
        case 0x39:	// ADD HL, SP
            cpu->HL = CPU_op_add_16(cpu, cpu->HL, cpu->SP);
            break;
        case 0xE8:	// ADD SP, n
            cpu->SP = CPU_op_add_16(cpu, cpu->HL, Memory_read_8(mem, cpu->PC++));
            break;


        case 0x03:	// INC BC
            cpu->BC++;
            break;
        case 0x13:	// INC DE
            cpu->DE++;
            break;
        case 0x23:	// INC HL
            cpu->HL++;
            break;
        case 0x33:	// INC SP
            cpu->SP++;
            break;


        case 0x0B:	// DEC BC
            cpu->BC--;
            break;
        case 0x1B:	// DEC DE
            cpu->DE--;
            break;
        case 0x2B:	// DEC HL
            cpu->HL--;
            break;
        case 0x3B:	// DEC SP
            cpu->SP--;
            break;

        case 0xCB:	// Some two byte opcodes here.
            break;
        case 0x27:   // DAA
            break;
        case 0x2F:   // CPL
            cpu->A = ~cpu->A;
            break;
        case 0x3F:   // CCF
            break;
        case 0x37:   // SCF
            break;
        case 0x76:   // HALT
            break;
        case 0x10:   // STOP
            cpu->stopped = 1;
            break;
        case 0xF3:	// DI
            break;
        case 0xFB:	// EI
            break;


        case 0x07:	// RLCA
            cpu->z = 0;
            cpu->h = 0;
            cpu->n = 0;
            cpu->cy = (cpu->A & 0x80 ) >> 7;
            cpu->A = cpu->A<<1;
            break;
        case 0x17:	// RLA
            ;int temp_cy = (cpu->A & 0x80) >> 7;
            cpu->A = cpu->A<<1;
            cpu->A |= cpu->cy;
            cpu->z = 0;
            cpu->h = 0;
            cpu->n = 0;
            cpu->cy = temp_cy;
            break;
        case 0x0F:	// RRCA
            cpu->z = 0;
            cpu->h = 0;
            cpu->n = 0;
            cpu->cy = cpu->A & 0x01;
            cpu->A = cpu->A>>1;
            cpu->A |= cpu->cy << 7;
            break;
        case 0x1F:	// RRA
            cpu->z = 0;
            cpu->h = 0;
            cpu->n = 0;
            cpu->cy = cpu->A & 0x01;
            cpu->A = cpu->A>>1;
            break;


        case 0xC3:  // JP nn
            cpu->PC = Memory_read_16(mem, cpu->PC);
            break;
        case 0xC2: 	// JP NZ, nn
            if(cpu->z == 0){
                cpu->PC = Memory_read_16(mem, cpu->PC);
            }
            cpu->PC += 2;
            break;
        case 0xCA: 	// JP Z, nn
            cpu->PC = Memory_read_16(mem, cpu->PC);
            cpu->PC += 2;
            if(cpu->z == 1){
                cpu->PC = Memory_read_16(mem, cpu->PC);
            }
            break;
        case 0xD2: 	// JP NC, nn
            cpu->PC = Memory_read_16(mem, cpu->PC);
            cpu->PC += 2;
            if(cpu->cy == 0){
                cpu->PC = Memory_read_16(mem, cpu->PC);
            }
            break;
        case 0xDA: 	// JP C, nn
            cpu->PC = Memory_read_16(mem, cpu->PC);
            cpu->PC += 2;
            if(cpu->cy == 0){
                cpu->PC = Memory_read_16(mem, cpu->PC);
            }
            break;
        case 0xE9:  // JP HL
            cpu->PC = cpu->HL;
            break;


        case 0x18:  // JR n
            cpu->PC = cpu->PC + 1 + Memory_read_8(mem, cpu->PC) - 127;
            break;
        case 0x20:  // JR NZ, n
            ;int8_t possible_new_PC = cpu->PC + 1 + Memory_read_8(mem, cpu->PC) - 127;
            if(cpu->z == 0){
                cpu->PC = possible_new_PC;
            }
            break;
        case 0x28:  // JR Z, n
            possible_new_PC = cpu->PC + 1 + Memory_read_8(mem, cpu->PC) - 127;
            if(cpu->z == 1){
                cpu->PC = possible_new_PC;
            }
            break;
        case 0x30:  // JR NC, n
            possible_new_PC = cpu->PC + 1 + Memory_read_8(mem, cpu->PC) - 127;
            if(cpu->cy == 0){
                cpu->PC = possible_new_PC;
            }
            break;
        case 0x38:  // JR C, n
            possible_new_PC = cpu->PC + 1 + Memory_read_8(mem, cpu->PC) - 127;
            if(cpu->cy == 1){
                cpu->PC = possible_new_PC;
            }
            break;


        case 0xCD:	// CALL nn
            cpu->PC += 2;
            CPU_op_push(cpu, mem, cpu->PC);
            cpu->PC = Memory_read_16(mem, cpu->PC - 2);
            break;
        case 0xC4:	// CALL NZ, nn
            cpu->PC += 2;
            if(cpu->z == 0){
                cpu->PC += 2;
                CPU_op_push(cpu, mem, cpu->PC);
                cpu->PC = Memory_read_16(mem, cpu->PC - 2);
            }
            break;
        case 0xCC:	// CALL Z, nn
            cpu->PC += 2;
            if(cpu->z == 1){
                CPU_op_push(cpu, mem, cpu->PC);
                cpu->PC = Memory_read_16(mem, cpu->PC - 2);
            }
            break;
        case 0xD4:	// CALL NC, nn
            cpu->PC += 2;
            if(cpu->cy == 0){
                CPU_op_push(cpu, mem, cpu->PC);
                cpu->PC = Memory_read_16(mem, cpu->PC - 2);
            }
            break;
        case 0xDC:	// CALL C, nn
            cpu->PC += 2;
            if(cpu->cy == 1){
                CPU_op_push(cpu, mem, cpu->PC);
                cpu->PC = Memory_read_16(mem, cpu->PC - 2);
            }
            break;


        case 0xC7:	// RST 0x00
            CPU_op_push(cpu, mem, cpu->PC);
            cpu->PC = 0x00;
            break;
        case 0xCF:	// RST 0x08
            CPU_op_push(cpu, mem, cpu->PC);
            cpu->PC = 0x08;
            break;
        case 0xD7:	// RST 0x10
            CPU_op_push(cpu, mem, cpu->PC);
            cpu->PC = 0x10;
            break;
        case 0xDF:	// RST 0x18
            CPU_op_push(cpu, mem, cpu->PC);
            cpu->PC = 0x18;
            break;
        case 0xE7:	// RST 0x20
            CPU_op_push(cpu, mem, cpu->PC);
            cpu->PC = 0x20;
            break;
        case 0xEF:	// RST 0x28
            CPU_op_push(cpu, mem, cpu->PC);
            cpu->PC = 0x28;
            break;
        case 0xF7:	// RST 0x30
            CPU_op_push(cpu, mem, cpu->PC);
            cpu->PC = 0x30;
            break;
        case 0xFF:	// RST 0x38
            CPU_op_push(cpu, mem, cpu->PC);
            cpu->PC = 0x38;
            break;


        case 0xC9:	// RET
            cpu->PC = CPU_op_pop(cpu, mem);
            break;
        case 0xC0:	// RET NZ
            if(cpu->z == 0){
                cpu->PC = CPU_op_pop(cpu, mem);
            }
            break;
        case 0xC8:	// RET Z
            if(cpu->z == 1){
                cpu->PC = CPU_op_pop(cpu, mem);
            }
            break;
        case 0xD0:	// RET NC
            if(cpu->cy == 0){
                cpu->PC = CPU_op_pop(cpu, mem);
            }
            break;
        case 0xD8:	// RET C
            if(cpu->cy == 1){
                cpu->PC = CPU_op_pop(cpu, mem);
            }
            break;
        case 0xD9:	// RETI
            cpu->PC = CPU_op_pop(cpu, mem);
            break;
        case 0x00:  // NOP
            break;

        default:
            break;
    }
}
