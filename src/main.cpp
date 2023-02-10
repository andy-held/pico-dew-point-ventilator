#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

#include "bme280.hpp"
#include "pimoroni_i2c.hpp"

#include "physics.hpp"

using namespace pimoroni;

void print_reading(const BME280::bme280_reading& reading)
{
    printf("%02d %0.2lf deg C, %0.2lf hPa, %0.2lf%%\n",
        reading.status,
        static_cast<double>(reading.temperature),
        static_cast<double>(reading.pressure),
        static_cast<double>(reading.humidity));

    const auto abs_lftf = absolute_luftfeuchtigkeit(reading.temperature, reading.humidity);
    printf("Absolute Luftfeuchtigkeit: %0.6lf kg/cm³\n", static_cast<double>(abs_lftf));
}

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int /*argc*/, const char ** /*argv*/)
{
    I2C i2c(I2C_DEFAULT_SDA, I2C_HEADER_SCL);
    BME280 bme280(&i2c);

    I2C i2c2(18, 19);
    BME280 bme280_2(&i2c2);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    stdio_init_all();

    if (!bme280.init()) { printf("Failed to init bme280!\n"); }
    if (!bme280_2.init()) { printf("Failed to init second bme280!\n"); }

    while (true)
    {
        printf("First sensor:\n");
        print_reading(bme280.read_forced());

        printf("Second sensor:\n");
        print_reading(bme280_2.read_forced());

        sleep_ms(1000);
    }

    return 0;
}
