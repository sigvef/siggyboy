#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint8_t* Cartridge_load(const char* filename){

    FILE* fp = fopen(filename, "rb");

    fseek(fp, 0L, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    uint8_t* cartridge = (uint8_t*) malloc(sizeof(uint8_t) * filesize);

    int result = fread(cartridge, filesize, sizeof(uint8_t), fp);

    fclose(fp);

    return cartridge;
}
