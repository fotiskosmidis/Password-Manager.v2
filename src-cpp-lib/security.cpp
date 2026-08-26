#include "security.h"
#include "app_paths.h"

// Libraries for hashing, encryption, error handling, and random number generation
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
// Include the nlohmann/json library for JSON handling
#include <nlohmann/json.hpp>

#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

namespace
{
string sha256Hex(const string& value)
{
	unsigned char digest[SHA256_DIGEST_LENGTH];
	SHA256(reinterpret_cast<const unsigned char*>(value.data()), value.size(), digest);

	ostringstream result;
	result << hex << setfill('0');
	for (unsigned char byte : digest)
		result << setw(2) << static_cast<unsigned int>(byte);
	return result.str();
}

vector<unsigned char> hexToBytes(const string& value)
{
	if (value.size() % 2 != 0)
		throw invalid_argument("Invalid hexadecimal value");

	vector<unsigned char> result;
	result.reserve(value.size() / 2);
	for (size_t index = 0; index < value.size(); index += 2)
	{
		unsigned int byte;
		istringstream parser(value.substr(index, 2));
		parser >> hex >> byte;
		if (parser.fail())
			throw invalid_argument("Invalid hexadecimal value");
		result.push_back(static_cast<unsigned char>(byte));
	}
	return result;
}

string bytesToHex(const unsigned char* bytes, size_t length)
{
	ostringstream result;
	result << hex << setfill('0');
	for (size_t index = 0; index < length; ++index)
		result << setw(2) << static_cast<unsigned int>(bytes[index]);
	return result.str();
}

vector<unsigned char> encryptionKey(const string& password)
{
	// The password hash is mixed with the password before the final SHA-256.
	const string passwordHash = sha256Hex(password);
	const string mixed = passwordHash + ":" + password + ":vault-key-v1";
	return hexToBytes(sha256Hex(mixed));
}
}

bool createKeyFile(const string& password)
{
	const filesystem::path keyPath = keyFilePath();
	ifstream existingFile(keyPath);
	if (existingFile.good())
		return false;

	ofstream file(keyPath, ios::out | ios::trunc);
	if (!file.is_open())
		return false;

	nlohmann::json data;
	data["hash"] = sha256Hex(password);
	file << data.dump(4);
	return file.good();
}

string encryptString(const string& plaintext)
{
	vector<unsigned char> iv(EVP_CIPHER_iv_length(EVP_aes_256_cbc()));
	if (RAND_bytes(iv.data(), static_cast<int>(iv.size())) != 1)
		throw runtime_error("Could not generate encryption IV");

	vector<unsigned char> ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH);
	vector<unsigned char> aesKey = encryptionKey(key);
	EVP_CIPHER_CTX* context = EVP_CIPHER_CTX_new();
	if (context == nullptr)
		throw runtime_error("Could not create encryption context");

	int encryptedLength = 0;
	int finalLength = 0;
	const bool initialized = EVP_EncryptInit_ex(context, EVP_aes_256_cbc(), nullptr, aesKey.data(), iv.data()) == 1;
	const bool updated = initialized && EVP_EncryptUpdate(context, ciphertext.data(), &encryptedLength,
		reinterpret_cast<const unsigned char*>(plaintext.data()), static_cast<int>(plaintext.size())) == 1;
	const bool finalized = updated && EVP_EncryptFinal_ex(context, ciphertext.data() + encryptedLength, &finalLength) == 1;
	EVP_CIPHER_CTX_free(context);
	if (!finalized)
		throw runtime_error("AES encryption failed");

	ciphertext.resize(encryptedLength + finalLength);
	string result = bytesToHex(iv.data(), iv.size());
	result += bytesToHex(ciphertext.data(), ciphertext.size());
	return result;
}

string decryptString(const string& encodedCiphertext)
{
	vector<unsigned char> input = hexToBytes(encodedCiphertext);
	const size_t ivLength = EVP_CIPHER_iv_length(EVP_aes_256_cbc());
	if (input.size() <= ivLength)
		throw invalid_argument("Ciphertext is too short");

	vector<unsigned char> plaintext(input.size() + EVP_MAX_BLOCK_LENGTH);
	vector<unsigned char> aesKey = encryptionKey(key);
	EVP_CIPHER_CTX* context = EVP_CIPHER_CTX_new();
	if (context == nullptr)
		throw runtime_error("Could not create decryption context");

	int plaintextLength = 0;
	int finalLength = 0;
	const bool initialized = EVP_DecryptInit_ex(context, EVP_aes_256_cbc(), nullptr, aesKey.data(), input.data()) == 1;
	const bool updated = initialized && EVP_DecryptUpdate(context, plaintext.data(), &plaintextLength,
		input.data() + ivLength, static_cast<int>(input.size() - ivLength)) == 1;
	const bool finalized = updated && EVP_DecryptFinal_ex(context, plaintext.data() + plaintextLength, &finalLength) == 1;
	EVP_CIPHER_CTX_free(context);
	if (!finalized)
		throw runtime_error("AES decryption failed");

	return string(reinterpret_cast<const char*>(plaintext.data()), plaintextLength + finalLength);
}

bool validatePassword(const string& password)
{
	key.clear();
	ifstream file(keyFilePath());
	if (!file.is_open())
		return false;

	try
	{
		nlohmann::json data;
		file >> data;
		const string savedHash = data.at("hash").get<string>();
		const string suppliedHash = sha256Hex(password);
		const bool valid = savedHash.size() == suppliedHash.size() &&
			CRYPTO_memcmp(savedHash.data(), suppliedHash.data(), suppliedHash.size()) == 0;
		if (valid)
			key = password;
		return valid;
	}
	catch (const exception&)
	{
		return false;
	}
}



