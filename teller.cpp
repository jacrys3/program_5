#include "teller.h"

Teller::Teller()
{
}

bool Teller::OpenAccount(int id, string last_name, string first_name)
{
    Account* temp = new Account(first_name, last_name, id);
    if(acct_database_.Insert(temp))
    {
        return true;
    }
    delete temp;
    cerr << "ERROR: Account ID " << id << " is already open. Open account refused." << endl;
    return false;
}

bool Teller::Withdraw(int id, int fund, int amount)
{
    Account* acct; 
    if(acct_database_.Retrieve(id, acct))
    {
        if(acct->fund_arr_[fund].balance_ >= amount)
        {
            acct->fund_arr_[fund].balance_ -= amount;
            acct->fund_arr_[fund].fund_history_.push_back("W " + to_string(id) + " " + to_string(fund) + " " + to_string(amount));
            return true;
        }
        else if(fund == 0 || fund == 1)
        {
            if(CheckLink1(amount, fund, acct))
            {
                acct->fund_arr_[fund].fund_history_.push_back("W " + to_string(id) + " " + to_string(fund) + " " + to_string(acct->fund_arr_[fund].balance_));
                amount -= acct->fund_arr_[fund].balance_;
                acct->fund_arr_[fund].balance_ = 0;

                int opposite = (fund == 0) ? 1 : 0;
                acct->fund_arr_[opposite].balance_ -= amount;
                acct->fund_arr_[opposite].fund_history_.push_back("W " + to_string(id) + " " + to_string(opposite) + " " + to_string(amount));
                return true;
            }
        }
        else if(fund == 2 || fund == 3)
        {
            if(CheckLink2(amount, fund, acct))
            {
                acct->fund_arr_[fund].fund_history_.push_back("W " + to_string(id) + " " + to_string(fund) + " " + to_string(acct->fund_arr_[fund].balance_));
                amount -= acct->fund_arr_[fund].balance_;
                acct->fund_arr_[fund].balance_ = 0;

                int opposite = (fund == 2) ? 3 : 2;
                acct->fund_arr_[opposite].balance_ -= amount;
                acct->fund_arr_[opposite].fund_history_.push_back("W " + to_string(id) + " " + to_string(opposite) + " " + to_string(amount));
                return true;
            }
        }
        else
        {
            acct->fund_arr_[fund].fund_history_.push_back("W " + to_string(id) + " " + to_string(fund) + " " + to_string(amount) + " (Failed)");
            cerr << "ERROR: Not enough funds to withdraw " << amount << " from " << acct->first_name() 
                << " " << acct->last_name() << " " << acct->fund_arr_[fund].fund_name_ << endl;
            return false;
        }
    }
    acct->fund_arr_[fund].fund_history_.push_back("W " + to_string(id) + " " + to_string(fund) + " " + to_string(amount) + " (Failed)");
    cerr << "ERROR: Account ID " << id << " not found. Withdrawal refused." << endl;
    return false;
}

bool Teller::Deposit(int id, int fund, int amount)
{
    Account* acct;
    if(acct_database_.Retrieve(id, acct))
    {
        acct->fund_arr_[fund].balance_ += amount;
        acct->fund_arr_[fund].fund_history_.push_back("D " + to_string(id) + " " + to_string(fund) + " " + to_string(amount));
        return true;
    }
    acct->fund_arr_[fund].fund_history_.push_back("D " + to_string(id) + " " + to_string(fund) + " " + to_string(amount) + " (Failed)");
    cerr << "ERROR: Account ID " << id << " not found. Deposit refused." << endl;
    return false;
}

bool Teller::Transfer(int id1, int fund1, int id2, int fund2, int amount)
{
    if(id1 != id2)
    {
        Account* acct1;
        Account* acct2;
        if(acct_database_.Retrieve(id1, acct1))
        {
            if(acct_database_.Retrieve(id2, acct2))
            {
                if(acct1->fund_arr_[fund1].balance_ >= amount)
                {
                    acct1->fund_arr_[fund1].balance_ -= amount;
                    acct2->fund_arr_[fund2].balance_ += amount;
                    acct1->fund_arr_[fund1].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                        + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount));
                    acct2->fund_arr_[fund2].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                        + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount));
                    return true;
                }

                else if(fund1 == 0 || fund1 == 1)
                {
                    if(CheckLink1(amount, fund1, acct1))
                    {
                        acct1->fund_arr_[fund1].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                            + to_string(id2) + " " + to_string(fund2) + " " + to_string(acct1->fund_arr_[fund1].balance_));
                        acct2->fund_arr_[fund2].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                            + to_string(id2) + " " + to_string(fund2) + " " + to_string(acct1->fund_arr_[fund1].balance_));
                        
                        acct2->fund_arr_[fund2].balance_ += amount;
                        amount -= acct1->fund_arr_[fund1].balance_;
                        acct1->fund_arr_[fund1].balance_ = 0;

                        int opposite = (fund1 == 0) ? 1 : 0;
                        acct1->fund_arr_[opposite].balance_ -= amount;
                        
                        acct1->fund_arr_[fund1].fund_history_.push_back("T " + to_string(id1) + " " + to_string(opposite) + " "
                            + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount));
                        acct2->fund_arr_[fund2].fund_history_.push_back("T " + to_string(id1) + " " + to_string(opposite) + " "
                            + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount));
                        return true;
                    }
                }
                else if(fund1 == 2 || fund1 == 3)
                {
                    if(CheckLink2(amount, fund1, acct1))
                    {
                        acct1->fund_arr_[fund1].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                            + to_string(id2) + " " + to_string(fund2) + " " + to_string(acct1->fund_arr_[fund1].balance_));
                        acct2->fund_arr_[fund2].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                            + to_string(id2) + " " + to_string(fund2) + " " + to_string(acct1->fund_arr_[fund1].balance_));

                        acct2->fund_arr_[fund2].balance_ += amount;
                        amount -= acct1->fund_arr_[fund1].balance_;
                        acct1->fund_arr_[fund1].balance_ = 0;

                        int opposite = (fund1 == 2) ? 3 : 2;
                        acct1->fund_arr_[opposite].balance_ -= amount;

                        acct1->fund_arr_[fund1].fund_history_.push_back("T " + to_string(id1) + " " + to_string(opposite) + " "
                            + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount));
                        acct2->fund_arr_[fund2].fund_history_.push_back("T " + to_string(id1) + " " + to_string(opposite) + " "
                            + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount));
                        return true;
                    }
                }

                acct1->fund_arr_[fund1].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                    + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount) + " (Failed)");
                acct2->fund_arr_[fund2].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                    + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount) + " (Failed)");
                cerr << "ERROR: Not enough funds to transfer " << amount << " from " << acct1->first_name() 
                    << " " << acct1->last_name() << " " << acct1->fund_arr_[fund1].fund_name_ << " to " << acct2->first_name() 
                    << " " << acct2->last_name() << " " << acct2->fund_arr_[fund2].fund_name_ << endl; 
                return false;
            }
            cerr << "ERROR: Account ID " << id2 << " not found. Transferal refused." << endl;
            return false;
        }
        cerr << "ERROR: Account ID " << id1 << " not found. Transferal refused." << endl;
        return false;
    }
    else
    {
        Account* acct;
        if(acct_database_.Retrieve(id1, acct))
        {
            if(acct->fund_arr_[fund1].balance_ >= amount)
            {
                acct->fund_arr_[fund1].balance_ -= amount;
                acct->fund_arr_[fund2].balance_ += amount;
                acct->fund_arr_[fund1].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                     + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount));
                acct->fund_arr_[fund2].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                     + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount));
                return true;
            }

            else if(fund1 == 0 || fund1 == 1)
            {
                if(CheckLink1(amount, fund1, acct))
                {
                    acct->fund_arr_[fund1].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                        + to_string(id2) + " " + to_string(fund2) + " " + to_string(acct->fund_arr_[fund1].balance_));
                    acct->fund_arr_[fund2].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                        + to_string(id2) + " " + to_string(fund2) + " " + to_string(acct->fund_arr_[fund1].balance_));
                    
                    acct->fund_arr_[fund2].balance_ += amount;
                    amount -= acct->fund_arr_[fund1].balance_;
                    acct->fund_arr_[fund1].balance_ = 0;

                    int opposite = (fund1 == 0) ? 1 : 0;
                    acct->fund_arr_[opposite].balance_ -= amount;
                    
                    acct->fund_arr_[opposite].fund_history_.push_back("T " + to_string(id1) + " " + to_string(opposite) + " "
                        + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount));
                    acct->fund_arr_[fund2].fund_history_.push_back("T " + to_string(id1) + " " + to_string(opposite) + " "
                        + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount));
                    return true;
                }
            }
            else if(fund1 == 2 || fund1 == 3)
            {
                if(CheckLink2(amount, fund1, acct))
                {
                    acct->fund_arr_[fund1].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                        + to_string(id2) + " " + to_string(fund2) + " " + to_string(acct->fund_arr_[fund1].balance_));
                    acct->fund_arr_[fund2].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                        + to_string(id2) + " " + to_string(fund2) + " " + to_string(acct->fund_arr_[fund1].balance_));

                    acct->fund_arr_[fund2].balance_ += amount;
                    amount -= acct->fund_arr_[fund1].balance_;
                    acct->fund_arr_[fund1].balance_ = 0;

                    int opposite = (fund1 == 2) ? 3 : 2;
                    acct->fund_arr_[opposite].balance_ -= amount;

                    acct->fund_arr_[opposite].fund_history_.push_back("T " + to_string(id1) + " " + to_string(opposite) + " "
                        + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount));
                    acct->fund_arr_[fund2].fund_history_.push_back("T " + to_string(id1) + " " + to_string(opposite) + " "
                        + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount));
                    return true;
                }
            }

            acct->fund_arr_[fund1].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                 + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount) + " (Failed)");
            acct->fund_arr_[fund2].fund_history_.push_back("T " + to_string(id1) + " " + to_string(fund1) + " "
                + to_string(id2) + " " + to_string(fund2) + " " + to_string(amount) + " (Failed)");
            cerr << "ERROR: Not enough funds to transfer " << amount << " from " << acct->first_name() 
                << " " << acct->last_name() << " " << acct->fund_arr_[fund1].fund_name_ << " to " << acct->fund_arr_[fund2].fund_name_ << endl; 
            return false;
        }
        cerr << "ERROR: Account ID " << id1 << " not found. Transferal refused." << endl;
        return false;
    }

}

bool Teller::DisplayAccountHistory(int id)
{
    Account* acct;
    if(acct_database_.Retrieve(id, acct))
    {
        cout << "Transaction History for " << acct->first_name() << " " << acct->last_name() 
            << " by fund." << endl;
        for(int i = 0; i < 8; i++)
        {
            if(acct->fund_arr_[i].fund_history_.size() > 0)
            {
                cout << acct->fund_arr_[i].fund_name_ << ": $" << acct->fund_arr_[i].balance_ << endl;
                for(string str : acct->fund_arr_[i].fund_history_)
                {
                    cout << "   " << str << endl;
                }
            }
        }
        cout << endl;
        return true;
    }
    cerr << "ERROR: Account ID " << id << " not found. Account display refused." << endl;
    return false;
}

bool Teller::DisplayFundHistory(int id, int fund)
{
    Account* acct;
    if(acct_database_.Retrieve(id, acct))
    {
        cout << "Transaction History for " << acct->first_name() << " " << acct->last_name() << " "
            << acct->fund_arr_[fund].fund_name_ << ": $" << acct->fund_arr_[fund].balance_ << endl;

        for(string str : acct->fund_arr_[fund].fund_history_)
        {
            cout << "   " << str << endl;
        }
        cout << endl;
        return true;
    }
    cerr << "ERROR: Account ID " << id << " not found. Fund display refused." << endl;
    return false;
}

bool Teller::ReadInTransactions(string file_name)
{
    ifstream in_file;
    in_file.open(file_name);
    if(!in_file.is_open())
    {
        in_file.close();
        return false;
    }
    else
    {
        while(!in_file.eof())
        {
            Transaction transaction;
            in_file >> transaction.type_;
            in_file >> transaction.id_;
            getline(in_file, transaction.info_);
            transactions_to_do_.push(transaction);
        }
        in_file.close();
    }
	return true;
}

void Teller::ExecuteTransactions()
{
    while(transactions_to_do_.size() > 0)
    {
        Transaction trans = transactions_to_do_.front();
        
        if(trans.type_ == "O")
        {
            istringstream ss(trans.info_);
            string last_name = "";
            ss >> last_name;
            
            string first_name = "";
            ss >> first_name;
            
            OpenAccount(trans.id_, last_name, first_name);
        }

        if(trans.type_ == "D")
        {
            istringstream ss(trans.info_);
            int fund = 0;
            ss >> fund;
            
            int amount = 0;
            ss >> amount;
            
            Deposit(trans.id_, fund, amount);
        }

        if(trans.type_ == "W")
        {
            istringstream ss(trans.info_);
            int fund = 0;
            ss >> fund;
            
            int amount = 0;
            ss >> amount;
            
            Withdraw(trans.id_, fund, amount);
        }

        if(trans.type_ == "T")
        {
            istringstream ss(trans.info_);
            int fund1 = 0;
            ss >> fund1;

            int id2 = 0;
            ss >> id2;
            
            int fund2 = 0;
            ss >> fund2;

            int amount = 0;
            ss >> amount;
            
            Transfer(trans.id_, fund1, id2, fund2, amount);
        }

        if(trans.type_ == "F")
        {
            istringstream ss(trans.info_);
            int fund = 0;
            ss >> fund;
            
            DisplayFundHistory(trans.id_, fund);
        }

        if(trans.type_ == "A")
        {
            
            DisplayAccountHistory(trans.id_);
        }

        transactions_to_do_.pop();
    }
}

bool Teller::CheckLink1(int amount, int from, Account* acct)
{
    if(from == 0)
    {
        if(acct->fund_arr_[from].balance_ + acct->fund_arr_[1].balance_ >= amount)
        {
            return true;
        }
        return false;
    }
    else if(from == 1)
    {
        if(acct->fund_arr_[from].balance_ + acct->fund_arr_[0].balance_ >= amount)
        {
            return true;
        }
        return false;
    }
    return false;
}

bool Teller::CheckLink2(int amount, int from, Account* acct)
{
    if(from == 2)
    {
        if(acct->fund_arr_[from].balance_ + acct->fund_arr_[3].balance_ >= amount)
        {
            return true;
        }
        return false;
    }
    else if(from == 3)
    {
        if(acct->fund_arr_[from].balance_ + acct->fund_arr_[2].balance_ >= amount)
        {
            return true;
        }
        return false;
    }
    return false;
}