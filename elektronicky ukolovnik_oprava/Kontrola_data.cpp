#include "Kontrola_data.h"
#include <string>
#include <cstdlib>
#include <iostream>
//#include <algorithm> // Pro std::all_of


bool jePrestupny(int rok) {
    return (rok % 4 == 0 && rok % 100 != 0) || (rok % 400 == 0);
}

//Základní kontrola delky
bool jePlatneDatum(const std::string& date) {
    if (date.size() != 10) {
        return false;
    }

    //Kontrola formátu DD-MM-YYYY 
    if (date[2] != '-' || date[5] != '-') {
        return false;
    }


    try {
        int den = std::stoi(date.substr(0, 2));  // DD
        int mesic = std::stoi(date.substr(3, 2)); // MM
        int rok = std::stoi(date.substr(6, 4));  // YYYY

        //Kontrola rozsahu mesice a roku
        if (rok < 1 || mesic < 1 || mesic > 12) {
            return false;
        }

        // 6. Urceni maximalniho poctu dni v mesici
        int maxDni;
        if (mesic == 2) {
            maxDni = jePrestupny(rok) ? 29 : 28;
        }
        else if (mesic == 4 || mesic == 6 || mesic == 9 || mesic == 11) {
            maxDni = 30;
        }
        else {
            maxDni = 31;
        }

        if (den < 1 || den > maxDni) {
            return false;
        }

        return true;
    }
    catch (const std::invalid_argument& e) {
        //Nevhodne znaky misto cisel
        return false;
    }
    catch (const std::out_of_range& e) {
        //cislo je prilis velke
        return false;
    }
}




















/*
#include "Kontrola_data.h"
#include <string>
#include <cstdlib>


bool jePlatneDatum(const std::string& date) {
    if (date.size() != 10) return false;
    if (date[5] != '-' || date[8] != '-') return false;

    int den = std::stoi(date.substr(0, 2)); //DD
    int mesic = std::stoi(date.substr(3, 2)); //MM
    int rok = std::stoi(date.substr(6, 4)); //YYYY

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
*/