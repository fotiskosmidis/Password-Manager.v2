#include "app_paths.h"

#include <cstdlib>
#include <stdexcept>
#include <string>

namespace
{
std::filesystem::path environmentPath(const char* name)
{
	const char* value = std::getenv(name);
	if (value != nullptr && *value != '\0')
		return value;
	return {};
}
}

std::filesystem::path appDataDirectory()
{
	std::filesystem::path baseDirectory;

#ifdef _WIN32
	baseDirectory = environmentPath("APPDATA");
	if (baseDirectory.empty())
		baseDirectory = environmentPath("LOCALAPPDATA");
#else
	baseDirectory = environmentPath("XDG_DATA_HOME");
	if (baseDirectory.empty())
	{
		baseDirectory = environmentPath("HOME");
		if (!baseDirectory.empty())
			baseDirectory /= ".local/share";
	}
#endif

	if (baseDirectory.empty())
		throw std::runtime_error("Could not find a user data directory");

	baseDirectory /= "Password-Manager.v2";
	std::filesystem::create_directories(baseDirectory);
	return baseDirectory;
}

std::filesystem::path keyFilePath()
{
	return appDataDirectory() / "key.json";
}

std::filesystem::path dataFilePath()
{
	return appDataDirectory() / "data.json";
}