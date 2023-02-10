add_executable(picodew
  main.cpp
  bme280.c
  bme280.cpp
  pimoroni_i2c.cpp
  physics.cpp)

target_link_libraries(
  picodew
  PUBLIC
  pico_stdlib
  hardware_i2c
  project_options
  project_warnings)

target_compile_definitions(picodew INTERFACE
  BME280_FLOAT_ENABLE
)

pico_add_extra_outputs(picodew)
