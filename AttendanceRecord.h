#pragma once
#include<iostream>
#include<string>
using namespace std;

class AttendanceRecord {
private:
	int employeeId;
	string name;
	string date;
	string inTime;
	string outTime;
public:
	AttendanceRecord(int empId,const string &name,const string &date, const string &inTime="", const string &outTime="");

	int getEmployeeId() const ;
	string getName() const;
	string getDate() const;
	string getInTime() const;
	string getOutTime() const;
	void setInTime(const string &in);
	void setOutTime(const string &out);
	string getDuration() const;

};