#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<chrono>
#include<iomanip>
#include<sstream>
#include<ctime>
using namespace std;
using namespace std::chrono;

class ClockInMethod {
public:
	virtual string getMethod() const = 0;
	virtual ~ClockInMethod() {}
};

class Biometric:public ClockInMethod {
public:
	string getMethod()const override {
		return "Biometric";
	}
};

class Card:public ClockInMethod {
public:
	string getMethod()const override {
		return "Card";
	}
};

class Employee {
private:
	int id;
	string name;
	string department;
	ClockInMethod *method;
	double payScalePerHour;

public:
	Employee():id(0),name(""),department(""),method(nullptr), payScalePerHour(0.0) {}
	Employee(int id, string name, string department, ClockInMethod* method, double payScalePerHour)
		:id(id), name(name), department(department), method(method), payScalePerHour(payScalePerHour) {}

	int getId() const {
		return id;
	}
	string getName() const {
		return name;
	}
	string getDepartment() const {
		return department;
	}
	string getClockInMethod() const {
		return method?method->getMethod():"Unknown";
	}
	double getPayScalePerHour() const {
		return payScalePerHour;
	}
};


class AttendanceRecord {
private:
	int employeeId;
	string name;
	string date;
	string inTime;
	string outTime;
public:
	AttendanceRecord(int empId,const string &name,const string &date, const string &inTime="", const string &outTime="")
		:employeeId(empId), name(name), date(date), inTime(inTime), outTime(outTime) {}

	int getEmployeeId() const {
		return employeeId;
	}
	string getName() const {
		return name;
	}
	string getDate() const {
		return date;
	}
	string getInTime() const {
		return inTime;
	}
	string getOutTime() const {
		return outTime;
	}

	void setInTime(const string &in) {
		inTime=in;
	}
	void setOutTime(const string &out) {
		outTime=out;
	}

	string getDuration() const {

		if (inTime.empty() || outTime.empty()) return "0:00";

		tm in_tm = {}, out_tm = {};
		istringstream(inTime) >> get_time(&in_tm, "%H:%M");
		istringstream(outTime) >> get_time(&out_tm, "%H:%M");

		auto in_tp = system_clock::from_time_t(mktime(&in_tm));
		auto out_tp = system_clock::from_time_t(mktime(&out_tm));
		auto duration = duration_cast<minutes>(out_tp - in_tp);

		ostringstream oss;
		oss << duration.count() / 60 << ":" << setfill('0') << setw(2) << duration.count() % 60;
		return oss.str();
	}

};


class AttendanceSystem {
private:
	map<int, Employee> employees;
	vector<AttendanceRecord> records;
	bool isWithInPeriod(const string &dateStr, const string &startStr, const string &endStr) const {
		tm tm_date= {}, tm_start= {}, tm_end= {};
		istringstream(dateStr) >> get_time(&tm_date,"%Y-%m-%d");
		istringstream(startStr) >> get_time(&tm_start, "%Y-%m-%d");
		istringstream(endStr) >> get_time(&tm_end,"%Y-%m-%d");
		auto d = mktime(&tm_date);
		auto s = mktime(&tm_end);
		auto e = mktime(&tm_start);

		return d>=s && d<=e;
	}
public:
	void addEmployee(const Employee& emp) {
		employees[emp.getId()]=emp;
	}

	void clockIn(int empId,const string &date, const string &inTime) {
		for(auto &rec: records) {
			if(rec.getEmployeeId()==empId && rec.getDate()==date) {
				rec.setInTime(inTime);
				return;
			}
		}
		records.emplace_back(empId,employees[empId].getName(),date, inTime, "");
	}

	void clockOut(int empId, const string &date, const string &outTime) {
		for(auto &rec: records) {
			if(rec.getEmployeeId()==empId && rec.getDate()==date) {
				rec.setOutTime(outTime);
				return;
			}
		}
	}

	int getEmployeeCount() const {
		return employees.size();
	}

	void getEmployeeReport(int empId, const string& startDate, const string &endDate) {
		cout << "\n---- Reports of employee ID: " << empId << " ----\n";
		for(auto &rec: records) {
			if(rec.getEmployeeId()==empId && isWithInPeriod(rec.getDate(), startDate, endDate)) {
				cout<<rec.getDate()<<"\tIn: "<<rec.getInTime()<<"\tOut: "<<rec.getOutTime()<<"\tDuration: "
				    <<rec.getDuration()<<"\n";
			}
		}
	}

	void getAllEmployeesSummary(const string &startDate, const string &endDate) {
		cout<<"\n---- summary report of all employees\n";
		map<int, int> minutesWorked;
		for(auto &rec: records) {
			if(isWithInPeriod(rec.getDate(), startDate, endDate)) {
				if(rec.getInTime().empty() || rec.getOutTime().empty()) {
					continue;
				}
				tm in_tm = {}, out_tm = {};
				istringstream(rec.getInTime()) >> get_time(&in_tm, "%H:%M");
				istringstream(rec.getOutTime()) >> get_time(&out_tm, "%H:%M");
				auto in_tp = system_clock::from_time_t(mktime(&in_tm));
				auto out_tp = system_clock::from_time_t(mktime(&out_tm));
				auto duration = duration_cast<minutes>(out_tp - in_tp);
				minutesWorked[rec.getEmployeeId()]+=duration.count();
			}
		}
		cout<<"\nID\tName\tDepartment\t Total Duration\n";
		for(auto &emp:employees) {
			int mints=minutesWorked[emp.first];
			cout<<emp.first<<"\t"<<emp.second.getName()<<"\t"<<emp.second.getDepartment()<<"\t\t"<<mints/60<<"h "<<mints%60<<"m\n";
		}
	}

	void getPresentAbsentReport(const string &date) {
		cout<<"\n---- Present Absent report for the day: "<<date<<"\n";
		map<int,bool> present;
		for(auto& rec: records) {
			if(rec.getDate()==date && !rec.getInTime().empty()) {
				present[rec.getEmployeeId()]=true;
			}
		}
		cout<<"\nID\tName\tdepart\t P/A\n";
		for(auto &emp: employees) {
			cout<<emp.first<<"\t"<<emp.second.getName()<<"\t"<<emp.second.getDepartment()<<"\t-> "<<(present[emp.first]?"Present":"absent")<<"\n";
		}
	}

	void getDepartmentReport(const string& departmentName, const string& startDate, const string& endDate) {
		cout<<"\n---- Reports for "<<departmentName<<" from "<<startDate<<" to "<<endDate<<"------\n";
		cout<<"ID\tName\tdate\t\tIN\tOUT\tDuration\n";
		for(auto &rec: records) {
			int empId = rec.getEmployeeId();
			if(employees[empId].getDepartment()==departmentName && isWithInPeriod(rec.getDate(), startDate, endDate)) {
				cout<<empId<<"\t"<<rec.getName()<<"\t"<<rec.getDate()<<"\t"<<rec.getInTime()<<"\t"<<rec.getOutTime()<<"\t"<<rec.getDuration()<<"\n";
			}
		}
	}

	void calculateMonthlyPayForEmployee(int empId, const string & startDate, const string & endDate) {
		double totalPay=0.0;
		for(auto& rec: records) {
			if(rec.getEmployeeId()==empId && isWithInPeriod(rec.getDate(), startDate, endDate)) {
				if(rec.getInTime().empty() || rec.getOutTime().empty()) {
					continue;
				}
				tm in_tm = {}, out_tm = {};
				istringstream(rec.getInTime()) >> get_time(&in_tm, "%H:%M");
				istringstream(rec.getOutTime()) >> get_time(&out_tm, "%H:%M");
				auto in_tp = system_clock::from_time_t(mktime(&in_tm));
				auto out_tp = system_clock::from_time_t(mktime(&out_tm));
				auto duration = duration_cast<minutes>(out_tp - in_tp).count();
				double hours=duration/60.0;
				totalPay+=hours* employees[empId].getPayScalePerHour();
			}
		}

		cout<<"\nTotal pay for empId: "<<empId<<" ("<<employees[empId].getName()<<") = "<<totalPay<<"\n";
	}

	void calculateMonthlyPayForDepartment(const string& departmentName, const string& startDate, const string& endDate) {
		cout<<"\n---- Pay report for Department: "<<departmentName<<"----\n";
		double departmentTotal=0;
		cout<<"ID\tName\tPay\n";
		for (auto& emp : employees) {
			if (emp.second.getDepartment() == departmentName) {
				double totalPay = 0.0;
				for (auto& rec : records) {
					if (rec.getEmployeeId() == emp.first && isWithInPeriod(rec.getDate(), startDate, endDate)) {
						if (rec.getInTime().empty() || rec.getOutTime().empty()) continue;
						tm in_tm = {}, out_tm = {};
						istringstream(rec.getInTime()) >> get_time(&in_tm, "%H:%M");
						istringstream(rec.getOutTime()) >> get_time(&out_tm, "%H:%M");
						auto in_tp = system_clock::from_time_t(mktime(&in_tm));
						auto out_tp = system_clock::from_time_t(mktime(&out_tm));
						auto duration = duration_cast<minutes>(out_tp - in_tp).count();
						totalPay += (duration / 60.0) * emp.second.getPayScalePerHour();
					}
				}
				departmentTotal += totalPay;
				cout << emp.first << "\t" << emp.second.getName()<<"\t" << totalPay << "\n";
			}
		}

		cout << "------ Total Department Pay: " << departmentTotal << "\n";
	}
	
	void calculateMonthlyPayForAll(const string & startDate, const string &endDate){
	    double totalOrgPay=0.0;
	    cout<<"\n---- Pay report of all employees ----\n";
	    cout<<"ID\tName\tPay\n";
	    for(auto &emp: employees){
	        double totalPay=0.0;
			for (auto& rec : records) {
				if (rec.getEmployeeId() == emp.first && isWithInPeriod(rec.getDate(), startDate, endDate)) {
					if (rec.getInTime().empty() || rec.getOutTime().empty()) continue;
					tm in_tm = {}, out_tm = {};
					istringstream(rec.getInTime()) >> get_time(&in_tm, "%H:%M");
					istringstream(rec.getOutTime()) >> get_time(&out_tm, "%H:%M");
					auto in_tp = system_clock::from_time_t(mktime(&in_tm));
					auto out_tp = system_clock::from_time_t(mktime(&out_tm));
					auto duration = duration_cast<minutes>(out_tp - in_tp).count();
					totalPay += (duration / 60.0) * emp.second.getPayScalePerHour();
				}
			}
			totalOrgPay += totalPay;
			cout << emp.first << "\t" << emp.second.getName()<<"\t" << totalPay << "\n";
	    }
	    cout<<"-----Total Orgranisation pay: "<<totalOrgPay<<"\n";
	}

};

int main()
{
	AttendanceSystem surveyaan;
	Biometric bio;
	Card card;

	int choice;
	while(true)
	{
		cout<<"\n\n==== Surveyaan Attendance Management ====\n";
		cout<<"1: Add employee\n";
		cout<<"2: Get employee report\n";
		cout<<"3: Get all employee summary\n";
		cout<<"4: Get present/absent report\n";
		cout<<"5: Get department report\n";
		cout<<"6: Calculate pay for one employee\n";
		cout<<"7: Calculate pay for a department\n";
		cout<<"8: Calculate pay for all employees\n";
		cout<<"9: Clock In\n";
		cout<<"10: Clock Out\n";
		cout<<"11: Exit\n";
		cout<<"Enter your choice: ";
		cin>>choice;

		if(choice==1)
		{
			int id = surveyaan.getEmployeeCount()+1;
			string name,dept;
			cout<<"\nEnter Name: ";
			cin>>name;
			cout<<"Enter department: ";
			cin>>dept;

			double pay;
			if(dept=="Dev") pay=350;
			else if(dept=="Prod") pay=380;
			else if(dept=="Sale") pay==300;
			else if(dept=="HR") pay=250;
			else pay=200;

			surveyaan.addEmployee(Employee(id, name,dept,&bio, pay));
			cout<<"Employee added with EmployeeId: "<<id<<" and Attendance by : Biometric";
		}
		else if(choice==2)
		{
			int empId;
			string start,end;
			cout<<"\nEnter EmployeeId: ";
			cin>>empId;
			cout<<"Enter the start date (YYYY-MM-DD): ";
			cin>>start;
			cout<<"Enter the end date (YYYY-MM-DD): ";
			cin>>end;
			surveyaan.getEmployeeReport(empId, start,end);
		}
		else if(choice==3)
		{
			string start, end;
			cout<<"\nEnter start date (YYYY-MM-DD): ";
			cin>>start;
			cout<<"Enter end date (YYYY-MM-DD): ";
			cin>>end;
			surveyaan.getAllEmployeesSummary(start,end);
		}
		else if(choice==4)
		{
			string date;
			cout<<"\nEnter date (YYYY-MM-DD): ";
			cin>>date;
			surveyaan.getPresentAbsentReport(date);
		}
		else if(choice==5)
		{
			string dept,start,end;
			cout<<"\nEnter the department: ";
			cin>>dept;
			cout<<"Enter the start date (YYYY-MM-DD): ";
			cin>>start;
			cout<<"Enter the end date (YYYY-MM-DD): ";
			cin>>end;
			surveyaan.getDepartmentReport(dept, start, end);
		}
		else if(choice==6)
		{
			int empId;
			string start, end;
			cout<<"\nEnter the EmployeeId: ";
			cin>>empId;
			cout<<"Enter the start date (YYYY-MM-DD): ";
			cin>>start;
			cout<<"Enter the end date (YYYY-MM-DD): ";
			cin>>end;
			surveyaan.calculateMonthlyPayForEmployee(empId,start,end);
		}
		else if(choice==7)
		{
			string dept,start,end;
			cout<<"\nEnter the department: ";
			cin>>dept;
			cout<<"Enter the start date (YYYY-MM-DD): ";
			cin>>start;
			cout<<"Enter the end date (YYYY-MM-DD): ";
			cin>>end;
			surveyaan.calculateMonthlyPayForDepartment(dept,start,end);
		}
		else if(choice==8)
		{
		    string start,end;
		    cout<<"\nEnter the start date (YYYY-MM-DD): ";
		    cin>>start;
		    cout<<"Enter the end date (YYYY-MM-DD): ";
		    cin>>end;
		    surveyaan.calculateMonthlyPayForAll(start,end);
		}
		else if(choice==9)
		{
			int empId;
			string date,inTime;
			cout<<"\nEnter EmployeeId: ";
			cin>>empId;
			cout<<"Enter date: ";
			cin>>date;
			cout<<"Enter In Time (HH:MM): ";
			cin>>inTime;
			surveyaan.clockIn(empId, date, inTime);
			cout<<"Clock In recorded\n";
		}
		else if(choice==10)
		{
			int empId;
			string date,outTime;
			cout<<"\nEnter the EmployeeId: ";
			cin>>empId;
			cout<<"Enter the date (YYYY-MM-DD): ";
			cin>>date;
			cout<<"Enter the Out Time (HH:MM): ";
			cin>>outTime;
			surveyaan.clockOut(empId, date, outTime);
			cout<<"Clock out recorded\n";
		}
		else if(choice==11)
		{
			cout<<"\nExiting the program";
			break;
		}
		else {
			cout<<"\nInvalid option try again\n";
		}
	}

	return 0;
}