#include "BankApplication.h"

bool isSaving (string id){
    for (auto it : att){
        if (it.ID == id){
            if (it.accountType == "Saving"){
                return true;
            }
        }
    }
    return false;
}

void GetData (){
    int cnt = 0 ;
    Bank.open("BankSystem.txt",ios::in);
    string line;
    int indx = 0 ;
    while(getline(Bank,line)){
        if (line.empty()) continue;
        if (cnt == 0) att[indx].ID = line;
        else if (cnt == 1) att[indx].name = line;
        else if (cnt == 2) att[indx].address = line;
        else if (cnt == 3) att[indx].phone = line;
        else if (cnt == 4) att[indx].accountType = line;
        else if(cnt == 5){
            att[indx].balance = stod(line);
            cnt = 0;
            indx++;
            continue;
        }
        cnt++;
    }
    Bank.close();
}
//------------------------------------------------------------------------------------------
void Client::setAccount (BankAccount& accountPtr){
    account = &accountPtr;
}
//------------------------------------------------------------------------------------------
void BankApplication::addClient(){
    cin.clear();
    cin.sync();
    Bank.open("BankSystem.txt",ios::in);
    int count_lines=0;
    string line;
    string name,address,phone;
    while(getline(Bank,line)){
        count_lines++;
    }
    Bank.close();
    Bank.open("BankSystem.txt",ios::app);
    Bank<<("FCI-");
    Bank<<to_string(count_lines/6+9772);
    Bank<<"\n";
    cout<<"Please enter client name : "<<endl;
    getline(cin,name);
    string temp = name ;
    temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());
    regex checkName ("[a-zA-Z]*");
    while (!regex_match(temp,checkName)){
        cout << "Please enter a valid client name : " << endl ;
        getline(cin,name);
        temp = name;
        temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());
    }
    Bank<<name;
    Bank<<"\n";
    cout<<"Please Enter Client Address : "<<endl;
    getline(cin,address);
    Bank<<address;
    Bank<<"\n";
    cout<<"Please Enter Client Phone : "<<endl;
    cin>>phone;
    regex checkPhoneNumber ("01+([0-1]|[5]){1}+[0-9]{8}");
    while (!regex_match(phone,checkPhoneNumber)){
        cout << "Please enter a valid phone number : " << endl ;
        cin>>phone;
    }
    Bank<<phone;

    Client *clientPtr = new Client(name , address , phone);

    Bank<<"\n";
    cout<<"What Type of Account Do You Like? (1) Basic (2) Saving - Type 1 or 2 "<<endl;
    int choice;
    cin>>choice;
    if(choice==1){
        Bank<<"Basic"<<endl;
    }
    else if(choice==2){
        Bank<<"Saving"<<endl;
    }
    else{
        while (choice != 1 && choice != 2){
            cout<<"Please enter a valid choice : "<<endl;
            cin >> choice ;
        }
    }
    cout<<"Please Enter the Starting Balance :" << endl ;
    double Balance;
    cin>>Balance;

    BankAccount *accountPtr = new BankAccount("FCI-"+to_string(9772+count_lines/6) , Balance);
    clientPtr->setAccount(*accountPtr);

    Bank<<to_string(Balance);
    Bank<<"\n";
    Bank.close();
    int indx = count_lines/6;
    att[indx].ID = "FCI-"+to_string(9772+count_lines/6);
    att[indx].name = name;
    att[indx].address = address;
    att[indx].phone = phone;
    if (choice == 1){
        att[indx].accountType = "Basic";
    }
    else {
        att[indx].accountType = "Saving";
    }
    att[indx].balance = Balance;

    cout<<"An account has been created successfully with ID : "<< "FCI-"<< 9772+count_lines/6<<endl;
}

//------------------------------------------------------------------------------------------
void BankApplication::listAccounts(){
    int cnt = 0 ;
    Bank.open("BankSystem.txt",ios::in);
    string line;
    while(getline(Bank,line)){
        if (line.empty())continue;
        if (cnt == 0) cout << "Client ID : " << line << endl ;
        else if (cnt == 1) cout << "Client Name : " << line << endl ;
        else if (cnt == 2) cout << "Client Address : " << line << endl ;
        else if (cnt == 3) cout << "Client Phone Number : " << line << endl ;
        else if (cnt == 4) cout << "Type of account : " << line << endl;
        else if (cnt == 5) {cout<<"Current Balance : " << line << endl;
        cout << "-------------------------------------------------------------------" << endl ;
        cnt = 0; continue;
        }
        cnt++;
    }
    Bank.close();
}
//------------------------------------------------------------------------------------------
void BankApplication::ClearAccounts(){
    Bank.open("BankSystem.txt",ofstream::out | ofstream::trunc);
    Bank.close();
}
//------------------------------------------------------------------------------------------
string BankApplication::GetBalance(string s){
    int cnt=0;
    string line;
    for (auto it : att){
        if (it.ID == s){
            return to_string(it.balance);
        }
    }
    cout<<"Sorry,Invalid ID"<<endl;
    return "0";
}
//------------------------------------------------------------------------------------------
void BankApplication::SetBalance(string id,string new_balance){
    fstream clone,file;
    clone.open("clonefile.txt",ios::out);
    file.open("BankSystem.txt",ios::in);
    int cnt=0;
    string line;
    bool found=false;// if the id was found
    while(getline(file,line)){
        if(cnt%6==0){
            if(line==id){
                found=true;
            }
        }
        if(cnt%6==5 && found){
            clone<<new_balance<<endl;
            found=false;
        }
        else{
            clone<<line<<endl;
        }
        cnt++;
    }
    // overwrite needed
    file.close();
    clone.close();
    clone.open("clonefile.txt",ios::in);
    file.open("BankSystem.txt",ios::out);
    ClearAccounts();
    while(getline(clone,line)){
        file<<line<<endl;
    }
    file.close();
    clone.close();
}
//------------------------------------------------------------------------------------------
BankAccount::BankAccount(){
    accountID="";
    currentBalance=0;
}
//------------------------------------------------------------------------------------------
BankAccount::BankAccount(string s,double d){
    accountID=s;
    currentBalance=d;
}
//------------------------------------------------------------------------------------------
string BankAccount::GetID(){
    return accountID;
}
//------------------------------------------------------------------------------------------
double BankAccount::GetCurrentBalance(){
    return currentBalance;
}
//------------------------------------------------------------------------------------------
void BankAccount::SetID(string s){
    accountID=s;
}
//------------------------------------------------------------------------------------------
void BankAccount::SetCurrentBalance(double d){
    currentBalance=d;
}
//------------------------------------------------------------------------------------------
void BankAccount::withdraw(string ID){
    BankApplication obj;
    string balance=obj.GetBalance(ID);
    BankAccount BA(ID,stod(balance));
    // basic account , saving account
    if(BA.currentBalance == 0){
        cout<<"Your current Balance is 0 , You can't withdraw money "<<endl;
        return;
    }
    double amount;
    cout<<"Please enter the amount you want to withdraw : "<<endl;
    cin>>amount;
    if(amount<=BA.currentBalance){
        BA.currentBalance-=amount;
        cout<<"Your new Current Balance : "<<BA.currentBalance<<endl;
        obj.SetBalance(ID,to_string(BA.currentBalance));
    }
    else{
        cout<<"Sorry , you are not allowed to withdraw this amount"<<endl;
    }
}
//------------------------------------------------------------------------------------------
void BankAccount::deposit(string ID){
    BankApplication obj;
    string balance=obj.GetBalance(ID);

    BankAccount BA(ID,stod(balance));
    // basic account , saving account
    double amount;
    cout<<"Please enter the amount you want to deposit : "<<endl;
    cin>>amount;
    BA.currentBalance+=amount;
    cout<<"Your new Balance : "<<BA.currentBalance<<endl;
    obj.SetBalance(ID,to_string(BA.currentBalance));
}
//------------------------------------------------------------------------------------------
SavingBankAccount::SavingBankAccount (string ID, double amount, double minBalance):BankAccount(ID,amount){
    minimumBalance=minBalance;
}
//------------------------------------------------------------------------------------------
string SavingBankAccount::GetID() {
    return accountID;
}
//------------------------------------------------------------------------------------------
double SavingBankAccount::GetMinBalance() {
    return minimumBalance;
}
//------------------------------------------------------------------------------------------
void SavingBankAccount::SetID(string s) {
    accountID=s;
}
//------------------------------------------------------------------------------------------
void SavingBankAccount::SetMinBalance(double d) {
    minimumBalance=d;
}
//------------------------------------------------------------------------------------------
double SavingBankAccount::GetCurrentBalance() {
    return currentBalance;
}
//------------------------------------------------------------------------------------------
void SavingBankAccount::SetBalance(double amount) {
    currentBalance=amount;
}
//------------------------------------------------------------------------------------------
void SavingBankAccount::withdraw(string ID) {
    BankApplication obj;
    string balance=obj.GetBalance(ID);
    if (balance == "-1"){
        return ;
    }
    SavingBankAccount BA(ID,stod(balance),1000);
    double minBalance=BA.GetMinBalance();
    if(BA.currentBalance<=minBalance){
        cout<<"Sorry, you can't withdraw money, your current balance is less than the minimum balance"<<endl;
        return;
    }
    double amount;
    cout<<"Please enter the amount you want to withdraw : "<<endl;
    cin>>amount;
    if(amount<=BA.currentBalance && (BA.currentBalance-amount) >= minBalance){
        BA.currentBalance-=amount;
        cout<<"Your new Current Balance : "<<BA.currentBalance<<endl;
        obj.SetBalance(ID,to_string(BA.currentBalance));
    }
    else{
        cout<<"Sorry , you are not allowed to withdraw this amount"<<endl;
    }
}
//------------------------------------------------------------------------------------------
void SavingBankAccount::deposit(string ID) {
    BankApplication obj;
    string balance = obj.GetBalance(ID);
    SavingBankAccount BA(ID, stod(balance), 1000);
    double amount;
    cout << "Please enter the amount you want to deposit : " << endl;
    cin >> amount;
    if (amount < 100) {
        cout << "Sorry, you can't deposit less than 100" << endl;
        return;
    } else {
        BA.currentBalance += amount;
        cout << "Your new Balance : " << BA.currentBalance << endl;
        obj.SetBalance(ID, to_string(BA.currentBalance));
    }
}
//------------------------------------------------------------------------------------------
int main() {
    GetData();
    int choice;
    BankApplication obj;
    BankAccount obj2;
    SavingBankAccount obj3;
    while (true) {
        cout << "Welcome to FCAI Banking Application" << endl;
        cout << "1. Create a New Account" << endl << "2. List Clients and Accounts" << endl
            << "3. Withdraw Money" << endl << "4. Deposit Money" << endl << "5. Exit" << endl;
        cout << "Please choose the operation you want to perform: " << endl;
        cin >> choice;
        while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5) {
            cout << "Please enter a valid choice" << endl;
            cin >> choice;
        }
        switch (choice) {
            case 1:
                obj.addClient();
                break;
            case 2:
                obj.listAccounts();
                break;
            case 3: {
                string ID;
                cout << "Please Enter client ID: " << endl;
                cin >> ID;
                if (isSaving(ID)) {
                    obj3.withdraw(ID);
                }
                else {
                    obj2.withdraw(ID);
                }
                break;
            }
            case 4: {
                string ID;
                cout << "Please Enter client ID: " << endl;
                cin >> ID;
                if (isSaving(ID)) {
                    obj3.deposit(ID);
                }
                else {
                    obj2.deposit(ID);
                }
                break;
            }
            case 5:
                exit(1);
                break;
        }
    }
}
