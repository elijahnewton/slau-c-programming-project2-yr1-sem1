#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 100
#define FILENAME "users.csv"

// Define the structure for a user
struct User {
    int id;
    char firstName[50];
    char lastName[50];
    char address[100];
    char status[20]; // e.g., "Full-time", "Part-time"
    double salary;
};

// Array to store users
struct User users[MAX_USERS];
int user_count = 0;

void saveUsers();
void loadUsers();
int getNextId();
void createUser();

// Function to display a consistent header
void displayHeader(const char* title) {
    system("cls"); // Clear the screen
    printf("=============================================================\n");
    printf("    _    ____  __  __ ___ _   _ ____  _     _____ __  __   \n");
    printf("   / \\  |  _ \\|  \\/  |  _  | \\ | |  _ \\| |   | ____|\\ \\/ /   \n");
    printf("  / _ \\ | | | | |\\/| | | | |  \\| | |_) | |   |  _|   \\  /    \n");
    printf(" / ___ \\| |_| | |  | | |_| | |\\  | |  __/| |___| |___  /  \\    \n");
    printf("/_/   \\_\\____/|_|  |_|____/|_| \\_|_|   |_____|_____|/_/\\_\\   \n");
    printf("                                                             \n");
    printf("=============================================================\n");
    printf("                     %s\n", title);
    printf("-------------------------------------------------------------\n\n");
}


// Function to create a new user
void createUser() {
    if (user_count >= MAX_USERS) {
        printf("Maximum number of users reached.\n");
        return;
    }

    displayHeader("Create New User");

    struct User newUser;
    newUser.id = getNextId();

    printf("  Enter First Name: ");
    fgets(newUser.firstName, sizeof(newUser.firstName), stdin);
    newUser.firstName[strcspn(newUser.firstName, "\n")] = 0;

    printf("  Enter Last Name: ");
    fgets(newUser.lastName, sizeof(newUser.lastName), stdin);
    newUser.lastName[strcspn(newUser.lastName, "\n")] = 0;

    printf("  Enter Address: ");
    fgets(newUser.address, sizeof(newUser.address), stdin);
    newUser.address[strcspn(newUser.address, "\n")] = 0;

    printf("  Enter Employment Status (e.g., Full-time): ");
    fgets(newUser.status, sizeof(newUser.status), stdin);
    newUser.status[strcspn(newUser.status, "\n")] = 0;

    printf("  Enter Salary: ");
    char salaryBuffer[20];
    fgets(salaryBuffer, sizeof(salaryBuffer), stdin);
    sscanf(salaryBuffer, "%lf", &newUser.salary);

    users[user_count] = newUser;
    user_count++;

    saveUsers();
    printf("\n  [SUCCESS] Employee '%s %s' created with ID %d.\n", newUser.firstName, newUser.lastName, newUser.id);
    printf("\n  Press Enter to continue...");
    getchar(); // Wait for user to press Enter
}

// Function to read and display all active users
void readUsers() {
    displayHeader("Active User List");
    int found = 0;
    for (int i = 0; i < user_count; i++) {
        printf("  ----------------------------------------\n");
        printf("  ID              : %d\n", users[i].id);
        printf("  Name            : %s %s\n", users[i].firstName, users[i].lastName);
        printf("  Address         : %s\n", users[i].address);
        printf("  Status          : %s\n", users[i].status);
        printf("  Salary          : %.2f\n", users[i].salary);
        found = 1;
    }
    if (!found) {
        printf("  No active users found.\n");
    }
    printf("  ----------------------------------------\n");
}

// Function to update a user's name by ID
void updateUser() {
    int id;
    int user_idx = -1;
    char buffer[20];
    
    displayHeader("Update User");
    readUsers();
    printf("\n  Enter user ID to update: ");

    if (fgets(buffer, sizeof(buffer), stdin)) {
        if (sscanf(buffer, "%d", &id) != 1) {
            printf("Invalid ID format.\n");
            return;
        }
    } else {
        return; // Handle input error or EOF
    }

    for (int i = 0; i < user_count; i++) {
        if (users[i].id == id) {
            user_idx = i;
            break;
        }
    }

    if (user_idx != -1) {
        int choice = 0;
        displayHeader("Select Field to Update");
        printf("  Updating Employee: %s %s (ID: %d)\n\n", users[user_idx].firstName, users[user_idx].lastName, id);
        printf("  [1] First Name\n");
        printf("  [2] Last Name\n");
        printf("  [3] Address\n");
        printf("  [4] Employment Status\n");
        printf("  [5] Salary\n");
        printf("  [0] Cancel\n");
        printf("-------------------------------------------------------------\n");
        printf("  Enter your choice: ");

        if (fgets(buffer, sizeof(buffer), stdin)) {
            sscanf(buffer, "%d", &choice);
        }

        switch(choice) {
            case 1:
                printf("  Enter new First Name: ");
                fgets(users[user_idx].firstName, sizeof(users[user_idx].firstName), stdin);
                users[user_idx].firstName[strcspn(users[user_idx].firstName, "\n")] = 0;
                break;
            case 2:
                printf("  Enter new Last Name: ");
                fgets(users[user_idx].lastName, sizeof(users[user_idx].lastName), stdin);
                users[user_idx].lastName[strcspn(users[user_idx].lastName, "\n")] = 0;
                break;
            case 3:
                printf("  Enter new Address: ");
                fgets(users[user_idx].address, sizeof(users[user_idx].address), stdin);
                users[user_idx].address[strcspn(users[user_idx].address, "\n")] = 0;
                break;
            case 4:
                printf("  Enter new Employment Status: ");
                fgets(users[user_idx].status, sizeof(users[user_idx].status), stdin);
                users[user_idx].status[strcspn(users[user_idx].status, "\n")] = 0;
                break;
            case 5:
                printf("  Enter new Salary: ");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%lf", &users[user_idx].salary);
                break;
            case 0:
                printf("\n  Update cancelled.\n");
                system("pause");
                return;
            default:
                printf("\n  [ERROR] Invalid choice.\n");
                system("pause");
                return;
        }
        saveUsers();
        printf("\n  [SUCCESS] User with ID %d updated.\n", id);
        system("pause");
        return;
    }
    printf("\n  [ERROR] User not found.\n");
    system("pause");
}

// Function to delete a user by ID (permanently)
void deleteUser() {
    int id;
    int i;
    int found_index = -1;
    char buffer[20], confirm[10];

    displayHeader("Delete User");
    readUsers();
    printf("  Enter user ID to delete: ");
    
    if (fgets(buffer, sizeof(buffer), stdin)) {
        if (sscanf(buffer, "%d", &id) != 1) {
            printf("Invalid ID format.\n");
            return;
        }
    } else {
        return; // Handle input error or EOF
    }

    // Find the index of the user to delete
    for (i = 0; i < user_count; i++) {
        if (users[i].id == id) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        printf("\n  Are you sure you want to delete '%s %s' (ID: %d)? (yes/no): ", users[found_index].firstName, users[found_index].lastName, id);
        fgets(confirm, sizeof(confirm), stdin);
        confirm[strcspn(confirm, "\n")] = 0; // Remove newline

        if (strcmp(confirm, "yes") == 0) {
            // Shift elements to overwrite the deleted user
            for (i = found_index; i < user_count - 1; i++) {
                users[i] = users[i + 1];
            }
            user_count--; // Decrement the count of users

            saveUsers(); // Save the modified user list to the file
            printf("\n  [SUCCESS] User with ID %d has been permanently deleted.\n", id);
            system("pause");
        } else {
            printf("\n  Deletion cancelled.\n");
            system("pause");
        }
    } else {
        printf("\n  [ERROR] User not found.\n");
        system("pause");
    }
}

// Function to save all users to the CSV file
void saveUsers() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fprintf(file, "id,firstName,lastName,address,status,salary\n");
    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%d,%s,%s,%s,%s,%.2f\n", users[i].id, users[i].firstName, users[i].lastName, users[i].address, users[i].status, users[i].salary);
    }
    fclose(file);
}

// Function to load users from the CSV file
void loadUsers() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        // If the file doesn't exist, create it with a header
        file = fopen(FILENAME, "w");
        if (file == NULL) {
            printf("Error creating file.\n");
            return;
        }
        fprintf(file, "id,firstName,lastName,address,status,salary\n");
        fclose(file);
        return; // Start with an empty user list
    }
    
    char line[100];
    // Skip header
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return; // File is empty or read error
    }

    user_count = 0;
    while (user_count < MAX_USERS && fgets(line, sizeof(line), file) != NULL) {
        // Use a more robust format string for CSV parsing, note the removed %d for active
        if (sscanf(line, "%d,%49[^,],%49[^,],%99[^,],%19[^,],%lf", 
            &users[user_count].id, 
            users[user_count].firstName, users[user_count].lastName,
            users[user_count].address, users[user_count].status,
            &users[user_count].salary
            ) == 6) {
            user_count++;
        }
    }
    fclose(file);
}

// Function to get the next available user ID
int getNextId() {
    int max_id = 0;
    for (int i = 0; i < user_count; i++) {
        if (users[i].id > max_id) {
            max_id = users[i].id;
        }
    }
    return max_id + 1;
}

// Function to display the main menu
void displayBannerAndMenu() {
    displayHeader("AdminPlex :: Main Menu");
    printf("  [1] Create User\n");
    printf("  [2] View All Users\n");
    printf("  [3] Update User\n");
    printf("  [4] Delete User\n");
    printf("  [5] Exit\n");
    printf("-------------------------------------------------------------\n");
    printf("  Enter your choice: ");
}


int main() {
    int choice = 0;
    char buffer[20];
    loadUsers();

    do {
        displayBannerAndMenu();

        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%d", &choice) != 1) {
                choice = 0; // Invalid input, reset choice
            }
        } else {
            break; // Exit on input error or EOF
        }

        switch (choice) {
            case 1:
                createUser();
                break;
            case 2:
                readUsers();
                printf("\n  Press Enter to return to menu...");
                getchar();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                                deleteUser();
                break;
            case 5:
                printf("\n  Exiting AdminPlex. Goodbye!\n");
                break;
            default:
                printf("\n  [ERROR] Invalid choice. Please try again.\n");


        }
    } while (choice != 5);

    return 0;
}
