#include "User.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <conio.h>
#include<queue>
#include<map>
#include <ctime> 

using namespace std;
transactions TransactionDateAndTime(transactions newtransaction) {
    //transactions newtransaction;
    time_t currentTime;
    struct tm now;
    time(&currentTime);  // Get the current time
    localtime_s(&now, &currentTime); // Convert to struct tm
    newtransaction.day = now.tm_mday;
    newtransaction.month = now.tm_mon + 1; // Months are 0-based, so add 1
    newtransaction.year = now.tm_year + 1900; // Years are years since 1900
    newtransaction.minute = now.tm_min;

    // Convert hour to 12-hour format and set AM/PM designation
    if (now.tm_hour == 0) {
        newtransaction.hour = 12;
        newtransaction.datetype = "AM";
    }
    else if (now.tm_hour < 12) {
        newtransaction.hour = now.tm_hour;
        newtransaction.datetype = "AM";
    }
    else if (now.tm_hour == 12) {
        newtransaction.hour = 12;
        newtransaction.datetype = "PM";
    }
    else {
        newtransaction.hour = now.tm_hour - 12;
        newtransaction.datetype = "PM";
    }
    return newtransaction;

}
requests RequestDateAndTime(requests newRequest) {
    time_t currentTime;
    struct tm now;
    time(&currentTime);  // Get the current time
    localtime_s(&now, &currentTime); // Convert to struct tm
    newRequest.day = now.tm_mday;
    newRequest.month = now.tm_mon + 1; // Months are 0-based, so add 1
    newRequest.year = now.tm_year + 1900; // Years are years since 1900
    newRequest.minute = now.tm_min;

    // Convert hour to 12-hour format and set AM/PM designation
    if (now.tm_hour == 0) {
        newRequest.hour = 12;
        newRequest.datetype = "AM";
    }
    else if (now.tm_hour < 12) {
        newRequest.hour = now.tm_hour;
        newRequest.datetype = "AM";
    }
    else if (now.tm_hour == 12) {
        newRequest.hour = 12;
        newRequest.datetype = "PM";
    }
    else {
        newRequest.hour = now.tm_hour - 12;
        newRequest.datetype = "PM";
    }
    return newRequest;
}

//vector<User> user;
//queue <requests> allRequests;
//queue<transactions> allTransactions;
//queue<int> balances;
//map<string, queue<requests>> userRequests;
//map<string, queue<transactions>> userTransactions;

map<string, User> getUser;

User::User(string username, string password, string email, double balance, bool suspended) : username(username), password(password), email(email), balance(balance), suspended(suspended) {}

bool User::checkEmailFormat(string email) {
    regex pattern(R"(^[a-zA-Z][a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, pattern);
}
bool User::isPasswordSecure(string password) {
    bool hasUpperCase = false;
    bool hasLowerCase = false;
    bool hasDigit = false;

    if (password.length() < 8) {
        return false;
    }

    for (char ch : password) {
        if (std::isupper(ch)) {
            hasUpperCase = true;
        }
        else if (std::islower(ch)) {
            hasLowerCase = true;
        }
        else if (std::isdigit(ch)) {
            hasDigit = true;
        }
    }

    return hasUpperCase && hasLowerCase && hasDigit;
}
string User::getHidePassword() {
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
User User::registerUser(vector<User>& users, int& nextChoice) {
    string newUsername, newPassword, newEmail;
    bool checkUsername = true;
    do {
        cout << "Enter Email : ";
        cin >> newEmail;
        if (!checkEmailFormat(newEmail)) {
            cout << "Invalid email address. Please enter a valid email address." << endl;
        }
    } while (!checkEmailFormat(newEmail));

    do {
        checkUsername = true;
        cout << "Enter username : ";
        cin >> newUsername;

        if (newUsername.length() < 9) {
            cout << "Username must be at least 10 characters long without spaces." << endl;
            checkUsername = false;
        }

        for (const auto& user : users) {
            if (user.username == newUsername) {
                checkUsername = false;
                cout << "Username already exists. Please choose a different one." << endl;
            }
        }

    } while (!checkUsername);

    do {
        cout << "Enter password: ";
        cin >> newPassword;
        if (!isPasswordSecure(newPassword)) {
            cout << "Password is not strong enough. Please try again." << endl;
        }
    } while (!isPasswordSecure(newPassword));

    User newUser(newUsername, newPassword, newEmail, 2000, false);
    users.push_back(newUser);
    cout << "User registration successful." << endl;
    cout << "press 2 to home page.\npress 0 to exit.\n";
    cin >> nextChoice;
    return newUser;
}
User User::loginUser(vector<User>& users, int& nextChoice) {
    string inputUsername, inputPassword;
    do {
        cout << "Enter username: ";
        cin >> inputUsername;
        inputPassword = getHidePassword();

        for (auto& user : users) {
            if (user.username == inputUsername && user.password == inputPassword) {
                cout << "Login successful. Welcome, " << user.username << endl;
                nextChoice = 8;
                return user;
            }
        }
        cout << "Invalid username or password." << endl;
        cout << "press 1 to Login again.\n" << endl;
        cin >> nextChoice;
    } while (nextChoice == 1);
    User u(NULL, NULL, NULL, 2000, false);
    return u;

}
void User::editProfile(vector<User>& users, int& UnextChoice) {
    string currentUsername, currentPassword, newPassword;
    bool exist = false;

    cout << "Enter username: ";
    cin >> currentUsername;
    cout << "Enter your old password: ";
    cin >> currentPassword;

    for (auto& user : users) {
        if (user.username == currentUsername && user.password == currentPassword) {
            exist = true;
            break;
        }
    }

    if (!exist) {
        cout << "You can't edit your password as you entered the wrong password or username." << endl;
        cout << "Press 2 to user home. Press 0 to home page." << endl;
        cin >> UnextChoice;
        return;
    }

    do {
        cout << "Enter new password: ";
        cin >> newPassword;
        if (!isPasswordSecure(newPassword)) {
            cout << "Password is not strong enough. Please try again." << endl;
        }
    } while (!isPasswordSecure(newPassword));

    for (auto& user : users) {
        if (user.username == currentUsername && user.password == currentPassword) {
            user.password = newPassword;
            break;
        }
    }

    cout << "You have successfully changed your password." << endl;
    cout << "Press 2 to user home. Press 0 to home page." << endl;
    cin >> UnextChoice;
}

void User::setUsername(string& Username)
{
    username = Username;
}

void User::setPassword(string& Password)
{
    password = Password;
}
void User::setEmail(string& email)
{
    email = email;
}
bool User::getSuspended() {
    return suspended;
}

void User::setSuspended(User& user, bool isSuspended) {
    user.suspended = isSuspended;
}
User User::getUserByUsername(vector<User>& users, string& username) {
    for (User& user : users) {
        if (user.username == username) {
            return user;
        }
    }
    throw std::runtime_error("User not found with username: " + username);
}


void User::SendMoney(queue<transactions>& allTransactions, vector<User>& users, string Senderusername, queue<int>& balances, map<string, queue<transactions>>& userTransactions) {
    User* sender = NULL;
    User* recipient = NULL;
    bool notover = false;
    transactions newtransaction;


    newtransaction = TransactionDateAndTime(newtransaction);
    //cout << "Enter Your Username: ";
    newtransaction.senderUsername = Senderusername;
    if (getUserByUsername(users, Senderusername).getSuspended()) {
        cout << "Your account is suspended. You cannot perform transactions.\n";
        return;
    }
    cout << "Enter The Recipient Username: ";
    cin >> newtransaction.recipientUsername;
    cout << "Enter the amount of money that you want to send: ";
    cin >> newtransaction.amount;






    for (int i = 0; i < users.size(); i++) {
        if (users[i].username == newtransaction.senderUsername) {
            sender = &users[i];
        }
        else if (users[i].username == newtransaction.recipientUsername) {
            recipient = &users[i];
        }
    }

    if (sender != NULL && recipient != NULL) {

        // Update balances
        if (sender->balance >= newtransaction.amount) {
            notover = true;
        }
        if (notover == true) {
            sender->balance -= newtransaction.amount;
            cout << "You have sent " << newtransaction.amount << " to " << recipient->username << endl;
            cout << "Your balance is now: " << sender->balance << endl;
            recipient->balance += newtransaction.amount;
            balances.push(recipient->balance);

            newtransaction.code = allTransactions.size() + 1;
            allTransactions.push(newtransaction);
            userTransactions[sender->username].push(newtransaction);
            userTransactions[recipient->username].push(newtransaction);

        }
        else {
            cout << "You can't send money over than your balance\n";
        }
    }
    else {
        if (recipient == NULL) {
            cout << "Recipient not found!" << endl;
        }
    }
}


void User::requestMoney(vector<User>& users, string Senderusername, queue <requests>& allRequests, map<string, queue<requests>>& userRequests) {
    requests newRequest;
    User* recipientUsername;
    bool found = false;

    newRequest = RequestDateAndTime(newRequest);
    newRequest.senderUsername = Senderusername;
    if (getUserByUsername(users, Senderusername).getSuspended()) {
        cout << "Your account is suspended. You cannot perform transactions.\n";
        return;
    }
    cout << "Enter the Username of who you want to request from: ";
    cin >> newRequest.recipientUsername;
    cout << "Enter the amount of money that you want to request: ";
    cin >> newRequest.amount;


    for (int i = 0; i < users.size(); i++) {
        if (users[i].username == newRequest.recipientUsername) {
            found = true;
            allRequests.push(newRequest);
            userRequests[newRequest.recipientUsername].push(newRequest);
            break;
        }
    }
    if (found == true) {
        cout << "Your request has been sent\n";
    }
    else {
        cout << "Recipient not found\n";
    }
}
void User::ShowNotification(queue<transactions>& allTransactions, vector<User>& users, User user, queue<int>& balances, map<string, queue<transactions>>& userTransactions) {
    User* currentUser = NULL;
    bool haveNotification = false;
    queue< transactions>tmp;
    for (int i = 0; i < users.size(); i++) {
        if (users[i].username == user.username) {
            currentUser = &users[i];
            break;
        }
    }
    while (!allTransactions.empty()) {
        transactions MyTransaction = allTransactions.front();
        if (MyTransaction.recipientUsername == user.username) {
            haveNotification = true;
            cout << "You have recieved " << MyTransaction.amount << " from " << MyTransaction.senderUsername << " on " << MyTransaction.day << "/" << MyTransaction.month << "/" << MyTransaction.year << " at " << MyTransaction.hour << ":" << MyTransaction.minute << " " << MyTransaction.datetype << endl;
            currentUser->balance = balances.front();
            cout << "Now, Your balance:" << balances.front() << endl;
            balances.pop();
        }
        tmp.push(allTransactions.front());
        allTransactions.pop();
    }
    if (haveNotification == false) {
        cout << "You don't have any notification\n";
    }
    while (!tmp.empty()) {
        allTransactions.push(tmp.front());
        tmp.pop();
    }

}

void User::CheckRequests(queue<transactions>& allTransactions, vector<User>& users, string recipientusername, queue<int>& balances, queue <requests>& allRequests, map<string, queue<requests>>& userRequests, map<string, queue<transactions>>& userTransactions) {
    User* currentUser = NULL;
    int choice;
    queue<requests> userallRequests = userRequests[recipientusername];
    for (int i = 0; i < users.size(); i++) {
        if (users[i].username == recipientusername) {
            currentUser = &users[i];
            break;
        }
    }
    if (!userallRequests.empty()) {
        while (!userallRequests.empty()) {
            requests myRequest = userallRequests.front();
            if (myRequest.recipientUsername == recipientusername) {
                cout << "You have recieved a request from " << myRequest.senderUsername << " on " << myRequest.day << "/" << myRequest.month << "/" << myRequest.year << " at " << myRequest.hour << ":" << myRequest.minute << " " << myRequest.datetype << " and the requested amount " << myRequest.amount << endl;
                cout << "Press 1 to send money and 2 to break" << endl;
                cin >> choice;
                switch (choice) {
                case 1: {
                    SendMoney(allTransactions, users, myRequest.recipientUsername, balances, userTransactions);
                    break;
                }
                case 2: {
                    cout << "you have refused the request\n";
                    break;
                }
                }
            }
            userallRequests.pop();
        }
    }
    else {
        cout << "you don't have any requests\n";
    }
}




void User::viewBalance(User user)
{
    cout << "Current Balance: " << user.balance << '$' << endl;
}

void User::viewTransactions(string username, map<string, queue<transactions>>& userTransactions) {
    if (userTransactions.find(username) == userTransactions.end()) {
        cout << "No transactions found for user: " << username << endl;
        return;
    }

    queue<transactions> userQueue = userTransactions[username];
    cout << "Transaction History:" << endl << endl;
    while (!userQueue.empty()) {
        transactions currentTransaction = userQueue.front();
        cout << "The Code of transaction: " << currentTransaction.code << endl;
        cout << "The sender's username: " << currentTransaction.senderUsername << endl;
        cout << "The recipient's username: " << currentTransaction.recipientUsername << endl;
        cout << "The date of transaction: " << currentTransaction.day << "/" << currentTransaction.month << "/" << currentTransaction.year << endl;
        cout << "The hour of transaction: " << currentTransaction.hour << ":" << currentTransaction.minute << " " << currentTransaction.datetype << endl;

        if (username == currentTransaction.senderUsername) {
            currentTransaction.type = "sent";
            cout << "The type of transaction: " << currentTransaction.type << endl << endl;
        }
        else if (username == currentTransaction.recipientUsername) {
            currentTransaction.type = "recieved";
            cout << "The type of transaction: " << currentTransaction.type << endl << endl;
        }
        userQueue.pop();
    }
}
