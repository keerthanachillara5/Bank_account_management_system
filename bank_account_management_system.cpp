#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Account {
    int accountNumber;
    char name[50];
    float balance;
};
void createAccount();
void viewAccounts();
void depositMoney();
void withdrawMoney();
void deleteAccount();
int main() {
    int choice;
	while (1) {
        printf("\n=== Bank Account Management System ===\n");
        printf("1. Create New Account\n");
        printf("2. View All Accounts\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Delete Account\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;

            case 2:
                viewAccounts();
                break;

            case 3:
                depositMoney();
                break;

            case 4:
                withdrawMoney();
                break;

            case 5:
                deleteAccount();
                break;

            case 6:
                printf("\nThank you for using the Bank Account Management System.\n");
                printf("Project completed successfully!\n");
                exit(0);

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

// Create a new account
void createAccount() {
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "ab");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nEnter account number: ");
    scanf("%d", &acc.accountNumber);

    printf("Enter account holder name: ");
    getchar();  // Clear newline

    fgets(acc.name, 50, stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0';

    printf("Enter initial deposit: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);

    printf("Account created successfully!\n");
}

// View all accounts
void viewAccounts() {
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "rb");

    if (fp == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("\n--- All Accounts ---\n");

    while (fread(&acc, sizeof(acc), 1, fp)) {
        printf("Account Number: %d\n", acc.accountNumber);
        printf("Name: %s\n", acc.name);
        printf("Balance: %.2f\n", acc.balance);
        printf("-------------------------\n");
    }

    fclose(fp);
}

// Deposit money
void depositMoney() {
    int accNum, found = 0;
    float amount;
    struct Account acc;

    FILE *fp = fopen("accounts.dat", "rb+");

    if (fp == NULL) {
        printf("File not found.\n");
        return;
    }

    printf("Enter account number to deposit into: ");
    scanf("%d", &accNum);

    while (fread(&acc, sizeof(acc), 1, fp)) {

        if (acc.accountNumber == accNum) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);

            acc.balance += amount;

            fseek(fp, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);

            printf("Deposit successful! New balance: %.2f\n",
                   acc.balance);

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(fp);
}

// Withdraw money
void withdrawMoney() {
    int accNum, found = 0;
    float amount;
    struct Account acc;

    FILE *fp = fopen("accounts.dat", "rb+");

    if (fp == NULL) {
        printf("File not found.\n");
        return;
    }

    printf("Enter account number to withdraw from: ");
    scanf("%d", &accNum);

    while (fread(&acc, sizeof(acc), 1, fp)) {

        if (acc.accountNumber == accNum) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);

            if (acc.balance >= amount) {
                acc.balance -= amount;

                fseek(fp, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, fp);

                printf("Withdrawal successful! New balance: %.2f\n",
                       acc.balance);
            } else {
                printf("Insufficient balance!\n");
            }

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(fp);
}

// Delete an account
void deleteAccount() {
    int accNum, found = 0;
    struct Account acc;

    FILE *fp = fopen("accounts.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (fp == NULL || temp == NULL) {
        printf("Error opening file.\n");

        if (fp != NULL) {
            fclose(fp);
        }

        if (temp != NULL) {
            fclose(temp);
        }

        return;
    }

    printf("Enter account number to delete: ");
    scanf("%d", &accNum);

    while (fread(&acc, sizeof(acc), 1, fp)) {

        if (acc.accountNumber == accNum) {
            found = 1;
            printf("Account with number %d deleted.\n",
                   acc.accountNumber);
            continue;
        }

        fwrite(&acc, sizeof(acc), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    if (!found) {
        printf("Account not found.\n");
    }
}
