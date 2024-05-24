#pragma once
#include "User.h"
#include <string>
#include<queue>
#include<string>
#include<vector>
#include<map>


using namespace std;
class Admin {
public:
    string username;
    string password;
    bool suspended;

public:
    bool isAdmin(string username, string password);
    Admin loginAdmin(int& nextChoice);
    string getHidePassword();
    void viewUsersHistory(vector<User> users, map<string, queue<transactions>> userTransactions);
    void adjustUserBalance(vector<User>& users);
    void printAllTransactions(vector<User>& users, queue<transactions>& allTransactions);
    string getUsername();
    string getPassword();
    void setUsername(string&);
    void setPassword(string&);
    bool check_unique_username(vector<User>&, string&);
    void displayAllAccounts(vector<User>&);
    void addAccount(vector<User>&, string&, string&, string&);
    void deleteAccount(vector<User>&, string&);
    void editAccount(vector<User>&);
    void suspendAccount(vector<User>&, string&);
    void reactivateAccount(vector<User>& users, string& username);

};
