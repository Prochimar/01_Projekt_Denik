

#include "Denik.h"
#include "Kontrola_data.h"
#include "Tyden.h"


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <windows.h>   // knihovna pro barvy v cmd
#include <iomanip> // put_time

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

    char buf[11]; //DD-MM-YYYY (10 znaku + null)
    strftime(buf, sizeof(buf), "%d-%m-%Y", &t); // převádí datum do písemného formátu
    return string(buf);
}

void saveTasks() { //ulozeni ukolu 
    ofstream file(FILENAME);
    for (auto& t : tasks) {
        file << t.title << "|" << t.date << "|" << t.done << "|" << t.priority << "\n";
    }
    file.close();
}

void loadTasks() {   //nahrani/start ukolu
    tasks.clear();
    ifstream file(FILENAME);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        Task t;
        size_t p1 = line.find("|");  // pozice oddělovačů | v řádku
        size_t p2 = line.find("|", p1 + 1);
        size_t p3 = line.find("|", p2 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue; //kdyz chybi oddelovac

        t.title = line.substr(0, p1); //prochazi jednotlive casti ukolu
        t.date = line.substr(p1 + 1, p2 - p1 - 1);
        t.done = stoi(line.substr(p2 + 1, p3 - p2 - 1));
        t.priority = stoi(line.substr(p3 + 1));

        tasks.push_back(t); // Přidá načtený úkol do seznamu
    }
    file.close();
}

// zobrazeni pomoci barev
void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}



//  vypis uloh 
void showTasks(const string& date) {
    cout << "\n Ukoly pro den " << date << ":\n";
    bool found = false;

    for (int i = 0; i < tasks.size(); i++) {  // Prochazi vsechny ukoly v seznamu podle data
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
        cout << "  (Zadne ukoly)\n";
    }
    setColor(7);   // 7-> kod barvy pro bilou
}





void addTask() {
    Task t;
    cout << "Nazev ukolu: ";
    getline(cin, t.title);
    cout << "Datum (DD-MM-YYYY) [" << todayDate() << "]: ";
    getline(cin, t.date);
    

    if (t.date.empty()) t.date = todayDate(); //kdyz nezada zadne datum
   
    if (!jePlatneDatum(t.date)) {
        cout << "Neplatne datum!" << endl;
        return;   
    }
    else {
        cout << "Datum je platne." << endl;
    }

    cout << "Priorita (1 - 5): ";  //-> 1 nejmin podstatny, 5 nejpodstatnejsi ukol
  
    cin >> t.priority;

    //Pro kontrolu vstuput
    
    if (t.priority < 1 || t.priority > 5) {
        cout << "Neplatna priorita! Musi byt v rozmezi 1-5.\n";
        return;
    }



    cin.ignore();
    t.done = false;
    tasks.push_back(t);
    saveTasks();
    cout << "Ukol pridan.\n";
    // ukol je uložen do file jako  t.title << "|" << t.date << "|" << t.done << "|" << t.priority
}







void markTask() {//set as done
    int index;
      cout << "Zadej cislo ukolu: ";
    cin >> index;
    cin.ignore();
    if (index >= 0 && index < tasks.size()) {
        tasks[index].done = !tasks[index].done;
        saveTasks();
        cout << "Ukol oznacen jako " << (tasks[index].done ? "done/finished.\n" : "not finished yet.\n");
    }
    else cout << "Neplatné cislo ukolu.\n";  //kdyz uz bylo dane cislo pouzite na jiny ukol
}



void setPriority() {
    int index;
    cout << "Zadej cislo ukolu: ";
    cin >> index;
    cin.ignore(10000, '\n');

    if (index >= 0 && index < tasks.size()) {

        cout << "Nova priorita (1 - 5): ";//-> 1 nejmin podstatny, 5 nejpodstatnejsi ukol
        int newPriority;
        cin >> newPriority;
        cin.ignore(10000, '\n');

        // kontrola rozsahu
        if (newPriority < 1 || newPriority > 5) {
            cout << "Neplatna priorita! Musi byt v rozmezi 1–5.\n";
            return;   
        }

        // uložení nové priority
        tasks[index].priority = newPriority;
        saveTasks();
        cout << "Priorita nastavena.\n";
    }
    else {
        cout << "Cislo neplati.\n";//kdyz uz bylo dane cislo pouzite
    }
}



void copyTask() {
    int index;
    cout << "Zadej cislo ukolu: ";
    cin >> index;
    cin.ignore();
    if (index >= 0 && index < tasks.size()) {
        Task copy = tasks[index];
        cout << "Zadej nove datum (DD-MM-YYY): ";
        getline(cin, copy.date);
        tasks.push_back(copy);
        saveTasks();
        cout << "Ukol zkopirovan.\n";
    }
    else cout << "Neplatne cislo.\n";           //kdyz uz bylo dane cislo pouzite
}





void moveTask() {
    int index;
    cout << "Zadej cislo ukolu: ";
    cin >> index;
    cin.ignore();
    if (index >= 0 && index < tasks.size()) {
        cout << "zadej novy datum (YYYY-MM-DD): ";
        getline(cin, tasks[index].date);
        saveTasks();
        cout << "Ukol byl presunuty.\n";
    }
    else cout << "neplatné cislo.\n";       //kdyz uz bylo dane cislo pouzite
}





void showWeek() {
    setColor(11);
    cout << "\n       --- Prehled tohoto tydne ---\n";
    setColor(7);

    // využití knihovny Tyden pro výpis aktuálního týdne
    time_t monday, sunday;
    getCurrentWeek(monday, sunday);

    std::cout << "Tento tyden je od: " << formatDate(monday)
              << " do " << formatDate(sunday) << std::endl << "\n";

    bool necoNalezeno = false;
    int idx = 1;  // číslování úkolů

    for (auto& t : tasks) {
        time_t td = PrevodData(t.date);

        if (td >= monday && td <= sunday) {
            cout << idx << ") " << t.date << " - " << t.title << "\n";
            idx++;
            necoNalezeno = true;
        }
    }

    if (!necoNalezeno) {
        cout << "V tomto tydnu nemas zadne ukoly.\n";
    }
}




// hlavni menu --> vysvetlivky
int main() {
    loadTasks();
    string currentDate = todayDate();
    int volba;

    while (true) {
        setColor(11);           // 11-> kod barvy pro tyrkisova
        cout << "\n ---- Elektronicky ukolovnik ----";
        cout << "\n    ---- Prochazka, Prokes ----\n";
        setColor(7);       // 7->kod barvy pro bilou
        cout << "1. Zobrazit ukoly pro dany den\n";
        cout << "2. Pridat ukol\n";
        cout << "3. Oznacit ukol jako hotovy\n";
        cout << "4. Nastavit prioritu\n";
        cout << "5. Kopirovat ukol na jiny datum\n";
        cout << "6. Presunout ukol na jiny datum\n";
        cout << "7. Zobrazit prehled celeho tydne\n";
        cout << "8. Konec\n";
        cout << "volba: ";
        

        if (!(cin >> volba)) { //Aby se to nerozbylo když zadám písmeno
            cin.clear();
            cin.ignore(10000, '\n'); //Smaže maximální délku bufferu aby tam nezbyla nějk písmena
            cout << "Neplatna volba! Zadej cislo.\n";
            continue;
        }
        cin.ignore(10000, '\n');

       
        //využití knihovny Tyden pro vypis aktualního tydne
       
        


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
            showWeek();
            markTask();
            break;
        case 4:
            system("cls");
            showWeek();
            setPriority();
            break;
        case 5:
            system("cls");
            showWeek();
            copyTask();
            break;
        case 6:
            system("cls");
            showWeek();
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