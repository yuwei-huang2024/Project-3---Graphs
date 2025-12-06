#include "CampusCompass.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <regex>

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
        for (int i = 0; i < n * 2; i++) {
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
        string ID;
        ss >> ID;
        int results = printStudentZone(ID);
        auto it = students.find(ID);
        Student &s = it->second;
        cout << "Student Zone Cost For " << s.getName() << ": "<< results << endl;
        return true;
    }
    else if (word == "verifySchedule") {
        string ID;
        ss >> ID;
        vector<classGap> results = verifySchedule(ID);
        if (results.size() == 0) {
            cout << "unsuccessful" << endl;
            return false;
        }
        auto it = students.find(ID);
        Student &s = it->second;
        cout << "Schedule Check for " << s.getName() << ":" << endl;
        for (auto it = results.begin(); it != results.end(); it++) {
            string classFrom = it->getFromClass();
            string classTo = it->getToClass();
            bool canMake = it->getCanMake();
            cout << classFrom << "-" << classTo;
            if (canMake) {
                cout << " \"Can make it!\"" << endl;
            }
            else {
                cout << " \"Cannot make it!\"" << endl;
            }
        }
        return true;
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

bool CampusCompass::isClassCode (string code) {
    regex pattern("^[A-Z]{3}[0-9]{4}$");
    if (!regex_match(code, regex(pattern))) {
        return false;
    }
    return true;
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
    //less than 1 class
    if (row.size() < 6) {return "unsuccessful";}
    int num = stoi(row[4]);

    //uf id is 8 long, numbers
    regex pattern("^[0-9]{8}$");
    regex pattern2("^[A-Za-z ]+$");
    if (!(regex_match(id, regex(pattern)))) {return "unsuccessful";}
    ///name is alphabet
    if (!(regex_match(name, regex(pattern2)))) {return "unsuccessful";}
    //student already exists based on ID
    if (checkID (id)) {return "unsuccessful";}
    //more than 6 classes
    if (num > 6) {return "unsuccessful";}
    //see if classes and num matches
    if (num != static_cast<int>(row.size()) - 5) {return "unsuccessful";}

    Student newStudent(name, id, location);
    for (int i = 0; i < num; i++) {
        if (!isClassCode (row[5 + i])) {return "unsuccessful";}
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
        int edge2 = edges[(i * 2) + 1];
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

int CampusCompass::printStudentZone(string id) {
    //get all class location ids
    auto it = students.find(id);
    Student &s = it->second;
    int residence = s.getResidence();
    vector<string> sClasses = s.getClasses();
    vector<int> classIDs;
    for (int i = 0; i < static_cast<int>(sClasses.size()); i++) {
        auto itClass = classes.find(sClasses[i]);
        if (itClass == classes.end()) {
            continue;
        }
        int location = stoi(itClass->second.getLocationId());
        classIDs.push_back(location);
    }

    int cost = graph.zoneCalc(residence, classIDs);
    return cost;
}

int timeConvertor(string time) {
    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));
    return hours * 60 + minutes;
}

vector<classGap> CampusCompass::verifySchedule(string ID) {
    vector<classGap> v;

    // find student; return empty vector if not found
    auto it = students.find(ID);
    if (it == students.end()) {
        return v;
    }
    Student &s = it->second;

    //find their classes
    vector<string> sClasses = s.getClasses();

    //if one class, return nothing -> print unsuccessful
    if (sClasses.size() <= 1) {
        return v;
    }

    // sort classes by start time (in minutes). If a class is missing,
    // treat its time as very large so it sorts to the end. Tie-break
    // using the class code for a deterministic order.
    sort(sClasses.begin(), sClasses.end(), [&](const string& a, const string& b) {
        auto ita = classes.find(a);
        auto itb = classes.find(b);
        int t1 = timeConvertor(ita->second.getStartTime());
        int t2 = timeConvertor(itb->second.getStartTime());
        return t1 < t2;
    });

    //for first class, calc time from end time to next
    //find distance between first class location and next class location
    //save in vector

    for (size_t i = 0; i + 1 < sClasses.size(); ++i) {
        auto itCa = classes.find(sClasses[i]);
        auto itCb = classes.find(sClasses[i + 1]);
        if (itCa == classes.end() || itCb == classes.end()) {
            // missing class info -> mark as cannot make
            classGap newClassGap(sClasses[i], sClasses[i + 1], false);
            v.push_back(newClassGap);
            continue;
        }
        Class& ca = itCa->second;
        Class& cb = itCb->second;
        int endTime = timeConvertor(ca.getEndTime());
        int startTime = timeConvertor(cb.getStartTime());
        int difference = startTime - endTime;
        int distance = graph.printShortestEdges(stoi(ca.getLocationId()), stoi(cb.getLocationId()));
        bool canMake;
        if (distance == -1) {
            canMake = false;
        }
        else if (distance > difference ) {
            canMake = false;
        }
        else {
            canMake = true;
        }
        classGap newClassGap(sClasses[i], sClasses[i + 1], canMake);
        v.push_back(newClassGap);
    }
    return v;
}
