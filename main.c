#include <stdio.h>
#include "CPU.h"
#include "Memory.h"
#include "Cartridge.h"
#include "LCD.h"
#include "ASCIIRenderer.h"

CPU cpu;
Memory* mem;
LCD* lcd;
uint8_t* cart;

int main(){
    
    mem = Memory_create();

    cart = Cartridge_load("roms/cpu_instrs/cpu_instrs.gb");
    if(cart == NULL){
        printf("Error loading cartridge!\n");
        return -1;
    }

    mem->ROM_bank_0 = cart;
    mem->ROM_bank_n = cart + 0x4000;
    mem->external_RAM = cart + 0xA000;
    mem->cart = cart;

    lcd = (LCD*) mem->VRAM;

    CPU_reset(&cpu);

    int t = 0;

    while(!cpu.stopped){
        printf("PC: 0x%X\n", cpu.PC);
        CPU_process_instruction(&cpu, mem);

        switch(lcd->mode_flag){
            case 0: /* during h-bank */
                if(t >= 204){
                    t = 0;
                    lcd->LY++;

                    if(lcd->LY == 143){
                        lcd->mode_flag = 1; 
                    }else{
                        lcd->mode_flag = 2;
                    }
                }
                break;

            case 1: /* during v-blank */
                if(t >= 456){
                    t = 0;                 
                    lcd->LY++;
                    if(lcd->LY > 153){
                        lcd->mode_flag = 2; 
                        lcd->LY = 0;
                    }
                }
                break;

            case 2: /* during searching OAM-RAM */
                if(t >= 80){
                    t = 0;  
                    lcd->mode_flag = 3;
                }
                break;

            case 3: /* during transferring data to LCD driver */
                if(t >= 172){
                    t = 0;  
                    lcd->mode_flag = 0;
                    render(lcd);
                }
                break;
            default:
                break;
        }

        t++;

        //getc(stdin);

    }

    return 0;
}
