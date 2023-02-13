#pragma once

#include <tuple>
#include <string>

#include <pico/cyw43_arch.h>
#include <pico/stdlib.h>

#include <lwip/apps/mqtt.h>

template<typename T>
std::tuple<const void*, uint32_t> get_data_view(const T& data)
{
    return {static_cast<const void*>(&data), sizeof(T)};
}

template<>
std::tuple<const void*, uint32_t> get_data_view<std::string>(const std::string& data);

void init_wifi(const char *ssid, const char *pass, uint32_t country, uint32_t auth = CYW43_AUTH_WPA2_AES_PSK, const uint32_t timeout = 10000);

struct mqtt_client
{
    mqtt_client(const char* hostname, const uint32_t port, const char* topic);

    void publish(const void* data, uint32_t data_len);

    template<typename T>
    void publish(const T& data)
    {
        auto [ptr, len] = get_data_view(data);
        publish(ptr, len);
    }

    const char* topic;
    ip_addr_t remote_addr;
    mqtt_client_t* lwip_mqtt_client;
    uint8_t receiving = 0;
    uint32_t received = 0;
    uint32_t counter = 0;
    uint32_t reconnect;
};