#pragma once

/// @param temperatur °C
/// @param relative_luftfeuchtigkeit [0-100]%
/// @return kg/m³
[[nodiscard]] float absolute_luftfeuchtigkeit(float temperatur, float relative_luftfeuchtigkeit);
