#include "Employee.h"
#include<iostream>
#include<string>
using namespace std;

string Biometric:: getMethod() const {
    return "Biometric";
}

string Card:: getMethod() const {
    return "Card";
}

Employee::Employee():
    id(0),name(""),department(""),method(nullptr), payScalePerHour(0.0) {}
    

Employee::Employee(int id, string name, string department, ClockInMethod* method, double payScalePerHour)
		:id(id), name(name), department(department), method(method), payScalePerHour(payScalePerHour) {}
		

int Employee::getId() const {
		return id;
}

string Employee::getName() const {
	return name;
}

string Employee::getDepartment() const {
	return department;
}

string Employee::getClockInMethod() const {
	return method?method->getMethod():"Unknown";
}

double Employee::getPayScalePerHour() const {
	return payScalePerHour;
}

