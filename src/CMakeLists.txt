add_executable(picodew main.cpp bme280.c)
target_link_libraries(
  picodew
  PUBLIC project_options project_warnings
  pico_stdlib)

pico_add_extra_outputs(picodew)
