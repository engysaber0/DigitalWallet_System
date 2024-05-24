#include "Admin.h"
#include"User.h"
#include <iostream>
#include<string>
#include <fstream>
#include <vector>
#include <conio.h>

using namespace std;

string Admin::getHidePassword() {
    string password;
    char ch;
    cout << "Enter password: ";
    while (true) {
        ch = _getch();
        if (ch == 13) {
            cout << endl;
            break;
        }
        else if (ch == 8) { // If Backspace key is pressed
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            cout << '*';
            password += ch;
        }
    }
    return password;
}

bool Admin::isAdmin(string username, string password) {
    string adminUsername = "admin";
    string adminPassword = "asmaa123";
    if (username == adminUsername && password == adminPassword) {
        return true;
    }
    else {
        return false;
    }
}

Admin Admin::loginAdmin(int& nextChoice) {
    Admin admin;
    string inUsername, inPassword;
    cout << "Enter username: ";
    cin >> inUsername;
    inPassword = getHidePassword();
    cout << inPassword;
    if (isAdmin(inUsername, inPassword)) {
        cout << "Login successful! Welcome, Admin." << endl;
        cout << "press 1 to admin functions\n" << endl;
        cout << "press 2 to home page\n";
        cin >> nextChoice;
        while (nextChoice != 1 && nextChoice != 2) {
            cout << "Invalid choice, Enter again!!\n";
            cin >> nextChoice;
        }

    }
    else {
        cout << "Invalid username or password. Access denied." << endl;
        cout << "press 2 to home page.\npress 0 to exit.\n" << endl;
        cin >> nextChoice;
    }
    return admin;


}


void Admin::viewUsersHistory(vector<User> users, map<string, queue<transactions>> userTransactions)
{
    if (users.empty())
    {
        cout << "There's no users!" << endl;
    }
    else
    {
        sort(users.begin(), users.end());
        for (User currentUser : users)
        {
            cout << currentUser.username << ':' << endl << endl;
            currentUser.viewBalance(currentUser);
            cout << endl;
            auto it = userTransactions.find(currentUser.username);
            if (it != userTransactions.end())
            {
                currentUser.viewTransactions(currentUser.username, userTransactions);
            }
            else
            {
                cout << currentUser.username << " didn't do any transactions yet!" << endl << endl;
            }
            cout << "---------------------------" << endl;
        }
    }
}
void Admin::adjustUserBalance(vector<User>& users) {
    string username;
    double newBalance;

    cout << "Enter the username of the user whose balance you want to adjust: ";
    cin >> username;

    // Search for the user in the vector of users
    auto it = find_if(users.begin(), users.end(), [&username](const User& user) {
        return user.username == username;
        });

    if (it != users.end()) {
        cout << "Enter the new balance for user " << username << ": ";
        cin >> newBalance;

        // Update the user's balance
        it->balance = newBalance;

        cout << "User " << username << "'s balance has been adjusted to " << newBalance << endl;
    }
    else {
        cout << "User " << username << " not found!" << endl;
    }
}
void Admin::printAllTransactions(vector<User>& users, queue<transactions>& allTransactions) {
    transactions transaction;
    queue<transactions> tmp1;
    if (users.empty()) {
        cout << "there is no users\n" << endl;
    }
    if (allTransactions.empty()) {
        cout << "there is no transactions\n" << endl;
    }
    while (!allTransactions.empty()) {
        // Print sorted transactions
        transaction = allTransactions.front();
        cout << "All Transactions:" << endl;
        cout << "Sender: " << transaction.senderUsername << ", ";
        cout << "Recipient: " << transaction.recipientUsername << ", ";
        cout << "Amount: " << transaction.amount << ", ";
        cout << "Date/Time: " << transaction.day << "/" << transaction.month << "/" << transaction.year;
        cout << " " << transaction.hour << ":" << transaction.minute << " " << transaction.datetype << endl;
        tmp1.push(allTransactions.front());
        allTransactions.pop();
    };


    while (!tmp1.empty()) {
        allTransactions.push(tmp1.front());
        tmp1.pop();
    }
}
string Admin::getUsername() {
    return username;
}

string Admin::getPassword() {
    return password;
}

void Admin::setUsername(string& Username)
{
    username = Username;
}

void Admin::setPassword(string& Password)
{
    password = Password;
}

bool Admin::check_unique_username(vector<User>& users, string& username) {

    for (int i = 0; i < users.size(); i++) {
        if (username == users[i].username)
            return false;

    }
    return true;
}

void Admin::displayAllAccounts(vector<User>& users) {

    for (int i = 0; i < users.size(); i++) {
        cout << "Username: " << users[i].username << ", Password: " << users[i].password << ", Email: " << users[i].email << endl;
    }
}

void Admin::addAccount(vector<User>& users, string& username, string& password, string& email) {
    if (users.empty()) {
        User newAccount(username, password, email, 2000.0, false);
        users.push_back(newAccount);
        cout << "Account Added Successfully" << endl;
        cout << "All Accounts :" << endl;
        displayAllAccounts(users);
    }
    else {
        if (!check_unique_username(users, username)) {
            cout << username << " Is Not Unique. Enter Another Username To Add Account: " << endl;
            cin >> username;
            //to call addAccount with new username
            addAccount(users, username, password, email);
            //not to continue run the remain of the fn to not add account with ununique username
            return;
        }
        else {
            User newAccount(username, password, email, 2000.0, false);
            users.push_back(newAccount);
            cout << "Account Added Successfully" << endl;
            cout << endl;
            cout << "All Accounts After addition :" << endl;
            displayAllAccounts(users);
        }

    }
}
void Admin::deleteAccount(vector <User>& users, string& username) {

    bool accountFound = false;
    for (int i = 0; i < users.size(); i++) {

        if (username == users[i].username) {

            users.erase(users.begin() + i);
            cout << "Account Deleted successfully" << endl;
            accountFound = true;
            cout << endl;
            //display all accounts after deletion
            cout << "All Accounts After Deletion :" << endl;
            displayAllAccounts(users);
            //to exit the loop after deleting account and the function exits
            break;
        }
    }
    if (!accountFound) {

        cout << "Username " << username << " Not Found " << endl;
        cout << endl;

    }

}

void Admin::editAccount(vector<User>& users) {
    cout << endl;
    cout << "All Accounts :" << endl;
    displayAllAccounts(users);
    cout << "To Edit Username Press 1" << endl;
    cout << "To Edit Password Press 2" << endl;
    cout << "To Edit Username Press 3" << endl;

    cout << "Enter 1 or 2 or 3" << endl;
    int choice;
    string Username;
    string Password;
    string email;
    string newEmail;
    string newUsername;
    string newPassword;
    cin >> choice;
    switch (choice) {
    case 1:
        cout << "Enter The Username To Edit It " << endl;
        cin >> Username;
        for (int i = 0; i < users.size(); i++) {
            if (Username == users[i].username) {
                cout << "Enter a New Value Of The Username " << endl;
                cin >> newUsername;
                if (check_unique_username(users, newUsername)) {
                    users[i].setUsername(newUsername);
                    cout << "The Edition Is successfully Completed" << endl;
                    cout << endl;
                    cout << "All Accounts After Edition :" << endl;
                    displayAllAccounts(users);
                    return;
                }
                else {
                    cout << "Username " << newUsername << " Is Already Taken" << endl;
                    return;
                }
            }
        }
        cout << "Account With " << Username << " Username Is Not Found" << endl;
        break;
    case 2:
        cout << "Enter Username To Edit Its Password" << endl;
        cin >> Username;
        for (int i = 0; i < users.size(); i++) {
            if (Username == users[i].username) {
                cout << "Enter a New Value Of The Password " << endl;
                cin >> newPassword;
                users[i].setPassword(newPassword);
                cout << "The Edition Is successfully Completed" << endl;
                cout << endl;
                cout << "All Accounts After Edition :" << endl;
                displayAllAccounts(users);
                return;
            }
        }
        cout << "Account With " << Username << " Username Is Not Found" << endl;
        break;
    case 3:
        cout << "Enter Username To Edit Its Email" << endl;
        cin >> Username;
        for (int i = 0; i < users.size(); i++) {
            if (Username == users[i].username) {
                cout << "Enter a New Value Of The Email " << endl;
                cin >> newEmail;
                users[i].setEmail(newEmail);
                cout << "The Edition Is successfully Completed" << endl;
                cout << endl;
                cout << "All Accounts After Edition :" << endl;
                displayAllAccounts(users);
                return;
            }
        }
        cout << "Account With " << Username << " Username Is Not Found" << endl;
        break;
    default:
        cout << "Invalid Input" << endl;

        break;
    }
}


void Admin::suspendAccount(vector<User>& users, string& username) {

    bool accountFound = false;
    for (int i = 0; i < users.size(); i++) {
        if (username == users[i].username) {
            users[i].setSuspended(users[i], true);
            accountFound = true;
            cout << "Account " << username << " Is Susbended Successfully " << endl;
            break;
        }
    }
    if (!accountFound) {
        cout << "Account With Username " << username << " Not Found." << endl;
    }
}

void Admin::reactivateAccount(vector<User>& users, string& username) {
    for (int i = 0; i < users.size(); i++) {
        if (username == users[i].username) {
            if (users[i].getSuspended()) {
                users[i].setSuspended(users[i], false);
                cout << "Account " << username << " Is Reactivated Successfully " << endl;
                return;
            }
            else {
                cout << "Account " << username << " is already active." << endl;
                return;
            }
        }
    }
    cout << "Account With Username " << username << " Not Found." << endl;
}
