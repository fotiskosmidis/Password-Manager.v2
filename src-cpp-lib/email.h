#ifndef EMAIL_H
#define EMAIL_H

#include <string>
#include <vector>

#include "account.h"

using namespace std;

class Email
{
private:
        string email;
        vector<Account> accounts;
public:
        Email(string email);
        
        // add account to the vector
        void addAccount(const Account& account);
        // remove account from the vector based on id
        void removeAccount(int id);

        // getter
        string getEmail() const;
        vector<Account> getAccountsArray() const;
};

#endif // EMAIL_H