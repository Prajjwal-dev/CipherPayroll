#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#define MAXUSERS 100
#define USERNAMELENGTH 50
#define PASSWORDLENGTH 50
#define UIDLENGTH 10
#define EMPLOYEE_FILE "Employees.bin"
#define FILENAME "Login.txt"

#define RESET 7
#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define BLUE (FOREGROUND_BLUE)
#define CYAN (FOREGROUND_GREEN | FOREGROUND_BLUE)

// Define the User structure
struct User {
    int uid;
    char username[USERNAMELENGTH];
    char password[PASSWORDLENGTH];
    char status; // 'P' for Pending, 'A' for Approved
};

struct Employee {
    char username[USERNAMELENGTH];
    char password[PASSWORDLENGTH];
    int uid;
    double salary;
    char status; // 'A' for Active, 'I' for Inactive, 'T' for Termination
    char position[USERNAMELENGTH];
    unsigned long long int contact_no;
    char email[100];
};

struct personal_details {
	unsigned long long int contact_no;
    char email[100];
};

//For client
struct User userList[MAXUSERS];
int currentUserCount = 0;
int latestUID = 1;  // Starting point for UIDs

//For admin
struct Employee employeeList[MAXUSERS];
int currentEmployeeCount = 0;

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void clearScreen() {
    system("cls");
}

void displayLoading() {
    setColor(YELLOW);
    printf("Loading");
    for (int i = 0; i < 3; i++) {
        setColor(YELLOW);
        printf(".");
        fflush(stdout);
        Sleep(500);
    }
    printf("\n");
    clearScreen();
}

void displayExiting() {
    clearScreen();
    setColor(RED);
    printf("Exiting");
    for (int i = 0; i < 3; i++) {
        setColor(RED);
        printf(".");
        fflush(stdout);
        Sleep(500);
    }
    printf("\n");
    setColor(RESET);
    exit(0);
}

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

void loadUserData() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        setColor(RED);
        printf("Error: Cannot open file for reading!\n");
        setColor(RESET);
        return;
    }

    fread(&currentUserCount, sizeof(int), 1, file);
    fread(&latestUID, sizeof(int), 1, file);  // Load latest UID
    fread(userList, sizeof(struct User), currentUserCount, file);
    fclose(file);
}

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

void loadEmployeeData() {
    FILE *file = fopen(EMPLOYEE_FILE, "rb");
    if (file == NULL) {
        setColor(RED);
        printf("Error: Cannot open employee file for reading!\n");
        setColor(RESET);
        return;
    }

    fread(&currentEmployeeCount, sizeof(int), 1, file);
    fread(employeeList, sizeof(struct Employee), currentEmployeeCount, file);
    fclose(file);
}

void createUID(int* uid) {
    *uid = latestUID++;
}

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

    setColor(CYAN);
    printf("Enter Password: ");
    setColor(RESET);
    getPasswordInput(newUser.password);
    printf("\n");

    userList[currentUserCount++] = newUser;
    saveUserData();

    setColor(GREEN);
    printf("Registration successful! Please wait for admin approval.\n");
    setColor(RESET);
}

void addPersonalDetails() {
    struct personal_details details;
    char email[100];
    
    // Prompt for and read contact number
    setColor(CYAN);
    printf("Enter contact number (10 digits only): ");
    scanf("%llu", &details.contact_no);
	setColor(RESET);
	
    // Prompt for and read email address
    setColor(CYAN);
    printf("Enter email address: ");
    scanf("%99s", email);
    setColor(RESET);

    // Validate email format
    if (strlen(email) < 10 || strcmp(email + strlen(email) - 10, "@gmail.com") != 0) {
    	setColor(RED);
        printf("Error: Invalid email format!\n");
        setColor(RESET);
        return;
    }
    strcpy(details.email, email);

    // Validate contact number length
    unsigned long long int temp = details.contact_no;
    int digitCount = 0;
    while (temp != 0) {
        temp /= 10;
        digitCount++;
    }

    if (digitCount != 10) {
    	setColor(RED);
        printf("Error: Contact number must have exactly 10 digits!\n");
        setColor(RESET);
        return;
    }

    // Open file for writing in append mode
    FILE *fp = fopen("personal_details.bin", "ab");
    if (fp == NULL) {
        printf("Error: Unable to open file!\n");
        return;
    }

    // Write details to file
    fwrite(&details, sizeof(struct personal_details), 1, fp);

    // Close the file
    fclose(fp);
	setColor(GREEN);
    printf("Personal details added successfully!\n");
    setColor(RESET);
}

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

    for (int i = 0; i < currentUserCount; i++) {
        if (userList[i].uid == uid && strcmp(userList[i].password, oldPassword) == 0) {
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

            strcpy(userList[i].password, newPassword);
            saveUserData();

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

void resetUserPassword() {
    int uid;
    char newPassword[PASSWORDLENGTH], reenterPassword[PASSWORDLENGTH];
    setColor(CYAN);
    printf("Enter your UID: ");
    setColor(RESET);
    scanf("%d", &uid);

    for (int i = 0; i < currentUserCount; i++) {
        if (userList[i].uid == uid) {
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

            strcpy(userList[i].password, newPassword);
            saveUserData();

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

void clearInputBuffer() {
    while (getchar() != '\n');
}

void clientMenu() {
    int option;

    while (1) {
        displayLoading();
        setColor(BLUE);
        printf("\nEmployee Payroll System - Client Menu\n=========================\n");
        setColor(RESET);
        setColor(CYAN);
        printf("1. Add Personal Details\n");
        printf("2. Go back to main client menu\n");
        printf("=========================\n");
        setColor(GREEN);
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                displayLoading();
                setColor(BLUE);
                printf("\nEmployee Payroll System - Add Personal Details\n=========================\n");
                setColor(RESET);
                addPersonalDetails();
                break;
            case 2:
                return; // Go back to the main client menu
            default:
                displayLoading();
                printf("\nEmployee Payroll System - Client Menu\n=========================\n");
                setColor(RED);
                printf("Error: Invalid option!\n");
                setColor(RESET);
                break;
        }
    }
}

int authenticateUser() {
    int uid;
    char password[PASSWORDLENGTH];
    setColor(CYAN);
    printf("Enter your UID: ");
    setColor(RESET);
    fflush(stdin);
    scanf("%d", &uid);
    setColor(CYAN);
    printf("Enter your password: ");
    setColor(RESET);
    getPasswordInput(password);  // Get password input securely
    printf("\n");

    for (int i = 0; i < currentUserCount; i++) {
        if (userList[i].uid == uid && strcmp(userList[i].password, password) == 0) {
            if (userList[i].status == 'A') {
                setColor(GREEN);
                printf("Login successful!\n");
                printf("Welcome, %s!\n", userList[i].username);
                setColor(RESET);
                clientMenu();  // Go to client menu after successful login
                return 1;
            } else {
                setColor(RED);
                printf("Error: Your account is not approved yet. Please wait for admin approval.\n");
                setColor(RESET);
                return 0;
            }
        }
    }

    setColor(RED);
    printf("Error: Invalid UID or password!\n");
    setColor(RESET);
    return 0;
}

void mainclientMenu() {
    int option;
    int isLoggedIn = 0;
    char check = '\0';

    while (1) {
        displayLoading();
        setColor(BLUE);
        printf("\nEmployee Payroll System\n=========================\n");
        setColor(RESET);
        setColor(CYAN);
        printf("1. Register\n");
        printf("2. Log In\n");
        printf("3. Change Password\n");
        printf("4. Forgot Password?\n");
        printf("5. Go back to main menu\n");
        printf("=========================\n");
        setColor(GREEN);
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                displayLoading();
                setColor(BLUE);
                printf("\nEmployee Payroll System\n=========================\n");
                setColor(RESET);
                setColor(CYAN);
                printf("1. Register\n");
                registerUser();
                setColor(CYAN);
                printf("Do you want to continue (Y/N): ");
                scanf(" %c", &check);
                if (check == 'Y' || check == 'y') {
                    break;
                } else {
                    displayExiting();
                }
                setColor(RESET);
                break;
            case 2:
                displayLoading();
                setColor(BLUE);
                printf("\nEmployee Payroll System\n=========================\n");
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
                printf("\nEmployee Payroll System\n=========================\n");
                setColor(RESET);
                setColor(CYAN);
                printf("3. Change Password\n");
                updatePassword();
                setColor(CYAN);
                printf("Do you want to continue (Y/N): ");
                scanf(" %c", &check);
                clearInputBuffer();
                if (check == 'Y' || check == 'y') {
                    break;
                } else {
                    displayExiting();
                }
                setColor(RESET);
                break;
            case 4:
                displayLoading();
                setColor(BLUE);
                printf("\nEmployee Payroll System\n=========================\n");
                setColor(RESET);
                setColor(CYAN);
                printf("4. Forgot Password?\n");
                resetUserPassword();
                setColor(CYAN);
                printf("Do you want to continue (Y/N): ");
                scanf(" %c", &check);
                clearInputBuffer();
                if (check == 'Y' || check == 'y') {
                    break;
                } else {
                    displayExiting();
                }
                setColor(RESET);
                break;
            case 5:
                return;
            default:
                displayLoading();
                printf("\nEmployee Payroll System\n=========================\n");
                setColor(RED);
                printf("Error: Invalid option!\n");
                setColor(RESET);
                break;
        }
    }
}

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

bool isInitialAdminPasswordSet() {
    FILE *file = fopen("AdminPassword.txt", "r");
    if (file != NULL) {
        fclose(file); // Close the file if it exists
        return true;
    } else {
        return false;
    }
}

void saveInitialAdminPassword() {
    FILE* file = fopen("AdminPassword.txt", "w");
    if (file == NULL) {
        setColor(RED);
        printf("Error: Cannot open AdminPassword.txt for writing!\n");
        setColor(RESET);
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
}

// Check user approval status
int checkUserApprovalStatus(int uid, char* password) {
    for (int i = 0; i < currentUserCount; i++) {
        if (userList[i].uid == uid && strcmp(userList[i].password, password) == 0) {
            return userList[i].status == 'A'; // Return 1 if approved, 0 otherwise
        }
    }
    return -1; // User not found
}

void adminApproveNewEmployee() {
    setColor(BLUE);
    printf("\nApproval for New Employee\n=========================\n");
    setColor(RESET);

    for (int i = 0; i < currentUserCount; i++) {
        if (userList[i].status == 'P') {
            printf("UID: %d, Username: %s\n", userList[i].uid, userList[i].username);
        }
    }

    int uid;
    setColor(CYAN);
    printf("Enter UID of the employee to approve: ");
    setColor(RESET);
    scanf("%d", &uid);

    for (int i = 0; i < currentUserCount; i++) {
        if (userList[i].uid == uid && userList[i].status == 'P') {
            userList[i].status = 'A'; // Set status to Approved

            struct Employee newEmployee;
            strcpy(newEmployee.username, userList[i].username);
            strcpy(newEmployee.password, userList[i].password);
            newEmployee.uid = userList[i].uid;
            newEmployee.salary = 0.0; // Set initial salary to 0
            newEmployee.status = 'A'; // Set status to Active
            strcpy(newEmployee.position, "Not Assigned"); // Set initial position
            newEmployee.contact_no = 0; // Initial contact number
            strcpy(newEmployee.email, ""); // Initial email

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

void adminEditEmployeeDetails() {
    int uid, option;
    setColor(BLUE);
    printf("\nEdit Employee Details\n=========================\n");
    setColor(RESET);

    setColor(CYAN);
    printf("Enter UID of the employee to edit: ");
    setColor(RESET);
    scanf("%d", &uid);

    for (int i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            while (1) {
                setColor(CYAN);
                printf("\nWhat would you like to edit?\n");
                printf("1. Username\n");
                printf("2. Password\n");
                printf("3. Salary\n");
                printf("4. Position\n");
                printf("5. Personal Details\n");
                printf("6. Go back\n");
                setColor(GREEN);
                printf("Enter your choice: ");
                setColor(RESET);
                scanf("%d", &option);

                switch (option) {
                    case 1:
                        setColor(CYAN);
                        printf("Enter new username: ");
                        setColor(RESET);
                        getchar();
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
                        printf("Enter new salary: ");
                        setColor(RESET);
                        scanf("%lf", &employeeList[i].salary);
                        break;
                    case 4:
                        setColor(CYAN);
                        printf("Enter new position: ");
                        setColor(RESET);
                        getchar();
                        fgets(employeeList[i].position, USERNAMELENGTH, stdin);
                        employeeList[i].position[strcspn(employeeList[i].position, "\n")] = '\0';
                        break;
                    case 5:
                        setColor(CYAN);
                        printf("Enter new contact number (10 digits only): ");
                        setColor(RESET);
                        scanf("%llu", &employeeList[i].contact_no);
                        setColor(CYAN);
                        printf("Enter new email address: ");
                        setColor(RESET);
                        scanf("%s", employeeList[i].email);
                        break;
                    case 6:
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

void adminDeleteEmployeeRecord() {
    int uid;
    setColor(BLUE);
    printf("\nDelete Employee Record\n=========================\n");
    setColor(RESET);

    setColor(CYAN);
    printf("Enter UID of the employee to delete: ");
    setColor(RESET);
    scanf("%d", &uid);

    for (int i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            for (int j = i; j < currentEmployeeCount - 1; j++) {
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

void adminSearchEmployeeRecord() {
    int uid;
    setColor(BLUE);
    printf("\nSearch Employee Record\n=========================\n");
    setColor(RESET);

    setColor(CYAN);
    printf("Enter UID of the employee to search: ");
    setColor(RESET);
    scanf("%d", &uid);

    for (int i = 0; i < currentEmployeeCount; i++) {
        if (employeeList[i].uid == uid) {
            setColor(GREEN);
            printf("\nEmployee Details\n=========================\n");
            printf("UID: %d\n", employeeList[i].uid);
            printf("Username: %s\n", employeeList[i].username);
            printf("Salary: %.2f\n", employeeList[i].salary);
            printf("Position: %s\n", employeeList[i].position);
            printf("Status: %c\n", employeeList[i].status);
            printf("Contact No: %llu\n", employeeList[i].contact_no);
            printf("Email: %s\n", employeeList[i].email);
            setColor(RESET);
            return;
        }
    }

    setColor(RED);
    printf("Error: UID not found!\n");
    setColor(RESET);
}

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

    for (int i = 0; i < currentEmployeeCount; i++) {
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

void adminViewAllEmployeeDetails() {
    setColor(BLUE);
    printf("\nAll Employee Details\n=========================\n");
    setColor(RESET);

    for (int i = 0; i < currentEmployeeCount; i++) {
        printf("UID: %d, Username: %s, Salary: %.2f, Position: %s, Status: %c, Contact No: %llu, Email: %s\n",
            employeeList[i].uid, employeeList[i].username, employeeList[i].salary, employeeList[i].position,
            employeeList[i].status, employeeList[i].contact_no, employeeList[i].email);
    }
}

void adminEmployeeInfoManagement() {
    int option;
	char check;
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
                fflush(stdin);
                setColor(GREEN);
                printf("Do you want to continue(Y/N): ");
                scanf("%c", &check);
                setColor(RESET);
                 if (check == 'Y' || check == 'y') {
                    break;
                } else {
                    displayExiting();
                }
                setColor(RESET);
                break;
            case 2:
                adminEditEmployeeDetails();
                fflush(stdin);
                setColor(GREEN);
                printf("Do you want to continue(Y/N): ");
                scanf("%c", &check);
                setColor(RESET);
                 if (check == 'Y' || check == 'y') {
                    break;
                } else {
                    displayExiting();
                }
                setColor(RESET);
                break;
            case 3:
                adminViewAllEmployeeDetails();
                fflush(stdin);
                setColor(GREEN);
                printf("Do you want to continue(Y/N): ");
                scanf("%c", &check);
                setColor(RESET);
                 if (check == 'Y' || check == 'y') {
                    break;
                } else {
                    displayExiting();
                }
                setColor(RESET);
                break;
            case 4:
                adminChangeEmployeeStatus();
                fflush(stdin);
                setColor(GREEN);
                printf("Do you want to continue(Y/N): ");
                scanf("%c", &check);
                setColor(RESET);
                 if (check == 'Y' || check == 'y') {
                    break;
                } else {
                    displayExiting();
                }
                setColor(RESET);
                break;
            case 5:
                adminDeleteEmployeeRecord();
                fflush(stdin);
                setColor(GREEN);
                printf("Do you want to continue(Y/N): ");
                scanf("%c", &check);
                setColor(RESET);
                 if (check == 'Y' || check == 'y') {
                    break;
                } else {
                    displayExiting();
                }
                setColor(RESET);
                break;
            case 6:
                adminSearchEmployeeRecord();
                fflush(stdin);
                setColor(GREEN);
                printf("Do you want to continue(Y/N): ");
                scanf("%c", &check);
                setColor(RESET);
                 if (check == 'Y' || check == 'y') {
                    break;
                } else {
                    displayExiting();
                }
                setColor(RESET);
                break;
            case 7:
                return;
            default:
                displayLoading();
                printf("\nEmployee Information Management\n=========================\n");
                setColor(RED);
                printf("Error: Invalid option!\n");
                setColor(RESET);
                break;
        }
    }
}

void adminMenu() {
    int option;

    while (1) {
        displayLoading();
        setColor(BLUE);
        printf("\nAdmin Menu\n=========================\n");
        setColor(RESET);
        setColor(CYAN);
        printf("1. Employee Information Management\n");
        printf("2. Change Admin's Password\n");
        printf("3. Back to Main Menu\n");
        printf("=========================\n");
        setColor(GREEN);
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                adminEmployeeInfoManagement();
                break;
            case 2:
                adminChangeAdminPassword();
                return;
            case 3:
                return;
            default:
                displayLoading();
                printf("\nAdmin Menu\n=========================\n");
                setColor(RED);
                printf("Error: Invalid option!\n");
                setColor(RESET);
                break;
        }
    }
}

// For first time login of admin the password is "admin123"
void mainMenu() {
    int option;
    char adminPassword[PASSWORDLENGTH];

    while (1) {
        displayLoading();
        setColor(BLUE);
        printf("\nEmployee Payroll System\n=========================\n");
        setColor(RESET);
        setColor(CYAN);
        printf("=========================\n");
        printf("1. Admin (HR)\n");
        printf("2. Client\n");
        printf("3. Exit\n");
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
                mainclientMenu();
                break;
            case 3:
                displayExiting();
                setColor(RESET);
                exit(0);
            default:
                displayLoading();
                printf("\nEmployee Payroll System\n=========================\n");
                setColor(RED);
                printf("Error: Invalid option!\n");
                setColor(RESET);
                break;
        }
    }
}

int main() {
    loadUserData();
    loadEmployeeData();

    // Setup initial admin password if not already set
    if (!isInitialAdminPasswordSet()) {
        saveInitialAdminPassword();
    }

    mainMenu();
    
    return 0;
}

