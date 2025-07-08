# 🕒 Attendance System with Payroll Management

A C++ application to manage employee attendance, calculate salaries based on working hours, and generate department-wise and organization-wide payroll summaries. The system uses OOP concepts like inheritance, polymorphism, and STL containers for efficient data handling.

---

## 📌 Features

### ✅ Employee Management
- Add employees with ID, name, department, clock-in method (e.g., Biometric, Card), and hourly pay rate.
- Supports multiple departments like **Tech**, **Sales**, etc.

### 🕐 Attendance Tracking
- Clock-in and clock-out with time stamps per employee and date.
- Track daily work duration and total worked hours over a date range.

### 💰 Payroll Management
- Calculates salary based on hourly rate and total minutes worked.
- Generate:
  - Employee-wise pay reports
  - Department-wise salary reports
  - Organization-wide payroll summary

### 🏢 Department Reports
- View all employees in a department.
- Track department's total pay over a period.

---

## 🧠 Concepts Used

- **OOP** (Classes, Inheritance, Virtual Functions)
- **Strategy Pattern** for clock-in methods
- **STL Containers**: `map`, `vector`
- **Date and Time Handling** using `chrono` and `ctime`
- **Precision formatting** for financial output

---

## 🛠️ How It Works

1. **Add Employees**
   ```cpp
   system.addEmployee(Employee(1, "Harish", "Tech", &bio, 500));
