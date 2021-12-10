#ifndef BSTREE_H_
#define BSTREE_H_
#include "account.h"
#include <iostream>

struct Node  
{  
    Account* p_acct; 
    Node* right; 
    Node* left; 
}; 

class BSTree
{ 
public: 
    BSTree(); 
    ~BSTree();

    bool Insert(Account* account); 
    bool Retrieve(const int& id, Account*& p_account) const; 
    void Display(Node* node);  
    Node* Empty(Node* node);  
    bool isEmpty() const;  
    
private: 
    Node* root_;  
}; 
#endif