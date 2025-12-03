#pragma once
#include <ctime>
#include <string>

// Funkce pro zjištìní pondìlí a nedìle aktuálního týdne
void getCurrentWeek(time_t& monday, time_t& sunday);

// Pomocná funkce pro pøevod timestampu na string DD-MM-YYYY
std::string formatDate(time_t t);

// Pøevod stringu DD-MM-YYYY na time_t
time_t PrevodData(const std::string& date);