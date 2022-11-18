/*
 * Soubor:  font-array-view.c
 * Datum:   15.11.2022 10:18
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Úloha: 
 * Popis:   
 ****************************************************/
#define _ISOC99_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>

//  convert -font 'Terminus-(TTF)' -pointsize 12 label:A A.xbm && vat A.xbm
//  https://stackoverflow.com/questions/2156572/c-header-file-with-bitmapped-fonts/2156872#2156872
#include "A.xbm"

#define bits A_bits
#define len A_height

int main(void)
{
    unsigned int mask = 1 << 7;

    for (int i = 0; i < len; i++) {
        mask = 1 << 7;
        printf("0x%02X  .", bits[i]);
        while (mask) {
            if (bits[i] & mask) {
                printf("#");
            } else {
                printf(" ");
            }
            mask = mask >> 1;
        }
        printf(".");
        printf("\n");

    }

    return 0;
}

