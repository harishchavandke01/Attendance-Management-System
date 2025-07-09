#pragma once
#include<iostream>
#include<string>
using namespace std;

class ClockInMethod {
public:
	virtual string getMethod() const = 0;
	virtual ~ClockInMethod() {}
};

class Biometric:public ClockInMethod {
public:
	string getMethod()const override;
};

class Card:public ClockInMethod {
public:
	string getMethod()const override;
};

class Employee {
private:
	int id;
	string name;
	string department;
	ClockInMethod *method;
	double payScalePerHour;

public:
	Employee();
	Employee(int id, string name, string department, ClockInMethod* method, double payScalePerHour);

	int getId() const;
	string getName() const;
	string getDepartment() const ;
	string getClockInMethod() const;
	double getPayScalePerHour() const ;
};