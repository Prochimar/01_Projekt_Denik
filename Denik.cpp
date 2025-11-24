

#include "Denik.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <windows.h>   // knihovna pro barvy v cmd

using namespace std;

struct Task {
    string title;
    string date;
    bool done = false;
    int priority = 0;
};

vector<Task> tasks;
const string FILENAME = "ukoly.txt";

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// pomocne funkce --> datum, ulozeni, nahrani/start ukolu



string todayDate() {//nastaveni automatickeho datumu
    time_t now = time(nullptr); //Vráctí aktuální čas nullptr neukládej do proměnné jen mi vrať čas
    tm t;
    localtime_s(&t, &now);// Uložení daného času do t

    char buf[11]; //YYYY-MM-DD
    strftime(buf, sizeof(buf), "%Y-%m-%d", &t); // převádí datum do písemného formátu
    return string(buf);
}
 b  
void saveTasks() {                   //ulozeni ukolu
    ofstream file(FILENAME);
    for (auto& t : tasks) {
        file << t.title << "|" << t.date << "|" << t.done << "|" << t.priority << "\n";
    }
    file.close();
}

void loadTasks() {                  //nahrani/start ukolu
    tasks.clear();
    ifstream file(FILENAME);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        Task t;
        size_t p1 = line.find("|");
        size_t p2 = line.find("|", p1 + 1);
        size_t p3 = line.find("|", p2 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        t.title = line.substr(0, p1);
        t.date = line.substr(p1 + 1, p2 - p1 - 1);
        t.done = stoi(line.substr(p2 + 1, p3 - p2 - 1));
        t.priority = stoi(line.substr(p3 + 1));

        tasks.push_back(t);
    }
    file.close();
}

// zobrazeni pomoci barev
void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

//  vypis uloh 
void showTasks(const string& date) {
    cout << "\n ukoly pro den " << date << ":\n";
    bool found = false;

    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].date == date) {
            found = true;

            if (tasks[i].done) setColor(10); // zelena -> hotovo            //10 kod barvy pro zelenou
            else if (tasks[i].priority >= 4) setColor(12); // cervena -> udelat co nejdriv              //12 kod barvy pro cervenou
            else if (tasks[i].priority >= 2) setColor(14); // zluta -> cas dokonceni ukolu je za dlouho     //14 kod barvy pro zlutou
            else setColor(7); // bila pro ostatni ukoly         //7 kod barvy pro bilou

            cout << i << ") "
                << (tasks[i].done ? "[x] " : "[ ] ")
                << tasks[i].title
                << " (priorita: " << tasks[i].priority << ")\n";
        }
    }

    if (!found) {
        setColor(7);        // 7-> kod barvy pro bilou
        cout << "  (zadne ukoly)\n";
    }
    setColor(7);   // 7-> kod barvy pro bilou
}

void addTask() {
    Task t;
    cout << "nazev ukolu: ";
    getline(cin, t.title);
    cout << "datum (YYYY-MM-DD) [" << todayDate() << "]: ";
    getline(cin, t.date);
    if (t.date.empty()) t.date = todayDate();
    cout << "priorita (0–5): ";  //-> 1 nejmin podstatny, 5 nejpodstatnejsi ukol
    cin >> t.priority;
    cin.ignore();
    t.done = false;
    tasks.push_back(t);
    saveTasks();
    cout << "ukol pridan.\n";
}

void markTask() {
    int index;
    cout << "zadej cislo ukolu: ";
    cin >> index;
    cin.ignore();
    if (index >= 0 && index < tasks.size()) {
        tasks[index].done = !tasks[index].done;
        saveTasks();
        cout << "ukol oznacen jako " << (tasks[index].done ? "done/finished.\n" : "not finished yet.\n");
    }
    else cout << "neplatné cislo ukolu.\n";  //kdyz uz bylo dane cislo pouzite na jiny ukol
}

void setPriority() {
    int index;
    cout << "zadej cislo ukolu: ";
    cin >> index;
    cin.ignore();
    if (index >= 0 && index < tasks.size()) {
        cout << "nova priorita (0–5): ";            //-> 1 nejmin podstatny, 5 nejpodstatnejsi ukol
        cin >> tasks[index].priority;
        cin.ignore();
        saveTasks();
        cout << "priorita nastavena.\n";
    }
    else cout << "cislo neplati.\n";               //kdyz uz bylo dane cislo pouzite
}

void copyTask() {
    int index;
    cout << "zadej cislo ukolu: ";
    cin >> index;
    cin.ignore();
    if (index >= 0 && index < tasks.size()) {
        Task copy = tasks[index];
        cout << "zadej novy datum (YYYY-MM-DD): ";
        getline(cin, copy.date);
        tasks.push_back(copy);
        saveTasks();
        cout << "ukol zkopirovan.\n";
    }
    else cout << "neplatne cislo.\n";           //kdyz uz bylo dane cislo pouzite
}

void moveTask() {
    int index;
    cout << "Zadej číslo úkolu: ";
    cin >> index;
    cin.ignore();
    if (index >= 0 && index < tasks.size()) {
        cout << "zadej novy datum (YYYY-MM-DD): ";
        getline(cin, tasks[index].date);
        saveTasks();
        cout << "ukol byl presunuty.\n";
    }
    else cout << "neplatné cislo.\n";       //kdyz uz bylo dane cislo pouzite
}

void showWeek() {
    cout << "\n prehled celeho tydne (pocet ukolu):\n";
    vector<string> dates;

    for (auto& t : tasks) {
        bool exists = false;
        for (auto& d : dates)
            if (d == t.date) exists = true;
        if (!exists) dates.push_back(t.date);
    }

    for (auto& d : dates) {
        int count = 0;
        for (auto& t : tasks) if (t.date == d) count++;
        cout << d << ": " << count << " ukol(u)\n";
    }
}

// hlavni menu --> vysvetlivky
int main() {
    loadTasks();
    string currentDate = todayDate();
    int volba;

    while (true) {
        setColor(11);           // 11-> kod barvy pro tyrkisova
        cout << "\n ---- elektronicky ukolovnik ----\n";
        setColor(7);       // 7->kod barvy pro bilou
        cout << "1. zobrazit ukoly pro dany den\n";
        cout << "2. pridat ukol\n";
        cout << "3. oznacit ukol jako hotovy\n";
        cout << "4. nastavit prioritu\n";
        cout << "5. kopirovat ukol na jiny datum\n";
        cout << "6. presunout ukol na jiny datum\n";
        cout << "7. zobrazit prehled celeho tydne\n";
        cout << "8. konec\n";
        cout << "volba: ";
        

        if (!(cin >> volba)) { //Aby se to nerozbylo když zadám písmeno
            cin.clear();
            cin.ignore(10000, '\n'); //Smaže maximální délku bufferu aby tam nezbyla nějk písmena
            cout << "Neplatna volba! Zadej cislo.\n";
            continue;
        }
        cin.ignore(10000, '\n');

       
        

        switch (volba) {
        case 1:
            system("cls");
            showTasks(currentDate);
            break;
        case 2:
            system("cls");
            addTask();
            break;
        case 3:
            system("cls");
            markTask();
            break;
        case 4:
            system("cls");
            setPriority();
            break;
        case 5:
            system("cls");
            copyTask();
            break;
        case 6:
            system("cls");
            moveTask();
            break;
        case 7:
            system("cls");
            showWeek();
            break;
        case 8:
            system("cls");
            setColor(7);
            cout << "konec programu.\n";     //ukoncit program --> zavrit
            saveTasks();
            return 0;
        default:
            cout << "neplatna volba:P.\n";   //co jsi si zvolil neplati
        }
    }
}