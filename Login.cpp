#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define MAXUSERS 100
#define USERNAMELENGTH 50
#define PASSWORDLENGTH 50
#define UIDLENGTH 10
#define FILENAME "Login.txt"

#define RESET 7
#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define BLUE (FOREGROUND_BLUE)
#define CYAN (FOREGROUND_GREEN | FOREGROUND_BLUE)

struct User {
    char username[USERNAMELENGTH];
    char password[PASSWORDLENGTH];
    char uid[UIDLENGTH];
};

// Structure to hold personal details
struct personal_details {
    unsigned long long int contact_no;
    char email[100];
};

struct Employee {
    char username[USERNAMELENGTH];
    char password[PASSWORDLENGTH];
    char uid[UIDLENGTH];
    double salary;
    char status; // 'A' for Active, 'I' for Inactive, 'T' for Termination
};

//For client
struct User userList[MAXUSERS];
int currentUserCount = 0;

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
    fread(userList, sizeof(struct User), currentUserCount, file);
    fclose(file);
}

void createUID(char* uid) {
    for (int i = 0; i < UIDLENGTH - 1; i++) {
        uid[i] = '0' + rand() % 10;
    }
    uid[UIDLENGTH - 1] = '\0';
}

void registerNewUser() {
    if (currentUserCount >= MAXUSERS) {
        setColor(RED);
        printf("Error: Maximum user limit reached!\n");
        setColor(RESET);
        return;
    }

    char username[USERNAMELENGTH], password[PASSWORDLENGTH], reenterPassword[PASSWORDLENGTH], uid[UIDLENGTH];
    setColor(CYAN);
    printf("Enter a username: ");
    setColor(RESET);
    getchar(); // Clear newline left by previous input
    fgets(username, USERNAMELENGTH, stdin);
    username[strcspn(username, "\n")] = 0;

    setColor(CYAN);
    printf("Enter a password: ");
    setColor(RESET);
    getPasswordInput(password);
    printf("\n");

    setColor(CYAN);
    printf("Re-enter your password: ");
    setColor(RESET);
    getPasswordInput(reenterPassword);
    printf("\n");

    if (strcmp(password, reenterPassword) != 0) {
        setColor(RED);
        printf("Error: Passwords do not match!\n");
        setColor(RESET);
        return;
    }

    for (int i = 0; i < currentUserCount; i++) {
        if (strcmp(userList[i].username, username) == 0) {
            setColor(RED);
            printf("Error: Username already taken!\n");
            setColor(RESET);
            return;
        }
    }

    createUID(uid);
    struct User newUser;
    strcpy(newUser.username, username);
    strcpy(newUser.password, password);
    strcpy(newUser.uid, uid);

    userList[currentUserCount] = newUser;
    currentUserCount++;
    saveUserData();

    setColor(GREEN);
    printf("Success: Registration complete! Your UID is %s\n and Your Password is %s\nCaution: Keep your Password note down and safe!\n", uid, password);
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
    char username[USERNAMELENGTH], oldPassword[PASSWORDLENGTH], newPassword[PASSWORDLENGTH], reenterPassword[PASSWORDLENGTH];
    setColor(CYAN);
    printf("Enter your username: ");
    setColor(RESET);
    getchar();
    fgets(username, USERNAMELENGTH, stdin);
    username[strcspn(username, "\n")] = '\0';

    setColor(CYAN);
    printf("Enter your old password: ");
    setColor(RESET);
    getPasswordInput(oldPassword);
    printf("\n");

    for (int i = 0; i < currentUserCount; i++) {
        if (strcmp(userList[i].username, username) == 0 && strcmp(userList[i].password, oldPassword) == 0) {
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
            printf("Password changed successfully!\nYour Password is %s\nCaution:Keep your Password note down and safe!\n", newPassword);
            setColor(RESET);
            return;
        }
    }

    setColor(RED);
    printf("Error: Invalid username or old password!\n");
    setColor(RESET);
}

void resetUserPassword() {
    char uid[UIDLENGTH], newPassword[PASSWORDLENGTH], reenterPassword[PASSWORDLENGTH];
    setColor(CYAN);
    printf("Enter your UID: ");
    setColor(RESET);
    getchar();
    fgets(uid, UIDLENGTH, stdin);
    uid[strcspn(uid, "\n")] = '\0';

    for (int i = 0; i < currentUserCount; i++) {
        if (strcmp(userList[i].uid, uid) == 0) {
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
    char username[USERNAMELENGTH], password[PASSWORDLENGTH];
    setColor(CYAN);
    printf("Enter your username: ");
    setColor(RESET);

    getchar(); // Clear newline left by previous input
    fgets(username, USERNAMELENGTH, stdin);
    username[strcspn(username, "\n")] = 0;  // Remove trailing newline character

    setColor(CYAN);
    printf("Enter your password: ");
    setColor(RESET);
    getPasswordInput(password);  // Get password input securely
    printf("\n");

    for (int i = 0; i < currentUserCount; i++) {
        if (strcmp(userList[i].username, username) == 0 && strcmp(userList[i].password, password) == 0) {
            setColor(GREEN);
            printf("Login successful!\n");
            printf("Welcome, %s!\n", userList[i].username);
            setColor(RESET);
            clientMenu();  // Go to client menu after successful login
            return 1;
        }
    }

    setColor(RED);
    printf("Error: Invalid username or password!\n");
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
                registerNewUser();
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

//Admin Section
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

void adminApproveNewEmployee() {
    // Load data from Login.txt
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        setColor(RED);
        printf("Error: Cannot open %s for reading!\n", FILENAME);
        setColor(RESET);
        return;
    }

    // Read all users from file and display those pending approval
    struct User tempUser;
    printf("List of Pending Registrations:\n");
    while (fread(&tempUser, sizeof(struct User), 1, file)) {
        // Check if user status is pending (you may need a status field in struct User)
        // Display only pending users
        // Example: if (tempUser.status == 'P') { printf("%s\n", tempUser.username); }
    }

    fclose(file);

    // Prompt admin for UID to approve
    char uid[UIDLENGTH];
    printf("Enter UID of user to approve: ");
    scanf("%s", uid);

    // Find user in userList array
    for (int i = 0; i < currentUserCount; i++) {
        if (strcmp(userList[i].uid, uid) == 0) {
            // Change user status to Active ('A') or set status field accordingly
            // Example: userList[i].status = 'A';
            // Update user in Login.txt (fwrite or update function)
            break;
        }
    }

    setColor(GREEN);
    printf("User with UID %s approved successfully!\n", uid);
    setColor(RESET);
}

void adminEditEmployeeDetails() {
    // Implementation to edit employee details like personal details, username, password, etc.
    // You can use similar techniques as in other functions (load data, modify, save data)
}

void adminViewAllEmployeeDetails() {
    // Load and display all employee details from Login.txt
    printf("List of All Employee Details:\n");
    for (int i = 0; i < currentUserCount; i++) {
        // Display each employee's details (username, UID, status, etc.)
    }
}

void adminChangeEmployeeStatus() {
    // Prompt for UID and change employee status
    char uid[UIDLENGTH];
    printf("Enter UID of employee: ");
    scanf("%s", uid);

    // Find employee in employeeList array
    for (int i = 0; i < currentEmployeeCount; i++) {
        if (strcmp(employeeList[i].uid, uid) == 0) {
            // Prompt for and update employee status ('A', 'I', 'T')
            // Example: employeeList[i].status = 'I';
            // Save employeeList to file (fwrite or update function)
            break;
        }
    }

    setColor(GREEN);
    printf("Employee status changed successfully!\n");
    setColor(RESET);
}

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
        printf("5. Back to Admin Menu\n");
        printf("=========================\n");
        setColor(GREEN);
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                adminApproveNewEmployee();
                break;
            case 2:
                adminEditEmployeeDetails();
                break;
            case 3:
                adminViewAllEmployeeDetails();
                break;
            case 4:
                adminChangeEmployeeStatus();
                break;
            case 5:
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
                break;
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

//For first time login of admin the password is nothing it is admin123 in format
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
    srand(time(NULL));
    loadUserData();

    // Setup initial admin password if not already set
    if (!isInitialAdminPasswordSet()) {
        saveInitialAdminPassword();
    }

    mainMenu();
    
    return 0;
}
