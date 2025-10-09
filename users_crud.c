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
    int input_id;
    int id_exists;
    struct User existing_user;
    FILE *user_file;

    while(1) {
        printf("Enter user ID (positive integer): ");
        if (scanf("%d", &input_id) != 1) {
            printf("Invalid input. Please enter a numeric ID.\n");
            while(getchar() != '\n'); 
            continue;
        }
        if (input_id <= 0) {
            printf("ID must be positive.\n");
            while(getchar() != '\n');
            continue;
        }

        
        id_exists  = 0;
        user_file  = fopen("users.txt", "r");
        if (user_file != NULL) {
            while(fscanf(user_file, "%d %49s %d", &existing_user.id, existing_user.name, &existing_user.age) == 3) {
                if(existing_user.id == input_id) {
                    id_exists = 1;
                    printf("User ID already exists. Please enter a unique ID.\n");
                    break;
                }
            }
            fclose(user_file);
        }
        if (id_exists) {
            while(getchar() != '\n');
            continue;
        }

        while(getchar() != '\n');
        break;
    }
    return input_id;
}

// helper function for validated age input
int get_valid_age() {
    int input_age;
    while(1) {
        printf("Enter user age (1-120): ");
        if (scanf("%d", &input_age) != 1) {
            printf("Invalid input. Please enter a numeric age.\n");
            while(getchar() != '\n'); 
            continue;
        }
        if (input_age < 1 || input_age > 120) {
            printf("Age must be between 1 and 120.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');
        break;
    }
    return input_age;
}

int main() {
    int  menu_choice;
    do {
        printf("\nWelcome to the Users Record Directory!\n");
        printf("1. Add user\n2. Display all users\n3. Update user by ID\n4. Delete user by ID\n5. Exit\n");
        printf("Enter your choice: ");
        if(scanf("%d", &menu_choice) != 1) {
            printf("Invalid choice. Try again.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');

        switch( menu_choice) {
            case 1: add_user(); break;
            case 2: display_users(); break;
            case 3: update_user(); break;
            case 4: delete_user(); break;
            case 5: printf("Exiting program...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while( menu_choice != 5);
    return 0;
}

void add_user() {
    struct User new_user;
    FILE *user_file = fopen("users.txt", "a");

    if (user_file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    new_user.id = get_valid_id();
    get_valid_name(new_user.name, sizeof(new_user.name));
    new_user.age = get_valid_age();

    fprintf(user_file, "%d\t%s\t%d\n", new_user.id, new_user.name, new_user.age);
    fclose(user_file);

    printf("User added successfully!\n");
}

void display_users() {
    struct User current_user;
    FILE *user_file = fopen("users.txt", "r");
    char file_line[256];

    if (user_file == NULL) {
        printf("No users found. File does not exist.\n");
        return;
    }

    printf("\nUsers List:\n");

    while (fgets(file_line, sizeof(file_line), user_file)) {
        if (sscanf(file_line, "%d\t%[^\t]\t%d", &current_user.id, current_user.name, &current_user.age) == 3) {
            printf("ID: %d | Name: %s | Age: %d\n", current_user.id, current_user.name, current_user.age);
        }
    }

    fclose(user_file);
}

void update_user() {
    int target_id;
    int user_found = 0;
    struct User current_user;
    FILE *user_file, *temp_file;
    char file_line[256];

    printf("Enter the ID of the user to update: ");
    if (scanf("%d", &target_id) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    user_file = fopen("users.txt", "r");
    if (user_file == NULL) {
        printf("No users found. File does not exist.\n");
        return;
    }

    temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file!\n");
        fclose(user_file);
        return;
    }

    while (fgets(file_line, sizeof(file_line), user_file)) {
        if (sscanf(file_line, "%d\t%[^\t]\t%d", &current_user.id, current_user.name, &current_user.age) == 3) {
            if (current_user.id == target_id) {
                user_found = 1;

                get_valid_name(current_user.name, sizeof(current_user.name));
                current_user.age = get_valid_age();
            }
            fprintf(temp_file, "%d\t%s\t%d\n", current_user.id, current_user.name, current_user.age);
        }
    }

    fclose(user_file);
    fclose(temp_file);

    if (user_found) {
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("User updated successfully!\n");
    } else {
        remove("temp.txt");
        printf("User with ID %d not found.\n", target_id);
    }
}



void delete_user() {
    int target_id;
    int user_found = 0;
    struct User current_user;
    FILE *user_file, *temp_file;
    char file_line[256];

    printf("Enter the ID of the user to delete: ");
    if (scanf("%d", &target_id) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    user_file = fopen("users.txt", "r");
    if (user_file == NULL) {
        printf("No users found. File does not exist.\n");
        return;
    }

    temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file!\n");
        fclose(user_file);
        return;
    }

    while (fgets(file_line, sizeof(file_line), user_file)) {
        if (sscanf(file_line, "%d\t%[^\t]\t%d", &current_user.id, current_user.name, &current_user.age) == 3) {
            if (current_user.id == target_id) {
                user_found = 1;
                continue; // Skip writing deleted user
            }
            fprintf(temp_file, "%d\t%s\t%d\n", current_user.id, current_user.name, current_user.age);
        }
    }

    fclose(user_file);
    fclose(temp_file);

    if (user_found) {
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("User deleted successfully!\n");
    } else {
        remove("temp.txt");
        printf("User with ID %d not found.\n", target_id);
    }
}
