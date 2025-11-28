#include "Class.h"

Class::Class(string gLocationId, string gStartTime, string gEndTime) {
    locationId = gLocationId;
    startTime = gStartTime;
    endTime = gEndTime;
}

string Class::getEndTime() {
    return endTime;
}

string Class::getStartTime() {
    return startTime;
}

string Class::getLocationId() {
    return locationId;
}