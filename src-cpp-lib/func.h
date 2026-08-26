#ifndef FUNC_H
#define FUNC_H

#include <string>
#include <iostream>
#include <vector>

#include "email.h"

using namespace std;

extern vector<int> usedIds; // All the used user ids
extern vector<Email> emails; // All the emails

// File manipulation functions
void loadData();
void saveData();

// Helper functions
int generateId();

// Add an email to the vector
void addEmail(const string emailAddress);
// Remove an email from the vector based on email address
void removeEmail(const string emailAddress);

#endif // FUNC_H