#include <stdio.h>
#include <string.h>

void add_user();
void delete_user();
void update_user();
void display_users();

struct User {
    int id;
    char name[50];
    int age;
};

// adding : helper function for validated name input
void get_valid_name(char *name, int size) {
    while(1) {
        printf("Enter user name: ");
        if (fgets(name, size, stdin) == NULL) {
            printf("Error reading input. Try again.\n");
            continue;
        }
        name[strcspn(name, "\n")] = '\0';
        if (strlen(name) == 0) {
            printf("Name cannot be empty.\n");
            continue;
        }
        break;
    }
}

// adding : helper function  for validated ID input with uniqueness check
int get_valid_id() {
    int id, found;
    struct User u;
    FILE *file;

    while(1) {
        printf("Enter user ID (positive integer): ");
        if (scanf("%d", &id) != 1) {
            printf("Invalid input. Please enter a numeric ID.\n");
            while(getchar() != '\n'); 
            continue;
        }
        if (id <= 0) {
            printf("ID must be positive.\n");
            while(getchar() != '\n');
            continue;
        }

        
        found = 0;
        file = fopen("users.txt", "r");
        if (file != NULL) {
            while(fscanf(file, "%d %49s %d", &u.id, u.name, &u.age) == 3) {
                if(u.id == id) {
                    found = 1;
                    printf("User ID already exists. Please enter a unique ID.\n");
                    break;
                }
            }
            fclose(file);
        }
        if (found) {
            while(getchar() != '\n');
            continue;
        }

        while(getchar() != '\n');
        break;
    }
    return id;
}

// helper function for validated age input
int get_valid_age() {
    int age;
    while(1) {
        printf("Enter user age (1-120): ");
        if (scanf("%d", &age) != 1) {
            printf("Invalid input. Please enter a numeric age.\n");
            while(getchar() != '\n'); 
            continue;
        }
        if (age < 1 || age > 120) {
            printf("Age must be between 1 and 120.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');
        break;
    }
    return age;
}

int main() {
    int choice;
    do {
        printf("\nWelcome to the Users Record Directory!\n");
        printf("1. Add user\n2. Display all users\n3. Update user by ID\n4. Delete user by ID\n5. Exit\n");
        printf("Enter your choice: ");
        if(scanf("%d", &choice) != 1) {
            printf("Invalid choice. Try again.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');

        switch(choice) {
            case 1: add_user(); break;
            case 2: display_users(); break;
            case 3: update_user(); break;
            case 4: delete_user(); break;
            case 5: printf("Exiting program...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while(choice != 5);
    return 0;
}

void add_user() {
    struct User u;
    FILE *file = fopen("users.txt", "a");
    if(file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    u.id = get_valid_id();
    get_valid_name(u.name, sizeof(u.name));
    u.age = get_valid_age();

    fprintf(file, "%d\t%s\t%d\n", u.id, u.name, u.age);
    fclose(file);

    printf("User added successfully!\n");
}

void display_users() {
    struct User u;
    FILE *file = fopen("users.txt", "r");
    char line[256];

    if (file == NULL) {
        printf("No users found. File does not exist.\n");
        return;
    }

    printf("\nUsers List\n");

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d\t%[^\t]\t%d", &u.id, u.name, &u.age) == 3) {
            printf("ID: %d | Name: %s | Age: %d\n", u.id, u.name, u.age);
        }
    }

    fclose(file);
}


void update_user() {
    int id, found = 0;
    struct User u;
    FILE *file, *tempFile;
    char line[256];

    printf("Enter the ID of the user to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("No users found. File does not exist.\n");
        return;
    }

    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d\t%[^\t]\t%d", &u.id, u.name, &u.age) == 3) {
            if (u.id == id) {
                found = 1;
                // printf("\nEnter new name: ");
                get_valid_name(u.name, sizeof(u.name));

                int age;
                while (1) {
                    printf("Enter new age (1-120): ");
                    if (scanf("%d", &age) != 1) {
                        printf("Invalid input. Please enter a numeric age.\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    if (age < 1 || age > 120) {
                        printf("Age must be between 1 and 120.\n");
                        continue;
                    }
                    u.age = age;
                    while (getchar() != '\n');
                    break;
                }
            }
            fprintf(tempFile, "%d\t%s\t%d\n", u.id, u.name, u.age);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("User updated successfully!\n");
    } else {
        remove("temp.txt");
        printf("User with ID %d not found.\n", id);
    }
}


void delete_user() {
    int userIdToDelete, found = 0;
    struct User currentUser;
    FILE *usersFile, *tempFile;
    char line[256];

    printf("Enter the ID of the user to delete: ");
    if (scanf("%d", &userIdToDelete) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    usersFile = fopen("users.txt", "r");
    if (usersFile == NULL) {
        printf("No users found. File does not exist.\n");
        return;
    }

    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file!\n");
        fclose(usersFile);
        return;
    }

    while (fgets(line, sizeof(line), usersFile)) {
        if (sscanf(line, "%d\t%[^\t]\t%d", &currentUser.id, currentUser.name, &currentUser.age) == 3) {
            if (currentUser.id == userIdToDelete) {
                found = 1;
                continue; 
            }
            fprintf(tempFile, "%d\t%s\t%d\n", currentUser.id, currentUser.name, currentUser.age);
        }
    }

    fclose(usersFile);
    fclose(tempFile);

    if (found) {
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("User deleted successfully!\n");
    } else {
        remove("temp.txt");
        printf("User with ID %d not found.\n", userIdToDelete);
    }
}
