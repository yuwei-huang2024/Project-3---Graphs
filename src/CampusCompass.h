#pragma once
#include <string>
#include <vector>
#include "Student.h"
#include "Class.h"
#include "Graph.h"
#include "unordered_map"

using namespace std;

class CampusCompass {
    Graph graph;
    unordered_map<string, Student> students;
    unordered_map<string, Class> classes;
public:
    CampusCompass();
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);
    bool insert (string name, string id, string residence, vector<string> classCodes);
    bool remove (string id);
    bool dropClass(string id, string classCode);
    bool replaceClass(string id, string classCode1, string classCode2);
    bool removeClass(string classCode);
    string verifySchedule(string id);
};
