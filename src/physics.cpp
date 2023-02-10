#include <cmath>

struct MagnusKoeffizienten
{
    float k1; // hPa
    float k2; // einheitslos
    float k3; // C°
};

/// @param temperatur C°
/// @return hPa
[[nodiscard]] float magnus_formel(const MagnusKoeffizienten koeff, const float temperatur) noexcept
{
    return koeff.k1 * std::pow(10.f, (koeff.k2 * temperatur) / (koeff.k3 + temperatur));
}

/// @param temperatur °C
/// @return hPa
[[nodiscard]]float saettigungsdampfdruck(const float temperatur) noexcept
{
    constexpr auto eis_schwelle = 0.f;
    // Koeffizienten von [Arden Buck 1981](https://web.archive.org/web/20160304093405/http://www.public.iastate.edu/~bkh/teaching/505/arden_buck_sat.pdf)
    constexpr MagnusKoeffizienten ueber_null{6.1121, 17.368, 238.88};
    constexpr MagnusKoeffizienten unter_null{6.1121, 17.966, 247.15};
    if (temperatur >= eis_schwelle)
    {
        // Saettigungsdampdruck ueber Wasser
        return magnus_formel(ueber_null, temperatur);
    }
    else
    {
        // Saettigungsdampdruck ueber Eis
        return magnus_formel(unter_null, temperatur);
    }
}

/// @param saettigungsdampfdruck hPa
/// @param relative_luftfeuchtigkeit [0-100]%
/// @return hPa
[[nodiscard]] float wasserdampf_partialdruck(const float saettigungsdampfdruck, const float relative_luftfeuchtigkeit) noexcept
{
    return saettigungsdampfdruck * relative_luftfeuchtigkeit;
}

/// @param temperatur °C
/// @return K
[[nodiscard]] float kelvin(float temperatur) noexcept { return temperatur + 273.15f; }

[[nodiscard]]float absolute_luftfeuchtigkeit(float temperatur, float relative_luftfeuchtigkeit) noexcept
{
    const auto sdd = saettigungsdampfdruck(temperatur);
    const auto wp = wasserdampf_partialdruck(sdd, relative_luftfeuchtigkeit);

    constexpr auto gaskonstante_wasserdampf = 461.51f;// J/(kg*K)
    return wp / (gaskonstante_wasserdampf * kelvin(temperatur));
}
