#include "account.h"

Account::Account(string first_name, string last_name, int id) : first_name_(first_name), last_name_(last_name), id_(id)
{
    for(int i = 0; i < 8; i++)
    {
        fund_arr_[i].balance_ = 0;
    }
    fund_arr_[0].fund_name_ = "Money Market";
    fund_arr_[1].fund_name_ = "Prime Money Market";
    fund_arr_[2].fund_name_ = "Long-Term Bond";
    fund_arr_[3].fund_name_ = "Short-Term Bond";
    fund_arr_[4].fund_name_ = "500 Index Fund";
    fund_arr_[5].fund_name_ = "Capital Value Fund";
    fund_arr_[6].fund_name_ = "Growth Equity Fund";
    fund_arr_[7].fund_name_ = "Growth Index Fund";
}

string Account::first_name() const
{
    return first_name_;
}

string Account::last_name() const
{
    return last_name_;
}

int Account::id() const
{
    return id_;
}