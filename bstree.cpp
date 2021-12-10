#include "bstree.h"


BSTree::BSTree() : root_(nullptr)
{
}

BSTree::~BSTree()
{
    cout << "FINAL BALANCES:" << endl;
    Display(root_);
    root_ = Empty(root_);
}
// WORKS
bool BSTree::Insert(Account* account)
{
    int id = account->id();

    if(root_ == nullptr)
    {
        root_ = new Node;
        root_->p_acct = account;
        root_->left = root_->right = nullptr;
        //cout << "Account ID " << id << " inserted" << endl;
        return true;
    }
    
    Node* prev = nullptr;
    Node* temp = root_;
    while(temp != nullptr)
    {
        if(temp->p_acct->id() > id)
        {
            prev = temp;
            temp = temp->left;
        }
        else if(temp->p_acct->id() < id)
        {
            prev = temp;
            temp = temp->right;
        }
        else
        {
            //cerr << "Error: Duplicate Account ID" << endl;
            return false;
        }
    }
    if(prev->p_acct->id() > id)
    {
        prev->left = new Node;
        prev->left->p_acct = account;
        prev->left->right = prev->left->left = nullptr;
        //cout << "Account ID " << id << " inserted" << endl;
        return true;
    }
    else 
    {
        prev->right = new Node;
        prev->right->p_acct = account;
        prev->right->right = prev->right->left = nullptr;
        //cout << "Account ID " << id << " inserted" << endl;
        return true;
    }
    return false;

}
// WORKS
bool BSTree::Retrieve(const int& id, Account*& p_account) const
{
    if(isEmpty())
    {
        p_account = nullptr;
        return false;
    }
    // look through tree for correct id based on < >
    Node* temp = root_;

    while(temp != nullptr)
    {
        if(temp->p_acct->id() == id)
        {
            p_account = temp->p_acct;
            return true;
        }
        else if(temp->p_acct->id() < id)
        {
            temp = temp->right;
        }
        else if(temp->p_acct->id() > id)
        {
            temp = temp->left;
        }
    }
    return false;
}
// WORKS
void BSTree::Display(Node* node)
{
    if(node == nullptr)
    {
        return;
    }
    Display(node->left);
    cout << node->p_acct->first_name() << " " << node->p_acct->last_name() << " Account ID: " << node->p_acct->id() << endl;
    for(int i = 0; i < 8; i++)
    {
        cout << "   " << node->p_acct->fund_arr_[i].fund_name_ << ": $" << node->p_acct->fund_arr_[i].balance_ << endl;
    }
    cout << endl;
    Display(node->right);
}
// WORKS
Node* BSTree::Empty(Node* node)
{
    // list already empty
    if(node == nullptr)
    {
        return nullptr;
    }
    // traverse through list and delete
    Empty(node->right);
    Empty(node->left);
    delete node->p_acct;
    node->p_acct = nullptr;
    delete node;
    node = nullptr;
    return nullptr;
}
// WORKS
bool BSTree::isEmpty() const
{
    if(root_ == nullptr)
    {
        return true;
    }
    return false;
}