#include "account.h"
#include "func.h"
#include <string>

using namespace std;

Account::Account(string company, string password)
{
        this->id = generateId();
        this->company = company; 
        this->password = password;
        this->name = "";
}

Account::Account(string company, string password, string name)
{
        this->id = generateId();
        this->company = company;
        this->password = password;
        this->name = name;
}

int Account::getId() const
{
        return id;
}

string Account::getCompany() const
{
        return company;
}

string Account::getPassword() const
{
        return password;
}

string Account::getName() const
{
        return name;
}

void Account::setCompany(string company)
{
        this->company = company;
}

void Account::setPassword(string password)
{
        this->password = password;
}

void Account::setName(string name)
{
        this->name = name;
}

void Account::setId(int id)
{
        this->id = id;
}