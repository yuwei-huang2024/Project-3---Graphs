#pragma once
#include <string>
#include <vector>
#include "Student.h"
using namespace std;

class Class {
    // for me later, code is already saved in unordered map as key
    string locationId;
    string startTime;
    string endTime;
public:
    Class(string gLocationId, string gStartTime, string gEndTime);
};