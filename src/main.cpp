#include "pico/stdlib.h"
#include <cmath>
#include <stdio.h>
#include <string.h>

#include "bme280.hpp"
#include "pimoroni_i2c.hpp"


float magnus_formel(float k1, float k2, float k3, float temperatur)
{
    return k1 * std::pow(10.f, (k2 * temperatur) / (k3 + temperatur));
}

/// @param temperatur °C
/// @return Pa
float saettigungsdampfdruck(float temperatur)
{
    constexpr auto eis_schwelle = 0.f;
    if (temperatur < eis_schwelle)
    {
        // Saettigungsdampdruck ueber Eis
        return magnus_formel(611.2f, 22.46f, 272.62f, temperatur);
    }
    else
    {
        // Saettigungsdampdruck ueber Wasser
        return magnus_formel(611.2f, 17.62f, 243.12f, temperatur);
    }
}

/// @param saettigungsdampfdruck Pa
/// @param relative_luftfeuchtigkeit -
/// @return Pa
float wasserdampf_partialdruck(float saettigungsdampfdruck, float relative_luftfeuchtigkeit)
{
    return saettigungsdampfdruck * relative_luftfeuchtigkeit;
}

/// @param temperatur °C
/// @return K
float kelvin(float temperatur) { return temperatur + 273.15f; }

/// @param wasserdampf_partialdruck Pa
/// @param temperatur_kelvin K
/// @return kg/m³
float absolute_luftfeuchtigkeit(float wasserdampf_partialdruck, float temperatur_kelvin)
{
    constexpr auto gaskonstante_wasserdampf = 461.51f;// J/(kg*K)
    return wasserdampf_partialdruck / (gaskonstante_wasserdampf * temperatur_kelvin);
}

/// @param temperatur °C
/// @param relative_luftfeuchtigkeit -
/// @return kg/m³
float absolute_luftfeuchtigkeit_aus_temp_und_rel(float temperatur, float relative_luftfeuchtigkeit)
{
    const auto sdd = saettigungsdampfdruck(temperatur);
    const auto wp = wasserdampf_partialdruck(sdd, relative_luftfeuchtigkeit);
    return absolute_luftfeuchtigkeit(wp, kelvin(temperatur));
}


float taupunkt(float temperatur, float relative_humidity)
{
    float a, b;
    if (temperatur >= 0.f)
    {
        a = 7.5f;
        b = 237.3f;
    }
    else
    {
        a = 7.6f;
        b = 240.7f;
    }

    // Sättigungsdampfdruck in hPa
    float sdd = 6.1078f * std::pow(10.f, (a * temperatur) / (b + temperatur));

    // Dampfdruck in hPa
    float dd = sdd * (relative_humidity / 100.f);

    // v-Parameter
    float v = std::log10(dd / 6.1078f);

    // Taupunkttemperatur (°C)
    float tt = (b * v) / (a - v);
    return tt;
}

using namespace pimoroni;

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int /*argc*/, const char ** /*argv*/)
{
    I2C i2c(BOARD::PICO_EXPLORER);
    BME280 bme280(&i2c);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    stdio_init_all();

    if (!bme280.init()) { printf("Failed to init bme280!\n"); }

    while (true)
    {
        BME280::bme280_reading result = bme280.read_forced();
        printf("%s %0.2lf deg C, %0.2lf hPa, %0.2lf%%\n",
            result.status == BME280_OK ? "OK" : "ER",
            result.temperature,
            result.pressure,
            result.humidity);

        const auto abs_lftf = absolute_luftfeuchtigkeit_aus_temp_und_rel(result.temperature, result.humidity/100.f);
        printf("Absolute Luftfeuchtigkeit: %0.4lf kg/cm³\n", abs_lftf);
        sleep_ms(1000);
    }

    return 0;
}
