#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "Memory.h"

typedef struct {

    union {

        uint8_t VRAM[VRAM_BANK_SIZE];

        struct{
            /* control register */
            union {
                uint8_t control;
                struct {
                    int BG_display: 1;
                    int OBJ_display_enable: 1;
                    int OBJ_size: 1;
                    int BG_tile_map_display_select: 1;
                    int BG_and_window_tile_data_select: 1;
                    int window_display_enable: 1;
                    int window_tile_map_display_select : 1;
                    int LCD_display_enable : 1;
                };
            };


            /* status register */
            union {
                uint8_t status;
                struct {
                    unsigned int mode_flag: 2;
                    int coincidence_flag: 1;
                    int mode_0_hblank_interrupt : 1;
                    int mode_1_vblank_interrupt : 1;
                    int mode_2_OAM_interrupt : 1;
                    int coincidence_interrupt: 1;
                };
            };

            uint8_t SCY;
            uint8_t SCX;
            uint8_t LY;
            uint8_t LYC;
            uint8_t WY;
            uint8_t WX;
            uint8_t BGP;
            uint8_t OBP0;
            uint8_t OBP1;

            uint8_t BCPS_BGPI;
            uint8_t BCPD_BGPD;
            uint8_t OCPS_OBPI;
            uint8_t OCPD_OBPD;

            uint8_t VBK;
            uint8_t DMA;

            uint8_t HDMA1;
            uint8_t HDMA2;
            uint8_t HDMA3;
            uint8_t HDMA4;
            uint8_t HDMA5;
        };
    };
} LCD;

#endif
