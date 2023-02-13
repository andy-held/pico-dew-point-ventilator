add_executable(picodew
  main.cpp
  3rdparty/BME280_driver/bme280.c
  bme280.cpp
  pimoroni_i2c.cpp
  physics.cpp
  mqtt_client.cpp)

target_include_directories(picodew PRIVATE
  3rdparty/BME280_driver
  ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(picodew PUBLIC
  pico_cyw43_arch_lwip_threadsafe_background
  pico_stdlib
  pico_lwip_mqtt
  hardware_i2c
  hardware_exception
  project_options
  project_warnings)

target_compile_definitions(picodew PUBLIC
  BME280_FLOAT_ENABLE
)

pico_add_extra_outputs(picodew)
