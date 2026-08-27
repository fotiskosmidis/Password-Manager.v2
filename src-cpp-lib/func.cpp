#include "func.h"
#include "app_paths.h"

#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>

#include "security.h"

// Download the nlohmann/json library if not already installed
#include <nlohmann/json.hpp>

using json = nlohmann::json;

vector<int> usedIds;
vector<Email> emails;

void loadData()
{
        const filesystem::path dataPath = dataFilePath();
        ifstream file(dataPath);

        if (!file.is_open())
        {
                cerr << "Could not open " << dataPath << '\n';
                return;
        }

        json data;
        file >> data;

        emails.clear();
        usedIds.clear();

        for (const auto& emailData : data)
        {
                addEmail(emailData.at("emailAddress").get<string>());

                Email& email = emails.back();

                for (const auto& accountData : emailData.at("accounts"))
                {
                        string encrypted = accountData.at("password").get<string>();
                        string dectypted = decryptString(encrypted);
                        Account account(accountData.at("company").get<string>(), dectypted, accountData.at("username").get<string>());
                        account.setId(accountData.at("id").get<int>());
                        email.addAccount(account);
                }
        }
}

void saveData()
{
        const filesystem::path dataPath = dataFilePath();
        json data;

        for (const auto& email : emails)
        {
                json emailData;
                emailData["emailAddress"] = email.getEmail();

                json accountsData = json::array();
                for (const auto& account : email.getAccountsArray())
                {
                        string encrypted = encryptString(account.getPassword());
                        
                        json accountData;
                        accountData["id"] = account.getId();
                        accountData["company"] = account.getCompany();
                        accountData["username"] = account.getName();
                        accountData["password"] = encrypted;
                        accountsData.push_back(accountData);
                }

                emailData["accounts"] = accountsData;
                data.push_back(emailData);
        }

        ofstream file(dataPath);
        if (!file.is_open())
        {
                cerr << "Could not open " << dataPath << " for writing\n";
                return;
        }

        file << data.dump(4); // Pretty print with 4 spaces indentation
}

int generateId()
{
        for (int i = 1; i < 10000; i++)
        {
                int flag = 0;
                for (auto it = usedIds.begin(); it != usedIds.end(); ++it)
                {
                        if (*it == i)
                        {
                                flag = 1;
                                break;
                        }
                }
                if (flag == 0)
                {
                        usedIds.push_back(i);
                        return i;
                }
        }
        return -1; // Return -1 if no available ID is found
}

void addEmail(const string emailAddress)
{
        Email newEmail(emailAddress);
        emails.push_back(newEmail);
}

void removeEmail(const string emailAddress)
{
        for (auto it = emails.begin(); it != emails.end(); ++it)
        {
                if (it->getEmail() == emailAddress)
                {
                        emails.erase(it);
                        break;
                }
        }
}