#include<iostream>
#include<math.h>
#include "sqlite3.h"

using namespace std;
void connection();

enum AccountType
{
    savings_account,
    current_account
};
enum LoanType
{
    gold_loan,
    car_loan,
    house_loan,
    education_loan
};
sqlite3 *db;
sqlite3_stmt *stmt;
int result;
string query;

class Printer
{
};

class DepositAccount
{
    int account_number;
    int CIF;
    double amount;
    string type;
    int generateAccountNumber();//RANDOM
public:
    void createAccount();
    void depositMoney();//.UPDATE
    void debitMoney();//.UPDATE
    void showTransactions();
    double getAmount(int);//.SELECT
    void getHolderName();
    void getAccountType();//.SELECT
};

class LoanAccount
{
    double loan_amount;
    double loan_paid;
    string type;
    int account_no;
    int period;
    void generateLoanAccountNumber(){
        
    }
    double getInterestRate(){
        if (type=="GoldLoan"){
            return 0.07;// rate=7%
        }
        else if(type=="PropertyLoan"){
            return 0.097;//rete=9.7%
        }
    }
    void generateEMI(int principle){
        double emi_amt,i;
        i=getInterestRate();
        i = i / (12 * 100); // one month interest
    period = period * 12; // one month period
    emi_amt = (principle * i * pow(1 + i, period)) / (pow(1 + i, period) - 1);
    }
public:
    void CreateNewLoan();
    void getLoanType();
    void getTotalLoanAmt();
    void getLoanPaid();
    void getLoanOwed();
};

class Bank
{
    /*int generateCIF()
    {//random
    }*/

public:
    void newCustomer();//GENERATE CIF()
    void searchFromCIF();
    void searchDepositAccount();
    void searchLoanAccount();
    void getLoansByType();
    void loanAccountsDue();
};

int main()
{
    connection();
    DepositAccount a;
    //a.createAccount();
    a.depositMoney();
    //int acc = 4000;
    //a.getAmount(acc);

    sqlite3_close(db);
    return 0;
}

void connection()
{
    if(sqlite3_open("bank.db", &db) == SQLITE_OK)
    {
        result = sqlite3_prepare_v2(db, "CREATE TABLE IF NOT EXISTS DEPOSITACC(ACCNO INT, TYPE TEXT, CIF INT, BALANCE REAL);",-1,&stmt,NULL);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if(result != SQLITE_OK){
            cout<<"Error: "<<sqlite3_errmsg(db)<<endl;
        }
        else
        {
            cout<<"Data Inserted Successfully."<<endl;
        }
    }
}

void DepositAccount::createAccount()
{
    char x;
     cout << "CIF Number:";
        cin >> CIF;
        account_number = 13400;
        cout << "\nSavings Account / Current Account :(S/C) ";
        cin >> x;
        if (x == 'S')
        {
            type = "SavingsAccount";
        }
        else
        {
            type = "CurrentAccount";
        }
        query = "INSERT INTO  DEPOSITACC(ACCNO, TYPE, CIF, BALANCE) VALUES(?,?,?,0);";
        result=sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        sqlite3_bind_int(stmt, 1, account_number);
        sqlite3_bind_text(stmt, 2, type.c_str(), type.length(), SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt,3,CIF);


        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if(result != SQLITE_OK){
            cout<<"Error: "<<sqlite3_errmsg(db)<<endl;
        }
        else
        {
            cout<<"Data Inserted Successfully."<<endl;
        }
}

void DepositAccount::depositMoney()
{
    double depo_amt;
    cout<<"\n Type account no. to deposit money: \n";
    cin>>account_number;
    getchar();

    cout<<"Enter the amount to be deposited: \n";
    cin>>depo_amt;

     double pre_bal = getAmount(account_number);

    query="UPDATE DEPOSITACC SET BALANCE = ? WHERE ACCNO = ?;";
    result=sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    sqlite3_bind_double(stmt,1,(depo_amt+pre_bal));
    sqlite3_bind_int(stmt, 2, account_number);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

        if(result != SQLITE_OK)
        {
            cout<<"FAILED: "<<sqlite3_errmsg(db)<<endl;
        }
        else
        {
            cout<<"Amount deposited Successfully."<<endl;
            getAmount(account_number);
        }

}

void DepositAccount::debitMoney()
{
     double debit_amt;
    cout<<"\n Type account no. to debit money: \n";
    cin>>account_number;
    getchar();

    cout<<"Enter the amount to be debit : \n";
    //cin>>depo_amt;

     double pre_bal = getAmount(account_number);

    query="UPDATE DEPOSITACC SET BALANCE = ? WHERE ACCNO = ?;";
    result=sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    sqlite3_bind_double(stmt,1,(depo_amt+pre_bal));
    sqlite3_bind_int(stmt, 2, account_number);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

        if(result != SQLITE_OK)
        {
            cout<<"FAILED: "<<sqlite3_errmsg(db)<<endl;
        }
        else
        {
            cout<<"Amount deposited Successfully."<<endl;
            getAmount(account_number);
        }
}

double DepositAccount::getAmount(int account_no) //ask the account number in main before calling the get amount function.
{
	query="SELECT * FROM DEPOSITACC WHERE ACCNO = ?;";

	result=sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, account_no);


        if(result != SQLITE_OK)
        {
            cout<<"ERROR: "<<sqlite3_errmsg(db)<<endl;
        }
        else
        {
              while(sqlite3_step(stmt)==SQLITE_ROW)
            {
                          double r = sqlite3_column_double(stmt,3);
            		cout<<"The balance in your account is: "<<r<<endl;
            		return r;
            }
        }
        return 0;
}
