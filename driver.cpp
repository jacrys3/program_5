#include "teller.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{

    // ./jolly_bank hi.txt > bank_trans_out.txt
    // ./jolly_bank hi.txt 2> bank_trans_err.txt

    string file = "";

    if(argc == 2)
    {
        //cout << "File found" << endl;
        file = string(argv[1]);
    }
    else
    {
        cerr << "USAGE: jolly_banker FILE_NAME" << endl;
        return -1;
    }

    Teller teller;
    if(!teller.ReadInTransactions(file))
    {
        cerr << "ERROR: File not found" << endl;
    }

    teller.ExecuteTransactions();
    return 0;
}