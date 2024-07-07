#include <stdio.h>     // Library for standard input and output
#include <stdlib.h>    // Library for memory allocation, process control, conversions, and more
#include <string.h>    // Library for string handling
#include <windows.h>   // Declarations for Windows-specific API
#include <conio.h>     // Functions for console input/output
#include <stdbool.h>   // Boolean Data type
#include <ctype.h>	   // For its function such as ispunct and isdigit

// Constants for user and employee management
#define MAXUSERS 100                // Users can't exceed this number
#define USERNAMELENGTH 50           // Username can't be longer than this
#define PASSWORDLENGTH 50           // Password can't be longer than this

// File paths for user and employee data storage
#define EMPLOYEE_FILE "Employees.bin" // Employee info goes in this binary file
#define FILENAME "Login.bin"          // Login info goes in this binary file

// Color codes for console output
#define RESET 7                     // Console's default shade
#define RED FOREGROUND_RED          // Red marks errors
#define GREEN FOREGROUND_GREEN      // Green shows success
#define YELLOW (FOREGROUND_RED | FOREGROUND_GREEN) // Yellow flags warnings
#define BLUE (FOREGROUND_BLUE)      // Blue hue
#define CYAN (FOREGROUND_GREEN | FOREGROUND_BLUE)  // Cyan tint

// XOR key for encryption and decryption
#define XOR_KEY 0xAA

// Define the User structure
struct User {
    int uid;                               // User ID
    char username[USERNAMELENGTH];         // Username of the user
    char password[PASSWORDLENGTH];         // Password of the user
    char status;                           // 'P' for Pending, 'A' for Approved
};

// Define the Payroll structure
struct Payroll {
    double salary;                         // Base pay
    double allowances;                     // Extra perks
    double overtime;                       // Extra hours compensation
    double bonuses;                        // Additional rewards
    double tax_deduction;                  // Money taken for taxes
    double pf_deduction;                   // Cash set aside for retirement
    double insurance_premium;              // Health coverage costs
    double net_salary;                     // Take-home pay
    int year;                              // Payroll year
};

// Employee structure definition
struct Employee {
    char username[USERNAMELENGTH];         // Employee's username
    char password[PASSWORDLENGTH];         // Employee's password
    int uid;                               // Employee's user ID
    struct Payroll payrolls[100];          // Multiple years of payroll records
    int currentYear;                       // Current payroll processing year
    char status;                           // 'A' means Active, 'I' means Inactive, 'T' means Terminated
    char position[USERNAMELENGTH];         // Employee's job title
    unsigned long long int contact_no;     // Employee's phone number
    char email[100];                       // Employee's email address
    char gender[2];                        // Employee's gender ('M' for Male, 'F' for Female, etc.)
    char marital_status[2];                // Employee's marital status ('M' for Married, 'S' for Single)
};

// Global variables handle user and employee data
struct User *userList;                     // Users live here
int currentUserCount = 0;                  // How many users we've got
int latestUID = 1;                         // User IDs kick off here

struct Employee *employeeList;             // Employees hang out in this list
int currentEmployeeCount = 0;              // Our current employee headcount

//Function Declaration
// Sets the console text color
void setColor(int);

// Encrypts multiple files
void encryptFiles();

// Handles console signals for encryption on exit
BOOL WINAPI ConsoleHandler(DWORD);

// Sets up the console handler for encryption on exit
void setupConsoleHandler();

// Clears the console screen
void clearScreen();

// Displays a loading animation
void displayLoading();

// Displays an exiting animation
void displayExiting();

// Securely gets password input from the user
void getPasswordInput(char *);

// Saves user data to file
void saveUserData();

// Loads user data from file
void loadUserData();

// Saves employee data to file
void saveEmployeeData();

// Loads employee data from file
void loadEmployeeData();

// Creates a unique user ID
void createUID(int*);

// Prompts the user to continue or exit
bool continuePrompt();

// Registers a new user
void registerUser();

// Updates personal details of an employee
bool updatePersonalDetails();

// Updates the password of an employee
void updatePassword();

// Resets the user's password
void resetUserPassword();

// Views personal information of an employee
void viewPersonalInformation(int);

// Views the payslip of an employee
void viewPaySlip(int);

//Views the Audit log of Client
void viewClientAuditLog(int);

// Displays the client menu
void clientMenu(int);

// Authenticates the user
int authenticateUser();

// Checks the user approval status
int checkUserApprovalStatus(int, char*);

// Checks approval notice for a user
void checkApprovalNotice(int);

// Displays employee status notice
void employeeStatusNotice();

// Displays the main client menu
void mainClientMenu();

// Saves the admin password to file
void saveAdminPassword(const char*);

// Loads the admin password from file
int loadAdminPassword(char* , int);

// Checks if the initial admin password is set
bool isInitialAdminPasswordSet();

// Saves the initial admin password to file
void saveInitialAdminPassword();

// Validates the admin password
int validateAdminPassword(const char*);

// Changes the admin password
void adminChangeAdminPassword();

// Approves a new employee
void adminApproveNewEmployee();

// Edits employee details
void adminEditEmployeeDetails();

// Deletes an employee record
void adminDeleteEmployeeRecord();

// Searches for an employee record
void adminSearchEmployeeRecord();

// Changes the status of an employee
void adminChangeEmployeeStatus();

// Views all employee details
void adminViewAllEmployeeDetails();

// Processes payroll for employees
void adminPayrollProcessing();

// Updates new payroll processing
void updateNewPayrollProcessing();

// Modifies a payroll entry
void modifyPayrollEntry();

// Views all employee payroll summary
void viewAllEmployeePayrollSummary();

// Searches for an employee payroll summary
void searchEmployeePayrollSummary();

// Displays the payroll guideline
void displayPayrollGuideline();

//Function for Menu of Audit Log in Admin section
void auditLogMenu();

// Views the audit log for all employees
void viewAllEmployeeAuditLog();

// Searches for an employee audit log
void searchEmployeeAuditLog();

// Encrypts a file using XOR encryption
void encryptFile(const char *);

// Decrypts a file using XOR encryption
void decryptFile(const char *);

// Manages employee information in the admin menu
void adminEmployeeInfoManagement();

// Displays the admin menu
void adminMenu();

// Displays the main menu
void mainMenu();

//Display the Provacy Policy of Our Program
void displayPrivacyPolicy();

//Display about the program and how it facilitates the admin and employee
void displayAboutUs();

//Main Function
int main() {
    setupConsoleHandler();
    atexit(encryptFiles);

    // Allocate memory for user and employee lists
    userList = (struct User*)malloc(MAXUSERS * sizeof(struct User));
    employeeList = (struct Employee*)malloc(MAXUSERS * sizeof(struct Employee));
    
    if (userList == NULL || employeeList == NULL) {
        setColor(RED);
        printf("Error: Memory allocation failed!\n");
        setColor(RESET);
        return 1;
    }

    // Ensure the necessary files are decrypted before use
    decryptFile("Login.bin");
    decryptFile("Employees.bin");
    decryptFile("AdminPassword.txt");

	//Loading the data of User and Employee before main menu
    loadUserData();
    loadEmployeeData();

    // Setup initial admin password if not already set
    if (!isInitialAdminPasswordSet()) {
        saveInitialAdminPassword();
    }

    // Main menu
    mainMenu();

    // Free allocated memory
    free(userList);
    free(employeeList);

    return 0;
}

//Function Definition
// Sets the console text color
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Clears the console screen
void clearScreen() {
    system("cls");
}

// Encrypts multiple files
void encryptFiles() {
    encryptFile("Login.bin");
	encryptFile("Employees.bin");
    encryptFile("AdminPassword.txt");
}

// Handles console signals for encryption on exit
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_CLOSE_EVENT || signal == CTRL_LOGOFF_EVENT || signal == CTRL_SHUTDOWN_EVENT) {
        encryptFiles();
        return TRUE;
    }
    return FALSE;
}

// Sets up the console handler for encryption on exit
void setupConsoleHandler() {
    if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
        exit(1);
    }
}

//Displays a Loading Animation
void displayLoading() {
    setColor(YELLOW);
    printf("Loading");
    int i;
    for (i = 0; i < 3; i++) {
        setColor(YELLOW);
        printf(".");
        fflush(stdout);
        Sleep(500);
    }
    printf("\n");
    clearScreen();
}

// Displays an exiting animation
void displayExiting() {
    clearScreen();
    int i = 0;
    setColor(RED);
    printf("Exiting");
    for (i = 0; i < 3; i++) {
        setColor(RED);
        printf(".");
        fflush(stdout);
        Sleep(500);
    }
    printf("\n");
    setColor(RESET);
    exit(0);
}

// Securely gets password input from the user
void getPasswordInput(char *password) {
    int i = 0;
    char ch;
    while (1) {
        ch = _getch();
        if (ch == '\r') {
            break;
        } else if (ch == '\b' && i > 0) {
            i--;
            printf("\b \b");
        } else if (ch != '\b') {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
}

// Saves user data to file
void saveUserData() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        setColor(RED);
        printf("Error: Cannot open file for writing!\n");
        setColor(RESET);
        return;
    }

    fwrite(&currentUserCount, sizeof(int), 1, file);
    fwrite(&latestUID, sizeof(int), 1, file);  // Save latest UID
    fwrite(userList, sizeof(struct User), currentUserCount, file);
    fclose(file);
}

// Loads user data from file
void loadUserData() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        return;
    }

    fread(&currentUserCount, sizeof(int), 1, file);
    fread(&latestUID, sizeof(int), 1, file);  // Load latest UID
    fread(userList, sizeof(struct User), currentUserCount, file);
    fclose(file);
}

// Saves employee data to file
void saveEmployeeData() {
    FILE *file = fopen(EMPLOYEE_FILE, "wb");
    if (file == NULL) {
        setColor(RED);
        printf("Error: Cannot open employee file for writing!\n");
        setColor(RESET);
        return;
    }

    fwrite(&currentEmployeeCount, sizeof(int), 1, file);
    fwrite(employeeList, sizeof(struct Employee), currentEmployeeCount, file);
    fclose(file);
}

// Loads employee data from file
void loadEmployeeData() {
    FILE *file = fopen(EMPLOYEE_FILE, "rb");
    if (file == NULL) {
        return;
    }

    fread(&currentEmployeeCount, sizeof(int), 1, file);
    fread(employeeList, sizeof(struct Employee), currentEmployeeCount, file);
    fclose(file);
}

// Creates a unique user ID
void createUID(int* uid) {
    *uid = latestUID++;
}

// Prompts the user to continue or exit
bool continuePrompt() {
    char check;
    setColor(CYAN);
    printf("Do you want to continue (Y/N): ");
    setColor(RESET);
    scanf(" %c", &check);
    while (check != 'Y' && check != 'y' && check != 'N' && check != 'n') {
        setColor(RED);
        printf("Error: Invalid input! Please enter Y or N: ");
        setColor(RESET);
        scanf(" %c", &check);
    }
    if (check == 'N' || check == 'n') {
        displayExiting();
        exit(0);
    }
    return (check == 'Y' || check == 'y');
}

// Registers a new user
void registerUser() {
    setColor(BLUE);
    printf("\nRegister New User\n=========================\n");
    setColor(RESET);

    struct User newUser;
    newUser.status = 'P'; // Pending approval

    createUID(&newUser.uid);  // Automatically generate a sequential UID

    setColor(CYAN);
    printf("Generated UID: %d\n", newUser.uid);
    setColor(CYAN);
    printf("Enter Username: ");
    setColor(RESET);
    fflush(stdin);
    fgets(newUser.username, USERNAMELENGTH, stdin);
    newUser.username[strcspn(newUser.username, "\n")] = '\0';

    while (1) {
        setColor(CYAN);
        printf("Enter Password: ");
        setColor(RESET);
        getPasswordInput(newUser.password);
        printf("\n");

        setColor(CYAN);
        printf("Re-enter Password: ");
        setColor(RESET);
        char reenterPassword[PASSWORDLENGTH];
        getPasswordInput(reenterPassword);
        printf("\n");

        if (strcmp(newUser.password, reenterPassword) != 0) {
            setColor(RED);
            printf("Error: Passwords do not match! Please try again.\n");
            setColor(RESET);
            continue;
        }
        
		int i = 0;
        bool hasSpecialChar = false;
        bool hasDigit = false;
        for (i = 0; i < strlen(newUser.password); i++) {
            if (ispunct(newUser.password[i])) {
                hasSpecialChar = true;
            }
            if (isdigit(newUser.password[i])) {
                hasDigit = true;
            }
        }

        if (!hasSpecialChar || !hasDigit) {
            setColor(RED);
            printf("Error: Password must contain at least one special character and one numeric character. Please try again.\n");
            setColor(RESET);
            continue;
        }

        break;
    }

    userList[currentUserCount++] = newUser;
    saveUserData();

    setColor(GREEN);
    printf("Registration successful! Please wait for admin approval.\n");
    setColor(RESET);
}

// Updates personal details of an employee
bool updatePersonalDetails(int uid) {
    struct Employee *employee = NULL;
    bool maritalStatusUpdated = false;

    // Find the employee by UID
    int i = 0;
    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            employee = &employeeList[i];
            break;
        }
    }

    if (employee == NULL) {
        setColor(RED);
        printf("Error: Invalid UID!\n");
        setColor(RESET);
        return false;
    }

    // Prompt for and read contact number
    setColor(CYAN);
    printf("Enter contact number (10 digits only): ");
    setColor(RESET);
    scanf("%llu", &employee->contact_no);

    // Validate contact number length
    unsigned long long int temp = employee->contact_no;
    int digitCount = 0;
    while (temp != 0) {
        temp /= 10;
        digitCount++;
    }

    if (digitCount != 10) {
        setColor(RED);
        printf("Error: Contact number must have exactly 10 digits!\n");
        setColor(RESET);
        return false;
    }

    // Prompt for and read email address
    setColor(CYAN);
    printf("Enter email address: ");
    setColor(RESET);
    scanf("%99s", employee->email);

    // Validate email format
    if (strlen(employee->email) < 10 || strcmp(employee->email + strlen(employee->email) - 10, "@gmail.com") != 0) {
        setColor(RED);
        printf("Error: Invalid email format!\n");
        setColor(RESET);
        return false;
    }

    // Prompt for and read gender
    setColor(CYAN);
    printf("Enter gender (M/F/O): ");
    setColor(RESET);
    scanf("%1s", employee->gender);

    if (strcmp(employee->gender, "M") != 0 && strcmp(employee->gender, "F") != 0 && strcmp(employee->gender, "O") != 0) {
        setColor(RED);
        printf("Error: Invalid gender input!\n");
        setColor(RESET);
        return false;
    }

    // Prompt for and read marital status
    char newMaritalStatus[2];
    setColor(CYAN);
    printf("Enter marital status (M/S): ");
    setColor(RESET);
    scanf("%1s", newMaritalStatus);

    if (strcmp(newMaritalStatus, "M") != 0 && strcmp(newMaritalStatus, "S") != 0) {
        setColor(RED);
        printf("Error: Invalid marital status input!\n");
        setColor(RESET);
        return false;
    }

    // Check if marital status was updated
    if (strcmp(employee->marital_status, newMaritalStatus) != 0) {
        strcpy(employee->marital_status, newMaritalStatus);
        maritalStatusUpdated = true;
    }

    saveEmployeeData();
    setColor(GREEN);
    printf("Personal details updated successfully!\n");
    setColor(RESET);

    return maritalStatusUpdated;
}

// Updates the password of an employee
void updatePassword() {
    int uid;
    char oldPassword[PASSWORDLENGTH], newPassword[PASSWORDLENGTH], reenterPassword[PASSWORDLENGTH];
    setColor(CYAN);
    printf("Enter your UID: ");
    setColor(RESET);
    scanf("%d", &uid);

    setColor(CYAN);
    printf("Enter your old password: ");
    setColor(RESET);
    getPasswordInput(oldPassword);
    printf("\n");
	int i = 0;

    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid && strcmp(employeeList[i].password, oldPassword) == 0) {
            if (employeeList[i].status != 'A') {
                setColor(RED);
                printf("Error: Your account is not active!\n");
                setColor(RESET);
                return;
            }

            setColor(CYAN);
            printf("Enter a new password: ");
            setColor(RESET);
            getPasswordInput(newPassword);
            printf("\n");

            setColor(CYAN);
            printf("Re-enter your new password: ");
            setColor(RESET);
            getPasswordInput(reenterPassword);
            printf("\n");

            if (strcmp(newPassword, reenterPassword) != 0) {
                setColor(RED);
                printf("Error: Passwords do not match!\n");
                setColor(RESET);
                return;
            }

            strcpy(employeeList[i].password, newPassword);
            saveEmployeeData();

            setColor(GREEN);
            printf("Password changed successfully!\nYour Password is %s\nCaution: Keep your Password noted down and safe!\n", newPassword);
            setColor(RESET);
            return;
        }
    }

    setColor(RED);
    printf("Error: Invalid UID or old password!\n");
    setColor(RESET);
}

// Resets the user's password
void resetUserPassword() {
    int uid;
    char newPassword[PASSWORDLENGTH], reenterPassword[PASSWORDLENGTH];
    setColor(CYAN);
    printf("Enter your UID: ");
    setColor(RESET);
    scanf("%d", &uid);
	int i = 0;
	
    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            setColor(CYAN);
            printf("Enter a new password: ");
            setColor(RESET);
            getPasswordInput(newPassword);
            printf("\n");

            setColor(CYAN);
            printf("Re-enter your new password: ");
            setColor(RESET);
            getPasswordInput(reenterPassword);
            printf("\n");

            if (strcmp(newPassword, reenterPassword) != 0) {
                setColor(RED);
                printf("Error: Passwords do not match!\n");
                setColor(RESET);
                return;
            }

            strcpy(employeeList[i].password, newPassword);
            saveEmployeeData();

            setColor(GREEN);
            printf("Password reset successfully!\nYour Password is %s\nCaution: Keep your Password noted down and safe!\n", newPassword);
            setColor(RESET);
            return;
        }
    }

    setColor(RED);
    printf("Error: Invalid UID!\n");
    setColor(RESET);
}

// Views personal information of an employee
void viewPersonalInformation(int uid) {
    struct Employee *employee = NULL;
	int i = 0;

    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            employee = &employeeList[i];
            break;
        }
    }

    if (employee == NULL) {
        setColor(RED);
        printf("Error: Invalid UID!\n");
        setColor(RESET);
        return;
    }

    clearScreen();
    setColor(GREEN);
    printf("\n======================== PERSONAL INFORMATION ========================\n");
    setColor(RESET);
    printf("| %-20s | %s\n", "Field", "Value");
    printf("---------------------------------------------------------------------\n");
    printf("| %-20s | %d\n", "UID", employee->uid);
    printf("| %-20s | %s\n", "Username", employee->username);
    printf("| %-20s | %s\n", "Gender", employee->gender);
    printf("| %-20s | %s\n", "Marital Status", employee->marital_status);
    printf("| %-20s | %s\n", "Position", employee->position);
    printf("| %-20s | %llu\n", "Contact No", employee->contact_no);
    printf("| %-20s | %s\n", "Email", employee->email);
    printf("=====================================================================\n");
}

// Views the payslip of an employee
void viewPaySlip(int uid) {
    struct Employee *employee = NULL;
	int i = 0;

    // Find the employee by UID
    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            employee = &employeeList[i];
            break;
        }
    }

    if (employee == NULL) {
        setColor(RED);
        printf("Error: Invalid UID!\n");
        setColor(RESET);
        return;
    }

    int year = employee->currentYear;
    struct Payroll *payroll = &employee->payrolls[year];
    double annual_salary = payroll->net_salary * 12;

    clearScreen();
    setColor(GREEN);
    printf("\n============================= PAY SLIP =============================\n");
    setColor(RESET);
    printf("Employee: %s (UID: %d)                  (Current Year)Year: %d\n", employee->username, employee->uid, year);
    printf("---------------------------------------------------------------------\n");
    printf("| %-25s | %20s |\n", "Description", "Amount (NRs)");
    printf("---------------------------------------------------------------------\n");
    printf("| %-25s | %20.2f |\n", "Basic Salary", payroll->salary);
    printf("| %-25s | %20.2f |\n", "Allowances", payroll->allowances);
    printf("| %-25s | %20.2f |\n", "Overtime", payroll->overtime);
    printf("| %-25s | %20.2f |\n", "Bonuses", payroll->bonuses);
    printf("---------------------------------------------------------------------\n");
    printf("| %-25s | %20.2f |\n", "Tax Deduction", payroll->tax_deduction);
    printf("| %-25s | %20.2f |\n", "Provident Fund", payroll->pf_deduction);
    printf("| %-25s | %20.2f |\n", "Insurance Premium", payroll->insurance_premium);
    printf("---------------------------------------------------------------------\n");
    setColor(CYAN);
    printf("| %-25s | %20.2f |\n", "Net Salary (Monthly)", payroll->net_salary);
    printf("| %-25s | %20.2f |\n", "Annual Salary", annual_salary);
    setColor(RESET);
    printf("=====================================================================\n");
}

//Views the Audit log of Client
void viewClientAuditLog(int uid) {
    struct Employee *employee = NULL;
	int i = 0;

    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            employee = &employeeList[i];
            break;
        }
    }

    if (employee == NULL) {
        setColor(RED);
        printf("Error: Invalid UID!\n");
        setColor(RESET);
        return;
    }

    clearScreen();
    setColor(GREEN);
    printf("\n======================= AUDIT LOG FOR %s =======================\n", employee->username);
    setColor(RESET);
	int year = 0;

    for (year = 1; year <= employee->currentYear; year++) {
        struct Payroll *payroll = &employee->payrolls[year];
        double annual_salary = payroll->net_salary * 12;

        printf("\nPayroll for Year %d\n", year);
        printf("---------------------------------------------------------------------\n");
        printf("| %-25s | %20s |\n", "Description", "Amount (NRs)");
        printf("---------------------------------------------------------------------\n");
        printf("| %-25s | %20.2f |\n", "Basic Salary", payroll->salary);
        printf("| %-25s | %20.2f |\n", "Allowances", payroll->allowances);
        printf("| %-25s | %20.2f |\n", "Overtime", payroll->overtime);
        printf("| %-25s | %20.2f |\n", "Bonuses", payroll->bonuses);
        printf("---------------------------------------------------------------------\n");
        printf("| %-25s | %20.2f |\n", "Tax Deduction", payroll->tax_deduction);
        printf("| %-25s | %20.2f |\n", "Provident Fund", payroll->pf_deduction);
        printf("| %-25s | %20.2f |\n", "Insurance Premium", payroll->insurance_premium);
        printf("---------------------------------------------------------------------\n");
        setColor(CYAN);
        printf("| %-25s | %20.2f |\n", "Net Salary (Monthly)", payroll->net_salary);
        printf("| %-25s | %20.2f |\n", "Annual Salary", annual_salary);
        setColor(RESET);
        printf("=====================================================================\n");
    }
}

// Displays the client menu
void clientMenu(int uid) {
    int option;
    char check;
    bool maritalStatusSet = false;
    int i = 0;

    // Update marital status if necessary
    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid && employeeList[i].marital_status[0] != '\0') {
            maritalStatusSet = true;
            break;
        }
    }

    while (1) {
        clearScreen();
        displayLoading();
        setColor(BLUE);
        printf("\nCipherPayroll - Employee Payroll System - Client Menu\n=========================\n");
        setColor(RESET);

        // Show UID and username after successful login
        struct Employee *loggedInEmployee = NULL;
        int i = 0;
        for (i = 0; i < currentEmployeeCount; i++) {
            if (employeeList[i].uid == uid) {
                loggedInEmployee = &employeeList[i];
                break;
            }
        }

        if (loggedInEmployee != NULL) {
            setColor(GREEN);
            printf("Logged in as UID: %d, Username: %s\n", loggedInEmployee->uid, loggedInEmployee->username);
            setColor(RESET);
        }

        if (!maritalStatusSet) {
            setColor(RED);
            printf("Notice: Please update your marital status in Personal Information.\n");
            setColor(RESET);
        }

        setColor(CYAN);
        printf("1. Personal Details\n");
        printf("2. Payroll Information\n");
        printf("3. View Audit Log\n");
        printf("4. Go back to main client menu\n");
        printf("=========================\n");
        setColor(GREEN);
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                displayLoading();
                setColor(BLUE);
                printf("\nEmployee Payroll System - Personal Details\n=========================\n");
                setColor(RESET);
                int subOption;
                setColor(CYAN);
                printf("1) View Personal Information\n");
                printf("2) Update Personal Information\n");
                printf("3) Go back\n");
                setColor(GREEN);
                printf("Enter your choice: ");
                scanf("%d", &subOption);
                switch (subOption) {
                    case 1:
                        viewPersonalInformation(uid);
                        break;
                    case 2:
                        if (updatePersonalDetails(uid)) {
                            maritalStatusSet = true; // Update the status after changing
                        }
                        break;
                    case 3:
                        break;
                    default:
                        setColor(RED);
                        printf("Error: Invalid option!\n");
                        setColor(RESET);
                        break;
                }
                break;
            case 2:
                displayLoading();
                setColor(BLUE);
                printf("\nEmployee Payroll System - Payroll Information\n=========================\n");
                setColor(RESET);
                setColor(CYAN);
                printf("1) View Pay Slip\n");
                printf("2) View Payroll Guideline\n");
                printf("3) Go back\n");
                setColor(GREEN);
                printf("Enter your choice: ");
                scanf("%d", &subOption);
                switch (subOption) {
                    case 1:
                        viewPaySlip(uid);
                        break;
                    case 2:
                        displayPayrollGuideline();
                        break;
                    case 3:
                        break;
                    default:
                        setColor(RED);
                        printf("Error: Invalid option!\n");
                        setColor(RESET);
                        break;
                }
                break;
            case 3:
                viewClientAuditLog(uid);
                break;
            case 4:
                return; // Go back to the main client menu
            default:
                displayLoading();
                printf("\nEmployee Payroll System - Client Menu\n=========================\n");
                setColor(RED);
                printf("Error: Invalid option!\n");
                setColor(RESET);
                break;
        }
        if (!continuePrompt()) {
            break;
        }
    }
}


// Authenticates the user
int authenticateUser() {
    int uid;
    char password[PASSWORDLENGTH];
    setColor(CYAN);
    printf("Enter your UID: ");
    setColor(RESET);
    scanf("%d", &uid);

    setColor(CYAN);
    printf("Enter your password: ");
    setColor(RESET);
    getPasswordInput(password);  // Get password input securely
    printf("\n");

	int i = 0;
    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid && strcmp(employeeList[i].password, password) == 0) {
            if (employeeList[i].status == 'A') {
                setColor(GREEN);
                printf("Login successful!\n");
                printf("Welcome, %s!\n", employeeList[i].username);
                setColor(RESET);
                displayLoading();
                clientMenu(uid);  // Go to client menu after successful login
                return 1;
            } else {
                setColor(RED);
                printf("Error: Your account is not active!\n");
                setColor(RESET);
                return 0;
            }
        }
    }

    setColor(RED);
    printf("Error: Invalid UID or password!\nThis occurs if you haven't been approved by Admin or You entered the Invalid UID or Password!\n");
    printf("Note: Be sure to Check if Your account is active through Cheeck Approval Notice from Client Menu\n");
    setColor(RESET);
    return 0;
}

// Check user approval status
int checkUserApprovalStatus(int uid, char* password) {
	int i = 0;
    for (i = 0; i < currentUserCount; i++) {
        if (userList[i].uid == uid && strcmp(userList[i].password, password) == 0) {
            return userList[i].status == 'A'; // Return 1 if approved, 0 otherwise
        }
    }
    return -1; // User not found
}

// Checks approval notice for a user
void checkApprovalNotice(int uid) {
    char password[PASSWORDLENGTH];
    setColor(CYAN);
    printf("Enter your UID: ");
    setColor(RESET);
    scanf("%d", &uid);

    setColor(CYAN);
    printf("Enter your password: ");
    setColor(RESET);
    getPasswordInput(password);  // Get password input securely
    printf("\n");

    int status = checkUserApprovalStatus(uid, password);
    if (status == 1) {
        setColor(GREEN);
        printf("Your registration is approved.\n");
    } else if (status == 0) {
        setColor(YELLOW);
        printf("Your registration is pending approval.\n");
    } else {
        setColor(RED);
        printf("Error: Invalid UID or password!\n");
    }
    setColor(RESET);
}

// Displays employee status notice
void employeeStatusNotice() {
    int uid;
    char password[PASSWORDLENGTH];

    setColor(CYAN);
    printf("Enter your UID: ");
    setColor(RESET);
    scanf("%d", &uid);

    setColor(CYAN);
    printf("Enter your password: ");
    setColor(RESET);
    getPasswordInput(password);  // Get password input securely
    printf("\n");

    struct Employee *employee = NULL;

	int i = 0;
    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid && strcmp(employeeList[i].password, password) == 0) {
            employee = &employeeList[i];
            break;
        }
    }

    if (employee == NULL) {
        setColor(RED);
        printf("Error: Invalid UID or password!\n");
        setColor(RESET);
        return;
    }

    clearScreen();
    setColor(GREEN);
    printf("\n======================= EMPLOYEE STATUS NOTICE =======================\n");
    setColor(RESET);

    setColor(CYAN);
    printf("\nEmployee Information\n");
    printf("---------------------------------------------------------------------\n");
    printf("| %-25s | %s |\n", "Username", employee->username);
    printf("| %-25s | %s |\n", "Position", employee->position);
    printf("---------------------------------------------------------------------\n");
    printf("| %-25s | ", "Status");

    if (employee->status == 'A') {
        setColor(GREEN);
        printf("Active");
    } else if (employee->status == 'I') {
        setColor(YELLOW);
        printf("Inactive");
    } else if (employee->status == 'T') {
        setColor(RED);
        printf("Terminated");
    }

    setColor(RESET);
    printf(" |\n");
    printf("=====================================================================\n");
}

// Displays the main client menu
void mainClientMenu() {
    int option;
    int isLoggedIn = 0;
    char check = '\0';
    int uid = 0;
    while (1) {
        clearScreen();
        displayLoading();
        setColor(BLUE);
        printf("\nCipherPayroll - Main Client Menu\n=========================\n");
        setColor(RESET);
        setColor(CYAN);
        printf("1. Register\n");
        printf("2. Log In\n");
        printf("3. Change Password\n");
        printf("4. Forgot Password?\n");
        printf("5. Check Approval Notice\n");
        printf("6. Employee Status Notice\n");
        printf("7. Go back to main menu\n");
        printf("=========================\n");
        setColor(GREEN);
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                displayLoading();
                setColor(BLUE);
                printf("\nCipherPayroll\n=========================\n");
                setColor(RESET);
                setColor(CYAN);
                printf("1. Register\n");
                registerUser();
                break;
            case 2:
                displayLoading();
                setColor(BLUE);
                printf("\nCipherPayroll\n=========================\n");
                setColor(RESET);
                setColor(CYAN);
                printf("2. Log In\n");
                isLoggedIn = authenticateUser();
                if (isLoggedIn) {
                    break;
                }
                break;
            case 3:
                displayLoading();
                setColor(BLUE);
                printf("\nCipherPayroll\n=========================\n");
                setColor(RESET);
                setColor(CYAN);
                printf("3. Change Password\n");
                updatePassword();
                break;
            case 4:
                displayLoading();
                setColor(BLUE);
                printf("\nCipherPayroll\n=========================\n");
                setColor(RESET);
                setColor(CYAN);
                printf("4. Forgot Password?\n");
                resetUserPassword();
                break;
            case 5:
                displayLoading();
                setColor(BLUE);
                printf("\nCipherPayroll\n=========================\n");
                setColor(RESET);
                setColor(CYAN);
                printf("5. Check Approval Notice\n");
                checkApprovalNotice(uid);
                break;
            case 6:
                displayLoading();
                setColor(BLUE);
                printf("\nCipherPayroll\n=========================\n");
                setColor(RESET);
                setColor(CYAN);
                printf("6. Employee Status and Position Notice\n");
                employeeStatusNotice();
                break;
            case 7:
                return;
            default:
                displayLoading();
                printf("\nCipherPayroll\n=========================\n");
                setColor(RED);
                printf("Error: Invalid option!\n");
                setColor(RESET);
                break;
        }
        if (!continuePrompt()) {
            break;
        }
    }
}

// Saves the admin password to file
void saveAdminPassword(const char* password) {
    FILE* file = fopen("AdminPassword.txt", "w");
    if (file == NULL) {
        setColor(RED);
        printf("Error: Cannot open AdminPassword.txt for writing!\n");
        setColor(RESET);
        return;
    }
    fprintf(file, "%s\n", password);
    fclose(file);
}

// Loads the admin password from file
int loadAdminPassword(char* password, int maxLength) {
    FILE* file = fopen("AdminPassword.txt", "r");
    if (file == NULL) {
        setColor(RED);
        printf("Error: Cannot open AdminPassword.txt for reading!\n");
        setColor(RESET);
        return 0;
    }
    if (fgets(password, maxLength, file) == NULL) {
        fclose(file);
        return 0; // File is empty or read error
    }
    // Remove newline character if present
    password[strcspn(password, "\n")] = '\0';
    fclose(file);
    return 1;
}

// Checks if the initial admin password is set
bool isInitialAdminPasswordSet() {
    FILE* file = fopen("AdminPassword.txt", "r");
    if (file == NULL) {
        return false; // File does not exist
    }
    char buffer[256];
    bool isSet = (fgets(buffer, sizeof(buffer), file) != NULL && strlen(buffer) > 0);
    fclose(file);
    return isSet;
}

// Saves the initial admin password to file
void saveInitialAdminPassword() {
    FILE* file = fopen("AdminPassword.txt", "w");
    if (file == NULL) {
        return;
    }
    fprintf(file, "admin123\n"); // Write the initial password
    fclose(file);
}

// Function to validate admin password
int validateAdminPassword(const char* password) {
    char adminPassword[PASSWORDLENGTH];
    if (loadAdminPassword(adminPassword, PASSWORDLENGTH)) {
        return strcmp(password, adminPassword) == 0;
    }
    return 0; // Return false if admin password loading failed
}

// Changes the admin password
void adminChangeAdminPassword() {
    char currentPassword[PASSWORDLENGTH];
    char newPassword[PASSWORDLENGTH];
    char reenterPassword[PASSWORDLENGTH];
    char currentPasswordFromFile[PASSWORDLENGTH]; // Declare currentPasswordFromFile

    clearScreen();
    setColor(BLUE);
    printf("\nChange Admin's Password\n=========================\n");
    setColor(RESET);

    // Check if the admin password is already set
    if (!loadAdminPassword(currentPasswordFromFile, PASSWORDLENGTH)) {
        // Initial setup: prompt for new password
        setColor(CYAN);
        printf("Enter a new admin password: ");
        setColor(RESET);
        getPasswordInput(newPassword);
        printf("\n");

        // Check for spaces in the new password
        if (strchr(newPassword, ' ') != NULL) {
            setColor(RED);
            printf("Error: Password cannot contain spaces!\n");
            setColor(RESET);
            printf("\nPress any key to continue...\n");
            _getch();
            return;
        }

        // Check for strong password criteria
        bool hasSpecialChar = false;
        bool hasDigit = false;
        int i = 0;
        for (i = 0; i < strlen(newPassword); i++) {
            if (ispunct(newPassword[i])) {
                hasSpecialChar = true;
            }
            if (isdigit(newPassword[i])) {
                hasDigit = true;
            }
        }

        if (!hasSpecialChar || !hasDigit) {
            setColor(RED);
            printf("Error: Password must contain at least one special character and one numeric character (0-9)!\n");
            setColor(RESET);
            printf("\nPress any key to continue...\n");
            _getch();
            return;
        }

        // Prompt to re-enter new password
        setColor(CYAN);
        printf("Re-enter the new password: ");
        setColor(RESET);
        getPasswordInput(reenterPassword);
        printf("\n");

        // Validate new passwords match
        if (strcmp(newPassword, reenterPassword) != 0) {
            setColor(RED);
            printf("Error: Passwords do not match!\n");
            setColor(RESET);
            printf("\nPress any key to continue...\n");
            _getch();
            return;
        }

        // Update admin password file
        saveAdminPassword(newPassword);

        setColor(GREEN);
        printf("Admin password set successfully!\nYour Password is %s\nCaution: Keep your Password noted down and safe!\n", newPassword);
        setColor(RESET);
        printf("\nPress any key to continue...\n");
        _getch();
    } else {
        // Subsequent password change: prompt for current password
        setColor(CYAN);
        printf("Enter current admin password: ");
        setColor(RESET);
        getPasswordInput(currentPassword);
        printf("\n");

        // Validate current admin password
        if (strcmp(currentPassword, "") == 0 || strcmp(currentPassword, currentPasswordFromFile) != 0) {
            setColor(RED);
            printf("Error: Incorrect current password!\n");
            setColor(RESET);
            printf("\nPress any key to continue...\n");
            _getch();
            return;
        }

        // Prompt for new password
        setColor(CYAN);
        printf("Enter a new password: ");
        setColor(RESET);
        getPasswordInput(newPassword);
        printf("\n");

        // Check for spaces in the new password
        if (strchr(newPassword, ' ') != NULL) {
            setColor(RED);
            printf("Error: Password cannot contain spaces!\n");
            setColor(RESET);
            printf("\nPress any key to continue...\n");
            _getch();
            return;
        }

        // Check for strong password criteria
        bool hasSpecialChar = false;
        bool hasDigit = false;
        int i = 0;
        for (i = 0; i < strlen(newPassword); i++) {
            if (ispunct(newPassword[i])) {
                hasSpecialChar = true;
            }
            if (isdigit(newPassword[i])) {
                hasDigit = true;
            }
        }

        if (!hasSpecialChar || !hasDigit) {
            setColor(RED);
            printf("Error: Password must contain at least one special character and one numeric character (0-9)!\n");
            setColor(RESET);
            printf("\nPress any key to continue...\n");
            _getch();
            return;
        }

        // Prompt to re-enter new password
        setColor(CYAN);
        printf("Re-enter the new password: ");
        setColor(RESET);
        getPasswordInput(reenterPassword);
        printf("\n");

        // Validate new passwords match
        if (strcmp(newPassword, reenterPassword) != 0) {
            setColor(RED);
            printf("Error: Passwords do not match!\n");
            setColor(RESET);
            printf("\nPress any key to continue...\n");
            _getch();
            return;
        }

        // Update admin password file
        saveAdminPassword(newPassword);

        setColor(GREEN);
        printf("Admin password changed successfully!\nYour Password is %s\nCaution: Keep your Password noted down and safe!\n", newPassword);
        setColor(RESET);
        printf("\nPress any key to continue...\n");
        _getch();
    }

    // Redirect to main menu after changing the admin's password
    mainMenu();
}

// Approves a new employee
void adminApproveNewEmployee() {
    setColor(BLUE);
    printf("\nApproval for New Employee\n=========================\n");
    setColor(RESET);
	int i = 0;

    for (i = 0; i < currentUserCount; i++) {
        if (userList[i].status == 'P') {
            printf("UID: %d, Username: %s\n", userList[i].uid, userList[i].username);
        }
    }

    int uid;
    setColor(CYAN);
    printf("Enter UID of the employee to approve: ");
    setColor(RESET);
    scanf("%d", &uid);

    for (i = 0; i < currentUserCount; i++) {
        if (userList[i].uid == uid && userList[i].status == 'P') {
            userList[i].status = 'A'; // Set status to Approved

            struct Employee newEmployee;
            strcpy(newEmployee.username, userList[i].username);
            strcpy(newEmployee.password, userList[i].password);
            newEmployee.uid = userList[i].uid;
            newEmployee.currentYear = 0; // No payroll data yet
            newEmployee.status = 'A'; // Set status to Active
            strcpy(newEmployee.position, "Not Assigned"); // Set initial position
            newEmployee.contact_no = 0; // Initial contact number
            strcpy(newEmployee.email, ""); // Initial email
            strcpy(newEmployee.gender, "");
            strcpy(newEmployee.marital_status, "");

            employeeList[currentEmployeeCount++] = newEmployee;

            setColor(GREEN);
            printf("Employee approved successfully!\n");
            printf("UID: %d\n", newEmployee.uid);
            printf("Password: %s\n", newEmployee.password);
            setColor(RESET);

            saveUserData();
            saveEmployeeData();
            return;
        }
    }

    setColor(RED);
    printf("Error: UID not found or already approved!\n");
    setColor(RESET);
}

// Edits employee details
void adminEditEmployeeDetails() {
    int uid, option;
    setColor(BLUE);
    printf("\nEdit Employee Details\n=========================\n");
    setColor(RESET);

    setColor(CYAN);
    printf("Enter UID of the employee to edit: ");
    setColor(RESET);
    if (scanf("%d", &uid) != 1) {
        setColor(RED);
        printf("Error: Invalid UID input!\n");
        setColor(RESET);
        return;
    }
    int i = 0, j = 0;

    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            while (1) {
                setColor(CYAN);
                printf("\nWhat would you like to edit?\n");
                printf("1. Username\n");
                printf("2. Password\n");
                printf("3. Position\n");
                printf("4. Personal Details\n");
                printf("5. Go back\n");
                setColor(GREEN);
                printf("Enter your choice: ");
                setColor(RESET);
                if (scanf("%d", &option) != 1) {
                    setColor(RED);
                    printf("Error: Invalid option input!\n");
                    setColor(RESET);
                    continue;
                }

                switch (option) {
                    case 1:
                        setColor(CYAN);
                        printf("Enter new username: ");
                        setColor(RESET);
                        getchar(); // Consume newline left by scanf
                        fgets(employeeList[i].username, USERNAMELENGTH, stdin);
                        employeeList[i].username[strcspn(employeeList[i].username, "\n")] = '\0';
                        break;
                    case 2:
                        setColor(CYAN);
                        printf("Enter new password: ");
                        setColor(RESET);
                        getPasswordInput(employeeList[i].password);
                        printf("\n");
                        break;
                    case 3:
                        setColor(CYAN);
                        printf("Enter new position (employee, manager, chairman): ");
                        setColor(RESET);
                        getchar(); // Consume newline left by scanf
                        fgets(employeeList[i].position, USERNAMELENGTH, stdin);
                        employeeList[i].position[strcspn(employeeList[i].position, "\n")] = '\0';
                        if (strcmp(employeeList[i].position, "employee") != 0 && strcmp(employeeList[i].position, "manager") != 0 && strcmp(employeeList[i].position, "chairman") != 0) {
                            setColor(RED);
                            printf("Error: Invalid position!\n");
                            setColor(RESET);
                            return;
                        }

                        // Check if there is already a chairman
                        if (strcmp(employeeList[i].position, "chairman") == 0) {
                            for (j = 0; j < currentEmployeeCount; j++) {
                                if (j != i && strcmp(employeeList[j].position, "chairman") == 0) {
                                    setColor(RED);
                                    printf("Error: There can only be one chairman in the company!\n");
                                    setColor(RESET);
                                    return;
                                }
                            }
                        }
                        break;
                    case 4:
                        {
                            unsigned long long int newContactNo;
                            char newEmail[USERNAMELENGTH];
                            char newGender[2];

                            setColor(CYAN);
                            printf("Enter new contact number (10 digits only): ");
                            setColor(RESET);
                            if (scanf("%llu", &newContactNo) != 1) {
                                setColor(RED);
                                printf("Error: Invalid contact number input!\n");
                                setColor(RESET);
                                break;
                            }

                            // Validate contact number length
                            unsigned long long int temp = newContactNo;
                            int digitCount = 0;
                            while (temp != 0) {
                                temp /= 10;
                                digitCount++;
                            }

                            if (digitCount != 10) {
                                setColor(RED);
                                printf("Error: Contact number must have exactly 10 digits!\n");
                                setColor(RESET);
                                break;
                            }

                            setColor(CYAN);
                            printf("Enter new email address: ");
                            setColor(RESET);
                            scanf("%s", newEmail);

                            // Validate email format
                            if (strlen(newEmail) < 10 || strcmp(newEmail + strlen(newEmail) - 10, "@gmail.com") != 0) {
                                setColor(RED);
                                printf("Error: Invalid email format!\n");
                                setColor(RESET);
                                break;
                            }

                            setColor(CYAN);
                            printf("Enter gender (M/F/O): ");
                            setColor(RESET);
                            scanf("%1s", newGender);
                            if (strcmp(newGender, "M") != 0 && strcmp(newGender, "F") != 0 && strcmp(newGender, "O") != 0) {
                                setColor(RED);
                                printf("Error: Invalid gender input!\n");
                                setColor(RESET);
                                break;
                            }

                            employeeList[i].contact_no = newContactNo;
                            strcpy(employeeList[i].email, newEmail);
                            strcpy(employeeList[i].gender, newGender);
                        }
                        break;
                    case 5:
                        saveEmployeeData();
                        setColor(GREEN);
                        printf("Employee details updated successfully!\n");
                        setColor(RESET);
                        return;
                    default:
                        setColor(RED);
                        printf("Error: Invalid option!\n");
                        setColor(RESET);
                        break;
                }
            }
        }
    }

    setColor(RED);
    printf("Error: UID not found!\n");
    setColor(RESET);
}

// Deletes an employee record
void adminDeleteEmployeeRecord() {
    int uid;
    setColor(BLUE);
    printf("\nDelete Employee Record\n=========================\n");
    setColor(RESET);

    setColor(CYAN);
    printf("Enter UID of the employee to delete: ");
    setColor(RESET);
    scanf("%d", &uid);
    int i = 0, j = 0;

    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            for (j = i; j < currentEmployeeCount - 1; j++) {
                employeeList[j] = employeeList[j + 1];
            }
            currentEmployeeCount--;

            saveEmployeeData();

            setColor(GREEN);
            printf("Employee record deleted successfully!\n");
            setColor(RESET);
            return;
        }
    }

    setColor(RED);
    printf("Error: UID not found!\n");
    setColor(RESET);
}

// Searches for an employee record
void adminSearchEmployeeRecord() {
	clearScreen();
    int uid;
    setColor(BLUE);
    printf("\n======================== SEARCH EMPLOYEE RECORD ========================\n");
    setColor(RESET);

    setColor(CYAN);
    printf("Enter UID of the employee to search: ");
    setColor(RESET);
    scanf("%d", &uid);
    int i = 0;

    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            struct Employee *employee = &employeeList[i];

            printf("\nEmployee: %s (UID: %d)\n", employee->username, employee->uid);
            printf("---------------------------------------------------------------------\n");
            printf("| %-25s | %20s |\n", "Field", "Value");
            printf("---------------------------------------------------------------------\n");
            printf("| %-25s | %d\n", "UID", employee->uid);
            printf("| %-25s | %s\n", "Username", employee->username);
            printf("| %-25s | %s\n", "Gender", employee->gender);
            printf("| %-25s | %s\n", "Marital Status", employee->marital_status);
            printf("| %-25s | %s\n", "Position", employee->position);
            printf("| %-25s | %llu\n", "Contact No", employee->contact_no);
            printf("| %-25s | %s\n", "Email", employee->email);
			printf("=====================================================================\n");
            return;
        }
    }

    setColor(RED);
    printf("Error: UID not found!\n");
    setColor(RESET);
}

// Changes the status of an employee
void adminChangeEmployeeStatus() {
    int uid;
    char status;
    setColor(BLUE);
    printf("\nChange Employee Status\n=========================\n");
    setColor(RESET);

    setColor(CYAN);
    printf("Enter UID of employee: ");
    setColor(RESET);
    scanf("%d", &uid);
	int i = 0;

    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            setColor(CYAN);
            printf("Enter new status (A for Active, I for Inactive, T for Terminated): ");
            setColor(RESET);
            getchar();
            status = getchar();

            if (status == 'A' || status == 'I') {
                employeeList[i].status = status;
                saveEmployeeData();
                setColor(GREEN);
                printf("Employee status changed successfully!\n");
                setColor(RESET);
            } else if (status == 'T') {
                adminDeleteEmployeeRecord();
            } else {
                setColor(RED);
                printf("Error: Invalid status!\n");
                setColor(RESET);
            }
            return;
        }
    }

    setColor(RED);
    printf("Error: UID not found!\n");
    setColor(RESET);
}

// Views all employee details
void adminViewAllEmployeeDetails() {
    clearScreen();
    setColor(BLUE);
    printf("\n========================= ALL EMPLOYEE DETAILS ========================\n");
    setColor(RESET);
	int i = 0;

    for (i = 0; i < currentEmployeeCount; i++) {
        struct Employee *employee = &employeeList[i];

        printf("\nEmployee: %s (UID: %d)\n", employee->username, employee->uid);
        printf("---------------------------------------------------------------------\n");
        printf("| %-25s | %20s |\n", "Field", "Value");
        printf("---------------------------------------------------------------------\n");
        printf("| %-25s | %d\n", "UID", employee->uid);
        printf("| %-25s | %s\n", "Username", employee->username);
        printf("| %-25s | %s\n", "Gender", employee->gender);
        printf("| %-25s | %s\n", "Marital Status", employee->marital_status);
        printf("| %-25s | %s\n", "Position", employee->position);
        printf("| %-25s | %llu\n", "Contact No", employee->contact_no);
        printf("| %-25s | %s\n", "Email", employee->email);
        printf("=====================================================================\n");
    }
}

// Processes payroll for employees
void adminPayrollProcessing() {
    int option;

    while (1) {
        displayLoading();
        setColor(BLUE);
        printf("\nPayroll Processing\n=========================\n");
        setColor(RESET);
        setColor(CYAN);
        printf("1. Update New Payroll Processing\n");
        printf("2. Modify Entry of Payroll Processing\n");
        printf("3. View All Employee Payroll Summary\n");
        printf("4. View Payroll Guideline\n");
        printf("5. Search Employee Payroll Summary\n");
        printf("6. Back to Admin Menu\n");
        printf("=========================\n");
        setColor(GREEN);
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                updateNewPayrollProcessing();
                break;
            case 2:
                modifyPayrollEntry();
                break;
            case 3:
                viewAllEmployeePayrollSummary();
                break;
            case 4:
                displayPayrollGuideline();
                break;
            case 5:
                searchEmployeePayrollSummary();
                break;
            case 6:
                return;
            default:
                setColor(RED);
                printf("Error: Invalid option!\n");
                setColor(RESET);
                break;
        }
        if (!continuePrompt()) {
            break;
        }
    }
}

// Updates new payroll processing
void updateNewPayrollProcessing() {
    int uid;
    double salary, allowances, overtime, bonuses, insurance_premium;

    setColor(CYAN);
    printf("\nUpdate New Payroll Processing\n=========================\n");
    setColor(RESET);
    
    printf("Enter UID of the employee: ");
    scanf("%d", &uid);
	int i = 0;

    // Check if the employee's marital status is recorded
    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid && strcmp(employeeList[i].marital_status, "") == 0) {
            setColor(RED);
            printf("Error: Employee must update their marital status first.\n");
            setColor(RESET);
            return;
        }
    }

    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            setColor(CYAN);
            printf("Enter new monthly salary: ");
            setColor(RESET);
            scanf("%lf", &salary);
            
            setColor(CYAN);
            printf("Enter new allowances: ");
            setColor(RESET);
            scanf("%lf", &allowances);
            
            setColor(CYAN);
            printf("Enter new overtime payment: ");
            setColor(RESET);
            scanf("%lf", &overtime);
            
            setColor(CYAN);
            printf("Enter new bonuses: ");
            setColor(RESET);
            scanf("%lf", &bonuses);

            setColor(CYAN);
            printf("Enter new insurance premium: ");
            setColor(RESET);
            scanf("%lf", &insurance_premium);

            // Update payroll for the next year
            int year = ++employeeList[i].currentYear;
            struct Payroll *payroll = &employeeList[i].payrolls[year];
            payroll->salary = salary;
            payroll->allowances = allowances;
            payroll->overtime = overtime;
            payroll->bonuses = bonuses;
            payroll->insurance_premium = insurance_premium;
            payroll->year = year;

            // Calculate taxes and PF according to the guidelines
            double gross_salary = salary + allowances + overtime + bonuses;
            double tax_rate = (employeeList[i].marital_status[0] == 'M') ? 0.09 : 0.10;  // 9% for married, 10% for single
            double tax_deduction = gross_salary * tax_rate;
            double pf_deduction = gross_salary * 0.10;  // 10% for PF
            double net_salary = gross_salary - tax_deduction - pf_deduction - insurance_premium;

            payroll->tax_deduction = tax_deduction;
            payroll->pf_deduction = pf_deduction;
            payroll->net_salary = net_salary;

            saveEmployeeData();

            setColor(GREEN);
            printf("Employee payroll updated successfully for Year %d!\n", year);
            setColor(RESET);
            return;
        }
    }

    setColor(RED);
    printf("Error: UID not found!\n");
    setColor(RESET);
}

// Modifies a payroll entry
void modifyPayrollEntry() {
    int uid;
    double salary, allowances, overtime, bonuses, insurance_premium;

    setColor(CYAN);
    printf("\nModify Entry of Payroll Processing\n=========================\n");
    setColor(RESET);
    
    printf("Enter UID of the employee: ");
    scanf("%d", &uid);
	int i = 0;

    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            int year = employeeList[i].currentYear;
            struct Payroll *payroll = &employeeList[i].payrolls[year];

            setColor(CYAN);
            printf("Enter new monthly salary: ");
            setColor(RESET);
            scanf("%lf", &salary);
            
            setColor(CYAN);
            printf("Enter new allowances: ");
            setColor(RESET);
            scanf("%lf", &allowances);
            
            setColor(CYAN);
            printf("Enter new overtime payment: ");
            setColor(RESET);
            scanf("%lf", &overtime);
            
            setColor(CYAN);
            printf("Enter new bonuses: ");
            setColor(RESET);
            scanf("%lf", &bonuses);

            setColor(CYAN);
            printf("Enter new insurance premium: ");
            setColor(RESET);
            scanf("%lf", &insurance_premium);
            
            // Update current year payroll
            payroll->salary = salary;
            payroll->allowances = allowances;
            payroll->overtime = overtime;
            payroll->bonuses = bonuses;
            payroll->insurance_premium = insurance_premium;

            // Calculate taxes and PF according to the guidelines
            double gross_salary = salary + allowances + overtime + bonuses;
            double tax_rate = (employeeList[i].marital_status[0] == 'M') ? 0.09 : 0.10;  // 9% for married, 10% for single
            double tax_deduction = gross_salary * tax_rate;
            double pf_deduction = gross_salary * 0.10;  // 10% for PF
            double net_salary = gross_salary - tax_deduction - pf_deduction - insurance_premium;

            payroll->tax_deduction = tax_deduction;
            payroll->pf_deduction = pf_deduction;
            payroll->net_salary = net_salary;

            saveEmployeeData();

            setColor(GREEN);
            printf("Employee payroll entry modified successfully for Year %d!\n", year);
            setColor(RESET);
            return;
        }
    }

    setColor(RED);
    printf("Error: UID not found!\n");
    setColor(RESET);
}

// Views all employee payroll summary
void viewAllEmployeePayrollSummary() {
    clearScreen();
    setColor(CYAN);
    printf("\n====================== ALL EMPLOYEE PAYROLL SUMMARY ======================\n");
    setColor(RESET);
	int i = 0;

    for (i = 0; i < currentEmployeeCount; i++) {
        struct Employee *employee = &employeeList[i];
        int year = employee->currentYear;
        struct Payroll *payroll = &employee->payrolls[year];
        double annual_salary = payroll->net_salary * 12;

        setColor(GREEN);
        printf("\nEmployee: %s (UID: %d)                  (Current Year)Year: %d\n", employee->username, employee->uid, year);
        setColor(RESET);
        printf("---------------------------------------------------------------------\n");
        printf("| %-25s | %20s |\n", "Description", "Amount (NRs)");
        printf("---------------------------------------------------------------------\n");
        printf("| %-25s | %20.2f |\n", "Basic Salary", payroll->salary);
        printf("| %-25s | %20.2f |\n", "Allowances", payroll->allowances);
        printf("| %-25s | %20.2f |\n", "Overtime", payroll->overtime);
        printf("| %-25s | %20.2f |\n", "Bonuses", payroll->bonuses);
        printf("---------------------------------------------------------------------\n");
        printf("| %-25s | %20.2f |\n", "Tax Deduction", payroll->tax_deduction);
        printf("| %-25s | %20.2f |\n", "Provident Fund", payroll->pf_deduction);
        printf("| %-25s | %20.2f |\n", "Insurance Premium", payroll->insurance_premium);
        printf("---------------------------------------------------------------------\n");
        setColor(CYAN);
        printf("| %-25s | %20.2f |\n", "Net Salary (Monthly)", payroll->net_salary);
        printf("| %-25s | %20.2f |\n", "Annual Salary", annual_salary);
        setColor(RESET);
        printf("=====================================================================\n");
    }
}

// Searches for an employee payroll summary
void searchEmployeePayrollSummary() {
	clearScreen();
    int uid;
    setColor(CYAN);
    printf("\n========================= SEARCH EMPLOYEE PAYROLL SUMMARY ========================\n");
    setColor(RESET);
    
	setColor(CYAN);
    printf("Enter UID of the employee: ");
    scanf("%d", &uid);
    setColor(RESET);
    int i = 0;
    
    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            struct Employee *employee = &employeeList[i];
            int year = employee->currentYear;
            struct Payroll *payroll = &employee->payrolls[year];
            double annual_salary = payroll->net_salary * 12;
            
			setColor(GREEN);
            printf("\nPayroll Summary for %s           Current Year(Year %d)\n", employee->username, year);
            setColor(RESET);
            printf("---------------------------------------------------------------------\n");
            printf("| %-25s | %20s |\n", "Description", "Amount (NRs)");
            printf("---------------------------------------------------------------------\n");
            printf("| %-25s | %20.2f |\n", "Basic Salary", payroll->salary);
            printf("| %-25s | %20.2f |\n", "Allowances", payroll->allowances);
            printf("| %-25s | %20.2f |\n", "Overtime", payroll->overtime);
            printf("| %-25s | %20.2f |\n", "Bonuses", payroll->bonuses);
            printf("---------------------------------------------------------------------\n");
            printf("| %-25s | %20.2f |\n", "Tax Deduction", payroll->tax_deduction);
            printf("| %-25s | %20.2f |\n", "Provident Fund", payroll->pf_deduction);
            printf("| %-25s | %20.2f |\n", "Insurance Premium", payroll->insurance_premium);
            printf("---------------------------------------------------------------------\n");
            setColor(CYAN);
            printf("| %-25s | %20.2f |\n", "Net Salary (Monthly)", payroll->net_salary);
            printf("| %-25s | %20.2f |\n", "Annual Salary", annual_salary);
            setColor(RESET);
            printf("=====================================================================\n");
            return;
        }
    }

    setColor(RED);
    printf("Error: UID not found!\n");
    setColor(RESET);
}

// Displays the payroll guideline
void displayPayrollGuideline() {
    setColor(CYAN);
    printf("\nEmployee Payroll Guideline\n=========================\n");
    setColor(GREEN);
    printf("1. Basic Salary: Base salary agreed upon employment.\n");
    printf("2. Allowances: Additional compensation (e.g., housing, transportation).\n");
    printf("3. Overtime: Pay for hours worked beyond standard work hours.\n");
    printf("4. Bonuses: Performance-based extra pay.\n");
    printf("5. Tax Deduction: Calculated based on the latest Nepal Government tax policies.\n");
    printf("   - For married employees: 1%% less tax.\n");
    printf("   - For single employees: Standard tax rate.\n");
    printf("6. Provident Fund (PF): Calculated based on the latest Nepal Government PF policies.\n");
    printf("   - Employee contribution: 10%%\n");
    printf("   - Employer contribution: 10%%\n");
    printf("7. Insurance Premium: Fixed amount as agreed.\n");
    printf("8. Net Salary: Gross salary minus tax, PF, and insurance premium.\n");
    setColor(RESET);
}

//Function for Menu of Audit Log in Admin section
void auditLogMenu() {
    int option;

    while (1) {
        clearScreen();
        displayLoading();
        setColor(BLUE);
        printf("\nCipherPayroll - Audit Log Menu\n=========================\n");
        setColor(RESET);
        setColor(CYAN);
        printf("1. View All Employee's Audit Log\n");
        printf("2. Search Employee's Audit Log\n");
        printf("3. Back to Admin Menu\n");
        printf("=========================\n");
        setColor(GREEN);
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                viewAllEmployeeAuditLog();
                break;
            case 2:
                searchEmployeeAuditLog();
                break;
            case 3:
                return;
            default:
                setColor(RED);
                printf("Error: Invalid option!\n");
                setColor(RESET);
                break;
        }
        if (!continuePrompt()) {
            break;
        }
    }
}

// Views the audit log for all employees
void viewAllEmployeeAuditLog() {
    clearScreen();
    setColor(CYAN);
    printf("\n==================== AUDIT LOG FOR ALL EMPLOYEES =====================\n");
    setColor(RESET);
    int i = 0;

    for (i = 0; i < currentEmployeeCount; i++) {
        struct Employee *employee = &employeeList[i];

        printf("\nAudit Log for %s\n", employee->username);
        printf("---------------------------------------------------------------------\n");
		int year = 0;

        for (year = 1; year <= employee->currentYear; year++) {
            struct Payroll *payroll = &employee->payrolls[year];
            double annual_salary = payroll->net_salary * 12;

            printf("\nPayroll for Year %d\n", year);
            printf("---------------------------------------------------------------------\n");
            printf("| %-25s | %20s |\n", "Description", "Amount (NRs)");
            printf("---------------------------------------------------------------------\n");
            printf("| %-25s | %20.2f |\n", "Basic Salary", payroll->salary);
            printf("| %-25s | %20.2f |\n", "Allowances", payroll->allowances);
            printf("| %-25s | %20.2f |\n", "Overtime", payroll->overtime);
            printf("| %-25s | %20.2f |\n", "Bonuses", payroll->bonuses);
            printf("---------------------------------------------------------------------\n");
            printf("| %-25s | %20.2f |\n", "Tax Deduction", payroll->tax_deduction);
            printf("| %-25s | %20.2f |\n", "Provident Fund", payroll->pf_deduction);
            printf("| %-25s | %20.2f |\n", "Insurance Premium", payroll->insurance_premium);
            printf("---------------------------------------------------------------------\n");
            setColor(CYAN);
            printf("| %-25s | %20.2f |\n", "Net Salary (Monthly)", payroll->net_salary);
            printf("| %-25s | %20.2f |\n", "Annual Salary", annual_salary);
            setColor(RESET);
            printf("=====================================================================\n");
        }
    }
}

// Searches for an employee audit log
void searchEmployeeAuditLog() {
	clearScreen();
    int uid;
    setColor(BLUE);
    printf("\n========================= SEARCH EMPLOYEE AUDIT LOG ========================\n");
    setColor(RESET);

    setColor(CYAN);
    printf("Enter UID of the employee to search: ");
    setColor(RESET);
    scanf("%d", &uid);
	int i = 0;

    for (i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            struct Employee *employee = &employeeList[i];

            printf("\nAudit Log for %s\n", employee->username);
            printf("---------------------------------------------------------------------\n");
			int year = 0;

            for (year = 1; year <= employee->currentYear; year++) {
                struct Payroll *payroll = &employee->payrolls[year];
                double annual_salary = payroll->net_salary * 12;

                printf("\nPayroll for Year %d\n", year);
                printf("---------------------------------------------------------------------\n");
                printf("| %-25s | %20s |\n", "Description", "Amount (NRs)");
                printf("---------------------------------------------------------------------\n");
                printf("| %-25s | %20.2f |\n", "Basic Salary", payroll->salary);
                printf("| %-25s | %20.2f |\n", "Allowances", payroll->allowances);
                printf("| %-25s | %20.2f |\n", "Overtime", payroll->overtime);
                printf("| %-25s | %20.2f |\n", "Bonuses", payroll->bonuses);
                printf("---------------------------------------------------------------------\n");
                printf("| %-25s | %20.2f |\n", "Tax Deduction", payroll->tax_deduction);
                printf("| %-25s | %20.2f |\n", "Provident Fund", payroll->pf_deduction);
                printf("| %-25s | %20.2f |\n", "Insurance Premium", payroll->insurance_premium);
                printf("---------------------------------------------------------------------\n");
                setColor(CYAN);
                printf("| %-25s | %20.2f |\n", "Net Salary (Monthly)", payroll->net_salary);
                printf("| %-25s | %20.2f |\n", "Annual Salary", annual_salary);
                setColor(RESET);
                printf("=====================================================================\n");
            }
            return;
        }
    }

    setColor(RED);
    printf("Error: UID not found!\n");
    setColor(RESET);
}

// Encrypts a file using XOR encryption
void encryptFile(const char* filename) {
    FILE* file = fopen(filename, "rb+");
    if (!file) {
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(fileSize);
    if (!buffer) {
        fclose(file);
        return;
    }

    fread(buffer, 1, fileSize, file);
    fseek(file, 0, SEEK_SET);
	long i = 0;

    for (i = 0; i < fileSize; i++) {
        buffer[i] ^= XOR_KEY;
    }

    fwrite(buffer, 1, fileSize, file);
    free(buffer);
    fclose(file);
}

// Decrypts a file using XOR encryption
void decryptFile(const char* filename) {
    FILE* file = fopen(filename, "rb+");
    if (!file) {
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(fileSize);
    if (!buffer) {
        fclose(file);
        return;
    }

    fread(buffer, 1, fileSize, file);
    fseek(file, 0, SEEK_SET);
	long i = 0;

    for (i = 0; i < fileSize; i++) {
        buffer[i] ^= XOR_KEY;
    }

    fwrite(buffer, 1, fileSize, file);
    free(buffer);
    fclose(file);
}

// Manages employee information in the admin menu
void adminEmployeeInfoManagement() {
    int option;

    while (1) {
        displayLoading();
        setColor(BLUE);
        printf("\nEmployee Information Management\n=========================\n");
        setColor(RESET);
        setColor(CYAN);
        printf("1. Approve New Employee\n");
        printf("2. Edit Employee Details\n");
        printf("3. View All Employee Details\n");
        printf("4. Change Employee Status\n");
        printf("5. Delete Employee Record\n");
        printf("6. Search Employee Record\n");
        printf("7. Back to Admin Menu\n");
        printf("=========================\n");
        setColor(GREEN);
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                adminApproveNewEmployee();
                if (!continuePrompt()) return;
                break;
            case 2:
                adminEditEmployeeDetails();
                if (!continuePrompt()) return;
                break;
            case 3:
                adminViewAllEmployeeDetails();
                if (!continuePrompt()) return;
                break;
            case 4:
                adminChangeEmployeeStatus();
                if (!continuePrompt()) return;
                break;
            case 5:
                adminDeleteEmployeeRecord();
                if (!continuePrompt()) return;
                break;
            case 6:
                adminSearchEmployeeRecord();
                if (!continuePrompt()) return;
                break;
            case 7:
                return;
            default:
                displayLoading();
                printf("\nEmployee Information Management\n=========================\n");
                setColor(RED);
                printf("Error: Invalid option!\n");
                setColor(RESET);
                if (!continuePrompt()) return;
                break;
        }
    }
}

// Displays the admin menu
void adminMenu() {
    int option;

    while (1) {
        displayLoading();
        setColor(BLUE);
        printf("\nCipherPayroll - Admin Menu\n=========================\n");
        setColor(RESET);
        setColor(CYAN);
        printf("1. Employee Information Management\n");
        printf("2. Payroll Processing\n");
        printf("3. Change Admin's Password\n");
        printf("4. Audit Log\n");
        printf("5. Back to Main Menu\n");
        printf("=========================\n");
        setColor(GREEN);
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                adminEmployeeInfoManagement();
                break;
            case 2:
                adminPayrollProcessing();
                break;
            case 3:
                adminChangeAdminPassword();
                break;
            case 4:
                auditLogMenu();
                break;
            case 5:
                return;
            default:
                displayLoading();
                printf("\nAdmin Menu\n=========================\n");
                setColor(RED);
                printf("Error: Invalid option!\n");
                setColor(RESET);
                break;
        }
        if (!continuePrompt()) {
            break;
        }
    }
}

//Display the Provacy Policy of Our Program
void displayPrivacyPolicy() {
    clearScreen();
    setColor(GREEN);
    printf("\n========================= PRIVACY POLICY =========================\n");
    setColor(RESET);
    setColor(CYAN);
    printf("At CipherPayroll, we take your privacy and data security seriously. \n");
    printf("To ensure the security and confidentiality of your data, we employ \n");
    printf("encryption technologies to protect sensitive information.\n\n");

    printf("All sensitive data is encrypted, ensuring that your information is \n");
    printf("protected at all times, both while it is stored on our servers and during \n");
    printf("transmission to and from our systems.\n\n");

    printf("We continuously monitor and update our security practices to safeguard \n");
    printf("your data against unauthorized access, disclosure, alteration, and destruction.\n");
    printf("=====================================================================\n");
    setColor(RESET);
}

//Display about the program and how it facilitates the admin and employee
void displayAboutUs() {
    clearScreen();
    setColor(GREEN);
    printf("\n=========================== ABOUT US ============================\n");
    setColor(RESET);
    setColor(CYAN);
    printf("Welcome to CipherPayroll, your trusted employee payroll management system.\n\n");
    printf("Our Mission:\n");
    printf("---------------------------------------------------------------------\n");
    printf("Our mission is to provide a secure, reliable, and efficient platform for \n");
    printf("managing employee payroll and related information. We strive to ensure \n");
    printf("data integrity and confidentiality while offering a user-friendly \n");
    printf("experience for both employees and administrators.\n\n");
    printf("Features:\n");
    printf("---------------------------------------------------------------------\n");
    printf("1. Secure Data Handling: Using encryption/decryption tool to protect sensitive data.\n");
    printf("2. Comprehensive Payroll Management: Handle salaries, allowances, bonuses,\n");
    printf("   and deductions efficiently.\n");
    printf("3. Detailed Employee Information: Maintain and manage employee details \n");
    printf("   with ease.\n");
    printf("4. User-Friendly Interface: Designed for simplicity and ease of use.\n\n");
    printf("Our team is composed of Prajjwal Maharjan, Rabin Pulami Magar and Durga Budha dedicated to \n");
    printf("developing innovative solutions for our clients. Our lead developer, \n");
    printf("Prajjwal Maharjan, has main role and brings expertise in software development and \n");
    printf("data security, ensuring that CipherPayroll is built to the highest \n");
    printf("standards of quality and security.\n\n");
    printf("Thank you for choosing CipherPayroll. We are committed to providing the \n");
    printf("best service and support for your payroll management needs.\n");
    printf("=====================================================================\n");
    setColor(RESET);
}

// Displays the main menu
void mainMenu() {
    int option;
    char adminPassword[PASSWORDLENGTH];

    while (1) {
        displayLoading();
        setColor(BLUE);
        printf("\nCipherPayroll - Employee Payroll System\n=========================\n");
        setColor(RESET);
        setColor(CYAN);
        printf("=========================\n");
        printf("1. Admin (HR)\n");
        printf("2. Client\n");
        printf("3. Privacy Policy\n");
        printf("4. About Us\n");
        printf("5. Exit\n");
        printf("=========================\n");
        setColor(GREEN);
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                clearScreen();
                setColor(BLUE);
                printf("\nAdmin Login\n=========================\n");
                setColor(RESET);
                setColor(CYAN);
                printf("Enter admin password: ");
                setColor(RESET);
                getPasswordInput(adminPassword);
                printf("\n");

                // Validate admin password
                if (strcmp(adminPassword, "") == 0 || !validateAdminPassword(adminPassword)) {
                    setColor(RED);
                    printf("Error: Incorrect admin password!\n");
                    setColor(RESET);
                    printf("\nPress any key to continue...\n");
                    _getch();
                    break;
                }

                adminMenu();
                break;
            case 2:
                mainClientMenu();
                break;
            case 3:
                displayPrivacyPolicy();
                printf("\nPress any key to return to the main menu...\n");
                _getch();
                break;
            case 4:
                displayAboutUs();
                printf("\nPress any key to return to the main menu...\n");
                _getch();
                break;
            case 5:
                displayExiting();
                setColor(RESET);
                return;
            default:
                displayLoading();
                printf("\nCipherPayroll - Employee Payroll System\n=========================\n");
                setColor(RED);
                printf("Error: Invalid option!\n");
                setColor(RESET);
                break;
        }
         if (!continuePrompt()) {
            break;
        }
    }
}

