#ifndef __FONT_H__
#define __FONT_H__
/*
 * Soubor:  font.h
 * Datum:   16.11.2022 18:39
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Úloha: 
 * Popis:   hlavičkový soubor pro font.c
 */

#include "stm8s.h"

/* Font data for Terminus (TTF) 9pt */
extern const uint8_t TerminusBitmaps[];
extern const uint16_t TerminusDescriptors[][2];
extern const char TerminusFontInfo[];

#define CHAR_HEIGHT TerminusFontInfo[0]
#define CHAR_START  TerminusFontInfo[1]
#define CHAR_END    TerminusFontInfo[2]


#endif
