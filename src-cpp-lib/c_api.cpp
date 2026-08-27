#include "c_api.h"

#include "func.h"
#include "security.h"
#include "email.h"
#include "app_paths.h"

#include <cstdlib>
#include <cstring>
#include <exception>
#include <string>

namespace
{
int isValidString(const char* value)
{
	return value != nullptr ? 1 : 0;
}

char* copyString(const std::string& value)
{
	char* result = static_cast<char*>(std::malloc(value.size() + 1));
	if (result == nullptr)
		return nullptr;

	std::memcpy(result, value.data(), value.size());
	result[value.size()] = '\0';
	return result;
}

Email* findEmail(const char* email_address)
{
	if (!isValidString(email_address))
		return nullptr;

	for (Email& email : emails)
	{
		if (email.getEmail() == email_address)
			return &email;
	}
	return nullptr;
}

Account* findAccount(const char* email_address, int account_id)
{
	Email* email = findEmail(email_address);
	if (email == nullptr)
		return nullptr;

	for (Account& account : email->getAccountsArray())
	{
		if (account.getId() == account_id)
			return &account;
	}
	return nullptr;
}
}

int pm_create_key_file(const char* password)
{
	if (!isValidString(password))
		return -1;

	try
	{
		return createKeyFile(password) ? 1 : 0;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

int pm_key_file_exists(void)
{
	try
	{
		return std::filesystem::is_regular_file(keyFilePath()) ? 1 : 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int pm_validate_password(const char* password)
{
	if (!isValidString(password))
		return -1;

	try
	{
		return validatePassword(password) ? 1 : 0;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

int pm_load_data(void)
{
	try
	{
		loadData();
		return 0;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

int pm_save_data(void)
{
	try
	{
		saveData();
		return 0;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

int pm_add_email(const char* email_address)
{
	if (!isValidString(email_address))
		return -1;

	try
	{
		addEmail(email_address);
		return 0;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

int pm_remove_email(const char* email_address)
{
	if (!isValidString(email_address))
		return -1;

	try
	{
		removeEmail(email_address);
		return 0;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

int pm_get_email_count(void)
{
	return static_cast<int>(emails.size());
}

char* pm_get_email_address(int index)
{
	if (index < 0 || static_cast<size_t>(index) >= emails.size())
		return nullptr;
	return copyString(emails[static_cast<size_t>(index)].getEmail());
}

int pm_add_account(const char* email_address, const char* company,
	const char* password, const char* name)
{
	if (!isValidString(company) || !isValidString(password) || !isValidString(name))
		return -1;

	try
	{
		Email* email = findEmail(email_address);
		if (email == nullptr)
			return -1;

		Account account(company, password, name);
		int account_id = account.getId();
		email->addAccount(account);
		return account_id;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

int pm_remove_account(const char* email_address, int account_id)
{
	try
	{
		Email* email = findEmail(email_address);
		if (email == nullptr)
			return -1;
		email->removeAccount(account_id);
		return 0;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

int pm_get_account_count(const char* email_address)
{
	Email* email = findEmail(email_address);
	return email == nullptr ? -1 : static_cast<int>(email->getAccountsArray().size());
}

int pm_get_account_id(const char* email_address, int index)
{
	Email* email = findEmail(email_address);
	if (email == nullptr || index < 0)
		return -1;

	std::vector<Account> accounts = email->getAccountsArray();
	if (static_cast<size_t>(index) >= accounts.size())
		return -1;
	return accounts[static_cast<size_t>(index)].getId();
}

char* pm_get_account_company(const char* email_address, int account_id)
{
	Account* account = findAccount(email_address, account_id);
	return account == nullptr ? nullptr : copyString(account->getCompany());
}

char* pm_get_account_password(const char* email_address, int account_id)
{
	Account* account = findAccount(email_address, account_id);
	return account == nullptr ? nullptr : copyString(account->getPassword());
}

char* pm_get_account_name(const char* email_address, int account_id)
{
	Account* account = findAccount(email_address, account_id);
	return account == nullptr ? nullptr : copyString(account->getName());
}

int pm_set_account_id(const char* email_address, int account_id, int value)
{
	Account* account = findAccount(email_address, account_id);
	if (account == nullptr)
		return -1;
	account->setId(value);
	return 0;
}

int pm_set_account_company(const char* email_address, int account_id, const char* value)
{
	Account* account = findAccount(email_address, account_id);
	if (account == nullptr || !isValidString(value))
		return -1;
	account->setCompany(value);
	return 0;
}

int pm_set_account_password(const char* email_address, int account_id, const char* value)
{
	Account* account = findAccount(email_address, account_id);
	if (account == nullptr || !isValidString(value))
		return -1;
	account->setPassword(value);
	return 0;
}

int pm_set_account_name(const char* email_address, int account_id, const char* value)
{
	Account* account = findAccount(email_address, account_id);
	if (account == nullptr || !isValidString(value))
		return -1;
	account->setName(value);
	return 0;
}

char* pm_encrypt_string(const char* plaintext)
{
	if (!isValidString(plaintext))
		return nullptr;

	try
	{
		return copyString(encryptString(plaintext));
	}
	catch (const std::exception&)
	{
		return nullptr;
	}
}

char* pm_decrypt_string(const char* ciphertext)
{
	if (!isValidString(ciphertext))
		return nullptr;

	try
	{
		return copyString(decryptString(ciphertext));
	}
	catch (const std::exception&)
	{
		return nullptr;
	}
}

void pm_string_free(char* value)
{
	std::free(value);
}
