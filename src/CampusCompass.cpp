#include "CampusCompass.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

CampusCompass::CampusCompass() {
    Graph graph;
    unordered_map<string, Student> students;
    unordered_map<string, Class> classes;
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    //edges
    ifstream edges(edges_filepath);
    if (!edges.is_open()) {
        cout << "Error opening edges file " << edges_filepath << endl;
        return false;
    }
    string line;
    getline(edges, line);
    while (getline(edges, line)) {
        vector<string> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        // LocationID_1,LocationID_2,Name_1,Name_2,Time
        string location1 = row[0];
        string location2 = row[1];
        // string name1 = row[2]; // is name important?
        // string name2 = row[3];
        string time = row[4];
        this->graph.addEdge(stoi(location1), stoi(location2), stoi(time));
    }

    edges.close();

    //read classes
    ifstream classes(classes_filepath);
    if (!classes.is_open()) {
        cout << "Error opening classes file " << classes_filepath << endl;
        return false;
    }
    getline(classes, line);
    while (getline(classes, line)) {
        vector<string> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        // ClassCode,LocationID,Start Time (HH:MM),End Time (HH:MM)
        string classcode = row[0];
        string locationID = row[1];
        string startTime = row[2];
        string endTime = row[3];
        Class c(locationID, startTime, endTime);
        this->classes.emplace(classcode, c);
    }
    classes.close();
    return true;
}

bool CampusCompass::ParseCommand(const string &command) {
    // do whatever regex you need to parse validity
    // hint: return a boolean for validation when testing. For example:
    bool is_valid = true; // replace with your actual validity checking

    return is_valid;
}

Graph& CampusCompass::GetGraph() {
    return this->graph;
}

unordered_map<string, Class> CampusCompass::GetClasses() {
    return this->classes;
}

unordered_map<string, Student> CampusCompass::GetStudents() {
    return this->students;
}
bool CampusCompass::insert (string name, string id, string residence, vector<string> classCodes) {
    return true;
}

bool CampusCompass::remove (string id) {return true;}
bool CampusCompass::dropClass(string id, string classCode) {return true;}
bool CampusCompass::replaceClass(string id, string classCode1, string classCode2) {return true;}
bool CampusCompass::removeClass(string classCode) {return true;}
string CampusCompass::verifySchedule(string id) {return "";}
