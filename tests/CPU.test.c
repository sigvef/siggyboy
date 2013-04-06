#include "../CPU.h"
#include "minunit.h"

CPU cpu;
uint8_t ram[0x80000];

/* tests the reset functionality of the CPU */
MU_TEST(reset){

    /* act */
    CPU_reset(&cpu);

    /* assert */
    mu_assert_int_eq(0, cpu.AF);
    mu_assert_int_eq(0, cpu.A);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(0, cpu.n);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(0, cpu.cy);
    mu_assert_int_eq(0, cpu.F);

    mu_assert_int_eq(0, cpu.BC);
    mu_assert_int_eq(0, cpu.B);
    mu_assert_int_eq(0, cpu.C);

    mu_assert_int_eq(0, cpu.DE);
    mu_assert_int_eq(0, cpu.D);
    mu_assert_int_eq(0, cpu.E);

    mu_assert_int_eq(0, cpu.HL);
    mu_assert_int_eq(0, cpu.H);
    mu_assert_int_eq(0, cpu.L);

    mu_assert_int_eq(0x100, cpu.PC);
    mu_assert_int_eq(0xFFFF, cpu.SP);
}


/* tests if the struct/union c layout works as intended */
MU_TEST(unionification){

    /* act */
    CPU_reset(&cpu);
    cpu.AF = 0xA952;

    /* assert */
    mu_assert_int_eq(0xA952, cpu.AF);
    mu_assert_int_eq(0x52, cpu.F);
    mu_assert_int_eq(0xA9, cpu.A);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(1, cpu.n);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(1, cpu.cy);
}


MU_TEST(instructions_ld_immediate_to_register){

    /* act */
    CPU_reset(&cpu);
    ram[0x100] = 0x06; /* LD B, n */
    ram[0x101] = 0x24; /* immediate value */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x24, cpu.B);
}


MU_TEST(instructions_ld_register_to_register){

    /* act */
    CPU_reset(&cpu);
    cpu.BC = 0xA952;
    ram[0x100] = 0x51; /* LD D, C */
    ram[0x101] = 0x48; /* LD C, B */
    CPU_process_instruction(&cpu, ram);
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0xA9, cpu.C);
    mu_assert_int_eq(0xA9, cpu.B);
    mu_assert_int_eq(0x52, cpu.D);
}


MU_TEST(instructions_ld_register_to_hl_ram){

    /* act */
    CPU_reset(&cpu);
    cpu.B = 0x3C;
    cpu.HL = 0x8AC5;
    ram[0x100] = 0x70; /* LD (HL), B */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x3C, ram[0x8AC5]);
}


MU_TEST(instructions_push){

    /* act */
    CPU_reset(&cpu);
    cpu.SP = 0xFFFE;
    cpu.BC = 0xABCD;
    ram[0x100] = 0xC5; /* PUSH BC */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0xCD, ram[0xFFFD]);
    mu_assert_int_eq(0xAB, ram[0xFFFC]);
}


MU_TEST(instructions_pop){

    /* act */
    CPU_reset(&cpu);
    cpu.SP = 0xFFFC;
    ram[0x100] = 0xC1; /* POP BC */
    ram[0xFFFD] = 0xCD;
    ram[0xFFFC] = 0xAB;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0xABCD, cpu.BC);
}


MU_TEST(instructions_add){

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x3A;
    cpu.B = 0xC6;
    ram[0x100] = 0x80; /* ADD A, B */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x00, cpu.A);
    mu_assert_int_eq(1, cpu.z);
    mu_assert_int_eq(1, cpu.h);
    mu_assert_int_eq(0, cpu.n);
    mu_assert_int_eq(1, cpu.cy);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x3A;
    cpu.B = 0xC5;
    ram[0x100] = 0x80; /* ADD A, B */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0xFF, cpu.A);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(0, cpu.n);
    mu_assert_int_eq(0, cpu.cy);
}


MU_TEST(instructions_sub){

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x3E;
    cpu.E = 0x3E;
    cpu.HL = 0x40;
    ram[0x100] = 0x93; /* SUB A, E */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x00, cpu.A);
    mu_assert_int_eq(1, cpu.z);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(1, cpu.n);
    mu_assert_int_eq(0, cpu.cy);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x3E;
    cpu.E = 0x3E;
    cpu.HL = 0x40;
    ram[0x100] = 0xD6; /* sub A, n */
    ram[0x101] = 0x0F; /* sub A, n */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x2F, cpu.A);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(1, cpu.h);
    mu_assert_int_eq(1, cpu.n);
    mu_assert_int_eq(0, cpu.cy);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x3E;
    cpu.E = 0x3E;
    cpu.HL = 0x1000;
    ram[0x100] = 0x96; /* sub A, (HL) */
    ram[0x1000] = 0x40;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0xFE, cpu.A);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(1, cpu.n);
    mu_assert_int_eq(1, cpu.cy);
}


MU_TEST(instructions_and){

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x5A;
    cpu.L = 0x3F;
    ram[0x100] = 0xA5; /* AND L */
    ram[cpu.HL] = 0x00;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x1A, cpu.A);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(1, cpu.h);
    mu_assert_int_eq(0, cpu.n);
    mu_assert_int_eq(0, cpu.cy);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x5A;
    cpu.L = 0x3F;
    ram[0x100] = 0xE6; /* AND n */
    ram[0x101] = 0x38; /* immediate value */
    ram[cpu.HL] = 0x00;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x18, cpu.A);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(1, cpu.h);
    mu_assert_int_eq(0, cpu.n);
    mu_assert_int_eq(0, cpu.cy);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x5A;
    cpu.L = 0x3F;
    ram[0x100] = 0xA6; /* AND (HL) */
    ram[cpu.HL] = 0x00;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x00, cpu.A);
    mu_assert_int_eq(1, cpu.z);
    mu_assert_int_eq(1, cpu.h);
    mu_assert_int_eq(0, cpu.n);
    mu_assert_int_eq(0, cpu.cy);
}


MU_TEST(instructions_or){

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x5A;
    ram[0x100] = 0xB7; /* OR L */
    ram[cpu.HL] = 0x0F;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x5A, cpu.A);
    mu_assert_int_eq(0, cpu.z);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x5A;
    ram[0x100] = 0xF6; 
    ram[0x101] = 0x03; /* immediate value */
    ram[cpu.HL] = 0x0F;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x5B, cpu.A);
    mu_assert_int_eq(0, cpu.z);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x5A;
    ram[0x100] = 0xB6; /* OR (HL) */
    ram[cpu.HL] = 0x0F;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x5F, cpu.A);
    mu_assert_int_eq(0, cpu.z);
}


MU_TEST(instructions_xor){

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0xFF;
    ram[0x100] = 0xAF; /* XOR A */
    ram[cpu.HL] = 0x8A;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x00, cpu.A);
    mu_assert_int_eq(1, cpu.z);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0xFF;
    ram[0x100] = 0xEE; /* XOR n */
    ram[0x101] = 0x0F; /* immediate value */
    ram[cpu.HL] = 0x8A;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0xF0, cpu.A);
    mu_assert_int_eq(0, cpu.z);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0xFF;
    ram[0x100] = 0xAE; /* XOR (HL) */
    ram[cpu.HL] = 0x8A;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x75, cpu.A);
    mu_assert_int_eq(0, cpu.z);
}


MU_TEST(instructions_cp){

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x3C;
    cpu.B = 0x2F;
    ram[0x100] = 0xB8; /* CP B */
    ram[cpu.HL] = 0x40;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(1, cpu.h);
    mu_assert_int_eq(1, cpu.n);
    mu_assert_int_eq(0, cpu.cy);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x3C;
    cpu.B = 0x2F;
    ram[0x100] = 0xFE; /* CP n */
    ram[0x101] = 0x3C; /* immediate value */
    ram[cpu.HL] = 0x40;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(1, cpu.z);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(1, cpu.n);
    mu_assert_int_eq(0, cpu.cy);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x3C;
    cpu.B = 0x2F;
    ram[0x100] = 0xBE; /* CP B */
    ram[cpu.HL] = 0x40;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(1, cpu.n);
    mu_assert_int_eq(1, cpu.cy);
}


MU_TEST(instructions_inc){

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0xFF;
    ram[0x100] = 0x3C; /* INC A */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0, cpu.A);
    mu_assert_int_eq(1, cpu.z);
    mu_assert_int_eq(1, cpu.h);
    mu_assert_int_eq(0, cpu.n);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0xFF;
    ram[0x100] = 0x34; /* INC (HL) */
    ram[cpu.HL] = 0x50;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x51, ram[cpu.HL]);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(0, cpu.n);
}


MU_TEST(instructions_dec){

    /* act */
    CPU_reset(&cpu);
    cpu.L = 0x01;
    ram[0x100] = 0x2D; /* DEC L */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x00, cpu.L);
    mu_assert_int_eq(1, cpu.z);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(1, cpu.n);

    /* act */
    CPU_reset(&cpu);
    ram[0x100] = 0x35; /* DEC (HL) */
    ram[cpu.HL] = 0x00; 
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0xFF, ram[cpu.HL]);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(1, cpu.h);
    mu_assert_int_eq(1, cpu.n);
}


MU_TEST(instructions_add_16){

    /* act */
    CPU_reset(&cpu);
    cpu.HL = 0x8A23;
    cpu.BC = 0x0605;
    ram[0x100] = 0x09; /* ADD HL BC */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x9028, cpu.HL);
    mu_assert_int_eq(1, cpu.h);
    mu_assert_int_eq(0, cpu.n);
    mu_assert_int_eq(0, cpu.cy);

    /* act */
    CPU_reset(&cpu);
    cpu.HL = 0x8A23;
    cpu.BC = 0x0605;
    ram[0x100] = 0x29; /* ADD HL BC */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x1446, cpu.HL);
    mu_assert_int_eq(1, cpu.h);
    mu_assert_int_eq(0, cpu.n);
    mu_assert_int_eq(1, cpu.cy);
}


MU_TEST(instructions_inc_16){

    /* act */
    CPU_reset(&cpu);
    cpu.DE = 0x235F;
    ram[0x100] = 0x13; /* INC DE */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x2360, cpu.DE);
}


MU_TEST(instructions_dec_16){

    /* act */
    CPU_reset(&cpu);
    cpu.DE = 0x235F;
    ram[0x100] = 0x1B; /* DEC DE */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x235E, cpu.DE);
}


MU_TEST(instructions_rla){

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x85;
    ram[0x100] = 0x07; /* RLCA */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x0A, cpu.A);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(0, cpu.n);
    mu_assert_int_eq(1, cpu.cy);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x95;
    cpu.cy = 1;
    ram[0x100] = 0x17; /* RLA */
    CPU_process_instruction(&cpu, ram);

    /* assert */
    mu_assert_int_eq(0x2B, cpu.A);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(0, cpu.n);
    mu_assert_int_eq(1, cpu.cy);
}


MU_TEST(instructions_rra){

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x3B;
    ram[0x100] = 0x0F; /* RRCA */
    CPU_process_instruction(&cpu, ram);

    /* assert */
    mu_assert_int_eq(0x9D, cpu.A);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(0, cpu.n);
    mu_assert_int_eq(1, cpu.cy);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x81;
    ram[0x100] = 0x1F; /* RRA */
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x40, cpu.A);
    mu_assert_int_eq(0, cpu.z);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(0, cpu.n);
    mu_assert_int_eq(1, cpu.cy);
}


MU_TEST(instructions_jp){

    /* act */
    CPU_reset(&cpu);
    ram[0x100] = 0xC3; /* JP nn */
    ram[0x101] = 0x00;
    ram[0x102] = 0xFF;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0xFF, cpu.PC);

    /* act */
    CPU_reset(&cpu);
    ram[0x100] = 0xC3; /* JP nn */
    ram[0x101] = 0x00;
    ram[0x102] = 0xFF;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0xFF, cpu.PC);
}


MU_TEST(instructions_jr){

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x81;
    ram[0x100] = 0x18; /* JR n */
    ram[0x101] = 0xFF;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x102 + 0xFF - 127, cpu.PC);

    /* act */
    CPU_reset(&cpu);
    cpu.A = 0x81;
    ram[0x100] = 0x18; /* JR n */
    ram[0x101] = 0x00;
    CPU_process_instruction(&cpu, ram);
    
    /* assert */
    mu_assert_int_eq(0x102 - 127, cpu.PC);
}


MU_TEST_SUITE(cpu_suite){
    MU_RUN_TEST(reset);
    MU_RUN_TEST(unionification);
    MU_RUN_TEST(instructions_ld_register_to_register);
    MU_RUN_TEST(instructions_ld_immediate_to_register);
    MU_RUN_TEST(instructions_ld_register_to_hl_ram);
    MU_RUN_TEST(instructions_push);
    MU_RUN_TEST(instructions_pop);
    MU_RUN_TEST(instructions_add);
    MU_RUN_TEST(instructions_sub);
    MU_RUN_TEST(instructions_and);
    MU_RUN_TEST(instructions_or);
    MU_RUN_TEST(instructions_xor);
    MU_RUN_TEST(instructions_cp);
    MU_RUN_TEST(instructions_inc);
    MU_RUN_TEST(instructions_dec);
    MU_RUN_TEST(instructions_add_16);
    MU_RUN_TEST(instructions_inc_16);
    MU_RUN_TEST(instructions_dec_16);
    MU_RUN_TEST(instructions_rla);
    MU_RUN_TEST(instructions_rra);
    MU_RUN_TEST(instructions_jp);
    MU_RUN_TEST(instructions_jr);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(cpu_suite);
    MU_REPORT();
    return minunit_fail;
}
