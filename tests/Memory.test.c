#include "../Memory.h"
#include "minunit.h"

Memory* mem;

MU_TEST(create){

    /* act */
    mem = Memory_create();

    /* assert */
    mu_check(mem != NULL);

    mu_check(mem->ROM_bank_0   == NULL);
    mu_check(mem->ROM_bank_n   == NULL);
    mu_check(mem->external_RAM == NULL);

    mu_check(mem->VRAM_banks   != NULL);
    for(int i=0;i<N_SWITCHABLE_VRAM_BANKS;i++){
        mu_check(mem->VRAM_banks[i] != NULL);
    }
    mu_check(mem->VRAM         != NULL);

    mu_check(mem->WRAM_banks   != NULL);
    for(int i=0;i<N_SWITCHABLE_WRAM_BANKS+1;i++){
        mu_check(mem->WRAM_banks[i] != NULL);
    }
    mu_check(mem->WRAM_bank_0  != NULL);
    mu_check(mem->WRAM_bank_1  != NULL);
    mu_check(mem->ECHO         != NULL);

    /* OTHER STUFF */
    mu_check(mem->OAM          != NULL);
    mu_check(mem->IO_ports     != NULL);
    mu_check(mem->HRAM         != NULL);
    mu_check(mem->IER          != NULL);
}


MU_TEST(ROM_bank_0){

    /* act */
    mem = Memory_create();
    uint8_t ROM_bank_0[0x4000];
    mem->ROM_bank_0 = ROM_bank_0;
    int location = 0x200;
    Memory_write_8(mem, location, 0xAC);

    /* assert */
    mu_assert_int_eq(0xAC, Memory_read_8(mem, location));
    mu_assert_int_eq(0xAC, mem->ROM_bank_0[location]);
}


MU_TEST(ROM_bank_n){

    /* act */
    mem = Memory_create();
    uint8_t ROM_bank_n[0x4000];
    mem->ROM_bank_n = ROM_bank_n;
    int location = 0x4200;
    int offset = 0x4000;
    Memory_write_16(mem, location, 0xECC0);

    /* assert */
    mu_assert_int_eq(0xECC0, Memory_read_16(mem, location));
    mu_assert_int_eq(0xEC, mem->ROM_bank_n[location-offset]);
    mu_assert_int_eq(0xC0, mem->ROM_bank_n[location-offset+1]);
}


MU_TEST(external_RAM){

    /* act */
    mem = Memory_create();
    uint8_t external_RAM[0x4000];
    mem->external_RAM = external_RAM;
    int location = 0xB000;
    int offset = 0xA000;
    Memory_write_16(mem, location, 0xECC0);

    /* assert */
    mu_assert_int_eq(0xECC0, Memory_read_16(mem, location));
    mu_assert_int_eq(0xEC, mem->external_RAM[location-offset]);
    mu_assert_int_eq(0xC0, mem->external_RAM[location-offset+1]);
}


MU_TEST(VRAM_banks){

    /* act */
    mem = Memory_create();
    int location = 0x9000;
    int offset = 0x8000;
    Memory_write_16(mem, location, 0xECC0);

    /* assert */
    mu_assert_int_eq(0xECC0, Memory_read_16(mem, location));
    mu_assert_int_eq(0xEC, mem->VRAM[location-offset]);
    mu_assert_int_eq(0xC0, mem->VRAM[location-offset+1]);
}


MU_TEST(WRAM_bank_0){

    /* act */
    mem = Memory_create();
    int location = 0xC803;
    int offset = 0xC000;
    Memory_write_16(mem, location, 0xECC0);

    /* assert */
    mu_assert_int_eq(0xECC0, Memory_read_16(mem, location));
    mu_assert_int_eq(0xEC, mem->WRAM_bank_0[location-offset]);
    mu_assert_int_eq(0xC0, mem->WRAM_bank_0[location-offset+1]);
}


MU_TEST(WRAM_bank_1){

    /* act */
    mem = Memory_create();
    int location = 0xDDDD;
    int offset = 0xD000;
    Memory_write_16(mem, location, 0xECC0);

    /* assert */
    mu_assert_int_eq(0xECC0, Memory_read_16(mem, location));
    mu_assert_int_eq(0xEC, mem->WRAM_bank_1[location-offset]);
    mu_assert_int_eq(0xC0, mem->WRAM_bank_1[location-offset+1]);
}


MU_TEST(ECHO){

    /* act */
    mem = Memory_create();
    int location = 0xE010;
    int offset = 0xE000;
    Memory_write_16(mem, location, 0xECC0);

    /* assert */
    mu_assert_int_eq(0xECC0, Memory_read_16(mem, location));
    mu_assert_int_eq(0xEC, mem->WRAM_bank_0[location-offset]);
    mu_assert_int_eq(0xC0, mem->WRAM_bank_0[location-offset+1]);
}


MU_TEST(OAM){

    /* act */
    mem = Memory_create();
    int location = 0xFE90;
    int offset = 0xFE00;
    Memory_write_16(mem, location, 0xECC0);

    /* assert */
    mu_assert_int_eq(0xECC0, Memory_read_16(mem, location));
    mu_assert_int_eq(0xEC, mem->OAM[location-offset]);
    mu_assert_int_eq(0xC0, mem->OAM[location-offset+1]);
}


MU_TEST(IO_ports){

}


MU_TEST(HRAM){

    /* act */
    mem = Memory_create();
    int location = 0xFF80;
    int offset = 0xFF80;
    Memory_write_16(mem, location, 0xECC0);

    /* assert */
    mu_assert_int_eq(0xECC0, Memory_read_16(mem, location));
    mu_assert_int_eq(0xEC, mem->HRAM[location-offset]);
    mu_assert_int_eq(0xC0, mem->HRAM[location-offset+1]);
}


MU_TEST(IER){

    /* act */
    mem = Memory_create();
    int location = 0xFFFF;
    int offset = 0xFFFF;
    Memory_write_8(mem, location, 0x7B);

    /* assert */
    mu_assert_int_eq(0x7B, Memory_read_8(mem, location));
    mu_assert_int_eq(0x7B, mem->IER[location-offset]);
}


MU_TEST_SUITE(memory_suite){
    MU_RUN_TEST(create);
    MU_RUN_TEST(ROM_bank_0);
    MU_RUN_TEST(ROM_bank_n);
    MU_RUN_TEST(external_RAM);
    MU_RUN_TEST(VRAM_banks);
    MU_RUN_TEST(WRAM_bank_0);
    MU_RUN_TEST(WRAM_bank_1);
    MU_RUN_TEST(ECHO);
    MU_RUN_TEST(OAM);
    MU_RUN_TEST(IO_ports);
    MU_RUN_TEST(HRAM);
    MU_RUN_TEST(IER);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(memory_suite);
    MU_REPORT();
    return minunit_fail;
}
