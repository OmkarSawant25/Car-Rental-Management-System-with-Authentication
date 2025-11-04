/*
===============================================================================
Project Title : Car Rental Management System (with Authentication)
Language      : C Programming
===============================================================================

Features:
    - User registration & login (password hidden while typing)
    - Admin authentication with special key
    - Admin menu: Add Car, List Cars, Delete Car
    - Customer menu: Rent Car, Return Car, List Rentals
    - File handling with robust fscanf/fprintf
    - Rent Car prints a bill-style receipt
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdio_ext.h>

// --------------------------- Definitions ---------------------------
#define MAX_USER 100
#define CREDENTIAL_LENGTH 30
#define MAX_CARS 100
#define MAX_RENTALS 100
#define ADMIN_KEY "admin123"

// --------------------------- Structures ---------------------------
typedef struct
{
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];
    int isAdmin;
} User;

typedef struct
{
    int id;
    char brand[30];
    char model[30];
    int year;
    float pricePerDay;
    int isAvailable;
} Car;

typedef struct
{
    int rentalId;
    char customerName[50];
    int carId;
    int days;
    float totalCost;
} Rental;

// --------------------------- Global Variables ---------------------------
User users[MAX_USER];
Car cars[MAX_CARS];
Rental rentals[MAX_RENTALS];

int user_count = 0;
int carCount = 0;
int rentalCount = 0;

// --------------------------- Function Prototypes ---------------------------
// Authentication
void register_user();
int login_user();
void input_credentials(char *username, char *password);
void fix_gets_input(char *s);

// Menus
void mainMenu(int userIndex);
void adminMenu();
void customerMenu(const char *username);

// Car functions
void addCar();
void listCars();
void deleteCar();

// Rental functions
void rentCar(const char *username);
void returnCar();
void listRentals();

// File handling
void loadData();
void saveData();

// Sorting Prototypes 
void sortById();
void sortByBrand();
void sortByModel();
void sortByYear();
void sortByPrice();
void sortByStatus();

// --------------------------- MAIN ---------------------------
int main()
{
    loadData();

    int option, user_index;
    while (1)
    {
        printf("\n===== Car Rental System =====\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Select an option: ");
        // scanf("%d", &option);
        if (scanf("%d", &option) != 1)
        {
            printf("❌ Invalid input! Enter a number.\n");
            while (getchar() != '\n')
                ; // clear buffer
            continue;
        }
        while (getchar() != '\n')
            ; // clear newline

        switch (option)
        {
        case 1:
            register_user();
            saveData();
            break;
        case 2:
            user_index = login_user();
            if (user_index >= 0)
            {
                printf("\n✅ Login Successful! Welcome %s.\n", users[user_index].username);
                mainMenu(user_index);
            }
            else
            {
                printf("\n❌ Login Failed! Incorrect Username or Password.\n");
            }
            break;
        case 3:
            printf("\nExiting...\n");
            saveData();
            return 0;
        default:
            printf("\nInvalid choice. Try again.\n");
            // __fpurge(stdin);
        }
    }
}

// --------------------------- Authentication ---------------------------

void register_user()
{
    if (user_count == MAX_USER)
    {
        printf("\n❌ Maximum %d users supported.\n", MAX_USER);
        return;
    }

    char username[CREDENTIAL_LENGTH], password[CREDENTIAL_LENGTH];
    input_credentials(username, password);

    // Check for duplicate username
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(username, users[i].username) == 0)
        {
            printf("❌ Username already exists! Try another.\n");
            return;
        }
    }

    int index = user_count;
    strcpy(users[index].username, username);
    strcpy(users[index].password, password);

    int choice;
    printf("Do you want to register as Admin? (1=Yes, 0=No): ");
    if (scanf("%d", &choice) != 1)
        choice = 0;
    while (getchar() != '\n')
        ;

    if (choice == 1)
    {
        char adminKey[50];
        printf("Enter Admin Authentication Key: ");
        fgets(adminKey, sizeof(adminKey), stdin);
        fix_gets_input(adminKey);

        if (strcmp(adminKey, ADMIN_KEY) == 0)
        {
            users[index].isAdmin = 1;
            printf("\n✅ Admin registration successful!\n");
        }
        else
        {
            users[index].isAdmin = 0;
            printf("\n❌ Invalid key! Registered as Customer.\n");
        }
    }
    else
    {
        users[index].isAdmin = 0;
        printf("\n✅ Registered as Customer.\n");
    }

    user_count++;
    printf("\n✅ Registration Completed!\n");
}

int login_user()
{
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];

    input_credentials(username, password);

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0)
            return i;
    }
    return -1;
}

void input_credentials(char *username, char *password)
{
    printf("\nEnter username: ");
    fgets(username, CREDENTIAL_LENGTH, stdin);
    fix_gets_input(username);

    printf("Enter password: ");
    fflush(stdout);

    struct termios old_props, new_props;
    tcgetattr(STDIN_FILENO, &old_props);
    new_props = old_props;
    new_props.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_props);

    char ch;
    int i = 0;
    while ((ch = getchar()) != '\n' && ch != EOF && i < CREDENTIAL_LENGTH - 1)
    {
        if (ch == '\b' || ch == 127)
        {
            if (i > 0)
                i--;
            printf("\b \b");
        }
        else
        {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    tcsetattr(STDIN_FILENO, TCSANOW, &old_props);
    printf("\n");
}

void fix_gets_input(char *s)
{
    int pos = strcspn(s, "\n");
    s[pos] = '\0';
}

// --------------------------- Menus ---------------------------
void mainMenu(int userIndex)
{
    if (users[userIndex].isAdmin)
    {
         printf("\n---------------------------------------------------------------------------------\n");
        printf("Admin Menu\n");
        printf("---------------------------------------------------------------------------------\n");
        adminMenu();

    }
    else
    {
        printf("\n---------------------------------------------------------------------------------\n");
        printf("Customer Menu\n");
        printf("---------------------------------------------------------------------------------\n");
        customerMenu(users[userIndex].username);

    }
}

void adminMenu()
{
    int choice;
    do
    {
        printf("\nAdmin Menu\n");
        printf("1. Add Car\n2. List Cars\n3. Delete Car\n4. Back to Main Menu\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
            choice = 4;
        while (getchar() != '\n')
            ;

        switch (choice)
        {
        case 1:
            addCar();
            saveData();
            break;
        case 2:
            listCars();
            break;
        case 3:
            deleteCar();
            saveData();
            break;
        case 4:
            return;
        default:
            printf("Invalid choice!\n");
        }
    } while (choice != 4);
}

void customerMenu(const char *username)
{
    int choice;
    do
    {
        printf("\nCustomer Menu\n");
        printf("1. List Cars\n2. Rent Car\n3. Return Car\n4. List Rentals\n5. Back to Main Menu\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
            choice = 5;
        while (getchar() != '\n')
            ;

        switch (choice)
        {
        case 1:
            listCars();
            break;
        case 2:
            rentCar(username);
            saveData();
            break;
        case 3:
            returnCar();
            saveData();
            break;
        case 4:
            listRentals();
            break;
        case 5:
            return;
        default:
            printf("Invalid choice!\n");
        }
    } while (choice != 5);
}

// --------------------------- Cars ---------------------------
void addCar()
{
    if (carCount >= MAX_CARS)
    {
        printf("❌ Car list full!\n");
        return;
    }

    Car c;
    c.id = carCount + 1;

    printf("Enter Car Brand: ");
    fgets(c.brand, sizeof(c.brand), stdin);
    fix_gets_input(c.brand);

    printf("Enter Car Model: ");
    fgets(c.model, sizeof(c.model), stdin);
    fix_gets_input(c.model);

     // Check for duplicate Brand+Model
    for (int i = 0; i < carCount; i++)
    {
        if (strcmp(c.brand, cars[i].brand) == 0 && strcmp(c.model, cars[i].model) == 0)
        {
            printf("❌ This car is already added!\n");
            return;
        }
    }

    printf("Enter Year: ");
    scanf("%d", &c.year);
    while (getchar() != '\n');

    printf("Enter Price per Day: ");
    scanf("%f", &c.pricePerDay);
    while (getchar() != '\n');

    c.isAvailable = 1;
    cars[carCount++] = c;
    printf("✅ Car Added Successfully!\n");
}

// void listCars()
// {
//     if (carCount == 0)
//     {
//         printf("❌ No cars available!\n");
//         return;
//     }
//     printf("---------------------------------------------------------------------------------\n");
//     printf("Car List\n");
//     printf("---------------------------------------------------------------------------------\n");
//     printf("%-8s %-15s %-15s %-10s %10s %15s\n", "Car ID", "Brand", "Model", "Year", "Cost/Day", "Status");
//     printf("---------------------------------------------------------------------------------\n");
//     for (int i = 0; i < carCount; i++)
//     {
//         printf("%-8d %-15s %-15s %-10d %10.02f %15s\n",
//                cars[i].id, cars[i].brand, cars[i].model, cars[i].year, cars[i].pricePerDay,
//                cars[i].isAvailable ? "Available" : "Rented");
//     }
//     printf("---------------------------------------------------------------------------------\n");
// }

void listCars()
{
    if (carCount == 0)
    {
        printf("❌ No cars available!\n");
        return;
    }

    char choice;
    do
    {
        printf("\n----------------------------------------------------\n");
        printf("List & Sort Cars\n");
        printf("----------------------------------------------------\n");
        printf("1. Sort by Car ID\n");
        printf("2. Sort by Brand\n");
        printf("3. Sort by Model\n");
        printf("4. Sort by Year\n");
        printf("5. Sort by Cost/Day\n");
        printf("6. Sort by Status\n");
        printf("7. Exit to Menu\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice)
        {
        case '1': sortById(); break;
        case '2': sortByBrand(); break;
        case '3': sortByModel(); break;
        case '4': sortByYear(); break;
        case '5': sortByPrice(); break;
        case '6': sortByStatus(); break;
        case '7': continue;
        default: printf("Invalid choice! Try again.\n"); continue;
        }

        printf("---------------------------------------------------------------------------------\n");
        printf("%-8s %-15s %-15s %-10s %10s %15s\n", "Car ID", "Brand", "Model", "Year", "Cost/Day", "Status");
        printf("---------------------------------------------------------------------------------\n");
        for (int i = 0; i < carCount; i++)
        {
            printf("%-8d %-15s %-15s %-10d %10.02f %15s\n",
                   cars[i].id, cars[i].brand, cars[i].model,
                   cars[i].year, cars[i].pricePerDay,
                   cars[i].isAvailable ? "Available" : "Rented");
        }
        printf("---------------------------------------------------------------------------------\n");

    } while (choice != '7');
}


// --------------------------- Rentals ---------------------------
void rentCar(const char *username)
{
    int id, days;
    printf("Enter Car ID to Rent: ");
    scanf("%d", &id);
    while (getchar() != '\n')
        ;

    int found = -1;
    for (int i = 0; i < carCount; i++)
        if (cars[i].id == id && cars[i].isAvailable)
        {
            found = i;
            break;
        }

    if (found == -1)
    {
        printf("❌ Car not available!\n");
        return;
    }

    printf("Enter Number of Days: ");
    scanf("%d", &days);
    while (getchar() != '\n')
        ;

    cars[found].isAvailable = 0;

    Rental r;
    r.rentalId = rentalCount + 1;
    strcpy(r.customerName, username);
    r.carId = cars[found].id;
    r.days = days;
    r.totalCost = days * cars[found].pricePerDay;

    rentals[rentalCount++] = r;

    // ----- Print Bill -----
    printf("\n========================================\n");
    printf("            CAR RENTAL BILL             \n");
    printf("========================================\n");
    printf("Rental ID       : %d\n", r.rentalId);
    printf("Customer Name   : %s\n", r.customerName);
    printf("Car ID          : %d\n", r.carId);
    printf("Car Brand       : %s\n", cars[found].brand);
    printf("Car Model       : %s\n", cars[found].model);
    printf("Rental Days     : %d\n", r.days);
    printf("Price per Day   : ₹%.2f\n", cars[found].pricePerDay);
    printf("----------------------------------------\n");
    printf("Total Cost      : ₹%.2f\n", r.totalCost);
    printf("Status          : %s\n", cars[found].isAvailable ? "Available" : "Rented");
    printf("========================================\n\n");
}

void returnCar()
{
    int rid;
    printf("Enter Rental ID to Return: ");
    scanf("%d", &rid);
    while (getchar() != '\n')
        ;

    int found = -1;
    for (int i = 0; i < rentalCount; i++)
    {
        if (rentals[i].rentalId == rid)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("❌ Rental not found!\n");
        return;
    }

    // Mark car available again
    int carIndex = -1;
    for (int i = 0; i < carCount; i++)
    {
        if (cars[i].id == rentals[found].carId)
        {
            cars[i].isAvailable = 1;
            carIndex = i;
            break;
        }
    }

    // Print return receipt
    printf("\n========================================\n");
    printf("         CAR RETURN RECEIPT             \n");
    printf("========================================\n");
    printf("Rental ID       : %d\n", rentals[found].rentalId);
    printf("Customer Name   : %s\n", rentals[found].customerName);
    printf("Car ID          : %d\n", rentals[found].carId);
    if (carIndex != -1)
    {
        printf("Car Brand       : %s\n", cars[carIndex].brand);
        printf("Car Model       : %s\n", cars[carIndex].model);
        printf("Price per Day   : ₹%.2f\n", cars[carIndex].pricePerDay);
    }
    printf("Rental Days     : %d\n", rentals[found].days);
    printf("----------------------------------------\n");
    printf("Total Cost      : ₹%.2f\n", rentals[found].totalCost);
    printf("Status          : Returned ✅\n");
    printf("========================================\n\n");

    printf("✅ Car Returned Successfully!\n");
}

// --------------------------- Delete Car ---------------------------
void deleteCar()
{
    int id;
    printf("Enter Car ID to Delete: ");
    scanf("%d", &id);
    while (getchar() != '\n')
        ; // clear newline

    int found = -1;
    for (int i = 0; i < carCount; i++)
    {
        if (cars[i].id == id)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("❌ Car not found!\n");
        return;
    }

    // Shift remaining cars left
    for (int i = found; i < carCount - 1; i++)
    {
        cars[i] = cars[i + 1];
    }

    carCount--;
    printf("✅ Car Deleted!\n");
}

void listRentals()
{
    if (rentalCount == 0)
    {
        printf("❌ No rentals yet!\n");
        return;
    }
    printf("\n---------------------------------------------------------------------------------\n");
    printf("Rental List\n");
    printf("---------------------------------------------------------------------------------\n");
    printf("%-10s %-30s %-10s %-10s %20s\n", "Rental ID", "User", "CarID", "Days", "Total Cost");
    printf("---------------------------------------------------------------------------------\n");

    for (int i = 0; i < rentalCount; i++)
        printf("%-10d %-30s %-10d %-10d ₹%20.2f\n", rentals[i].rentalId, rentals[i].customerName, rentals[i].carId, rentals[i].days, rentals[i].totalCost);

    printf("---------------------------------------------------------------------------------\n");
}

// --------------------------- File Handling ---------------------------
void saveData()
{
    FILE *fp;

    // Users
    fp = fopen("users.txt", "w");
    if (fp)
    {
        fprintf(fp, "%d\n", user_count);
        for (int i = 0; i < user_count; i++)
            fprintf(fp, "%s,%s,%d\n", users[i].username, users[i].password, users[i].isAdmin);
        fclose(fp);
    }

    // Cars
    fp = fopen("cars.txt", "w");
    if (fp)
    {
        fprintf(fp, "%d\n", carCount);
        for (int i = 0; i < carCount; i++)
            fprintf(fp, "%d,%s,%s,%d,%.2f,%d\n", cars[i].id, cars[i].brand, cars[i].model, cars[i].year, cars[i].pricePerDay, cars[i].isAvailable);
        fclose(fp);
    }

    // Rentals
    fp = fopen("rentals.txt", "w");
    if (fp)
    {
        fprintf(fp, "%d\n", rentalCount);
        for (int i = 0; i < rentalCount; i++)
            fprintf(fp, "%d,%s,%d,%d,%.2f\n", rentals[i].rentalId, rentals[i].customerName, rentals[i].carId, rentals[i].days, rentals[i].totalCost);
        fclose(fp);
    }
}

void loadData()
{
    FILE *fp;

    // Users
    fp = fopen("users.txt", "r");
    if (fp)
    {
        fscanf(fp, "%d\n", &user_count);
        for (int i = 0; i < user_count; i++)
            fscanf(fp, " %[^,],%[^,],%d\n", users[i].username, users[i].password, &users[i].isAdmin);
        fclose(fp);
    }
    else
        user_count = 0;

    // Cars
    fp = fopen("cars.txt", "r");
    if (fp)
    {
        fscanf(fp, "%d\n", &carCount);
        for (int i = 0; i < carCount; i++)
            fscanf(fp, " %d,%[^,],%[^,],%d,%f,%d\n", &cars[i].id, cars[i].brand, cars[i].model, &cars[i].year, &cars[i].pricePerDay, &cars[i].isAvailable);
        fclose(fp);
    }
    else
        carCount = 0;

    // Rentals
    fp = fopen("rentals.txt", "r");
    if (fp)
    {
        fscanf(fp, "%d\n", &rentalCount);
        for (int i = 0; i < rentalCount; i++)
            fscanf(fp, " %d,%[^,],%d,%d,%f\n", &rentals[i].rentalId, rentals[i].customerName, &rentals[i].carId, &rentals[i].days, &rentals[i].totalCost);
        fclose(fp);
    }
    else
        rentalCount = 0;
}

void sortById()
{
    for (int i = 0; i < carCount - 1; i++)
    {
        for (int j = 0; j < carCount - i - 1; j++)
        {
            if (cars[j].id > cars[j + 1].id)
            {
                Car temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
            }
        }
    }
}

void sortByBrand()
{
    for (int i = 0; i < carCount - 1; i++)
    {
        for (int j = 0; j < carCount - i - 1; j++)
        {
            if (strcmp(cars[j].brand, cars[j + 1].brand) > 0)
            {
                Car temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
            }
        }
    }
}

void sortByModel()
{
    for (int i = 0; i < carCount - 1; i++)
    {
        for (int j = 0; j < carCount - i - 1; j++)
        {
            if (strcmp(cars[j].model, cars[j + 1].model) > 0)
            {
                Car temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
            }
        }
    }
}

void sortByYear()
{
    for (int i = 0; i < carCount - 1; i++)
    {
        for (int j = 0; j < carCount - i - 1; j++)
        {
            if (cars[j].year > cars[j + 1].year)
            {
                Car temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
            }
        }
    }
}

void sortByPrice()
{
    for (int i = 0; i < carCount - 1; i++)
    {
        for (int j = 0; j < carCount - i - 1; j++)
        {
            if (cars[j].pricePerDay > cars[j + 1].pricePerDay)
            {
                Car temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
            }
        }
    }
}

void sortByStatus()
{
    for (int i = 0; i < carCount - 1; i++)
    {
        for (int j = 0; j < carCount - i - 1; j++)
        {
            if (cars[j].isAvailable < cars[j + 1].isAvailable) // Available first
            {
                Car temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
            }
        }
    }
}
