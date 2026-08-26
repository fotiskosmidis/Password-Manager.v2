#ifndef SECURITY_H
#define SECURITY_H

#include <string>

using namespace std;

static string key;

string encryptString(const string& plaintext);
string decryptString(const string& encodedCiphertext);
bool validatePassword(const string& password);
bool createKeyFile(const string& password);


#endif // SECURITY_H