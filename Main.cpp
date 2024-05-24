#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include "User.h"
#include "Admin.h"
#include <sstream>

using namespace std;

// Function to save users vector to file
void saveUsers(const vector<User>& users, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& user : users) {
            file << user.username << "," << user.password << "," << user.email << "," << user.balance << "," << user.suspended << "\n";
        }
        file.close();
    }
    else {
        cout << "Unable to open file: " << filename << endl;
    }
}

// Function to read users vector from file
void readUsers(vector<User>& users, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string username, email, password;
            double balance;
            bool suspended;
            if (getline(ss, username, ',') && getline(ss, email, ',') &&
                getline(ss, password, ',') && (ss >> balance) && ss.ignore() &&
                (ss >> suspended)) {
                users.emplace_back(User(username, email, password, balance, suspended));
            }
            else {
                cout << "Error reading line: " << line << endl;
            }
        }
        file.close();
    }
    else {
        cout << "Unable to open file: " << filename << endl;
    }
}



// Function to save transactions queue to file
void saveTransactions(queue<transactions>& transactionsQueue, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        while (!transactionsQueue.empty()) {
            const transactions& trans = transactionsQueue.front();
            file << trans.transactionDate << "," << trans.type << "," << trans.code << "," << trans.senderUsername << ","
                << trans.recipientUsername << "," << trans.amount << "," << trans.day << "," << trans.month << ","
                << trans.year << "," << trans.hour << "," << trans.minute << "," << trans.datetype << "\n";
            transactionsQueue.pop();
        }
        file.close();
    }
    else {
        cout << "Unable to open file: " << filename << endl;
    }
}

// Function to read transactions queue from file
void readTransactions(queue<transactions>& transactionsQueue, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            transactions trans;
            sscanf_s(line.c_str(), "%[^,],%[^,],%d,%[^,],%[^,],%lf,%d,%d,%d,%d,%d,%[^,]",
                trans.transactionDate, &trans.type[0], &trans.code, &trans.senderUsername[0],
                &trans.recipientUsername[0], &trans.amount, &trans.day, &trans.month, &trans.year,
                &trans.hour, &trans.minute, &trans.datetype[0]);
            transactionsQueue.push(trans);
        }
        file.close();
    }
    else {
        cout << "Unable to open file: " << filename << endl;
    }
}

// Function to save user requests to file
void saveRequests(queue<requests>& requestsQueue, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        while (!requestsQueue.empty()) {
            const requests& req = requestsQueue.front();
            file << req.senderUsername << "," << req.recipientUsername << "," << req.amount << ","
                << req.day << "," << req.month << "," << req.year << "," << req.hour << ","
                << req.minute << "," << req.datetype << "\n";
            requestsQueue.pop();
        }
        file.close();
    }
    else {
        cout << "Unable to open file: " << filename << endl;
    }
}

// Function to read user requests from file
void readRequests(queue<requests>& requestsQueue, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            requests req;
            sscanf_s(line.c_str(), "%[^,],%[^,],%lf,%d,%d,%d,%d,%d,%[^,]",
                &req.senderUsername[0], &req.recipientUsername[0], &req.amount,
                &req.day, &req.month, &req.year, &req.hour, &req.minute, &req.datetype[0]);
            requestsQueue.push(req);
        }
        file.close();
    }
    else {
        cout << "Unable to open file: " << filename << endl;
    }
}


void userFunctions(User user, vector<User>& users, queue<transactions>& allTransactions, queue<int>& balances, queue <requests>& allRequests, map<string, queue<requests>>& userRequests, map<string, queue<transactions>>& userTransactions) {
    int choice, UnextChoice = 0;
    do {
        cout << "press 1 to change your password \n";
        cout << "press 0 to logout\n";
        cout << "press 2 to send money\n";
        cout << "press 3 to show my notification\n";
        cout << "press 4 to request money\n";
        cout << "press 5 to check your requests\n";
        cout << "press 6 to view your balance\n";
        cout << "press 7 to view transactions history\n";

        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            user.editProfile(users, UnextChoice);
            break;
        }
        case 2:
        {
            user.SendMoney(allTransactions, users, user.username, balances, userTransactions);
            break;
        }
        case 3:
        {
            user.ShowNotification(allTransactions, users, user, balances, userTransactions);
            break;
        }
        case 4:
        {
            user.requestMoney(users, user.username, allRequests, userRequests);
            break;
        }
        case 5:
        {
            user.CheckRequests(allTransactions, users, user.username, balances, allRequests, userRequests, userTransactions);
            break;
        }
        case 6:
        {
            user.viewBalance(user);
            break;
        }
        case 7:
        {
            user.viewTransactions(user.username, userTransactions);
            break;
        }
        case 0:
        {
            cout << "You logged out\n";
            return;
            break;
        }
        default:
            break;
        }
    } while (UnextChoice == 2);


}
void adminFunctions(Admin& admin, vector<User>& users, map<string, queue<transactions>>& userTransactions, queue<transactions >& allTransactions) {
    int choice;
    string username;
    string password;
    string email;
    do
    {
        cout << "press 1 to adjust user's balance\n";
        cout << "press 2 to view users transaction history\n";
        cout << "press 3 to view All Transactions\n";
        cout << "press 4 to Add User Account " << endl;
        cout << "press 5 to Delete User Account " << endl;
        cout << "press 6 to Edit User Account " << endl;
        cout << "press 7 to Suspend User Account " << endl;
        cout << "press 8 to reactivate User Account " << endl;
        cout << "press 0 to logout\n ";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            admin.adjustUserBalance(users);
            break;
        }
        case 2:
        {
            admin.viewUsersHistory(users, userTransactions);
            //readTransactionsFromFile(allTransactions, "transaction.txt");
            break;
        }
        case 3:
        {
            admin.printAllTransactions(users, allTransactions);
            break;
        }
        case 4:
        {
            cout << endl;
            cout << "Enter Username" << endl;
            cin >> username;
            cout << "Enter Password" << endl;
            cin >> password;
            cout << "Enter email" << endl;
            cin >> email;
            admin.addAccount(users, username, password, email);
            break;
        }
        case 5:
        {
            cout << endl;
            cout << "Enter Username" << endl;
            cin >> username;
            admin.deleteAccount(users, username);
            break;
        }
        case 6:
        {
            cout << endl;
            Admin edAccount;
            edAccount.editAccount(users);
            break;
        }
        case 7:
        {
            cout << endl;
            cout << "Enter Username" << endl;
            cin >> username;
            Admin susAccount;
            susAccount.suspendAccount(users, username);
            break;
        }
        case 8:
        {
            cout << endl;
            cout << "enter username" << endl;
            cin >> username;
            Admin susAccount;
            susAccount.reactivateAccount(users, username);

            break;

        }

        case 0: {
            cout << "You logged out\n";
            return;
            break;
        }

        default:
            break;
        }
    } while (choice != 0);
}

int main() {
    cout << "Welcome to CyberCash Carry Wallet !" << endl;
    vector<User>users;
    queue<transactions> allTransactions;
    queue<int> balances;
    queue <requests> allRequests;
    map<string, queue<requests>> userRequests;
    map<string, queue<transactions>> userTransactions;
    vector<User>userAccounts;

    readUsers(users, "users.txt");
    readTransactions(allTransactions, "transactions.txt");
    readRequests(allRequests, "requests.txt");

    int choice, nextChoice = 0;
    do {
        cout << "press 1 to register as user \npress 2 to login as user\npress 3 to login as admin\npress 0 to exit\n";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            User user = user.registerUser(users, nextChoice);
            break;
        }
        case 2:
        {
            User user = user.loginUser(users, nextChoice);
            if (nextChoice == 8) {
                userFunctions(user, users, allTransactions, balances, allRequests, userRequests, userTransactions);
            }
            break;
        }
        case 3: {
            Admin admin = admin.loginAdmin(nextChoice);

            if (nextChoice == 1)
                adminFunctions(admin, users, userTransactions, allTransactions);
            // adminFunctions(Admin admin, vector<Admin>userAccounts, vector<User> users, map<string, queue<transactions>> userTransactions, queue<transactions > allTransactions) {
            break;
        }
        case 0:
            cout << "exit..";
            return 0;
        default:
        {
            nextChoice = 0;
            break;
        }

        }
    } while (nextChoice != 0);

    // Save data to files before exiting
    saveTransactions(allTransactions, "transactions.txt");
    saveRequests(allRequests, "requests.txt");
    saveUsers(users, "users.txt");

    return 0;
}
