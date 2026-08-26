#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

using namespace std;

// define the class account
class Account
{
private:
        // name sometimes can be empty
        int id;
        string company;
        string password;
        string name;
public:
        Account(string company, string password);
        Account(string company, string password, string name);
        
        // getter functions
        int getId() const;
        string getCompany() const;
        string getPassword() const;
        string getName() const;

        // setter functions
        void setId(int id);
        void setCompany(string company);
        void setPassword(string password);
        void setName(string name);
};
#endif // ACCOUNT_H