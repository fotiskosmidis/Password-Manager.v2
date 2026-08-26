#ifndef APP_PATHS_H
#define APP_PATHS_H

#include <filesystem>

std::filesystem::path appDataDirectory();
std::filesystem::path keyFilePath();
std::filesystem::path dataFilePath();

#endif // APP_PATHS_H