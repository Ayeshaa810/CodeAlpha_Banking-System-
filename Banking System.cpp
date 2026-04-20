#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

class Transaction {
public:
    string type;
    double amount;
    string timestamp;

    Transaction(string t, double amt) : type(t), amount(amt) {
        time_t now = time(0);
        char buf[26];
        // ctime_s requires a buffer and its size to prevent overflows
        ctime_s(buf, sizeof(buf), &now);
        timestamp = buf;
        if (!timestamp.empty()) timestamp.pop_back();
    }
};

class Account {
private:
    int accountNumber;
    double balance;
    vector<Transaction> history;

public:
    Account(int accNum) : accountNumber(accNum), balance(0.0) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            history.emplace_back("Deposit", amount);
            cout << "Successfully deposited $" << amount << endl;
        }
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient funds!" << endl;
            return false;
        }
        balance -= amount;
        history.emplace_back("Withdrawal", amount);
        cout << "Successfully withdrew $" << amount << endl;
        return true;
    }

    void addTransferRecord(string type, double amount) {
        history.emplace_back(type, amount);
    }

    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    void displayHistory() {
        cout << "\n--- Transaction History for Account " << accountNumber << " ---" << endl;
        for (const auto& tx : history) {
            cout << "[" << tx.timestamp << "] " << tx.type << ": $" << tx.amount << endl;
        }
        cout << "Current Balance: $" << balance << endl;
    }
};

// Represents a bank user who can own multiple accounts
class Customer {
public:
    string name;
    vector<Account*> accounts;

    Customer(string n) : name(n) {}

    void addAccount(Account* acc) {
        accounts.push_back(acc);
    }
};

// Main System to manage Customers, Accounts, and Transfers
class BankingSystem {
private:
    vector<Customer*> customers;
    vector<Account*> allAccounts;

public:
    void createCustomer(string name, int accNum) {
        Customer* newCust = new Customer(name);
        Account* newAcc = new Account(accNum);
        newCust->addAccount(newAcc);

        customers.push_back(newCust);
        allAccounts.push_back(newAcc);
        cout << "Account created for " << name << " with ID " << accNum << endl;
    }

    Account* findAccount(int accNum) {
        for (auto acc : allAccounts) {
            if (acc->getAccountNumber() == accNum) return acc;
        }
        return nullptr;
    }

    void transfer(int fromAccNum, int toAccNum, double amount) {
        Account* fromAcc = findAccount(fromAccNum);
        Account* toAcc = findAccount(toAccNum);

        if (fromAcc && toAcc) {
            if (fromAcc->withdraw(amount)) {
                toAcc->deposit(amount);
                // Custom records for transfer history
                fromAcc->addTransferRecord("Transfer Sent to " + to_string(toAccNum), amount);
                toAcc->addTransferRecord("Transfer Received from " + to_string(fromAccNum), amount);
                cout << "Transfer of $" << amount << " completed." << endl;
            }
        }
        else {
            cout << "One or both accounts not found." << endl;
        }
    }
};

int main() {
    BankingSystem bank;

    // 1. Create Customers and Accounts
    bank.createCustomer("Ayesha", 1001);
    bank.createCustomer("Coco", 1002);

    Account* ayeshaAcc = bank.findAccount(1001);
    Account* cocoAcc = bank.findAccount(1002);

    // 2. Perform Deposits/Withdrawals
    if (ayeshaAcc) {
        ayeshaAcc->deposit(1000.0);
        ayeshaAcc->withdraw(200.0);
    }

    // 3. Perform Fund Transfer
    bank.transfer(1001, 1002, 350.0);

    // 4. Display Account Info and Transaction Details
    if (ayeshaAcc) ayeshaAcc->displayHistory();
    if (cocoAcc) cocoAcc->displayHistory();

    return 0;
}