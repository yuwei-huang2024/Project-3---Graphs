#pragma once
#include <string>
#include <vector>

using namespace std;

class Student {
    string name;
    string id;
    int residence;
    vector<string> classes;
public:
    Student(string gName, string gID, int gResidence);
    bool addClass(string code);
    bool dropClass(string code);
    bool isClass(string code);
    bool replaceClass(string class1, string class2);
    vector<string> getClasses();
    string getName();
    string getID();
    int getResidence();
};