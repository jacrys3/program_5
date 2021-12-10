#ifndef ACCOUNT_H_
#define ACCOUNT_H_
#include <string>
#include <vector>
#include <iostream>
using namespace std;

struct Fund
{
    int balance_;
    vector<string> fund_history_;
    string fund_name_;
};

class Account
{
public:
    
    Account(string first_name, string last_name, int id);

    string first_name() const;
    string last_name() const;
    int id() const;
    
    Fund fund_arr_[8];
    
private:
    string first_name_;
    string last_name_;
    int id_;
};

#endif