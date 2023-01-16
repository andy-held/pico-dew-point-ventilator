#pragma once
#include "pico/stdlib.h"
#include <climits>
#include <stdint.h>

#define PIMORONI_I2C_DEFAULT_INSTANCE i2c0
#define PIMORONI_SPI_DEFAULT_INSTANCE spi0

namespace pimoroni
{
constexpr unsigned int PIN_UNUSED = INT_MAX;// Intentionally INT_MAX to avoid overflowing MicroPython's int type

// I2C
constexpr unsigned int I2C_DEFAULT_BAUDRATE = 400000;
constexpr unsigned int I2C_DEFAULT_SDA = 20;
constexpr unsigned int I2C_DEFAULT_SCL = 21;
constexpr unsigned int I2C_DEFAULT_INT = 22;

constexpr unsigned int I2C_BG_SDA = 4;
constexpr unsigned int I2C_BG_SCL = 5;
constexpr unsigned int I2C_BG_INT = 3;

constexpr unsigned int I2C_HEADER_SDA = 20;
constexpr unsigned int I2C_HEADER_SCL = 21;
constexpr unsigned int I2C_HEADER_INT = 19;

// SPI
constexpr unsigned int SPI_DEFAULT_MOSI = 19;
constexpr unsigned int SPI_DEFAULT_MISO = 16;
constexpr unsigned int SPI_DEFAULT_DC = 16;
constexpr unsigned int SPI_DEFAULT_SCK = 18;

constexpr unsigned int SPI_BG_FRONT_PWM = 20;
constexpr unsigned int SPI_BG_FRONT_CS = 17;

constexpr unsigned int SPI_BG_BACK_PWM = 21;
constexpr unsigned int SPI_BG_BACK_CS = 22;

enum BG_SPI_SLOT
{
    BG_SPI_FRONT,
    BG_SPI_BACK,
    PICO_EXPLORER_ONBOARD
};
}
