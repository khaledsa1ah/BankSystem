#ifndef _BANKAPPLICATION_H
#define _BANKAPPLICATION_H
#include<bits/stdc++.h>
using namespace std;
#define endl "\n"

fstream Bank;
class Client;

struct Attributes {
    string ID;
    string name;
    string address;
    string phone;
    string accountType;
    double balance;
};

vector <Attributes> att(500);

class BankApplication{
    public:
        void addClient();
        void listAccounts();
        void ClearAccounts();
        void SetBalance(string id,string b);
        string GetBalance(string s);
        bool isSaving(string id);
};
class BankAccount{
    protected:
        string accountID;
        Client *client = nullptr;
        double currentBalance;
    public:
        BankAccount();
        BankAccount(string s,double d);
        virtual string GetID();
        virtual double GetCurrentBalance();
        virtual void SetID(string s);
        void SetCurrentBalance(double d);
        virtual void withdraw(string ID);
        virtual void deposit(string ID);
};
class SavingBankAccount : public BankAccount{
    protected:
        double minimumBalance;
    public:
        SavingBankAccount():BankAccount(){};
        SavingBankAccount(string s,double amount,double minBalance);
        string GetID();
        double GetCurrentBalance();
        double GetMinBalance();
        void SetID(string s);
        void SetBalance(double amount);
        void SetMinBalance(double minBalance);
        void withdraw(string ID);
        void deposit(string ID);
};

class Client {
    private:
        string name;
        string address;
        string phoneNumber;
        BankAccount *account = nullptr;
    public:
        Client();
        Client(string name , string address , string phoneNumber){
            this->name = name;
            this->address = address;
            this->phoneNumber = phoneNumber;
        }
        ~Client(){
            delete account;
        }
        void setAccount (BankAccount& accountPtr);
};

#endif
