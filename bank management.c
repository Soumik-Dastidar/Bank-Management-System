#include <stdio.h>
#include <string.h>

#define FILE_NAME "account.dat"

typedef struct {
    char name[100];
    int age;
    char number[20];   // Mobile number as string
    int acc_no;
    int pin;           // 4-digit PIN
    float balance;
} account;

// Function declarations
void createAccount();
int verifyAccount(int acc_no, int pin, account *acc);
void depositMoney();
void withdrawMoney();
void checkBalance();
void viewAccountDetails();

int main() {
    int choice;

    while (1) {
        printf("\n=====================================\n");
        printf("    Welcome to Pubali Bank LTD.\n");
        printf("=====================================\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. View Account Details\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }
        getchar(); // Clear newline left by scanf

        switch (choice) {
            case 1: createAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5: viewAccountDetails(); break;
            case 6:
                printf("\nThank you for choosing Pubali Bank LTD.\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

void createAccount() {
    account acc;
    FILE *file = fopen(FILE_NAME, "ab");
    if (!file) {
        printf("Failed to open file!\n");
        return;
    }

    printf("Enter your Name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0';  // Remove trailing newline

    printf("Enter your Age: ");
    while (scanf("%d", &acc.age) != 1 || acc.age <= 0) {
        printf("Invalid age. Please enter a positive number: ");
        while (getchar() != '\n'); // Clear invalid input
    }
    getchar(); // Clear newline

    printf("Enter your Mobile Number: ");
    fgets(acc.number, sizeof(acc.number), stdin);
    acc.number[strcspn(acc.number, "\n")] = '\0';

    printf("Enter your Account Number: ");
    while (scanf("%d", &acc.acc_no) != 1 || acc.acc_no <= 0) {
        printf("Invalid account number. Please enter a positive number: ");
        while (getchar() != '\n'); // Clear invalid input
    }
    getchar(); // Clear newline

    // Enter PIN and validate it is 4 digits
    do {
        printf("Set a 4-Digit PIN: ");
        if (scanf("%d", &acc.pin) != 1) {
            printf("Invalid input. Please enter a 4-digit number.\n");
            while (getchar() != '\n'); // Clear invalid input
            acc.pin = 0; // reset to invalid
        } else if (acc.pin < 1000 || acc.pin > 9999) {
            printf("PIN must be exactly 4 digits.\n");
            acc.pin = 0; // reset to invalid
            while (getchar() != '\n'); // Clear newline
        } else {
            while (getchar() != '\n'); // Clear newline after valid input
            break;
        }
    } while (1);

    acc.balance = 0.0f;

    fwrite(&acc, sizeof(acc), 1, file);
    fflush(file); // data should be written to the file immidiately
    fclose(file);

    printf("\nCONGRATULATIONS!\n");

    printf("Account created successfully!\n");
}

int verifyAccount(int acc_no, int pin, account *acc) {  // Useful for refactoring
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        printf("Failed to open file!\n");
        return 0;
    }

    while (fread(acc, sizeof(account), 1, file)) {
        if (acc->acc_no == acc_no && acc->pin == pin) {
            fclose(file);
            return 1;  // verified successfully
        }
    }
    fclose(file);
    return 0;  // not found or PIN mismatch
}

void depositMoney() {
    int acc_no, pin;
    float amount;
    int found = 0;
    account acc;

    printf("Enter your account number: ");
    if (scanf("%d", &acc_no) != 1) {
        printf("Invalid account number input.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    printf("Enter your 4-digit PIN: ");
    if (scanf("%d", &pin) != 1) {
        printf("Invalid PIN input.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    FILE *file = fopen(FILE_NAME, "rb+");
    if (!file) {
        printf("Failed to open file!\n");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no && acc.pin == pin) {
            printf("Enter amount to deposit: ");
            if (scanf("%f", &amount) != 1 || amount <= 0) {
                printf("Invalid amount.\n");
                while (getchar() != '\n');
                fclose(file);
                return;
            }
            getchar();

            acc.balance += amount;

            fseek(file, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, file);
            fflush(file);
            found = 1;
            printf("Deposit successful! New Balance: %.2f BDT\n", acc.balance);
            break;
        }
    }

    if (!found) {
        printf("Account number or PIN incorrect!\n");
    }

    fclose(file);
}

void withdrawMoney() {
    int acc_no, pin;
    float amount;
    int found = 0;
    account acc;

    printf("Enter your account number: ");
    if (scanf("%d", &acc_no) != 1) {
        printf("Invalid account number input.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    printf("Enter your 4-digit PIN: ");
    if (scanf("%d", &pin) != 1) {
        printf("Invalid PIN input.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    FILE *file = fopen(FILE_NAME, "rb+");
    if (!file) {
        printf("Failed to open file!\n");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no && acc.pin == pin) {
            printf("Enter amount to withdraw: ");
            if (scanf("%f", &amount) != 1 || amount <= 0) {
                printf("Invalid amount.\n");
                while (getchar() != '\n');
                fclose(file);
                return;
            }
            getchar();

            if (acc.balance >= amount) {
                acc.balance -= amount;

                fseek(file, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, file);
                fflush(file);
                found = 1;
                printf("Withdrawal successful! Remaining Balance: %.2f BDT\n", acc.balance);
            } else {
                printf("Insufficient balance!\n");
            }
            break;
        }
    }

    if (!found) {
        printf("Account number or PIN incorrect!\n");
    }

    fclose(file);
}

void checkBalance() {
    int acc_no, pin, found = 0;
    account acc;

    printf("Enter your account number: ");
    if (scanf("%d", &acc_no) != 1) {
        printf("Invalid account number input.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    printf("Enter your 4-digit PIN: ");
    if (scanf("%d", &pin) != 1) {
        printf("Invalid PIN input.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        printf("Failed to open file!\n");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no && acc.pin == pin) {
            printf("Your current balance: %.2f BDT\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account number or PIN incorrect!\n");
    }

    fclose(file);
}

void viewAccountDetails() {
    int acc_no, pin, found = 0;
    account acc;

    printf("Enter your account number: ");
    if (scanf("%d", &acc_no) != 1) {
        printf("Invalid account number input.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    printf("Enter your 4-digit PIN: ");
    if (scanf("%d", &pin) != 1) {
        printf("Invalid PIN input.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        printf("Failed to open file!\n");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no && acc.pin == pin) {
            printf("\n--- Account Details ---\n");
            printf("Name          : %s\n", acc.name);
            printf("Age           : %d\n", acc.age);
            printf("Mobile Number : %s\n", acc.number);
            printf("Account Number: %d\n", acc.acc_no);
            printf("Balance       : %.2f BDT\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account number or PIN incorrect!\n");
    }

    fclose(file);
}











