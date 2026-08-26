#include "c_api.h"

#include "func.h"
#include "security.h"

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
