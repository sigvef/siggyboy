#include "../CPU.h"
#include "minunit.h"

CPU cpu;
uint8_t ram[0x8000];

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
    mu_assert_int_eq(0, cpu.SP);
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


MU_TEST_SUITE(cpu_suite){
    MU_RUN_TEST(reset);
    MU_RUN_TEST(unionification);
    MU_RUN_TEST(instructions_ld_register_to_register);
    MU_RUN_TEST(instructions_add);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(cpu_suite);
    MU_REPORT();
    return minunit_fail;
}
