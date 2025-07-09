#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
using namespace std;
using namespace std::chrono;
#include "AttendanceSystem.h"

bool AttendanceSystem::isWithInPeriod(const string &dateStr, const string &startStr, const string &endStr) const
{
    int y1, m1, d1, y2, m2, d2, y3, m3, d3;

    if (sscanf(dateStr.c_str(), "%d-%d-%d", &y1, &m1, &d1) != 3 ||
        sscanf(startStr.c_str(), "%d-%d-%d", &y2, &m2, &d2) != 3 ||
        sscanf(endStr.c_str(), "%d-%d-%d", &y3, &m3, &d3) != 3)
        return false;

    // Convert to comparable integers: YYYYMMDD
    int date = y1 * 10000 + m1 * 100 + d1;
    int start = y2 * 10000 + m2 * 100 + d2;
    int end = y3 * 10000 + m3 * 100 + d3;

    return date >= start && date <= end;
}

void AttendanceSystem::addEmployee(const Employee &emp)
{
    employees[emp.getId()] = emp;
}

void AttendanceSystem::clockIn(int empId, const string &date, const string &inTime)
{
    for (auto &rec : records)
    {
        if (rec.getEmployeeId() == empId && rec.getDate() == date)
        {
            rec.setInTime(inTime);
            return;
        }
    }
    records.emplace_back(empId, employees[empId].getName(), date, inTime, "");
}

void AttendanceSystem::clockOut(int empId, const string &date, const string &outTime)
{
    for (auto &rec : records)
    {
        if (rec.getEmployeeId() == empId && rec.getDate() == date)
        {
            rec.setOutTime(outTime);
            return;
        }
    }
}

int AttendanceSystem::getEmployeeCount() const
{
    return employees.size();
}

void AttendanceSystem::getEmployeeReport(int empId, const string &startDate, const string &endDate)
{
    cout << "\n---- Reports of employee ID: " << empId << " ----\n";
    for (auto &rec : records)
    {
        if (rec.getEmployeeId() == empId && isWithInPeriod(rec.getDate(), startDate, endDate))
        {
            cout << rec.getDate() << "\tIn: " << rec.getInTime() << "\tOut: " << rec.getOutTime() << "\tDuration: "
                 << rec.getDuration() << "\n";
        }
    }
}

void AttendanceSystem::getAllEmployeesSummary(const string &startDate, const string &endDate)
{
    cout << "\n---- summary report of all employees\n";
    map<int, int> minutesWorked;
    for (auto &rec : records)
    {
        if (isWithInPeriod(rec.getDate(), startDate, endDate))
        {
            if (rec.getInTime().empty() || rec.getOutTime().empty())
            {
                continue;
            }

            int inHour, inMin, outHour, outMin;
            if (sscanf(rec.getInTime().c_str(), "%d:%d", &inHour, &inMin) == 2 &&
                sscanf(rec.getOutTime().c_str(), "%d:%d", &outHour, &outMin) == 2)
            {

                int inTotal = inHour * 60 + inMin;
                int outTotal = outHour * 60 + outMin;
                int diff = outTotal - inTotal;

                if (diff > 0)
                {
                    minutesWorked[rec.getEmployeeId()] += diff;
                }
            }
        }
    }

    cout << "\nID\tName\tDepartment\t Total Duration\n";
    for (auto &emp : employees)
    {
        int mints = minutesWorked[emp.first];
        cout << emp.first << "\t" << emp.second.getName() << "\t" << emp.second.getDepartment() << "\t\t" << mints / 60 << "h " << mints % 60 << "m\n";
    }
}

void AttendanceSystem::getPresentAbsentReport(const string &date)
{
    cout << "\n---- Present Absent report for the day: " << date << "\n";
    map<int, bool> present;
    for (auto &rec : records)
    {
        if (rec.getDate() == date && !rec.getInTime().empty())
        {
            present[rec.getEmployeeId()] = true;
        }
    }
    cout << "\nID\tName\tdepart\t P/A\n";
    for (auto &emp : employees)
    {
        cout << emp.first << "\t" << emp.second.getName() << "\t" << emp.second.getDepartment() << "\t-> " << (present[emp.first] ? "Present" : "absent") << "\n";
    }
}

void AttendanceSystem::getDepartmentReport(const string &departmentName, const string &startDate, const string &endDate)
{
    cout << "\n---- Reports for " << departmentName << " from " << startDate << " to " << endDate << "------\n";
    cout << "ID\tName\tdate\t\tIN\tOUT\tDuration\n";
    for (auto &rec : records)
    {
        int empId = rec.getEmployeeId();
        if (employees[empId].getDepartment() == departmentName && isWithInPeriod(rec.getDate(), startDate, endDate))
        {
            cout << empId << "\t" << rec.getName() << "\t" << rec.getDate() << "\t" << rec.getInTime() << "\t" << rec.getOutTime() << "\t" << rec.getDuration() << "\n";
        }
    }
}

void AttendanceSystem::calculateMonthlyPayForEmployee(int empId, const string &startDate, const string &endDate)
{
    double totalPay = 0.0;
    for (auto &rec : records)
    {
        if (rec.getEmployeeId() == empId && isWithInPeriod(rec.getDate(), startDate, endDate))
        {
            if (rec.getInTime().empty() || rec.getOutTime().empty())
            {
                continue;
            }

            int inHour, inMin, outHour, outMin;
            if (sscanf(rec.getInTime().c_str(), "%d:%d", &inHour, &inMin) == 2 &&
                sscanf(rec.getOutTime().c_str(), "%d:%d", &outHour, &outMin) == 2)
            {
                int inTotal = inHour * 60 + inMin;
                int outTotal = outHour * 60 + outMin;
                int diff = outTotal - inTotal;

                if (diff > 0)
                {
                    double hours = diff / 60.0;
                    totalPay += hours * employees[empId].getPayScalePerHour();
                }
            }
        }
    }

    cout << "\nTotal pay for empId: " << empId << " (" << employees[empId].getName() << ") = " << totalPay << "\n";
}

void AttendanceSystem::calculateMonthlyPayForDepartment(const string &departmentName, const string &startDate, const string &endDate)
{
    cout << "\n---- Pay report for Department: " << departmentName << "----\n";
    double departmentTotal = 0;
    cout << "ID\tName\tPay\n";
    for (auto &emp : employees)
    {
        if (emp.second.getDepartment() == departmentName)
        {
            double totalPay = 0.0;
            for (auto &rec : records)
            {
                if (rec.getEmployeeId() == emp.first && isWithInPeriod(rec.getDate(), startDate, endDate))
                {
                    if (rec.getInTime().empty() || rec.getOutTime().empty())
                        continue;

                    int inHour, inMin, outHour, outMin;
                    if (sscanf(rec.getInTime().c_str(), "%d:%d", &inHour, &inMin) == 2 &&
                        sscanf(rec.getOutTime().c_str(), "%d:%d", &outHour, &outMin) == 2)
                    {
                        int inTotal = inHour * 60 + inMin;
                        int outTotal = outHour * 60 + outMin;
                        int diff = outTotal - inTotal;

                        if (diff > 0)
                        {
                            totalPay += (diff / 60.0) * emp.second.getPayScalePerHour();
                        }
                    }
                }
            }
            departmentTotal += totalPay;
            cout << emp.first << "\t" << emp.second.getName() << "\t" << totalPay << "\n";
        }
    }

    cout << "------ Total Department Pay: " << departmentTotal << "\n";
}

void AttendanceSystem::calculateMonthlyPayForAll(const string &startDate, const string &endDate)
{
    double totalOrgPay = 0.0;
    cout << "\n---- Pay report of all employees ----\n";
    cout << "ID\tName\tPay\n";
    for (auto &emp : employees)
    {
        double totalPay = 0.0;
        for (auto &rec : records)
        {
            if (rec.getEmployeeId() == emp.first && isWithInPeriod(rec.getDate(), startDate, endDate))
            {
                if (rec.getInTime().empty() || rec.getOutTime().empty())
                    continue;

                int inHour, inMin, outHour, outMin;
                if (sscanf(rec.getInTime().c_str(), "%d:%d", &inHour, &inMin) == 2 &&
                    sscanf(rec.getOutTime().c_str(), "%d:%d", &outHour, &outMin) == 2)
                {
                    int inTotal = inHour * 60 + inMin;
                    int outTotal = outHour * 60 + outMin;
                    int diff = outTotal - inTotal;

                    if (diff > 0)
                    {
                        totalPay += (diff / 60.0) * emp.second.getPayScalePerHour();
                    }
                }
            }
        }

        totalOrgPay += totalPay;
        cout << emp.first << "\t" << emp.second.getName() << "\t" << totalPay << "\n";
    }
    cout << "-----Total Orgranisation pay: " << totalOrgPay << "\n";
}

void AttendanceSystem::run()
{
    Biometric bio;
    Card card;

    int choice;
    while (true)
    {
        cout << "\n\n==== Surveyaan Attendance Management ====\n";
        cout << "1: Add employee\n";
        cout << "2: Get employee report\n";
        cout << "3: Get all employee summary\n";
        cout << "4: Get present/absent report\n";
        cout << "5: Get department report\n";
        cout << "6: Calculate pay for one employee\n";
        cout << "7: Calculate pay for a department\n";
        cout << "8: Calculate pay for all employees\n";
        cout << "9: Clock In\n";
        cout << "10: Clock Out\n";
        cout << "11: Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            int id = getEmployeeCount() + 1;
            string name, dept;
            cout << "\nEnter Name: ";
            cin >> name;
            cout << "Enter department: ";
            cin >> dept;

            double pay;
            if (dept == "Dev")
                pay = 350;
            else if (dept == "Prod")
                pay = 380;
            else if (dept == "Sale")
                pay = 300;
            else if (dept == "HR")
                pay = 250;
            else
                pay = 200;

            addEmployee(Employee(id, name, dept, &bio, pay));
            cout << "Employee added with EmployeeId: " << id << " and Attendance by : Biometric";
        }
        else if (choice == 2)
        {
            int empId;
            string start, end;
            cout << "\nEnter EmployeeId: ";
            cin >> empId;
            cout << "Enter the start date (YYYY-MM-DD): ";
            cin >> start;
            cout << "Enter the end date (YYYY-MM-DD): ";
            cin >> end;
            getEmployeeReport(empId, start, end);
        }
        else if (choice == 3)
        {
            string start, end;
            cout << "\nEnter start date (YYYY-MM-DD): ";
            cin >> start;
            cout << "Enter end date (YYYY-MM-DD): ";
            cin >> end;
            getAllEmployeesSummary(start, end);
        }
        else if (choice == 4)
        {
            string date;
            cout << "\nEnter date (YYYY-MM-DD): ";
            cin >> date;
            getPresentAbsentReport(date);
        }
        else if (choice == 5)
        {
            string dept, start, end;
            cout << "\nEnter the department: ";
            cin >> dept;
            cout << "Enter the start date (YYYY-MM-DD): ";
            cin >> start;
            cout << "Enter the end date (YYYY-MM-DD): ";
            cin >> end;
            getDepartmentReport(dept, start, end);
        }
        else if (choice == 6)
        {
            int empId;
            string start, end;
            cout << "\nEnter the EmployeeId: ";
            cin >> empId;
            cout << "Enter the start date (YYYY-MM-DD): ";
            cin >> start;
            cout << "Enter the end date (YYYY-MM-DD): ";
            cin >> end;
            calculateMonthlyPayForEmployee(empId, start, end);
        }
        else if (choice == 7)
        {
            string dept, start, end;
            cout << "\nEnter the department: ";
            cin >> dept;
            cout << "Enter the start date (YYYY-MM-DD): ";
            cin >> start;
            cout << "Enter the end date (YYYY-MM-DD): ";
            cin >> end;
            calculateMonthlyPayForDepartment(dept, start, end);
        }
        else if (choice == 8)
        {
            string start, end;
            cout << "\nEnter the start date (YYYY-MM-DD): ";
            cin >> start;
            cout << "Enter the end date (YYYY-MM-DD): ";
            cin >> end;
            calculateMonthlyPayForAll(start, end);
        }
        else if (choice == 9)
        {
            int empId;
            string date, inTime;
            cout << "\nEnter EmployeeId: ";
            cin >> empId;
            cout << "Enter date: ";
            cin >> date;
            cout << "Enter In Time (HH:MM): ";
            cin >> inTime;
            clockIn(empId, date, inTime);
            cout << "Clock In recorded\n";
        }
        else if (choice == 10)
        {
            int empId;
            string date, outTime;
            cout << "\nEnter the EmployeeId: ";
            cin >> empId;
            cout << "Enter the date (YYYY-MM-DD): ";
            cin >> date;
            cout << "Enter the Out Time (HH:MM): ";
            cin >> outTime;
            clockOut(empId, date, outTime);
            cout << "Clock out recorded\n";
        }
        else if (choice == 11)
        {
            cout << "\nExiting the program";
            break;
        }
        else
        {
            cout << "\nInvalid option try again\n";
        }
    }
}