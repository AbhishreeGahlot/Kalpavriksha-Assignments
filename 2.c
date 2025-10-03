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

int main() {
    int key;
    printf("Welcome to the Users Record Directory !\n");
    printf("Enter the key for the following commands:\n");
    printf(" 1.Add user\n 2.Display all users \n 3.Update user by ID \n 4.Delete user by ID \n 5.Exit");
    printf("\nEnter your choice: ");
    scanf("%d", &key);

    switch(key) {
        case 1: add_user(); break;
        case 2: display_users(); break;
        case 3: update_user(); break;
        case 4: delete_user(); break;
        case 5: printf("Exiting program...\n"); break;
        default: printf("Invalid choice!\n");
    }

    return 0;
}

void add_user() {
    struct User u;
    FILE *f = fopen("users.txt", "a");
    if(f == NULL) { printf("\nError opening file!\n"); return; }

    printf("\nEnter user ID: "); scanf("%d", &u.id); getchar();
    printf("Enter user name: "); fgets(u.name, sizeof(u.name), stdin);
    u.name[strcspn(u.name, "\n")] = '\0';
    printf("Enter user age: "); scanf("%d", &u.age);

    fprintf(f, "%d %s %d\n", u.id, u.name, u.age);
    fclose(f);

    printf("User added successfully!\n");
}

void display_users() {
    struct User u;
    FILE *f = fopen("users.txt", "r");
    if(f == NULL) {
        printf("\nNo users found. File does not exist.\n");
        return;
    }

    printf("\n Users List  \n");

    while(fscanf(f, "%d %49s %d", &u.id, u.name, &u.age) == 3) {
        printf("ID: %d | Name: %s | Age: %d\n", u.id, u.name, u.age);
    }

    fclose(f);
}

void update_user() {
    int id, found = 0;
    struct User u;
    FILE *f, *temp;

    printf("\nEnter the ID of the user to update: ");
    scanf("%d", &id);
    getchar(); 

    f = fopen("users.txt", "r");
    if(f == NULL) {
        printf("\nNo users found. File does not exist.\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if(temp == NULL) {
        printf("\nError creating temporary file!\n");
        fclose(f);
        return;
    }

    while(fscanf(f, "%d %49s %d", &u.id, u.name, &u.age) == 3) {
        if(u.id == id) {
            found = 1;
            printf("Enter new name: ");
            fgets(u.name, sizeof(u.name), stdin);
            u.name[strcspn(u.name, "\n")] = '\0';
            printf("Enter new age: ");
            scanf("%d", &u.age);
            getchar(); 
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(f);
    fclose(temp);

    if(found) {
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("\nUser updated successfully!\n");
    } else {
        remove("temp.txt");
        printf("\nUser with ID %d not found!\n", id);
    }
}

void delete_user() {
    int id, found = 0;
    struct User u;
    FILE *f, *temp;

    printf("\nEnter the ID of the user to delete: ");
    scanf("%d", &id);

    f = fopen("users.txt", "r");
    if(f == NULL) {
        printf("\nNo users found. File does not exist.\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if(temp == NULL) {
        printf("\nError creating temporary file!\n");
        fclose(f);
        return;
    }

    while(fscanf(f, "%d %49s %d", &u.id, u.name, &u.age) == 3) {
        if(u.id == id) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(f);
    fclose(temp);

    if(found) {
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("\nUser deleted successfully!\n");
    } else {
        remove("temp.txt");
        printf("\nUser with ID %d not found!\n", id);
    }
}
