#ifndef TELLER_H_
#define TELLER_H_
#include "bstree.h"
#include "account.h"
#include <string>
#include <queue>
#include <fstream>
#include <iostream>
#include <sstream>

struct Transaction
{
    string type_;
    int id_;
    string info_;
};

class Teller
{

public: 
    Teller();

    bool OpenAccount(int id, string last_name, string first_name);
    bool Withdraw(int id, int fund, int amount);
    bool Deposit(int id, int fund, int amount);
    bool Transfer(int id1, int fund1, int id2, int fund2, int amount);
    bool DisplayAccountHistory(int id);
    bool DisplayFundHistory(int id, int fund);

    bool ReadInTransactions(string file_name);
    void ExecuteTransactions();

private:

    bool CheckLink1(int amount, int from, Account* acct);
    bool CheckLink2(int amount, int from, Account* acct);

    BSTree acct_database_;
    queue<Transaction> transactions_to_do_;
};

#endif