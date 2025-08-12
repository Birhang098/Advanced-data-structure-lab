#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY 100
#define MAX_MEANING 500

typedef struct Node {
    char key[MAX_KEY];
    char meaning[MAX_MEANING];
    struct Node *left, *right;
    int height;
} Node;

int max(int a, int b) { return (a > b) ? a : b; }
int height(Node *n) { return (n == NULL) ? 0 : n->height; }

Node* createNode(char key[], char meaning[]) {
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->key, key);
    strcpy(node->meaning, meaning);
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}
int getBalance(Node* n) { return (n == NULL) ? 0 : height(n->left) - height(n->right); }

Node* insert(Node* node, char key[], char meaning[]) {
    if (node == NULL) return createNode(key, meaning);

    if (strcmp(key, node->key) < 0)
        node->left = insert(node->left, key, meaning);
    else if (strcmp(key, node->key) > 0)
        node->right = insert(node->right, key, meaning);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && strcmp(key, node->left->key) < 0) return rightRotate(node);
    if (balance < -1 && strcmp(key, node->right->key) > 0) return leftRotate(node);
    if (balance > 1 && strcmp(key, node->left->key) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && strcmp(key, node->right->key) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL) current = current->left;
    return current;
}

Node* deleteNode(Node* root, char key[]) {
    if (root == NULL) return root;

    if (strcmp(key, root->key) < 0)
        root->left = deleteNode(root->left, key);
    else if (strcmp(key, root->key) > 0)
        root->right = deleteNode(root->right, key);
    else {
        if (root->left == NULL || root->right == NULL) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            Node* temp = minValueNode(root->right);
            strcpy(root->key, temp->key);
            strcpy(root->meaning, temp->meaning);
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL) return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

Node* search(Node* root, char key[], int* comparisons) {
    (*comparisons)++;
    if (root == NULL || strcmp(root->key, key) == 0) return root;
    if (strcmp(key, root->key) < 0) return search(root->left, key, comparisons);
    return search(root->right, key, comparisons);
}

void update(Node* root, char key[], char newMeaning[]) {
    int comps = 0;
    Node* found = search(root, key, &comps);
    if (found) {
        strcpy(found->meaning, newMeaning);
        printf("Updated successfully.\n");
    } else {
        printf("Keyword not found.\n");
    }
}

void inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%s : %s\n", root->key, root->meaning);
        inorder(root->right);
    }
}
void reverseInorder(Node* root) {
    if (root != NULL) {
        reverseInorder(root->right);
        printf("%s : %s\n", root->key, root->meaning);
        reverseInorder(root->left);
    }
}

int main() {
    Node* root = NULL;
    int choice;
    char key[MAX_KEY], meaning[MAX_MEANING];

    do {
        printf("\nDictionary Menu:\n");
        printf("1. Add keyword\n");
        printf("2. Delete keyword\n");
        printf("3. Update meaning\n");
        printf("4. Search keyword\n");
        printf("5. Display Ascending\n");
        printf("6. Display Descending\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter keyword: ");
                fgets(key, MAX_KEY, stdin); key[strcspn(key, "\n")] = 0;
                printf("Enter meaning: ");
                fgets(meaning, MAX_MEANING, stdin); meaning[strcspn(meaning, "\n")] = 0;
                root = insert(root, key, meaning);
                break;
            case 2:
                printf("Enter keyword to delete: ");
                fgets(key, MAX_KEY, stdin); key[strcspn(key, "\n")] = 0;
                root = deleteNode(root, key);
                break;
            case 3:
                printf("Enter keyword to update: ");
                fgets(key, MAX_KEY, stdin); key[strcspn(key, "\n")] = 0;
                printf("Enter new meaning: ");
                fgets(meaning, MAX_MEANING, stdin); meaning[strcspn(meaning, "\n")] = 0;
                update(root, key, meaning);
                break;
            case 4: {
                printf("Enter keyword to search and diaplay comparisions: ");
                fgets(key, MAX_KEY, stdin); key[strcspn(key, "\n")] = 0;
                int comps = 0;
                Node* found = search(root, key, &comps);
                if (found) printf("Meaning: %s (Comparisons: %d)\n", found->meaning, comps);
                else printf("Keyword not found (Comparisons: %d)\n", comps);
                break;
            }
            case 5:
                printf("\nAscending Order:\n");
                inorder(root);
                break;
            case 6:
                printf("\nDescending Order:\n");
                reverseInorder(root);
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 7);

    return 0;
}