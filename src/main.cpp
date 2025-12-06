#include <iostream>
#include <unordered_map>

#include "CampusCompass.h"

using namespace std;

int main() {
    CampusCompass compass = CampusCompass();
    compass.ParseCSV("data/edges.csv", "data/classes.csv");
    string command;
    getline(cin, command);
    int no_of_lines = stoi(command);

    for (int i = 0; i < no_of_lines; i++) {
        getline(cin, command);
        compass.ParseCommand(command);
    }
}