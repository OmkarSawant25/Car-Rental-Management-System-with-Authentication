# 🚗 Car Rental Management System (with Authentication)

## 📘 Overview
This project implements a **Car Rental Management System** in **C programming** with an integrated **User Authentication module**.  
It allows **Admins** to manage cars and **Customers** to rent or return vehicles efficiently — all through a **console-based interface** with **persistent file storage**.

The system uses structured data handling, secure password entry, and text file storage for a complete functional car rental application.

---

## ⚙️ Features
✅ User Authentication with secure password input  
👨‍💼 Admin & Customer roles with different permissions  
🚘 Add, Delete, and View Cars (Admin only)  
📋 Rent, Return, and List Cars (Customer only)  
📄 Persistent data saved in text files  
🧠 Modular design with structured C code  
🔍 Sorting options by brand, model, price, and status  
💬 Console-based interactive interface  

---

## 🧩 Project Structure
```
├── main.c              # Entry point (handles menus and user interaction)
├── users.txt           # Stores registered users (username, password, role)
├── cars.txt            # Stores car details (brand, model, year, price, availability)
├── rentals.txt         # Stores rental records
└── README.md           # Project documentation
```
---

## 🧠 Working Principle
The program operates through a **menu-driven interface**:

1. **Login or Register**
   - New users register as **Admin** (with secret key) or **Customer**.
   - Password input is hidden for privacy.
2. **Admin Panel**
   - Add or remove cars.
   - View all cars sorted by various attributes.
3. **Customer Panel**
   - View available cars.
   - Rent a car and receive a **bill receipt**.
   - Return a car and view a **return summary**.
4. **Persistent Storage**
   - All records are stored in text files for reuse after restarting the program.

---

## 🧮 Authentication System
- Each user record contains:
  - Username  
  - Password  
  - Role (Admin or Customer)
- Admin registration requires the **secret key**: `admin123`
- Password input uses character masking (not visible while typing).

---

## 🧾 Admin Functionalities
1. Add Car  
2. List Cars (with sorting options)  
3. Delete Car  
4. Back to Main Menu  

---

## 🚘 Customer Functionalities
1. List Cars  
2. Rent Car (generates detailed bill receipt)  
3. Return Car (updates car status and rental record)  
4. List Rentals (view history)  
5. Back to Main Menu  

---

## 🧮 Data Structures

### `User`
```c
typedef struct {
    char username[50];
    char password[50];
    int isAdmin;
} User;
```

### `Car`
```c
typedef struct {
    int id;
    char brand[30];
    char model[30];
    int year;
    float pricePerDay;
    int isAvailable;  // 1 = available, 0 = rented
} Car;
```

### `Rental`
```c
typedef struct {
    int rentalId;
    char customerName[50];
    int carId;
    int days;
    float totalCost;
    int isReturned;  // 0 = active, 1 = returned
} Rental;
```

---

## 💻 Sample Console Output

### 🧱 Admin Login
```
========================================
     🚗 CAR RENTAL MANAGEMENT SYSTEM
========================================

🔐 Login as Admin
Username : admin
Password : ********

✅ Login successful!
```

### 👑 Admin Menu
```
1. Add Car
2. List Cars (Sort by ID/Brand/Price/Status)
3. Delete Car
4. Back to Main Menu
```

### 🙋 Customer Menu
```
1. List Cars
2. Rent Car
3. Return Car
4. View Rental History
5. Back to Main Menu
```

### 🧾 Sample Bill Receipt
```
========================================
            CAR RENTAL BILL
========================================
Rental ID       : 1
Customer Name   : omkar
Car ID          : 2
Car Brand       : Toyota
Car Model       : Camry
Rental Days     : 3
Price per Day   : ₹1500.00
----------------------------------------
Total Cost      : ₹4500.00
Status          : Rented
========================================
```

---

## 🧭 Command-Line Execution

### 🧱 Compilation
```bash
gcc main.c -o car_rental
```

### ▶️ Run the Program
```bash
./car_rental
```

---

## 🧠 File Handling Summary

| File Name | Description |
|------------|--------------|
| `users.txt` | Stores usernames, passwords, and roles |
| `cars.txt` | Stores details of all cars |
| `rentals.txt` | Stores rental history and return status |

---

## 🚀 Future Enhancements
🔹 Add password encryption (e.g., MD5/SHA hashing)  
🔹 Include search and filter options  
🔹 Add date-based rental tracking  
🔹 Implement GUI version using GTK or Qt  
🔹 Enable JSON or binary data storage  

---

## 👨‍💻 Author
**Omkar Ashok Sawant**  
📍 *Bengaluru, Karnataka, India*  
💡 *Passionate about C Programming and Embedded Systems.*

---

### 🏷️ Tagline
> **"Drive the logic — Rent with code."**
