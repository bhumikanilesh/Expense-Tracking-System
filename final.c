#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERS 1000
#define MAX_FAMILIES 100
#define MAX_EXPENSES 1000
#define CATEGORIES 5
#define DAYS_IN_MONTH 10
#define MONTHS_IN_YEAR 12

// Expense categories
const char* categories[] = {"Rent", "Utility", "Grocery", "Stationary", "Leisure"};

// Structures
typedef struct Individual {
    int userID;
    char userName[50];
    float income;
    struct Individual *left;
    struct Individual *right;
    int height;
} Individual;

typedef struct Expense {
    int expenseID;
    int userID;
    int category;
    float amount;
    int day;
    int month;
    struct Expense *left;
    struct Expense *right;
    int height;
} Expense;

typedef struct FamilyMember {
    int userID;
    struct FamilyMember *next;
} FamilyMember;

typedef struct Family {
    int familyID;
    char familyName[50];
    FamilyMember *members;
    float totalIncome;
    float totalExpense;
    struct Family *left;
    struct Family *right;
    int height;
} Family;

typedef struct ExpenseAccumulator {
    int targetUserID;
    float total;
    float categoriesTotal[CATEGORIES];
} ExpenseAccumulator;

// Date range filter structure
typedef struct {
    int startDay, startMonth;
    int endDay, endMonth;
    bool hasResults;
} DateRangeFilter;

// ID range filter structure
typedef struct {
    int userID;
    int startID;
    int endID;
    bool hasResults;
} IDRangeFilter;

// Daily expense tracker structure
typedef struct {
    int familyID;
    float dailyExpenses[MONTHS_IN_YEAR][DAYS_IN_MONTH];
} DailyExpenseTracker;


typedef struct {
    int userID;
    char name[50];
    float amount;
} Contribution;


typedef struct {
    int category;
    Contribution *contributions;
    int memberCount;
    float *total;
} CategoryContext;


Individual *individualsRoot = NULL;
Family *familiesRoot = NULL;
Expense *expensesRoot = NULL;

// Function to check if date is valid
bool isValidDate(int day, int month) {
    return (day >= 1 && day <= DAYS_IN_MONTH) && 
           (month >= 1 && month <= MONTHS_IN_YEAR);
}

// AVL Tree utility functions
int max(int a, int b) {
    return (a > b) ? a : b;
}

int heightIndividual(Individual *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

int heightFamily(Family *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

int heightExpense(Expense *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

Individual *rightRotateIndividual(Individual *y) {
    Individual *x = y->left;
    Individual *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(heightIndividual(y->left), heightIndividual(y->right)) + 1;
    x->height = max(heightIndividual(x->left), heightIndividual(x->right)) + 1;

    return x;
}

Family *rightRotateFamily(Family *y) {
    Family *x = y->left;
    Family *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(heightFamily(y->left), heightFamily(y->right)) + 1;
    x->height = max(heightFamily(x->left), heightFamily(x->right)) + 1;

    return x;
}

Expense *rightRotateExpense(Expense *y) {
    Expense *x = y->left;
    Expense *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(heightExpense(y->left), heightExpense(y->right)) + 1;
    x->height = max(heightExpense(x->left), heightExpense(x->right)) + 1;

    return x;
}

Individual *leftRotateIndividual(Individual *x) {
    Individual *y = x->right;
    Individual *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(heightIndividual(x->left), heightIndividual(x->right)) + 1;
    y->height = max(heightIndividual(y->left), heightIndividual(y->right)) + 1;

    return y;
}

Family *leftRotateFamily(Family *x) {
    Family *y = x->right;
    Family *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(heightFamily(x->left), heightFamily(x->right)) + 1;
    y->height = max(heightFamily(y->left), heightFamily(y->right)) + 1;

    return y;
}

Expense *leftRotateExpense(Expense *x) {
    Expense *y = x->right;
    Expense *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(heightExpense(x->left), heightExpense(x->right)) + 1;
    y->height = max(heightExpense(y->left), heightExpense(y->right)) + 1;

    return y;
}

int getBalanceIndividual(Individual *node) {
    if (node == NULL)
        return 0;
    return heightIndividual(node->left) - heightIndividual(node->right);
}

int getBalanceFamily(Family *node) {
    if (node == NULL)
        return 0;
    return heightFamily(node->left) - heightFamily(node->right);
}

int getBalanceExpense(Expense *node) {
    if (node == NULL)
        return 0;
    return heightExpense(node->left) - heightExpense(node->right);
}

// Individual AVL operations
Individual* insertIndividual(Individual* node, int userID, char* userName, float income) {
    if (node == NULL) {
        Individual* newNode = (Individual*)malloc(sizeof(Individual));
        newNode->userID = userID;
        strcpy(newNode->userName, userName);
        newNode->income = income;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    if (userID < node->userID)
        node->left = insertIndividual(node->left, userID, userName, income);
    else if (userID > node->userID)
        node->right = insertIndividual(node->right, userID, userName, income);
    else
        return node; 

    node->height = 1 + max(heightIndividual(node->left), heightIndividual(node->right));

    int balance = getBalanceIndividual(node);

    // if we want to insert at left then leftside becomes heavier so we right rotate
    if (balance > 1 && userID < node->left->userID)
        return rightRotateIndividual(node);

    //same but here left insertion
    if (balance < -1 && userID > node->right->userID)
        return leftRotateIndividual(node);

    //left right case
    if (balance > 1 && userID > node->left->userID) {
        node->left = leftRotateIndividual(node->left);
        return rightRotateIndividual(node);
    }

    // rigght left case
    if (balance < -1 && userID < node->right->userID) {
        node->right = rightRotateIndividual(node->right);
        return leftRotateIndividual(node);
    }

    return node;
}

Individual* searchIndividual(Individual* root, int userID) {
    if (root == NULL || root->userID == userID)
        return root;

    if (root->userID < userID)
        return searchIndividual(root->right, userID);

    return searchIndividual(root->left, userID);
}

// Family AVL operations
Family* insertFamily(Family* node, int familyID, char* familyName) {
    if (node == NULL) {
        Family* newNode = (Family*)malloc(sizeof(Family));
        newNode->familyID = familyID;
        strcpy(newNode->familyName, familyName);
        newNode->members = NULL;
        newNode->totalIncome = 0.0;
        newNode->totalExpense = 0.0;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    if (familyID < node->familyID)
        node->left = insertFamily(node->left, familyID, familyName);
    else if (familyID > node->familyID)
        node->right = insertFamily(node->right, familyID, familyName);
    else
        return node; // Duplicate familyIDs not allowed

    node->height = 1 + max(heightFamily(node->left), heightFamily(node->right));

    int balance = getBalanceFamily(node);

    // Left Left Case
    if (balance > 1 && familyID < node->left->familyID)
        return rightRotateFamily(node);

    // Right Right Case
    if (balance < -1 && familyID > node->right->familyID)
        return leftRotateFamily(node);

    // Left Right Case
    if (balance > 1 && familyID > node->left->familyID) {
        node->left = leftRotateFamily(node->left);
        return rightRotateFamily(node);
    }

    // Right Left Case
    if (balance < -1 && familyID < node->right->familyID) {
        node->right = rightRotateFamily(node->right);
        return leftRotateFamily(node);
    }

    return node;
}

Family* searchFamily(Family* root, int familyID) {
    if (root == NULL || root->familyID == familyID)
        return root;

    if (root->familyID < familyID)
        return searchFamily(root->right, familyID);

    return searchFamily(root->left, familyID);
}

// Helper function to find family by user ID
void searchFamilies(Family* node, int userID, Family** result) {
    if (node == NULL || *result != NULL) return;
    
    FamilyMember *m = node->members;
    while (m != NULL) {
        if (m->userID == userID) {
            *result = node;
            return;
        }
        m = m->next;
    }
    
    searchFamilies(node->left, userID, result);
    searchFamilies(node->right, userID, result);
}

// Expense AVL operations
Expense* insertExpense(Expense* node, int expenseID, int userID, int category, float amount, int day, int month) {
    if (node == NULL) {
        Expense* newNode = (Expense*)malloc(sizeof(Expense));
        newNode->expenseID = expenseID;
        newNode->userID = userID;
        newNode->category = category;
        newNode->amount = amount;
        newNode->day = day;
        newNode->month = month;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    if (expenseID < node->expenseID)
        node->left = insertExpense(node->left, expenseID, userID, category, amount, day, month);
    else if (expenseID > node->expenseID)
        node->right = insertExpense(node->right, expenseID, userID, category, amount, day, month);
    else
        return node; // Duplicate expenseIDs not allowed

    node->height = 1 + max(heightExpense(node->left), heightExpense(node->right));

    int balance = getBalanceExpense(node);

    // Left Left Case
    if (balance > 1 && expenseID < node->left->expenseID)
        return rightRotateExpense(node);

    // Right Right Case
    if (balance < -1 && expenseID > node->right->expenseID)
        return leftRotateExpense(node);

    // Left Right Case
    if (balance > 1 && expenseID > node->left->expenseID) {
        node->left = leftRotateExpense(node->left);
        return rightRotateExpense(node);
    }

    // Right Left Case
    if (balance < -1 && expenseID < node->right->expenseID) {
        node->right = rightRotateExpense(node->right);
        return leftRotateExpense(node);
    }

    return node;
}

Expense* searchExpense(Expense* root, int expenseID) {
    if (root == NULL || root->expenseID == expenseID)
        return root;

    if (root->expenseID < expenseID)
        return searchExpense(root->right, expenseID);

    return searchExpense(root->left, expenseID);
}

// Family member operations
void addFamilyMember(Family *family, int userID) {
    FamilyMember *newMember = (FamilyMember*)malloc(sizeof(FamilyMember));
    newMember->userID = userID;
    newMember->next = family->members;
    family->members = newMember;
    
    // Update family income
    Individual *ind = searchIndividual(individualsRoot, userID);
    if (ind != NULL) {
        family->totalIncome += ind->income;
    }
}

bool isMember(Family *family, int userID) {
    FamilyMember *current = family->members;
    while (current != NULL) {
        if (current->userID == userID)
            return true;
        current = current->next;
    }
    return false;
}

int countMembers(Family *family) {
    int count = 0;
    FamilyMember *current = family->members;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

Individual* deleteIndividual(Individual* root, int userID) {
    if (root == NULL) return root;

    if (userID < root->userID)
        root->left = deleteIndividual(root->left, userID);
    else if(userID > root->userID)
        root->right = deleteIndividual(root->right, userID);
    else {
        if((root->left == NULL) || (root->right == NULL)) {
            Individual *temp = root->left ? root->left : root->right;
            if(temp == NULL) {
                temp = root;
                root = NULL;
            } else 
                *root = *temp;
            free(temp);
        } else {
            Individual* temp = root->right;
            while(temp->left != NULL)
                temp = temp->left;
            root->userID = temp->userID;
            strcpy(root->userName, temp->userName);
            root->income = temp->income;
            root->right = deleteIndividual(root->right, temp->userID);
        }
    }

    if(root == NULL) return root;

    root->height = 1 + max(heightIndividual(root->left), heightIndividual(root->right));
    int balance = getBalanceIndividual(root);

    // rebalancing
    if(balance > 1 && getBalanceIndividual(root->left) >= 0)
        return rightRotateIndividual(root);

    if(balance > 1 && getBalanceIndividual(root->left) < 0) {
        root->left = leftRotateIndividual(root->left);
        return rightRotateIndividual(root);
    }
    if(balance < -1 && getBalanceIndividual(root->right) <= 0)
        return leftRotateIndividual(root);

    if(balance < -1 && getBalanceIndividual(root->right) > 0) {
        root->right = rightRotateIndividual(root->right);
        return leftRotateIndividual(root);
    }

    return root;
}

Family* deleteFamily(Family* root, int familyID) {
    if(root == NULL) return root;


    if(familyID < root->familyID)
        root->left = deleteFamily(root->left, familyID);
    else if(familyID > root->familyID)
        root->right = deleteFamily(root->right, familyID);
    else {
        if((root->left == NULL) || (root->right == NULL)) {
            Family *temp = root->left ? root->left : root->right;
            if(temp == NULL) {
                temp = root;
                root = NULL;
            } 
            else 
                *root = *temp;
            free(temp);
        } 
        else {
            Family* temp = root->right;
            while(temp->left != NULL)
                temp = temp->left;
            root->familyID = temp->familyID;
            strcpy(root->familyName, temp->familyName);
            root->right = deleteFamily(root->right, temp->familyID);
        }
    }

    if(root == NULL) return root;

    root->height = 1 + max(heightFamily(root->left), heightFamily(root->right));
    int balance = getBalanceFamily(root);

    //left balanced so right rotate
    if(balance > 1 && getBalanceFamily(root->left) >= 0)
        return rightRotateFamily(root);
    //left-right case
    if(balance > 1 && getBalanceFamily(root->left) < 0) {
        root->left = leftRotateFamily(root->left);
        return rightRotateFamily(root);
    }
    //left case
    if(balance < -1 && getBalanceFamily(root->right) <= 0)
        return leftRotateFamily(root);

    //right-left
    if(balance < -1 && getBalanceFamily(root->right) > 0) {
        root->right = rightRotateFamily(root->right);
        return leftRotateFamily(root);
    }

    return root;
}

Expense* deleteExpense(Expense* root, int expenseID) {
    if(root == NULL) return root;

    if(expenseID < root->expenseID)
        root->left = deleteExpense(root->left, expenseID);
    else if(expenseID > root->expenseID)
        root->right = deleteExpense(root->right, expenseID);
    else {
        if((root->left == NULL) || (root->right == NULL)) {
            Expense *temp = root->left ? root->left : root->right;
            if(temp == NULL) {
                temp = root;
                root = NULL;
            } else 
                *root = *temp;
            free(temp);
        } else {
            Expense* temp = root->right;
            while(temp->left != NULL)
                temp = temp->left;
            root->expenseID = temp->expenseID;
            root->userID = temp->userID;
            root->category = temp->category;
            root->amount = temp->amount;
            root->day = temp->day;
            root->month = temp->month;
            root->right = deleteExpense(root->right, temp->expenseID);
        }
    }

    if(root == NULL) return root;

    root->height = 1 + max(heightExpense(root->left), heightExpense(root->right));
    int balance = getBalanceExpense(root);

    // Rebalancing
    if(balance > 1 && getBalanceExpense(root->left) >= 0)
        return rightRotateExpense(root);
    if(balance > 1 && getBalanceExpense(root->left) < 0) {
        root->left = leftRotateExpense(root->left);
        return rightRotateExpense(root);
    }
    if(balance < -1 && getBalanceExpense(root->right) <= 0)
        return leftRotateExpense(root);
    if(balance < -1 && getBalanceExpense(root->right) > 0) {
        root->right = rightRotateExpense(root->right);
        return leftRotateExpense(root);
    }

    return root;
}

// Find family by user ID
Family* findFamilyByUserID(int userID) {
    Family* result = NULL;
    searchFamilies(familiesRoot, userID, &result);
    return result;
}

// Required functions
void Add_User() {
    int userID;
    char userName[50];
    float income;
    
    while (1) {
        printf("Enter User ID(1-1000): ");
        scanf("%d", &userID);
        
        if (userID < 0 || userID > 1000) {
            printf("Error: Invalid User ID. Please enter a valid userid(1-1000).\n");
            continue;
        }
        
        if (searchIndividual(individualsRoot, userID) != NULL) {
            printf("Error: User ID %d already exists. Please enter a different ID.\n", userID);
            continue;
        }
        
        // Check if user is already in any family
        Family* existingFamily = NULL;
        searchFamilies(familiesRoot, userID, &existingFamily);
        if (existingFamily != NULL) {
            printf("Error: User ID %d is already a member of family %s. Please enter a different ID.\n", 
                  userID, existingFamily->familyName);
            continue;
        }
        
        break;
    }
    
    printf("Enter User Name: ");
    scanf("%s", userName);
    printf("Enter Income: ");
    scanf("%f", &income);
    
    individualsRoot = insertIndividual(individualsRoot, userID, userName, income);
    printf("User added successfully!\n");
}

void Add_Expense() {
    int expenseID, userID, category, day, month;
    float amount;
    
    while (1) {
        printf("Enter Expense ID (1 - 1000): ");
        scanf("%d", &expenseID);
        
        if (expenseID < 0 || expenseID > 1000) {
            printf("Error: Invalid Expense ID.\n");
            continue;
        }
        
        if (searchExpense(expensesRoot, expenseID) != NULL) {
            printf("Error: Expense ID %d already exists.\n", expenseID);
            continue;
        }
        
        break;
    }
    
    while (1) {
        printf("Enter User ID: ");
        scanf("%d", &userID);
        
        if (searchIndividual(individualsRoot, userID) == NULL) {
            printf("Error: User not found!\n");
            continue;
        }
        break;
    }
    
    while (1) {
        printf("Enter Category (0-Rent, 1-Utility, 2-Grocery, 3-Stationary, 4-Leisure): ");
        scanf("%d", &category);
        
        if (category < 0 || category >= CATEGORIES) {
            printf("Error: Invalid category!\n");
            continue;
        }
        break;
    }
    
    printf("Enter Amount: ");
    scanf("%f", &amount);
    
    while (1) {
        printf("Enter Day (1-%d): ", DAYS_IN_MONTH);
        scanf("%d", &day);
        printf("Enter Month (1-%d): ", MONTHS_IN_YEAR);
        scanf("%d", &month);
        
        if (!isValidDate(day, month)) {
            printf("Error: Invalid date! Please enter a valid day and month.\n");
            continue;
        }
        break;
    }
    
    expensesRoot = insertExpense(expensesRoot, expenseID, userID, category, amount, day, month);
    
    // Update family expense if user is in a family
    Family* family = findFamilyByUserID(userID);
    if (family != NULL) {
        family->totalExpense += amount;
    }
    
    printf("Expense added successfully!\n");
}
//handler is a void pointer to the expense tree that is used to traverse
void traverseExpensesWithContext(Expense* root, void (*handler)(Expense*, void*), void* context) {
    if (root != NULL) {
        traverseExpensesWithContext(root->left, handler, context);
        handler(root, context);
        traverseExpensesWithContext(root->right, handler, context);
    }
}

void expenseAccumulatorCallback(Expense* exp, void* context) {
    ExpenseAccumulator* acc = (ExpenseAccumulator*)context;
    if (exp->userID == acc->targetUserID) {
        acc->total += exp->amount;
        acc->categoriesTotal[exp->category] += exp->amount;
    }
}

void Create_Family() {
    int familyID;
    char familyName[50];
    
    while (1) {
        printf("Enter Family ID: ");
        scanf("%d", &familyID);
        
        if (familyID < 0) {
            printf("Error: Family ID cannot be negative. Please enter a positive number.\n");
            continue;
        }
        
        if (searchFamily(familiesRoot, familyID) != NULL) {
            printf("Error: Family ID %d already exists. Please enter a different ID.\n", familyID);
            continue;
        }
        
        break;
    }
    
    printf("Enter Family Name: ");
    scanf("%s", familyName);
    
    familiesRoot = insertFamily(familiesRoot, familyID, familyName);
    Family *family = searchFamily(familiesRoot, familyID);
    
    int numMembers;
    while (1) {
        printf("Enter number of members (1-4): ");
        scanf("%d", &numMembers);
        
        if (numMembers < 1 || numMembers > 4) {
            printf("Error: Invalid number of members. Must be between 1 and 4.\n");
            continue;
        }
        break;
    }
    
    for (int i = 0; i < numMembers; i++) {
        int userID;
        
        while (1) {
            printf("Enter User ID for member %d: ", i+1);
            scanf("%d", &userID);
            
            Individual *ind = searchIndividual(individualsRoot, userID);
            if (ind == NULL) {
                printf("Error: User not found. Please enter a valid user ID.\n");
                continue;
            }
            
            // Check if user is already in another family
            Family* existingFamily = NULL;
            searchFamilies(familiesRoot, userID, &existingFamily);
            if (existingFamily != NULL && existingFamily != family) {
                printf("Error: User %s (ID: %d) is already a member of family %s.\n", 
                      ind->userName, userID, existingFamily->familyName);
                continue;
            }
            
            addFamilyMember(family, userID);
            break;
        }
    }
    
    // Calculate total monthly expenses for the family
    family->totalExpense = 0.0;
    FamilyMember *member = family->members;
    while (member != NULL) {
        ExpenseAccumulator acc = {
            .targetUserID = member->userID,
            .total = 0
        };
        traverseExpensesWithContext(expensesRoot, expenseAccumulatorCallback, &acc);
        family->totalExpense += acc.total;
        member = member->next;
    }
    
    printf("\nFamily created successfully!\n");
    printf("Family Name: %s\n", family->familyName);
    printf("Total Members: %d\n", numMembers);
    printf("Total Monthly Income: %.2f\n", family->totalIncome);
    printf("Total Monthly Expenses: %.2f\n", family->totalExpense);
    
    float balance = family->totalIncome - family->totalExpense;
    if (balance >= 0) {
        printf("Remaining Balance: %.2f\n", balance);
    } else {
        printf("Deficit: %.2f\n", -balance);
    }
}

void Update_or_delete_Individual_Family_details() {
    int choice;
    printf("1. Update Individual\n2. Delete Individual\n3. Update Family\n4. Delete Family\nEnter choice: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        int userID;
        printf("Enter User ID to update: ");
        scanf("%d", &userID);
        
        Individual *ind = searchIndividual(individualsRoot, userID);
    
        if (ind == NULL) {
            printf("User not found!\n");
            return;
        }
        
        // Display current details
        printf("\nCurrent details:\n");
        printf("----------------\n");
        printf("User ID: %d\n", ind->userID);
        printf("Name: %s\n", ind->userName);
        printf("Income: %.2f\n", ind->income);
        
        // Get updates
        printf("\nEnter new details (enter '-' to keep current value):\n");
        
        printf("Name (%s): ", ind->userName);
        char newName[50];
        scanf("%s", newName);
        
        printf("Income (%.2f): ", ind->income);
        float newIncome;
        scanf("%f", &newIncome);
        
        // Store old values for comparison
        char oldName[50];
        strcpy(oldName, ind->userName);
        float oldIncome = ind->income;
        
        // Apply updates
        if (strcmp(newName, "-") != 0) {
            strcpy(ind->userName, newName);
        }
        
        if (newIncome != -1) {
            // Update family incomes if this user is in any families
            Family* family = findFamilyByUserID(userID);
            if (family != NULL) {
                family->totalIncome += (newIncome - ind->income);
            }
            ind->income = newIncome;
        }
        
        // Display updated details
        printf("\nUpdate successful!\n");
        printf("\nBefore update:\n");
        printf("-------------\n");
        printf("Name: %s\n", oldName);
        printf("Income: %.2f\n", oldIncome);
        
        printf("\nAfter update:\n");
        printf("------------\n");
        printf("Name: %s\n", ind->userName);
        printf("Income: %.2f\n", ind->income);
    }
    else if (choice == 2) {
    int userID;
    printf("Enter User ID to delete: ");
    scanf("%d", &userID);
    
    Individual *ind = searchIndividual(individualsRoot, userID);
    if (ind == NULL) {
        printf("User not found!\n");
        return;
    }
    
  
    printf("\nUser to be deleted:\n");
    printf("------------------\n");
    printf("User ID: %d\n", ind->userID);
    printf("Name: %s\n", ind->userName);
    printf("Income: %.2f\n", ind->income);
    
    char confirm;
    printf("\nAre you sure you want to delete this user? (y/n): ");
    scanf(" %c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
       
        Family* family = findFamilyByUserID(userID);
        if (family != NULL) {
            int flag=1;
            FamilyMember **ptr = &(family->members);
            while (*ptr != NULL && flag) {
                if ((*ptr)->userID == userID) {
                    FamilyMember *toDelete = *ptr;
                    *ptr = (*ptr)->next;
                    free(toDelete);
                    family->totalIncome -= ind->income;
                    flag=0;
                    break;
                }
                ptr = &((*ptr)->next);
            }
            
            // Checking if this was the last member
            if (family->members == NULL) {
                printf("\nThis was the last member of family %s (ID: %d).\n", 
                      family->familyName, family->familyID);
                printf("The family will also be deleted.\n");
                
                familiesRoot = deleteFamily(familiesRoot, family->familyID);
            }
        }
        
        individualsRoot = deleteIndividual(individualsRoot, userID);
        printf("User Deleted Successfully!\n");
    } else {
        printf("Deletion cancelled.\n");
    }
	}
    else if (choice == 3) {
        int familyID;
        printf("Enter Family ID to update: ");
        scanf("%d", &familyID);
        
        Family *fam = searchFamily(familiesRoot, familyID);
        if (fam == NULL) {
            printf("Family not found!\n");
            return;
        }
        
        // Display current details
        printf("\nCurrent family details:\n");
        printf("----------------------\n");
        printf("Family ID: %d\n", fam->familyID);
        printf("Name: %s\n", fam->familyName);
        printf("Members: %d\n", countMembers(fam));
        printf("Total Income: %.2f\n", fam->totalIncome);
        printf("Total Expenses: %.2f\n", fam->totalExpense);
        
        // Get updates
        printf("\nEnter new details (enter '-' to keep current value):\n");
        printf("Name (%s): ", fam->familyName);
        char newName[50];
        scanf("%s", newName);
        
        // Store old value for comparison
        char oldName[50];
        strcpy(oldName, fam->familyName);
        
        // Apply updates
        if (strcmp(newName, "-") != 0) {
            strcpy(fam->familyName, newName);
        }
        
        // Display updated details
        printf("\nUpdate successful!\n");
        printf("\nBefore update:\n");
        printf("-------------\n");
        printf("Name: %s\n", oldName);
        
        printf("\nAfter update:\n");
        printf("------------\n");
        printf("Name: %s\n", fam->familyName);
    }
    else if (choice == 4) {
    int familyID;
    printf("Enter Family ID to delete: ");
    scanf("%d", &familyID);

    Family *fam = searchFamily(familiesRoot, familyID);
    if (fam == NULL) {
        printf("Family not found!\n");
        return;
    }
    
    // Display details before deletion
    printf("\nFamily to be deleted:\n");
    printf("--------------------\n");
    printf("Family ID: %d\n", fam->familyID);
    printf("Name: %s\n", fam->familyName);
    printf("Members: %d\n", countMembers(fam));
    printf("Total Income: %.2f\n", fam->totalIncome);
    printf("Total Expenses: %.2f\n", fam->totalExpense);
    
    char confirm;
    printf("\nAre you sure you want to delete this family? (y/n): ");
    scanf(" %c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
    
        FamilyMember *current = fam->members;
        while (current != NULL) {
            FamilyMember *next = current->next;
            
            free(current);
            current = next;
        }
        
        familiesRoot = deleteFamily(familiesRoot, familyID);
        printf("Family Deleted.\n");
    } else {
        printf("Deletion cancelled.\n");
    }
}
}

void Update_delete_expense() {
    int choice;
    printf("1. Update Expense\n2. Delete Expense\nEnter choice: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        int expenseID;
        printf("Enter Expense ID to update: ");
        scanf("%d", &expenseID);
        
        Expense *exp = searchExpense(expensesRoot, expenseID);
        if (exp == NULL) {
            printf("Expense not found!\n");
            return;
        }
        
        printf("Current details:\n");
        printf("User ID: %d\nCategory: %s\nAmount: %.2f\nDate: %d/%d\n", 
               exp->userID, categories[exp->category], exp->amount, exp->day, exp->month);
        
        printf("Enter new category (0-Rent, 1-Utility, 2-Grocery, 3-Stationary, 4-Leisure or -1 to keep): ");
        int newCategory;
        scanf("%d", &newCategory);
        if (newCategory >= 0 && newCategory < CATEGORIES) {
            exp->category = newCategory;
        }
        
        printf("Enter new amount (or -1 to keep): ");
        float newAmount;
        scanf("%f", &newAmount);
        if (newAmount != -1) {
            // Update family total if needed
            Family* family = findFamilyByUserID(exp->userID);
            if (family != NULL) {
                family->totalExpense += (newAmount - exp->amount);
            }
            exp->amount = newAmount;
        }
        
        printf("Enter new day (1-10 or -1 to keep): ");
        int newDay;
        scanf("%d", &newDay);
        if (newDay >= 1 && newDay <= DAYS_IN_MONTH) {
            exp->day = newDay;
        }
        
        printf("Enter new month (1-12 or -1 to keep): ");
        int newMonth;
        scanf("%d", &newMonth);
        if (newMonth >= 1 && newMonth <= 12) {
            exp->month = newMonth;
        }
        
        printf("Expense updated successfully!\n");
    }
    else if (choice == 2) {
        int expenseID;
        printf("Enter Expense ID to delete: ");
        scanf("%d", &expenseID);

        Expense *exp = searchExpense(expensesRoot, expenseID);
        if (exp == NULL) {
            printf("Expense not found!\n");
            return;
        }
        
        // Update family total if needed
        Family* family = findFamilyByUserID(exp->userID);
        if (family != NULL) {
            family->totalExpense -= exp->amount;
        }
        
        expensesRoot = deleteExpense(expensesRoot, expenseID);
        printf("Expense deleted!\n");
    }
    else {
        printf("Invalid choice!\n");
    }
}



// Callback function for accumulating expenses
void individualExpenseCallback(Expense* exp, void* context) {
    ExpenseAccumulator* acc = (ExpenseAccumulator*)context;
    if (exp->userID == acc->targetUserID) {
        acc->total += exp->amount;
        acc->categoriesTotal[exp->category] += exp->amount;
    }
}

void Get_individual_expense() {
    int userID;
    printf("Enter User ID: ");
    scanf("%d", &userID);
    
    Individual *ind = searchIndividual(individualsRoot, userID);
    if (ind == NULL) {
        printf("User not found!\n");
        return;
    }
    
    ExpenseAccumulator acc = {
        .targetUserID = userID,
        .total = 0,
        .categoriesTotal = {0}
    };
    
    // Traverse expenses with context
    traverseExpensesWithContext(expensesRoot, individualExpenseCallback, &acc);
    
    // Display results
    printf("\nExpense Report for %s (ID: %d)\n", ind->userName, userID);
    printf("--------------------------------\n");
    printf("Total Monthly Expense: %.2f\n\n", acc.total);
    
    // Sort categories by amount (descending)
    int sorted[CATEGORIES] = {0,1,2,3,4};
    for(int i=0; i<CATEGORIES-1; i++) {
        for(int j=0; j<CATEGORIES-i-1; j++) {
            if(acc.categoriesTotal[sorted[j]] < acc.categoriesTotal[sorted[j+1]]) {
                int temp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = temp;
            }
        }
    }
    
    // Print breakdown
    printf("Category Breakdown:\n");
    printf("-------------------\n");
    for(int i=0; i<CATEGORIES; i++) {
        int cat = sorted[i];
        if(acc.categoriesTotal[cat] > 0) {
            printf("%-10s: %.2f (%.1f%%)\n", 
                  categories[cat], 
                  acc.categoriesTotal[cat],
                  (acc.categoriesTotal[cat]/acc.total)*100);
        }
    }
    printf("\n");
}

void dateRangeCallback(Expense* exp, void* context) {
    DateRangeFilter* filter = (DateRangeFilter*)context;
    
    // Check if expense is within date range
    if ((exp->month > filter->startMonth || 
         (exp->month == filter->startMonth && exp->day >= filter->startDay)) &&
        (exp->month < filter->endMonth || 
         (exp->month == filter->endMonth && exp->day <= filter->endDay))) {
        
        Individual* ind = searchIndividual(individualsRoot, exp->userID);
        printf("ID: %-5d Date: %2d/%-2d %-10s %-9s %7.2f (User: %s)\n",
               exp->expenseID,
               exp->day, exp->month,
               categories[exp->category],
               "", // Padding
               exp->amount,
               ind ? ind->userName : "Unknown");
        filter->hasResults = true;
    }
}

void Get_expense_in_period() {
    int day1, month1, day2, month2;
    printf("Enter start date (day month): ");
    scanf("%d %d", &day1, &month1);
    printf("Enter end date (day month): ");
    scanf("%d %d", &day2, &month2);
    
    if (month1 < 1 || month1 > 12 || month2 < 1 || month2 > 12 ||
        day1 < 1 || day1 > DAYS_IN_MONTH || day2 < 1 || day2 > DAYS_IN_MONTH) {
        printf("Invalid date!\n");
        return;
    }
    
    printf("\nExpenses between %d/%d/25 and %d/%d/25:\n", day1, month1, day2, month2);
    printf("------------------------------------------------\n");
    
    //a struct that records the range limits
    //this struuct is passed to the traverse function where 
    //each node is compared with this filter and then printed
    
    DateRangeFilter filter = {
        .startDay = day1,
        .startMonth = month1,
        .endDay = day2,
        .endMonth = month2,
        .hasResults = false
    };
    
    traverseExpensesWithContext(expensesRoot, dateRangeCallback, &filter);
    
    if (!filter.hasResults) {
        printf("No expenses found in this period.\n");
    }
    printf("\n");
}



void Get_total_expense() {
    int familyID;
    printf("Enter Family ID: ");
    scanf("%d", &familyID);
    
    Family *family = searchFamily(familiesRoot, familyID);
    if (family == NULL) {
        printf("Family not found!\n");
        return;
    }
    
    // Recalculate total expense
    family->totalExpense = 0.0;
    FamilyMember *member = family->members;
    while (member != NULL) {
        ExpenseAccumulator acc = {
            .targetUserID = member->userID,
            .total = 0
        };
        traverseExpensesWithContext(expensesRoot, expenseAccumulatorCallback, &acc);
        family->totalExpense += acc.total;
        member = member->next;
    }
    
    printf("\nFamily: %s (ID: %d)\n", family->familyName, family->familyID);
    printf("--------------------------------\n");
    printf("Total Monthly Income:    %.2f\n", family->totalIncome);
    printf("Total Monthly Expenses:  %.2f\n", family->totalExpense);
    
    float balance = family->totalIncome - family->totalExpense;
    
    printf("\nExpense Analysis:\n");
    printf("-----------------\n");
    if (balance >= 0) {
        printf("The family's expenses (%.2f) are WITHIN their income (%.2f).\n", 
              family->totalExpense, family->totalIncome);
        printf("Remaining Balance: %.2f\n", balance);
    } else {
        printf("WARNING: The family's expenses (%.2f) SURPASS their income (%.2f).\n", 
              family->totalExpense, family->totalIncome);
        printf("Deficit: %.2f\n", -balance);
    }
    
    // Calculate percentage of income spent
    if (family->totalIncome > 0) {
        float percentage = (family->totalExpense / family->totalIncome) * 100;
        printf("\nExpense-to-Income Ratio: %.1f%%\n", percentage);
        
        if (percentage > 100) {
            printf("The family is spending %.1f%% MORE than their income!\n", percentage - 100);
        } else if (percentage == 100) {
            printf("The family is spending exactly 100%% of their income.\n");
        } else {
            printf("The family is spending %.1f%% of their income.\n", percentage);
        }
    } else {
        printf("\nNote: Family has no income (income = 0).\n");
    }
    printf("\n");
}
void categoricalCallback(Expense *exp, void *context) {
    CategoryContext *ctx = (CategoryContext *)context;
    if (exp->category == ctx->category) {
        for (int i = 0; i < ctx->memberCount; i++) {
            if (exp->userID == ctx->contributions[i].userID) {
                ctx->contributions[i].amount += exp->amount;
                *(ctx->total) += exp->amount;
                break;
            }
        }
    }
}



 
void Get_categorical_expense() {
    int familyID, category;
    printf("Enter Family ID: ");
    scanf("%d", &familyID);
    printf("Enter Category (0-Rent, 1-Utility, 2-Grocery, 3-Stationary, 4-Leisure): ");
    scanf("%d", &category);
    
    if (category < 0 || category >= CATEGORIES) {
        printf("Invalid category!\n");
        return;
    }
    
    Family *family = searchFamily(familiesRoot, familyID);
    if (family == NULL) {
        printf("Family not found!\n");
        return;
    }

   Contribution contributions[4];
    
    int memberCount = 0;
    float total = 0.0;

    // Initialize contributions array
    FamilyMember *member = family->members;
    while (member != NULL && memberCount < 4) {
        Individual *ind = searchIndividual(individualsRoot, member->userID);
        if (ind) {
            contributions[memberCount].userID = ind->userID;
            strcpy(contributions[memberCount].name, ind->userName);
            contributions[memberCount].amount = 0.0;
            memberCount++;
        }
        member = member->next;
    }

    // Create and populate context
  	CategoryContext context;
	context.category = category;
	context.contributions = contributions;
	context.memberCount = memberCount;
	context.total = &total;


    // Traverse expenses with context
    traverseExpensesWithContext(expensesRoot, categoricalCallback, &context);

    // Sort contributions (bubble sort)
    for (int i = 0; i < memberCount-1; i++) {
        for (int j = 0; j < memberCount-i-1; j++) {
            if (contributions[j].amount < contributions[j+1].amount) {
                 Contribution temp = contributions[j];
                contributions[j] = contributions[j+1];
                contributions[j+1] = temp;
            }
        }
    }

    // Display results
    printf("\n%s Expenses for Family %s\n", categories[category], family->familyName);
    printf("Total: %.2f\n", total);
    printf("Individual Contributions:\n");
    
    for (int i = 0; i < memberCount; i++) {
        if (contributions[i].amount > 0) {
            printf("- %s (ID: %d): %.2f\n", 
                   contributions[i].name, 
                   contributions[i].userID,
                   contributions[i].amount);
        }
    }
    printf("\n");
}

void dailyExpenseCallback(Expense* exp, void* context) {
    DailyExpenseTracker* tracker = (DailyExpenseTracker*)context;
    Family* family = searchFamily(familiesRoot, tracker->familyID);
    if (family && isMember(family, exp->userID)) {
        if (exp->month >= 1 && exp->month <= 12 &&
            exp->day >= 1 && exp->day <= DAYS_IN_MONTH) {
            tracker->dailyExpenses[exp->month-1][exp->day-1] += exp->amount;
        }
    }
}

void Get_highest_expense_day() {
    int familyID;
    printf("Enter Family ID: ");
    scanf("%d", &familyID);
    
    Family *family = searchFamily(familiesRoot, familyID);
    if (family == NULL) {
        printf("Family not found!\n");
        return;
    }
    //struct to keep a track 
    //put this constraint wala struct in traverse() 
    DailyExpenseTracker tracker = {
        .familyID = familyID,
        .dailyExpenses = {0}
    };
    
    traverseExpensesWithContext(expensesRoot, dailyExpenseCallback, &tracker);
    
    float maxExpense = 0;
    int maxDay = 1, maxMonth = 1;
    
    for (int m = 0; m < MONTHS_IN_YEAR; m++) {
        for (int d = 0; d < DAYS_IN_MONTH; d++) {
            if (tracker.dailyExpenses[m][d] > maxExpense) {
                maxExpense = tracker.dailyExpenses[m][d];
                maxDay = d + 1;
                maxMonth = m + 1;
            }
        }
    }
    
    if (maxExpense > 0) {
        printf("Highest expense day for family %s: %d/%d/25 with total expense: %.2f\n", 
              family->familyName, maxDay, maxMonth, maxExpense);
    } else {
        printf("No expenses found for this family.\n");
    }
}

//checks each node with the range
//basically compares
void idRangeCallback(Expense* exp, void* context) {
    IDRangeFilter* filter = (IDRangeFilter*)context;
    
    if (exp->userID == filter->userID && 
        exp->expenseID >= filter->startID && 
        exp->expenseID <= filter->endID) {
        
        printf("ID: %-5d Date: %2d/%-2d %-10s %7.2f\n",
               exp->expenseID,
               exp->day, exp->month,
               categories[exp->category],
               exp->amount);
        filter->hasResults = true;
    }
}


void Get_expense_in_range() {
    int userID, expID1, expID2;
    printf("Enter User ID: ");
    scanf("%d", &userID);
    printf("Enter start Expense ID: ");
    scanf("%d", &expID1);
    printf("Enter end Expense ID: ");
    scanf("%d", &expID2);
    
    Individual *ind = searchIndividual(individualsRoot, userID);
    if (ind == NULL) {
        printf("User not found!\n");
        return;
    }
    
    if (expID1 > expID2) {
        printf("Invalid range!\n");
        return;
    }
    
    printf("\nExpenses for %s (ID: %d) between IDs %d and %d:\n", 
           ind->userName, userID, expID1, expID2);
    printf("------------------------------------------------\n");
    
    //basically a struct that stores the curr range, this struct is passed 
    //while traversing so we set the start and end points
    
    IDRangeFilter filter = {
        .userID = userID,
        .startID = expID1,
        .endID = expID2,
        .hasResults = false
    };
    
    traverseExpensesWithContext(expensesRoot, idRangeCallback, &filter);
    
    if (!filter.hasResults) {
        printf("No expenses found in this range.\n");
    }
    printf("\n");
}

// File handling functions
void saveIndividualsToFile() {
    FILE *file = fopen("individuals.txt", "w+");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    
    // Need to implement tree traversal and saving
    fclose(file);
}

void saveFamiliesToFile() {
    FILE *file = fopen("families.txt", "w+");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    
    // Need to implement tree traversal and saving
    fclose(file);
}

void saveExpensesToFile() {
    FILE *file = fopen("expenses.txt", "w+");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    
    // Need to implement tree traversal and saving
    fclose(file);
}

void loadIndividualsFromFile() {
    FILE *file = fopen("individuals.txt", "r+");
    if (file == NULL) {
        printf("No existing individuals data found. Starting fresh.\n");
        return;
    }
    
    // Need to implement loading and tree reconstruction
    fclose(file);
}

void loadFamiliesFromFile() {
    FILE *file = fopen("families.txt", "r+");
    if (file == NULL) {
        printf("No existing families data found. Starting fresh.\n");
        return;
    }
    
    // Need to implement loading and tree reconstruction
    fclose(file);
}

void loadExpensesFromFile() {
    FILE *file = fopen("expenses.txt", "w+");
    if (file == NULL) {
        printf("No existing expenses data found. Starting fresh.\n");
        return;
    }
    
    // Need to implement loading and tree reconstruction
    fclose(file);
}

void displayMenu() {
	
    printf("\n\tChoose from the menu given below!");
    printf("\n-----------------------------------------------------------\n");
    printf("1. Add User\n");
    printf("2. Add Expense\n");
    printf("3. Create Family\n");
    printf("4. Update/Delete Individual/Family\n");
    printf("5. Update/Delete Expense\n");
    printf("6. Get Total Family Expense\n");
    printf("7. Get Categorical Expense\n");
    printf("8. Get Highest Expense Day\n");
    printf("9. Get Individual Expense\n");
    printf("10. Get Expenses in Date Range\n");
    printf("11. Get Expenses in ID Range\n");
    printf("12. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    loadIndividualsFromFile();
    loadFamiliesFromFile();
    loadExpensesFromFile();
    printf("-----------------------------------------------------------");
    printf("\n\tWelcome to our Expense Tracking System!\n");
    printf("\n-----------------------------------------------------------");
    int choice;
    do {
        displayMenu();
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: Add_User(); break;
            case 2: Add_Expense(); break;
            case 3: Create_Family(); break;
            case 4: Update_or_delete_Individual_Family_details(); break;
            case 5: Update_delete_expense(); break;
            case 6: Get_total_expense(); break;
            case 7: Get_categorical_expense(); break; 
            case 8: Get_highest_expense_day(); break;
            case 9: Get_individual_expense(); break;
            case 10: Get_expense_in_period(); break;
            case 11: Get_expense_in_range(); break;
            case 12: 
                saveIndividualsToFile();
                saveFamiliesToFile();
                saveExpensesToFile();
                printf("Data saved. Exiting...\n");
                break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 12);
    
    return 0;
}
