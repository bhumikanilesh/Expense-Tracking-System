# Family Expense Management System - USING ARRAY OF STRUCTURES

## Overview
This program is a **Family Expense Management System** written in C. It allows users to manage users, families, and expenses efficiently. The system supports the addition, retrieval, and updating of records for users, families, and expenses.

---
##Source Code in temp.c
## Features
1. **Add User**: Create a user with a unique ID, name, and income.
2. **Add Expense**: Record expenses for users with details like category, amount, and date.
3. **Create Family**: Define families, linking members by their user IDs.
4. **Get Total Expenses**: Calculate the total expenses for a specific user.
5. **Categorical Expenses**: Calculate the total expense for a specific category within a family.
6. **Highest Expense Day**: Determine the day with the highest expense for a family.
7. **Individual Expenses**: Retrieve and display all expenses for a specific user.
8. **Update/Delete Family**: Modify or remove family details.
9. **Update/Delete Expense**: Edit or delete specific expense records.

---

## How to Use

### Menu Options
When you run the program, you will be presented with a menu:

### Input Details
#### Add User
- Enter a unique `User ID`.
- Provide the `Name` of the user.
- Specify the `Income`.

#### Add Expense
- Enter a unique `Expense ID`.
- Specify the `User ID` associated with the expense.
- Input the `Category`, `Amount`, and `Date` (in `YYYY-MM-DD` format).

#### Create Family
- Enter a unique `Family ID`.
- Provide a `Family Name`.
- Input the number of members and their corresponding `User IDs`.

#### Get Total Expense
- Input the `User ID` to calculate their total expenses.

#### Categorical Expenses
- Provide the `Family ID` and a specific `Category` to calculate expenses.

#### Highest Expense Day
- Enter the `Family ID` to find the day with the highest expense.

#### Individual Expenses
- Specify the `User ID` to view all related expenses.

#### Update/Delete Family
- Input the `Family ID` to either update or delete its details.

#### Update/Delete Expense
- Enter the `Expense ID` to modify or remove an expense.

---

## Code Structure
### Data Structures
1. **`User`**:
   - `userID`: Unique identifier for a user.
   - `name`: User's name.
   - `income`: User's income.
2. **`Expense`**:
   - `expenseID`: Unique identifier for an expense.
   - `userID`: ID of the user who incurred the expense.
   - `category`: Expense category (e.g., food, travel).
   - `amount`: Expense amount.
   - `date`: Date of the expense (`YYYY-MM-DD` format).
3. **`Family`**:
   - `familyID`: Unique identifier for a family.
   - `familyName`: Name of the family.
   - `memberIDs`: Array of user IDs in the family.
   - `numMembers`: Number of family members.

---

## Limitations
- **User Limits**: Maximum of 100 users (`MAX_USERS`).
- **Expense Limits**: Maximum of 100 expenses (`MAX_EXPENSES`).
- **Family Limits**: Maximum of 100 families (`MAX_FAMILIES`).

---

## Future Enhancements
- Implement file I/O for persistent storage.
- Optimize `Get_highest_expense_day` for more accurate daily tracking.
- Introduce advanced expense filtering and sorting options.
- Add error handling for invalid inputs.

---

## How to Compile and Run
1. Save the code in a file (e.g., `expense_management.c`).
2. Open a terminal and navigate to the file directory.
3. Compile the program using GCC:
   ```bash
   gcc -o expense_management expense_management.c

1. Add User
2. Add Expense
3. Create Family
4. Get Total Expense
5. Get Categorical Expense
6. Get Highest Expense Day
7. Get Individual Expense
8. Update or Delete Individual Family Details
9. Update or Delete Expense
10. Exit
Enter your choice: 1
Enter User ID: 101
Enter Name: JohnDoe
Enter Income: 5000
User added successfully!
