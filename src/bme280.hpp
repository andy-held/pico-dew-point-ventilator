#pragma once

#include "bme280.h"
#include "bme280_defs.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pimoroni_i2c.hpp"
#include "stdio.h"

#include <tuple>

namespace pimoroni
{
constexpr uint8_t DEFAULT_I2C_ADDRESS = 0x76;
constexpr uint8_t ALTERNATE_I2C_ADDRESS = 0x77;
constexpr uint8_t DEFAULT_INT_PIN = I2C_DEFAULT_INT;

class BME280
{
  public:

    struct bme280_reading
    {
        float temperature;
        float pressure;
        float humidity;
        int status;
    };

    bool init();
    bool configure(uint8_t filter,
        uint8_t standby_time,
        uint8_t os_pressure,
        uint8_t os_temp,
        uint8_t os_humidity,
        uint8_t mode = BME280_FORCED_MODE);

    BME280() : BME280(new I2C()) {}
    BME280(uint8_t address_in) : BME280(new I2C(), address_in) {}
    BME280(I2C *i2c_in, uint8_t address_in = DEFAULT_I2C_ADDRESS, uint interrupt_in = PIN_UNUSED)
        : i2c(i2c_in), address(address_in), interrupt(interrupt_in)
    {}

    bme280_reading read();
    BME280::bme280_reading read_forced();

  private:
    bme280_dev device;

    I2C *i2c;

    // interface pins with our standard defaults where appropriate
    int8_t address = DEFAULT_I2C_ADDRESS;
    uint interrupt = DEFAULT_INT_PIN;
    std::tuple<I2C *, int8_t> callback_data;
};
}// namespace pimoroni
