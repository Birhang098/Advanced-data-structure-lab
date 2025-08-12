#include <stdio.h>
#include <string.h>
#define MAX_NAME 100
#define TABLE_SIZE 20
typedef struct {
    char name[MAX_NAME];
    char phone[15];
} Client;
Client hash_table[TABLE_SIZE];
int hash(char *name) {
    int sum = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        sum += name[i];
    }
    return sum % TABLE_SIZE;
}
void insert(char *name, char *phone) {
    int index = hash(name);
    strcpy(hash_table[index].name, name);
    strcpy(hash_table[index].phone, phone);
    printf("Stored %s -> %s at index %d\n", name, phone, index);
}
void search(char *name) {
    int index = hash(name);
    if (strcmp(hash_table[index].name, name) == 0) {
        printf("Phone number of %s is %s\n", name, hash_table[index].phone);
    } else {
        printf("Client %s not found.\n", name);
    }
}
int main() {
    char name[MAX_NAME], phone[15];
    int choice;
    while (1) {
        printf("\n1. Insert\n2. Search\n3. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        getchar();
        if (choice == 1) {
            printf("Enter name: ");
            scanf("%s", name); 
            printf("Enter phone number: ");
            scanf("%s", phone); 
            insert(name, phone);
        } else if (choice == 2) {
            printf("Enter the name of client: ");
            scanf("%s", name);
            search(name);
        } else {
            break;

        }
    }
    return 0;
}

