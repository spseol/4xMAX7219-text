#include "swspi.h"


void swspi_init(void)
{
    GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
}

// Odesílá 16bit dat MSB first, SPI mode, sama provádí manipulaci s CS
void swspi_tx16(uint16_t data)
{
    uint16_t maska = 1U << 15;

    CS_L;
    while (maska) {
        if (maska & data) {
            DIN_H;
        } else {
            DIN_L;
        }
        CLK_H;
        maska = maska >> 1;
        CLK_L;
    }
    CS_H;
}


void swspi_tx2x8(uint8_t address, uint8_t data)
{
    uint16_t mask;

    // adresa
    mask = 1 << 7;
    while (mask) {
        CLR(CLK);
        if (address & mask) {
            SET(DIN);
        } else {
            CLR(DIN);
        }
        SET(CLK);
        mask >>= 1;
        CLR(CLK);
    }

    // data 
    mask = 1 << 7;
    while (mask) {
        CLR(CLK);
        if (data & mask) {
            SET(DIN);
        } else {
            CLR(DIN);
        }
        SET(CLK);
        mask >>= 1;
        CLR(CLK);
    }

    //SET(CS);                    // končím vysílán
}

void swspi_tx_1A1D(uint8_t address, uint8_t data)
{
    TX_START;
    swspi_tx2x8(address, data);
    TX_STOP;
}

void swspi_tx_1A4D(uint8_t address, uint32_t data)
{
    TX_START;
    swspi_tx2x8(address, data >> 24);
    swspi_tx2x8(address, data >> 16);
    swspi_tx2x8(address, data >> 8);
    swspi_tx2x8(address, data);
    TX_STOP;
}

void swspi_tx_4conf(uint8_t address, uint8_t data)
{
    TX_START;
    swspi_tx2x8(address, data);
    swspi_tx2x8(address, data);
    swspi_tx2x8(address, data);
    swspi_tx2x8(address, data);
    TX_STOP;
}
