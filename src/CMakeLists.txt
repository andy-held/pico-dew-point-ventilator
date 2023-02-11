add_executable(picodew
  main.cpp
  3rdparty/BME280_driver/bme280.c
  bme280.cpp
  pimoroni_i2c.cpp
  physics.cpp)

target_include_directories(picodew PRIVATE
  3rdparty/BME280_driver)

target_link_libraries(picodew PUBLIC
  pico_stdlib
  hardware_i2c
  project_options
  project_warnings)

target_compile_definitions(picodew PUBLIC
  BME280_FLOAT_ENABLE
)

pico_add_extra_outputs(picodew)
