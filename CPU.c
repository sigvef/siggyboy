#include "CPU.h"

void CPU_reset(CPU*cpu){
    cpu->AF = 0;
    cpu->BC = 0;
    cpu->DE = 0;
    cpu->HL = 0;
    cpu->SP = 0;
    cpu->PC = 0x100;
}


uint8_t CPU_op_add(CPU*cpu, uint8_t a, uint8_t b){
    uint8_t result = a + b;
    cpu->z = result ? 0 : 1;
	cpu->h = (0x0F - (a & 0x0F) < (b & 0x0F)) ? 1 : 0;
    cpu->n = 0;
	cpu->cy = (0xFF - a < b) ? 1 : 0;
    return result;
}


/* inspired by https://gbemulator.googlecode.com/svn/trunk/src/core.c */

/* a good resource is page 84- of http://chrisantonellis.com/files/gameboy/gb-programming-manual.pdf */


void CPU_process_instruction(CPU*cpu, uint8_t* ram){
    int opcode = ram[cpu->PC];
    switch(opcode){
        /* 8 bit loads, immediate to register */
        /* TODO: actually read next byte, increment PC etc */
        case 0x06:
            cpu->B = 0;
            break;

        case 0x0E:
            cpu->C = 0;
            break;

        case 0x16:
            cpu->D = 0;
            break;

        case 0x1E:
            cpu->E = 0;
            break;

        case 0x26:
            cpu->H = 0;
            break;

        case 0x2E:
            cpu->L = 0;
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
            cpu->A = ram[cpu->HL];
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
            cpu->B = ram[cpu->HL];
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
            cpu->C = ram[cpu->HL];
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
            cpu->D = ram[cpu->HL];
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
            cpu->E = ram[cpu->HL];
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
            cpu->H = ram[cpu->HL];
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
            cpu->L = ram[cpu->HL];
            break;


            /* 8bit loads: reg -> (HL) */
        case 0x70:  /* LD (HL), B */
            ram[cpu->HL] = cpu->B;
            break;
        case 0x71:  /* LD (HL), C */
            ram[cpu->HL] = cpu->C;
            break;
        case 0x72:  /* LD (HL), D */
            ram[cpu->HL] = cpu->D;
            break;
        case 0x73:  /* LD (HL), E */
            ram[cpu->HL] = cpu->E;
            break;
        case 0x74:  /* LD (HL), H */
            ram[cpu->HL] = cpu->H;
            break;
        case 0x75:  /* LD (HL), L */
            ram[cpu->HL] = cpu->L;
            break;
        case 0x36:  /* LD (HL), n */
            ram[cpu->HL] = ram[++cpu->PC];
            break;
        case 0x0A:  /* LD A, (BC) */
            cpu->A = ram[cpu->BC];
            break;
        case 0x1A:  /* LD A, (DE) */
            cpu->A = ram[cpu->DE];
            break;
        case 0xFA:  /* LD A, (nn) */
            ;uint16_t immediate = ram[++cpu->PC];
            immediate = (immediate << 8) | ram[++cpu->PC];
            cpu->A = ram[immediate];
            break;
        case 0x3E:  /* LD A, n */
            cpu->A = ram[++cpu->PC];
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
            ram[cpu->BC] = cpu->A;
            break;
        case 0x12:  /* LD (DE), A */
            ram[cpu->DE] = cpu->A;
            break;
        case 0x77:  /* LD (HL), A */
            ram[cpu->HL] = cpu->A;
            break;
        case 0xEA:  /* LD (nn), A */
            immediate = ram[++cpu->PC];
            immediate = (immediate << 8) | ram[++cpu->PC];
            ram[immediate] = cpu->A;
            break;
        case 0xF2:  /* LD A, (C) */
            cpu->A = ram[cpu->A];
            break;
        case 0xE2:  /* LD (C), A */
            ram[cpu->C] = cpu->A;
            break;


            /* 8bit loads/dec/inc */
        case 0x3A:  /* LDD A, (HL) */
            break;
        case 0x32:  /* LDD (HL), A */
            break;
        case 0x2A:  /* LDI A, (HL) */
            break;
        case 0x22:  /* LDI (HL), A */
            break;
        case 0xE0:  /* LDH (n), A */
            break;
        case 0xF0:  /* LDH A, (n) */
            break;


            /* 16bit loads */
        case 0x01:  /* LD BC, nn */
            break;
        case 0x11:  /* LD DE, nn */
            break;
        case 0x21:  /* LD HL, nn */
            break;
        case 0x31:  /* LD SP, nn */
            break;
        case 0xF9:  /* LD SP, HL */
            break;
        case 0xF8:  /* LDHL SP, n */
            break;
        case 0x08: // LD (nn), SP
            break;


        case 0xF5:	// PUSH AF
            break;
        case 0xC5:	// PUSH BC
            break;
        case 0xD5:	// PUSH DE
            break;
        case 0xE5:	// PUSH HL
            break;
        case 0xF1:	// POP AF
            break;
        case 0xC1:	// POP BC
            break;
        case 0xD1:	// POP DE
            break;
        case 0xE1:	// POP HL
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
            break;
        case 0xC6:	// ADD A, n
            break;
        case 0x8F:	// ADC A, A
            break;
        case 0x88:	// ADC A, B
            break;
        case 0x89:	// ADC A, C
            break;
        case 0x8A:	// ADC A, D
            break;
        case 0x8B:	// ADC A, E
            break;
        case 0x8C:	// ADC A, H
            break;
        case 0x8D:	// ADC A, L
            break;
        case 0x8E:	// ADC A, (HL)
            break;
        case 0xCE:	// ADC A, n
            break;
        case 0x97:	// SUB A, A
            break;
        case 0x90:	// SUB A, B
            break;
        case 0x91:	// SUB A, C
            break;
        case 0x92:	// SUB A, D
            break;
        case 0x93:	// SUB A, E
            break;
        case 0x94:	// SUB A, H
            break;
        case 0x95:	// SUB A, L
            break;
        case 0x96:	// SUB A, (HL)
            break;
        case 0xD6:	// SUB A, n
            break;
        case 0x9F:	// SBC A, A
            break;
        case 0x98:	// SBC A, B
            break;
        case 0x99:	// SBC A, C
            break;
        case 0x9A:	// SBC A, D
            break;
        case 0x9B:	// SBC A, E
            break;
        case 0x9C:	// SBC A, H
            break;
        case 0x9D:	// SBC A, L
            break;
        case 0x9E:	// SBC A, (HL)
            break;
        case 0xDE:	// SBC A, n
            break;
        case 0xA7:	// AND A
            break;
        case 0xA0:	// AND B
            break;
        case 0xA1:	// AND C
            break;
        case 0xA2:	// AND D
            break;
        case 0xA3:	// AND E
            break;
        case 0xA4:	// AND H
            break;
        case 0xA5:	// AND L
            break;
        case 0xA6:	// AND (HL)
            break;
        case 0xE6:	// AND n
            break;
        case 0xB7:	// OR A
            break;
        case 0xB0:	// OR B
            break;
        case 0xB1:	// OR C
            break;
        case 0xB2:	// OR D
            break;
        case 0xB3:	// OR E
            break;
        case 0xB4:	// OR H
            break;
        case 0xB5:	// OR L
            break;
        case 0xB6:	// OR (HL)
            break;
        case 0xF6:	// OR n
            break;
        case 0xAF:	// XOR A
            break;
        case 0xA8:	// XOR B
            break;
        case 0xA9:	// XOR C
            break;
        case 0xAA:	// XOR D
            break;
        case 0xAB:	// XOR E
            break;
        case 0xAC:	// XOR H
            break;
        case 0xAD:	// XOR L
            break;
        case 0xAE:	// XOR (HL)
            break;
        case 0xEE:	// XOR n
            break;
        case 0xBF:	// CP A
            break;
        case 0xB8:	// CP B
            break;
        case 0xB9:	// CP C
            break;
        case 0xBA:	// CP D
            break;
        case 0xBB:	// CP E
            break;
        case 0xBC:	// CP H
            break;
        case 0xBD:	// CP L
            break;
        case 0xBE:	// CP (HL)
            break;
        case 0xFE:	// CP n
            break;
        case 0x3C:	// INC A
            break;
        case 0x04:	// INC B
            break;
        case 0x0C:	// INC C
            break;
        case 0x14:	// INC D
            break;
        case 0x1C:	// INC E
            break;
        case 0x24:	// INC H
            break;
        case 0x2C:	// INC L
            break;
        case 0x34:	// INC (HL)
            break;
        case 0x3D:	// DEC A
            break;
        case 0x05:	// DEC B
            break;
        case 0x0D:	// DEC C
            break;
        case 0x15:	// DEC D
            break;
        case 0x1D:	// DEC E
            break;
        case 0x25:	// DEC H
            break;
        case 0x2D:	// DEC L
            break;
        case 0x35:	// DEC (HL)
            break;
        case 0x09:	// ADD HL, BC
            break;
        case 0x19:	// ADD HL, DE
            break;
        case 0x29:	// ADD HL, HL
            break;
        case 0x39:	// ADD HL, SP
            break;
        case 0xE8:	// ADD SP, n
            break;
        case 0x03:	// INC BC
            break;
        case 0x13:	// INC DE
            break;
        case 0x23:	// INC HL
            break;
        case 0x33:	// INC SP
            break;
        case 0x0B:	// DEC BC
            break;
        case 0x1B:	// DEC DE
            break;
        case 0x2B:	// DEC HL
            break;
        case 0x3B:	// DEC SP
            break;
        case 0xCB:	// Some two byte opcodes here.
            break;
        case 0x27:   // DAA
            break;
        case 0x2F:   // CPL
            break;
        case 0x3F:   // CCF
            break;
        case 0x37:   // SCF
            break;
        case 0x76:   // HALT
            break;
        case 0x10:   // STOP
            break;
        case 0xF3:	// DI
            break;
        case 0xFB:	// EI
            break;
        case 0x07:	// RLCA
            break;
        case 0x17:	// RLA
            break;
        case 0x0F:	// RRCA
            break;
        case 0x1F:	// RRA
            break;
        case 0xC3:   // JP imm
            break;
        case 0xC2: 	// JP NZ, nn
            break;
        case 0xCA: 	// JP Z, nn
            break;
        case 0xD2: 	// JP NC, nn
            break;
        case 0xDA: 	// JP C, nn
            break;
        case 0xE9:   // JP HL
            break;
        case 0x18:   // JR n
            break;
        case 0x20:   // JR NZ, n
            break;
        case 0x28:   // JR Z, n
            break;
        case 0x30:   // JR NC, n
            break;
        case 0x38:   // JR C, n
            break;
        case 0xCD:	// CALL nn
            break;
        case 0xC4:	// CALL NZ, nn
            break;
        case 0xCC:	// CALL Z, nn
            break;
        case 0xD4:	// CALL NC, nn
            break;
        case 0xDC:	// CALL C, nn
            break;
        case 0xC7:	// RST 0x00
            break;
        case 0xCF:	// RST 0x08
            break;
        case 0xD7:	// RST 0x10
            break;
        case 0xDF:	// RST 0x18
            break;
        case 0xE7:	// RST 0x20
            break;
        case 0xEF:	// RST 0x28
            break;
        case 0xF7:	// RST 0x30
            break;
        case 0xFF:	// RST 0x38
            break;
        case 0xC9:	// RET
            break;
        case 0xC0:	// RET NZ
            break;
        case 0xC8:	// RET Z
            break;
        case 0xD0:	// RET NC
            break;
        case 0xD8:	// RET C
            break;
        case 0xD9:	// RETI
            break;
        case 0x00:  // NOP
            break;

        default:
            break;
    }


    cpu->PC++;
}
