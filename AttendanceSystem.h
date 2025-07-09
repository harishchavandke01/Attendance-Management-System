#pragma once
#include "Employee.h"
#include "AttendanceRecord.h"
#include<iostream>
#include<string>
#include<vector>
#include<map>
using namespace std;
class AttendanceSystem {
private:
	map<int, Employee> employees;
	vector<AttendanceRecord> records;
	bool isWithInPeriod(const string &dateStr, const string &startStr, const string &endStr) const ;
	
public:
	void addEmployee(const Employee& emp);
	void clockIn(int empId,const string &date, const string &inTime) ;
	void clockOut(int empId, const string &date, const string &outTime);
	int getEmployeeCount() const ;
	void getEmployeeReport(int empId, const string& startDate, const string &endDate);
	void getAllEmployeesSummary(const string &startDate, const string &endDate);
	void getPresentAbsentReport(const string &date) ;
	void getDepartmentReport(const string& departmentName, const string& startDate, const string& endDate);
	void calculateMonthlyPayForEmployee(int empId, const string & startDate, const string & endDate) ;
	void calculateMonthlyPayForDepartment(const string& departmentName, const string& startDate, const string& endDate) ;
	void calculateMonthlyPayForAll(const string & startDate, const string &endDate);
	
	void run();

};