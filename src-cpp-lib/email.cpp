#include "email.h"
#include <string>

using namespace std;

Email::Email(string email)
{
        this->email = email;
}

void Email::addAccount(const Account& account)
{
        accounts.push_back(account);
}

void Email::removeAccount(int id)
{
        for (auto it = accounts.begin(); it != accounts.end(); ++it)
        {
                if (it->getId() == id)
                {
                        accounts.erase(it);
                        break;
                }
        }
}

string Email::getEmail() const
{
        return email;
}

vector<Account>& Email::getAccountsArray()
{
        return accounts;
}

vector<Account> Email::getAccountsArray() const
{
        return accounts;
}