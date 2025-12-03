#include "CampusCompass.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

CampusCompass::CampusCompass() {
    Graph graph;
    unordered_map<string, Student> students; //id to student
    unordered_map<string, Class> classes; //id to class?
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

bool returnHelper(string results) {
    if (results == "successful") {return true;}
    return false;
}

bool CampusCompass::ParseCommand(const string &command) {
    stringstream ss(command);
    string word;
    ss >> word;

    if (word == "insert") {
        string results = insert(command);
        cout << results << endl;
        return returnHelper(results);
    }

    else if (word == "remove") {
        string ID;
        ss >> ID;
        string results = remove(ID);
        cout << results << endl;
        return returnHelper(results);
    }
    else if (word == "dropClass") {
        string ID;
        string classID;
        ss >> ID >> classID;
        string results = dropClass(ID, classID);
        cout << results << endl;
        return returnHelper(results);
    }
    else if (word == "replaceClass") {
        string ID, classcode1, classcode2;
        ss >> ID >> classcode1 >> classcode2;
        string results = replaceClass(ID, classcode1, classcode2);
        cout << results << endl;
        return returnHelper(results);
    }
    else if (word == "removeClass") {
        string classcode;
        ss >> classcode;
        int results = removeClass(classcode);
        cout << results << endl;
        return true;
    }
    else if (word == "toggleEdgesClosure") {
        int n;
        int location;
        vector<int> edges;
        ss >> n;
        for (int i = 0; i <= n; i++) {
            ss >> location;
            edges.push_back(location);
        }
        string results = toggleEdgesClosure(n, edges);
        cout << results << endl;
        return returnHelper(results);
    }
    else if (word == "checkEdgeStatus") {
        int edge1, edge2;
        ss >> edge1 >> edge2;
        string results = checkEdgeStatus(edge1, edge2);
        cout << results << endl;
        return true;
    }
    else if (word == "isConnected") {
        int edge1, edge2;
        ss >> edge1 >> edge2;
        string results = isConnected(edge1, edge2);
        cout << results << endl;
        return returnHelper(results);
    }
    else if (word == "printShortestEdges") {
        string ID;
        ss >> ID;
        map<string, int> times = printShortestEdges(ID);
        auto it = students.find(ID);
        Student &s = it->second;
        // [ClassCode1] | Total Time: [Time]
        cout << "Name: " << s.getName() << endl;
        for (auto it = times.begin(); it != times.end(); it++) {
            cout << it->first << " | Total Time: " << it->second << endl;
        }
        return true;
    }
    else if (word == "printStudentZone") {
        cout << "printStudentZone" << endl;
    }
    else if (word == "verifySchedule") {
        cout << "verifySchedule" << endl;
    }
    cout << "unsuccessful" << endl;
    return false;
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

bool CampusCompass::checkID (string ID) { //return true if exists
    if (students.find(ID) != students.end()) {
        return true;
    }
    return false;
}

string CampusCompass::insert (const string &command) {
    //get first word, don't need
    vector<string> row;
    stringstream ss(command);
    string word;
    ss >> word;
    row.push_back(word);

    //get name
    char quote;
    ss >> quote;
    string name;
    getline(ss, name, '"');
    row.push_back(name);

    //get everything else
    while (ss >> word) {
        row.push_back(word);
    }

    string id = row[2];
    int location = stoi(row[3]);
    int num = stoi(row[4]);

    if (checkID (id)) {
        return "unsuccessful";
    }

    Student newStudent(name, id, location);
    for (int i = 0; i < num; i++) {
        newStudent.addClass(row[5 + i]);
    }
    this->students.emplace(id, newStudent);
    return "successful";
}

string CampusCompass::remove (string ID) {
    if (!checkID(ID)) {
        return "unsuccessful";
    }
    students.erase(ID);
    return "successful";
}

string CampusCompass::dropClass(string ID, string classCode) {
    //not a class code
    if (classes.find(classCode) == classes.end()) {
        return "unsuccessful";
    }
    auto it = students.find(ID);
    //not a student
    if (it == students.end()) {
        return "unsuccessful";
    }
    Student &s = it->second;
    //not a class with this student
    if (!(s.isClass(classCode))) {
        return "unsuccessful";
    }
    bool dropped = s.dropClass(classCode);
    if (dropped) {
        if (s.getClasses().size() == 0) {
            this->remove(s.getID());
        }
        return "successful";
    }
    return "unsuccessful";
}

string CampusCompass::replaceClass(string ID, string classCode1, string classCode2) {
    //check is class code 2 exists
    if (classes.find(classCode2) == classes.end()) {
        return "unsuccessful";
    }
    auto it = students.find(ID);
    //check if student exists
    if (it == students.end()) {
        return "unsuccessful";
    }
    Student &s = it->second;
    //does student have class 1?
    if (!(s.isClass(classCode1))) {
        return "unsuccessful";
    }
    //does student have class 2 already?
    if (s.isClass(classCode2)) {
        return "unsuccessful";
    }
    //replace
    bool replaced = s.replaceClass(classCode1, classCode2);
    if (replaced) {
        return "successful";
    }
    return "unsuccessful";
}

int CampusCompass::removeClass(string classCode) {
    int count = 0;
    //check if class exists
    if (classes.find(classCode) == classes.end()) {
        return 0;
    }

    vector<string> ids;
    for (auto it = students.begin(); it != students.end(); it++) {
        ids.push_back(it->first);
    }
    //call dropClass on each student.
    for (int i = 0; i < static_cast<int>(ids.size()); i++) {
        string dropped = dropClass(ids[i], classCode);
        if (dropped == "successful") {
            count++;
        }
    }
    //if class was dropped, add 1 to count
    return count;
}

string CampusCompass::toggleEdgesClosure(int n, vector<int> edges) {
    for (int i = 0; i <= n / 2; i++) {
        int edge1 = edges[i * 2];
        int edge2 = edges[i * 2 + 1];
        this->graph.toggleEdge(edge1, edge2);
    }
    return "successful";
}

string CampusCompass::checkEdgeStatus(int edge1, int edge2) {
    return graph.checkEdge(edge1, edge2);
}

string CampusCompass::isConnected(int edge1, int edge2) {
    if (graph.isConnected(edge1, edge2)) {
        return "successful";
    }
    return "unsuccessful";
}

map<string, int> CampusCompass::printShortestEdges(string ID) {
    map<string, int> distances;

    //find student.
    auto it = students.find(ID);
    Student &s = it->second;

    //get student's classes
    vector<string> sClasses = s.getClasses();

    //get students residence location
    int residence = s.getResidence();

    //for each class, find shortest path
    for (int i = 0; i < static_cast<int>(sClasses.size()); i++) {
        string classCode = sClasses[i];
        auto it2 = classes.find(classCode);
        Class &class1 = it2->second;
        string classID = class1.getLocationId();
        int dist = graph.printShortestEdges(residence, stoi(classID));
        //add that path to a map of format <class name, time>
        distances[classCode] = dist;
    }
    return distances;
}

int timeConvertor(string time) {
    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));
    return hours * 60 + minutes;
}

string CampusCompass::verifySchedule(string id) {
    return "";
}
