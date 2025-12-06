
#include "Student.h"
#include "algorithm"

using namespace std;

Student::Student(string gName, string gID, int gResidence) {
  name = gName;
  id = gID;
  residence = gResidence;
  vector<string> classes;
}

bool Student::isClass(string code) {
  auto it = std::find(classes.begin(), classes.end(), code);
  if (it != classes.end()) {
    return true;
  }
  return false;
}

bool Student::addClass(string code) {
  classes.push_back(code);
  return true;
}

bool Student::dropClass(string code) {
  if (isClass(code)) {
    classes.erase(remove(classes.begin(), classes.end(), code), classes.end());
    return true;
  }
  return false;
}

bool Student::replaceClass(string class1, string class2) {
  if (isClass(class1)) {
    dropClass(class1);
    addClass(class2);
    return true;
  }
  return false;
}


vector<string> Student::getClasses() {
  sort(classes.begin(), classes.end());
  return classes;
}
string Student::getName() {
  return name;
}
string Student::getID() {
  return id;
}
int Student::getResidence() {
  return residence;
}