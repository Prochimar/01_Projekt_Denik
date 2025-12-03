#include "Tyden.h"
#include <iomanip>
#include <sstream>
#include <iostream>

// Vrátí timestamp pondìlí a nedìle aktuálního týdne
void getCurrentWeek(time_t& monday, time_t& sunday) {
    time_t now = time(nullptr);
    tm tnow = *localtime(&now);

    tnow.tm_hour = 0;
    tnow.tm_min = 0;
    tnow.tm_sec = 0;

    time_t today = mktime(&tnow);

    int dayOfWeek = tnow.tm_wday;
    if (dayOfWeek == 0) dayOfWeek = 7; // nedìle = 7

    monday = today - (dayOfWeek - 1) * 24 * 60 * 60;
    sunday = monday + 6 * 24 * 60 * 60;
}

// Pøevod timestampu na DD-MM-YYYY
std::string formatDate(time_t t) {
    tm* ptm = localtime(&t);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y", ptm);
    return std::string(buffer);
}

// Pøevod stringu DD-MM-YYYY na time_t
time_t PrevodData(const std::string& date) {
    tm t = {};
    t.tm_mday = std::stoi(date.substr(0, 2));    // Pøeète den (první 2 znaky) [1. arugment poèáteèní znak 2. argument poèet znakù]
    t.tm_mon = std::stoi(date.substr(3, 2)) - 1; // mìsíc (0-11)
    t.tm_year = std::stoi(date.substr(6, 4)) - 1900; // rok od 1900
    t.tm_hour = 0; t.tm_min = 0; t.tm_sec = 0; // pùlnoc
    return mktime(&t);
}