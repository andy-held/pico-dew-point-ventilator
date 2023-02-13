#include <stdio.h>
#include <string.h>

#include <bme280.hpp>
#include <pico/cyw43_arch.h>
#include <pico/stdlib.h>

#include <physics.hpp>
#include <pimoroni_i2c.hpp>
#include <mqtt_client.hpp>

using namespace pimoroni;

void print_reading(const BME280::bme280_reading &reading, float abs_lftf)
{
    printf("%02d %0.2lf deg C, %0.2lf hPa, %0.2lf%%\n",
        reading.status,
        static_cast<double>(reading.temperature),
        static_cast<double>(reading.pressure),
        static_cast<double>(reading.humidity));

    printf("Absolute Luftfeuchtigkeit: %0.6lf kg/cm³\n", static_cast<double>(abs_lftf));
}

constexpr const char* ssid = "";
constexpr const char* pass = "";
constexpr const char* hostname = "";
constexpr const uint32_t port = 1883;
constexpr const char* topic = "absolute_humidity";

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int /*argc*/, const char ** /*argv*/)
{
    I2C i2c(I2C_DEFAULT_SDA, I2C_HEADER_SCL);
    BME280 bme280_sensor0(&i2c);

    I2C i2c2(18, 19);
    BME280 bme280_sensor1(&i2c2);

    stdio_init_all();

    init_wifi(ssid, pass, CYW43_COUNTRY_GERMANY);

    auto client = mqtt_client(hostname, port, topic);

    if (!bme280_sensor0.init()) { printf("Failed to init bme280!\n"); }
    if (!bme280_sensor1.init()) { printf("Failed to init second bme280!\n"); }

    while (true)
    {
        printf("First sensor:\n");
        auto sensor0_reading = bme280_sensor0.read_forced();
        const auto abs_lftf0 = absolute_luftfeuchtigkeit(sensor0_reading.temperature, sensor0_reading.humidity);
        print_reading(sensor0_reading, abs_lftf0);
        client.publish(std::to_string(abs_lftf0));


        printf("Second sensor:\n");
        auto sensor1_reading = bme280_sensor1.read_forced();
        const auto abs_lftf1 = absolute_luftfeuchtigkeit(sensor1_reading.temperature, sensor1_reading.humidity);
        print_reading(sensor1_reading, abs_lftf1);
        client.publish(std::to_string(abs_lftf1));

        sleep_ms(1000);
    }

    return 0;
}
