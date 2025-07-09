#include "AttendanceRecord.h"
#include<iostream>
#include<string>
#include<chrono>
#include<iomanip>
#include<sstream>
#include<ctime>
using namespace std;
using namespace std::chrono;

AttendanceRecord::AttendanceRecord(int empId,const string &name,const string &date, const string &inTime, const string &outTime)
		:employeeId(empId), name(name), date(date), inTime(inTime), outTime(outTime) {}

int AttendanceRecord::getEmployeeId() const {
	return employeeId;
}
string AttendanceRecord::getName() const {
	return name;
}
string AttendanceRecord::getDate() const {
	return date;
}
string AttendanceRecord::getInTime() const {
	return inTime;
}
string AttendanceRecord::getOutTime() const {
	return outTime;
}

void AttendanceRecord::setInTime(const string &in) {
	inTime=in;
}
void AttendanceRecord::setOutTime(const string &out) {
	outTime=out;
}

string AttendanceRecord::getDuration() const {
    if (inTime.empty() || outTime.empty())
        return "0:00";

    int inHour, inMin, outHour, outMin;

    if (sscanf(inTime.c_str(), "%d:%d", &inHour, &inMin) != 2 ||
        sscanf(outTime.c_str(), "%d:%d", &outHour, &outMin) != 2)
        return "0:00";

    int totalInMinutes = inHour * 60 + inMin;
    int totalOutMinutes = outHour * 60 + outMin;
    int diff = totalOutMinutes - totalInMinutes;

    if (diff < 0)
        return "0:00";

    int hours = diff / 60;
    int minutes = diff % 60;

    char buffer[6];
    sprintf(buffer, "%d:%02d", hours, minutes);
    return string(buffer);
}
