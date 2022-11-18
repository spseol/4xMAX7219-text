#include <stdbool.h>

#include "stm8s.h"
#include "milis.h"

#include "swspi.h"
#include "font.h"

//#include <stdio.h>
//#include "uart1.h"

#define _ISOC99_SOURCE
#define _GNU_SOURCE

#define LED_PORT GPIOD
#define LED_PIN  GPIO_PIN_0
#define LED_HIGH   GPIO_WriteHigh(LED_PORT, LED_PIN)
#define LED_LOW  GPIO_WriteLow(LED_PORT, LED_PIN)
#define LED_REVERSE GPIO_WriteReverse(LED_PORT, LED_PIN)

uint32_t videoram[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    init_milis();
    swspi_init();
    //init_uart1();

    swspi_tx_4conf(DECODE_MODE, DECODE_NONE);
    swspi_tx_4conf(SCAN_LIMIT, 7); // velikost displeje 8 cifer (počítáno od nuly, proto je argument číslo 7)
    swspi_tx_4conf(INTENSITY, 1);  // volíme ze začátku nízký jas (vysoký jas může mít velkou spotřebu - až 0.25A !)
    swspi_tx_4conf(DISPLAY_TEST, DISPLAY_TEST_OFF);        // 
    swspi_tx_4conf(SHUTDOWN, DISPLAY_ON);  // zapneme displej

}

const char string[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
//const char string[] = "Mikroprocesor STM8S105 a radic MAX7219 vas vitaji na dni otevrenych dveri...   ";
uint8_t index_char = 0;
uint8_t index_col = 0;
uint8_t order_char;  // pořadí znaku
uint8_t width_char;  // šířka znaku
uint16_t start_char; // začátek znaku v Bitmapě
uint8_t bit_char;    // bit -- sloupec v jednom znaku


void refresh(void)
{

    for (uint8_t i = 0; i < 8; ++i) {
        videoram[i] <<= 1;      // posun o bit doleva
        bool bit =
            TerminusBitmaps[start_char + i] & (0b10000000 >> bit_char);
        videoram[i] |= bit;
    }
    bit_char++;
    if (bit_char > width_char) {

        if (string[++index_char] == '\0') {
            index_char = 0;
        }
        order_char = string[index_char] - CHAR_START;   // pořadí znaku
        width_char = TerminusDescriptors[order_char][0];        // šířka znaku
        start_char = TerminusDescriptors[order_char][1];        // začátek znaku Bitmapách
        bit_char = 0;
    }
    // zobrazím videoram
    for (uint8_t i = 0; i < 8; ++i) {
        swspi_tx_1A4D(i + 1, videoram[i]);
    }

}

int main(void)
{
    uint32_t time = 0;
    setup();


    // poloha a velikost znaku
    order_char = string[index_char] - CHAR_START;       // pořadí znaku
    width_char = TerminusDescriptors[order_char][0];    // šířka znaku
    start_char = TerminusDescriptors[order_char][1];    // začátek znaku Bitmapách
    bit_char = 0;

    while (1) {
        if (milis() - time > 111) {
            time = milis();
            LED_REVERSE;
            refresh();
        }
    }

}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
