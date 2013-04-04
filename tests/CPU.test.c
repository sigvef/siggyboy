#include "../CPU.h"
#include "minunit.h"

CPU cpu;

/* tests the reset functionality of the CPU */
MU_TEST(reset){

    /* act */
    CPU_reset(&cpu);

    /* assert */
    mu_check(cpu.AF == 0);
    mu_check(cpu.A == 0);
    mu_check(cpu.zf == 0);
    mu_check(cpu.n  == 0);
    mu_check(cpu.h  == 0);
    mu_check(cpu.cy == 0);
    mu_check(cpu.F == 0);

    mu_check(cpu.BC == 0);
    mu_check(cpu.B == 0);
    mu_check(cpu.C == 0);

    mu_check(cpu.DE == 0);
    mu_check(cpu.D == 0);
    mu_check(cpu.E == 0);

    mu_check(cpu.HL == 0);
    mu_check(cpu.H == 0);
    mu_check(cpu.L == 0);
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
    mu_assert_int_eq(0, cpu.zf);
    mu_assert_int_eq(1, cpu.n);
    mu_assert_int_eq(0, cpu.h);
    mu_assert_int_eq(1, cpu.cy);
}



MU_TEST_SUITE(cpu_suite){
    MU_RUN_TEST(reset);
    MU_RUN_TEST(unionification);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(cpu_suite);
    MU_REPORT();
    return minunit_fail;
}
