#include "Kontrola_data.h"
#include <string>
#include <cstdlib>

bool jePlatneDatum(const std::string& date) {
    if (date.size() != 10) return false;
    if (date[2] != '.' || date[5] != '.') return false;

    int den = std::stoi(date.substr(0, 2));
    int mesic = std::stoi(date.substr(3, 2));
    int rok = std::stoi(date.substr(6, 4));

    if (mesic < 1 || mesic > 12) return false;
    if (den < 1 || den > 31) return false;

    // mìsíce s 30 dny
    if ((mesic == 4 || mesic == 6 || mesic == 9 || mesic == 11) && den > 30)
        return false;

    // únor (bez pøestupného roku)
    if (mesic == 2 && den > 29)
        return false;

    return true;
}