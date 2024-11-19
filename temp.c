//DONE BY - BT23CSE006 && BT23CSE007

#include <stdio.h>
#include <string.h>

#define MAX_USERS 1000
#define MAX_EXPENSES 100
#define MAX_FAMILIES 100


typedef struct {
    int userID;
    char name[50];
    float income;
} User;


typedef struct {
    int expenseID; 
    int userID; 
    char category[50]; 
    float amount; 
    char date[11]; 
} Expense;


typedef struct {
    int familyID;
    char familyName[50];
    int memberIDs[MAX_USERS];
    int numMembers;
} Family;


User users[MAX_USERS];
Expense expenses[MAX_EXPENSES];
Family families[MAX_FAMILIES];
int userCount = 0;
int expenseCount = 0;
int familyCount = 0;

char uid_file[] = "user_id_counter.txt";
char fid_file[] = "family_id_counter.txt"; 
char eid_file[] = "expense_id_counter.txt"; 
void initialize_user_id();
int get_next_user_id();

void initialize_user_id() 
{	
    
    FILE *fp = fopen(uid_file, "w+");
    if (!fp) 
    {
        fp = fopen(uid_file, "w");
        if (fp) 
        {
            fprintf(fp, "0"); 
            fclose(fp);
        }
    } 
    
    else 
    {
        fclose(fp); 
    }
}

int get_next_user_id()  
{
    int user_id = 0;
    FILE *fp = fopen(uid_file, "w+");
    
    if (fp) 
    {
        fscanf(fp, "%d", &user_id); 
        fclose(fp);
    } 
    else 
    {
        perror("Failed to open user ID file for reading");
        return -1;  
    }
    
    user_id++;

    fp = fopen(uid_file, "w");
    if (fp) 
    {
        fprintf(fp, "%d", user_id); 
        fclose(fp);
    } 
    else 
    {
        perror("Failed to open user ID file for writing");
        return -1;  
    }

    return user_id;  
}

void initialize_family_id() 
{
    FILE *fp = fopen(fid_file, "w+");
    if (!fp) 
    {
        fp = fopen(fid_file, "w");
        if (fp) 
        {
            fprintf(fp, "0"); 
            fclose(fp);
        }
    } 
    
    else 
    {
        fclose(fp); 
    }
}

int get_next_family_id()  
{
    int family_id = 0;
    FILE *fp = fopen(fid_file, "w+");
    
    if (fp) 
    {
        fscanf(fp, "%d", &family_id); 
        fclose(fp);
    } 
    else 
    {
        perror("Failed to open user ID file for reading");
        return -1;  
    }
    
    family_id++;

    fp = fopen(fid_file, "w");
    if (fp) 
    {
        fprintf(fp, "%d", family_id); 
        fclose(fp);
    } 
    else 
    {
        perror("Failed to open family ID file for writing");
        return -1;  
    }

    return family_id;  
}

void initialize_expense_id() 
{
    FILE *fp = fopen(eid_file, "w+");
    if (!fp) 
    {
        fp = fopen(eid_file, "w");
        if (fp) 
        {
            fprintf(fp, "0"); 
            fclose(fp);
        }
    } 
    
    else 
    {
        fclose(fp); 
    }
}

int get_next_expense_id() 
{
    int expense_id = 0;
    FILE *fp = fopen(eid_file, "w+");
    
    if (fp) 
    {
        fscanf(fp, "%d", &expense_id); 
        fclose(fp);
    } 
    else 
    {
        perror("Failed to open expense ID file for reading");
        return -1;  
    }
    
    expense_id++;

    fp = fopen(eid_file, "w");
    if (fp) 
    {
        fprintf(fp, "%d", expense_id); 
        fclose(fp);
    } 
    else 
    {
        perror("Failed to open expense ID file for writing");
        return -1;  
    }

    return expense_id;  
}


void AddUser() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached!\n");
        return;
    }
   
    User newUser;
    Expense expenses;
    int user_id=get_next_user_id();
    
    newUser.userID=user_id;
    expenses.userID=user_id;
    printf("This is your user ID: %d\n",newUser.userID);
   
    
    printf("Enter Name: ");
    scanf("%s", newUser.name); 
    
    printf("Enter Income: ");
    scanf("%f", &newUser.income);
    
    users[userCount++] = newUser;
    printf("User added successfully!\n");
}


void AddExpense() {
    if (expenseCount >= MAX_EXPENSES) {
        printf("Expense limit reached!\n");
        return;
    }
    
    Expense newExpense;
    
    int exp_id = get_next_expense_id();
    newExpense.expenseID = exp_id;
    printf("Your Expense ID is: %d\n", exp_id);
    
   
    printf("Enter User ID: ");
    scanf("%d", &newExpense.userID);  

    printf("Enter Category: ");
    scanf("%s", newExpense.category); 
    printf("Enter Amount: ");
    scanf("%f", &newExpense.amount);
    
    printf("Enter Date (YYYY-MM-DD): ");
    scanf("%s", newExpense.date); 
    
    expenses[expenseCount++] = newExpense; 
    printf("Expense added successfully!\n");
}


void CreateFamily() {
    if (familyCount >= MAX_FAMILIES) {
        printf("Family limit reached!\n");
        return;
    }
    
    Family newFamily;
    
    
    int fam_id = get_next_family_id();
    newFamily.familyID = fam_id;
    
    printf("Enter Family Name: ");
    scanf("%s", newFamily.familyName); 
    
    printf("Enter Number of Members: ");
    scanf("%d", &newFamily.numMembers);

    
    if (newFamily.numMembers > MAX_USERS) {
        printf("Error: Number of members exceeds allowed maximum.\n");
        return;
    }

    
    printf("Enter Member IDs (space-separated):\n");
    for (int i = 0; i < newFamily.numMembers; i++) {
        int memberID;
        scanf("%d", &memberID);
        

        
        int found = 0; 
        for (int j = 0; j < userCount; j++) {
            if (users[j].userID == memberID) {
                newFamily.memberIDs[i] = memberID; 
                found = 1;
                break;
            }
        }
        
        if (!found) {
            printf("Error: User ID %d not found! Please enter a valid user ID.\n", memberID);
            i--; 
        }
    }
    
    families[familyCount++] = newFamily;
    printf("Family created successfully!\n");
}



float Get_total_expense(int familyID) 
{
    float totalExpense = 0.0;
    float totalIncome = 0.0;
    
    for (int i = 0; i < familyCount; i++) 
    {
        if (families[i].familyID == familyID) 
        {
            
            for (int j = 0; j < families[i].numMembers; j++) 
            {
                int memberID = families[i].memberIDs[j];
                
                
                for (int k = 0; k < userCount; k++) 
                {
                    if (users[k].userID == memberID) {
                        totalIncome += users[k].income;
                        break;
                    }
                }
                
                for (int k = 0; k < expenseCount; k++) 
                {
                    if (expenses[k].userID == memberID) {
                        totalExpense += expenses[k].amount;
                    }
                }
            }
            
            break; 
        }
    }


    if (totalExpense <= totalIncome) {
        printf("The family's expenses are affordable.\n");
    } else {
        printf("The family's expenses exceed their income and are not affordable!\n");
    }

    return totalExpense;
}


void Get_categorical_expense() {
    int familyID;
    char category[50];
    
    printf("Enter Family ID: ");
    scanf("%d", &familyID);
    
    printf("Enter Category: ");
    scanf("%s", category); 

    float total = 0.0;

    for (int i = 0; i < expenseCount; i++) {
        if (expenses[i].userID == familyID && strcmp(expenses[i].category, category) == 0) {
            total += expenses[i].amount;
        }
    }

    printf("Total %s expenses for Family ID %d: %.2f\n", category, familyID, total);
}


void Get_highest_expense_day() {
    int familyID;
    
    printf("Enter Family ID: ");
    scanf("%d", &familyID);
    
    float maxExpense = 0.0;
    char highestExpenseDate[11];

    
    for (int i = 0; i < expenseCount; i++) {
        if (expenses[i].userID == familyID && expenses[i].amount > maxExpense) {
            maxExpense = expenses[i].amount;
            strcpy(highestExpenseDate, expenses[i].date);
        }
    }

    printf("Highest expense for Family ID %d is %.2f on %s\n", familyID, maxExpense, highestExpenseDate);
}


void Get_individual_expense() {
    int userID;

    printf("Enter User ID: ");
    scanf("%d", &userID);
    
    printf("Expenses for User ID %d:\n", userID);
    for (int i = 0; i < expenseCount; i++) 
    {
        if (expenses[i].userID == userID) {
            printf("Expense ID: %d\n, Category: %s\n, Amount: %.2f\n, Date: %s\n",
                   expenses[i].expenseID, expenses[i].category, expenses[i].amount, expenses[i].date);
        }
    }
}


void Update_or_delete_individual_family_details() {
    int familyID;
    printf("Enter Family ID to update/delete: ");
    scanf("%d", &familyID);
   
    while (getchar() != '\n'); 

    for (int i = 0; i < familyCount; i++) {
        if (families[i].familyID == familyID) 
        {
            printf("Family found: %s\n", families[i].familyName);
            printf("Do you want to (1) Update or (2) Delete? ");
            
            int choice;
            scanf("%d", &choice);
            
            while (getchar() != '\n'); 

            if (choice == 1) 
            {
                printf("Enter new Family Name: ");
                fgets(families[i].familyName, sizeof(families[i].familyName), stdin);
                families[i].familyName[strcspn(families[i].familyName, "\n")] = 0; 
                printf("Family updated successfully!\n");
            } 
            else if (choice == 2) 
            {
                
                for (int j = i; j < familyCount - 1; j++) 
                {
                    families[j] = families[j + 1];
                }
                familyCount--;
                printf("Family deleted successfully!\n");
            } 
            else {
                printf("Invalid choice! Returning to main menu.\n");
            	}
            
        }
        else{
        printf("Family not found!\n");}
    }
    	
}



void Update_delete_expense() {
    int expenseID;
    printf("Enter Expense ID to update/delete: ");
    scanf("%d", &expenseID);

    for (int i = 0; i < expenseCount; i++) {
        if (expenses[i].expenseID == expenseID) {
            printf("Expense found: %s\n", expenses[i].category);
            printf("Do you want to (1) Update or (2) Delete? ");
            int choice;
            scanf("%d", &choice);

            if (choice == 1) {
                printf("Enter new Category: ");
                scanf("%s", expenses[i].category);
                printf("Enter new Amount: ");
                scanf("%f", &expenses[i].amount);
                printf("Enter new Date (YYYY-MM-DD): ");
                scanf("%s", expenses[i].date);
                printf("Expense updated successfully!\n");
            } else if (choice == 2) {
                
                for (int j = i; j < expenseCount - 1; j++) {
                    expenses[j] = expenses[j + 1];
                }
                expenseCount--;
                printf("Expense deleted successfully!\n");
            }
            return;
        }
    }
    printf("Expense not found!\n");
}


int main() {
    int choice;

    while (1) {
        printf("\n================== Expense Tracking System ==================\n");
        printf("Options:\n");
        printf("1. Add user\n");
        printf("2. Add expense\n");
        printf("3. Create family\n");
        printf("4. Get total expense \n");
        printf("5. Get categorical expense Update or delete expense\n");
        printf("6. Get highest expense day\n");
        printf("7. Get individual expense\n");
        printf("8. Update or delete individual family details\n");
        printf("9. Update or delete expenses\n");
        printf("0. Exit\n");
        printf("\n\n");
        
        printf("Please enter your choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
            case 1:
                AddUser();
                break;
            case 2:
                AddExpense();
                break;
            case 3:
                CreateFamily();
                break;
            case 4: {
                int userID;
                printf("Enter User ID to get total expenses: ");
                scanf("%d", &userID);
                float total = Get_total_expense(userID);
                printf("Total expenses for User ID %d: %.2f\n", userID, total);
                break;
            }
            case 5:
                Get_categorical_expense();
                break;
            case 6:
                Get_highest_expense_day();
                break;
            case 7:
                Get_individual_expense();
                break;
            case 8:
                Update_or_delete_individual_family_details();
                break;
            case 9:
                Update_delete_expense();
                break;
            case 0:
                printf("Exiting...\n");
                return 0; 
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}
