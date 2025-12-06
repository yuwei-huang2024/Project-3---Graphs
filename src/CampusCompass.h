#pragma once
#include <string>
#include <vector>
#include "Student.h"
#include "Class.h"
#include "Graph.h"
#include "unordered_map"
#include "map"

using namespace std;

struct classGap {
    string fromClass;
    string toClass;
    bool canMake;

    classGap(string from, string to, bool canMake) {
        this->fromClass = from;
        this->toClass = to;
        this->canMake = canMake;
    }

    string getFromClass() {return fromClass;}
    string getToClass() {return toClass;}
    bool getCanMake() {return canMake;}
};

class CampusCompass {
    Graph graph;
    unordered_map<string, Student> students;
    unordered_map<string, Class> classes;
public:
    CampusCompass();
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);
    Graph& GetGraph();
    unordered_map<string, Student> GetStudents();
    unordered_map<string, Class> GetClasses();
    bool checkID (string id);
    string insert (const string &command);
    string remove (string id);
    string dropClass(string id, string classCode);
    string replaceClass(string id, string classCode1, string classCode2);
    int removeClass(string classCode);
    string toggleEdgesClosure(int n, vector<int> edges);
    string checkEdgeStatus(int edge1, int edge2);
    string isConnected(int edge1, int edge2);
    map<string, int> printShortestEdges(string id);
    int printStudentZone(string id);
    vector<classGap> verifySchedule(string id);
    bool isClassCode (string code);
};
